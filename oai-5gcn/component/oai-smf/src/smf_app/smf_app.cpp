/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file smf_app.cpp
 \brief
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr
 */

#include "smf_app.hpp"

#include <boost/uuid/random_generator.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "3gpp_24.007.h"
#include "3gpp_24.501.h"
#include "3gpp_29.500.h"
#include "3gpp_29.502.h"
#include "3gpp_conversions.hpp"
#include "ProblemDetails.h"
#include "RefToBinaryData.h"
#include "SmContextCreateError.h"
#include "SmContextCreatedData.h"
#include "SmContextMessage.h"
#include "SmContextUpdateError.h"
#include "async_shell_cmd.hpp"
#include "common_defs.h"
#include "conversions.hpp"
#include "itti.hpp"
#include "itti_msg_nx.hpp"
#include "logger.hpp"
#include "pfcp.hpp"
#include "smf.h"
#include "smf_event.hpp"
#include "smf_n1.hpp"
#include "smf_sbi.hpp"
#include "smf_n4.hpp"
#include "smf_paa_dynamic.hpp"
#include "string.hpp"

extern "C" {
#include "dynamic_memory_check.h"
#include "nas_message.h"
}

using namespace smf;

extern util::async_shell_cmd* async_shell_cmd_inst;
extern smf_app* smf_app_inst;
extern smf_config smf_cfg;
smf_n4* smf_n4_inst   = nullptr;
smf_sbi* smf_sbi_inst = nullptr;
extern itti_mw* itti_inst;

void smf_app_task(void*);

//------------------------------------------------------------------------------
int smf_app::apply_config(const smf_config& cfg) {
  Logger::smf_app().info("Apply config...");

  paa_t paa = {};
  for (int ia = 0; ia < cfg.num_dnn; ia++) {
    if (cfg.dnn[ia].pool_id_iv4 >= 0) {
      int pool_id = cfg.dnn[ia].pool_id_iv4;
      int range   = be32toh(cfg.ue_pool_range_high[pool_id].s_addr) -
                  be32toh(cfg.ue_pool_range_low[pool_id].s_addr);
      paa_dynamic::get_instance().add_pool(
          cfg.dnn[ia].dnn, pool_id, cfg.ue_pool_range_low[pool_id], range);
      // TODO: check with dnn_label
      Logger::smf_app().info("Applied config %s", cfg.dnn[ia].dnn.c_str());
      paa.ipv4_address = cfg.ue_pool_range_low[pool_id];
    }
    if (cfg.dnn[ia].pool_id_iv6 >= 0) {
      int pool_id = cfg.dnn[ia].pool_id_iv6;
      paa_dynamic::get_instance().add_pool(
          cfg.dnn[ia].dnn, pool_id, cfg.paa_pool6_prefix[pool_id],
          cfg.paa_pool6_prefix_len[pool_id]);
      paa.ipv6_address = cfg.paa_pool6_prefix[pool_id];

      // TODO: check with dnn_label
      Logger::smf_app().info(
          "Applied config for IPv6 %s", cfg.dnn[ia].dnn.c_str());
    }
  }

  Logger::smf_app().info("Applied config");
  return RETURNok;
}

//------------------------------------------------------------------------------
uint64_t smf_app::generate_seid() {
  std::unique_lock<std::mutex> ls(m_seid_n4_generator);
  uint64_t seid = ++seid_n4_generator;
  while ((is_seid_n4_exist(seid)) || (seid == UNASSIGNED_SEID)) {
    seid = ++seid_n4_generator;
  }
  set_seid_n4.insert(seid);
  ls.unlock();
  return seid;
}

//------------------------------------------------------------------------------
void smf_app::generate_smf_context_ref(std::string& smf_ref) {
  smf_ref = std::to_string(sm_context_ref_generator.get_uid());
}

//------------------------------------------------------------------------------
scid_t smf_app::generate_smf_context_ref() {
  return sm_context_ref_generator.get_uid();
}

//------------------------------------------------------------------------------
void smf_app::generate_ev_subscription_id(std::string& sub_id) {
  sub_id = std::to_string(evsub_id_generator.get_uid());
}

//------------------------------------------------------------------------------
evsub_id_t smf_app::generate_ev_subscription_id() {
  return evsub_id_generator.get_uid();
}

//------------------------------------------------------------------------------
bool smf_app::is_seid_n4_exist(const uint64_t& seid) const {
  return bool{set_seid_n4.count(seid) > 0};
}

//------------------------------------------------------------------------------
void smf_app::free_seid_n4(const uint64_t& seid) {
  std::unique_lock<std::mutex> ls(m_seid_n4_generator);
  set_seid_n4.erase(seid);
  ls.unlock();
}

//------------------------------------------------------------------------------
void smf_app::set_seid_2_smf_context(
    const seid_t& seid, std::shared_ptr<smf_context>& pc) {
  std::unique_lock lock(m_seid2smf_context);
  seid2smf_context[seid] = pc;
}

//------------------------------------------------------------------------------
bool smf_app::seid_2_smf_context(
    const seid_t& seid, std::shared_ptr<smf_context>& pc) const {
  std::shared_lock lock(m_seid2smf_context);
  std::map<seid_t, std::shared_ptr<smf_context>>::const_iterator it =
      seid2smf_context.find(seid);
  if (it != seid2smf_context.end()) {
    pc = it->second;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void smf_app::delete_smf_context(std::shared_ptr<smf_context> spc) {
  supi64_t supi64 = smf_supi_to_u64(spc.get()->get_supi());
  std::unique_lock lock(m_supi2smf_context);
  supi2smf_context.erase(supi64);
}

//------------------------------------------------------------------------------
void smf_app::restore_n4_sessions(const seid_t& seid) const {
  std::shared_lock lock(m_seid2smf_context);
  // TODO
}

//------------------------------------------------------------------------------
void smf_app_task(void*) {
  const task_id_t task_id = TASK_SMF_APP;
  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case N4_SESSION_ESTABLISHMENT_RESPONSE:
        if (itti_n4_session_establishment_response* m =
                dynamic_cast<itti_n4_session_establishment_response*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N4_SESSION_MODIFICATION_RESPONSE:
        if (itti_n4_session_modification_response* m =
                dynamic_cast<itti_n4_session_modification_response*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N4_SESSION_DELETION_RESPONSE:
        if (itti_n4_session_deletion_response* m =
                dynamic_cast<itti_n4_session_deletion_response*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N4_SESSION_REPORT_REQUEST:
        smf_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_n4_session_report_request>(
                shared_msg));
        break;

      case N4_NODE_FAILURE:
        smf_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_n4_node_failure>(shared_msg));
        break;

      case N11_SESSION_N1N2_MESSAGE_TRANSFER_RESPONSE_STATUS:
        if (itti_n11_n1n2_message_transfer_response_status* m =
                dynamic_cast<itti_n11_n1n2_message_transfer_response_status*>(
                    msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N11_SESSION_UPDATE_PDU_SESSION_STATUS:
        if (itti_n11_update_pdu_session_status* m =
                dynamic_cast<itti_n11_update_pdu_session_status*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N11_SESSION_CREATE_SM_CONTEXT_RESPONSE:
        if (itti_n11_create_sm_context_response* m =
                dynamic_cast<itti_n11_create_sm_context_response*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE:
        if (itti_n11_update_sm_context_response* m =
                dynamic_cast<itti_n11_update_sm_context_response*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE:
        if (itti_n11_release_sm_context_response* m =
                dynamic_cast<itti_n11_release_sm_context_response*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N11_REGISTER_NF_INSTANCE_RESPONSE:
        if (itti_n11_register_nf_instance_response* m =
                dynamic_cast<itti_n11_register_nf_instance_response*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case N11_UPDATE_NF_INSTANCE_RESPONSE:
        if (itti_n11_update_nf_instance_response* m =
                dynamic_cast<itti_n11_update_nf_instance_response*>(msg)) {
          smf_app_inst->handle_itti_msg(std::ref(*m));
        }
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::smf_app().info("TIME-OUT event timer id %d", to->timer_id);
          switch (to->arg1_user) {
            case TASK_SMF_APP_TRIGGER_T3591:
              smf_app_inst->timer_t3591_timeout(to->timer_id, to->arg2_user);
              break;
            case TASK_SMF_APP_TIMEOUT_NRF_HEARTBEAT:
              smf_app_inst->timer_nrf_heartbeat_timeout(
                  to->timer_id, to->arg2_user);
              break;
            case TASK_SMF_APP_TIMEOUT_NRF_DEREGISTRATION:
              smf_app_inst->timer_nrf_deregistration(
                  to->timer_id, to->arg2_user);
              break;
            default:;
          }
        }
        break;

      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::smf_app().info("Received terminate message");
          return;
        }
        break;

      case HEALTH_PING:
        break;

      default:
        Logger::smf_app().info("no handler for msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
smf_app::smf_app(const std::string& config_file)
    : m_seid2smf_context(),
      m_supi2smf_context(),
      m_scid2smf_context(),
      m_sm_context_create_promises(),
      m_sm_context_update_promises(),
      m_sm_context_release_promises() {
  Logger::smf_app().startup("Starting...");

  supi2smf_context  = {};
  set_seid_n4       = {};
  seid_n4_generator = 0;

  apply_config(smf_cfg);

  if (itti_inst->create_task(TASK_SMF_APP, smf_app_task, nullptr)) {
    Logger::smf_app().error("Cannot create task TASK_SMF_APP");
    throw std::runtime_error("Cannot create task TASK_SMF_APP");
  }

  try {
    smf_n4_inst  = new smf_n4();
    smf_sbi_inst = new smf_sbi();
  } catch (std::exception& e) {
    Logger::smf_app().error("Cannot create SMF_APP: %s", e.what());
    throw;
  }

  // TODO: should be done when SMF select UPF for a particular UE (should be
  // verified)
  for (std::vector<pfcp::node_id_t>::const_iterator it = smf_cfg.upfs.begin();
       it != smf_cfg.upfs.end(); ++it) {
    start_upf_association(*it);
  }

  if (smf_cfg.discover_upf) {
    // Trigger NFStatusNotify subscription to be noticed when a new UPF becomes
    // available (if this option is enabled)
    trigger_upf_status_notification_subscribe();
  }

  // Register to NRF (if this option is enabled)
  if (smf_cfg.register_nrf) {
    unsigned int microsecond = 10000;  // 10ms
    usleep(microsecond);
    register_to_nrf();
  }

  Logger::smf_app().startup("Started");
}

//------------------------------------------------------------------------------
void smf_app::start_upf_association(const pfcp::node_id_t& node_id) {
  std::time_t time_epoch = std::time(nullptr);
  uint64_t tv_ntp        = time_epoch + SECONDS_SINCE_FIRST_EPOCH;

  pfcp_associations::get_instance().add_peer_candidate_node(node_id);
  std::shared_ptr<itti_n4_association_setup_request> n4_asc =
      std::shared_ptr<itti_n4_association_setup_request>(
          new itti_n4_association_setup_request(TASK_SMF_APP, TASK_SMF_N4));

  // n4_asc->trxn_id = smf_n4_inst->generate_trxn_id();
  pfcp::cp_function_features_s cp_function_features;
  cp_function_features      = {};
  cp_function_features.load = 1;
  cp_function_features.ovrl = 1;

  pfcp::node_id_t this_node_id = {};
  if (smf_cfg.get_pfcp_node_id(this_node_id) == RETURNok) {
    n4_asc->pfcp_ies.set(this_node_id);
    pfcp::recovery_time_stamp_t r = {.recovery_time_stamp = (uint32_t) tv_ntp};
    n4_asc->pfcp_ies.set(r);

    n4_asc->pfcp_ies.set(cp_function_features);
    if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
      n4_asc->r_endpoint =
          endpoint(node_id.u1.ipv4_address, pfcp::default_port);
      int ret = itti_inst->send_msg(n4_asc);
      if (RETURNok != ret) {
        Logger::smf_app().error(
            "Could not send ITTI message %s to task TASK_SMF_N4",
            n4_asc.get()->get_msg_name());
      }
    } else {
      Logger::smf_app().warn("Start_association() node_id IPV6, FQDN!");
    }
  }
}

//------------------------------------------------------------------------------
void smf_app::start_upf_association(
    const pfcp::node_id_t& node_id, const upf_profile& profile) {
  std::time_t time_epoch = std::time(nullptr);
  uint64_t tv_ntp        = time_epoch + SECONDS_SINCE_FIRST_EPOCH;

  pfcp_associations::get_instance().add_peer_candidate_node(node_id, profile);
  std::shared_ptr<itti_n4_association_setup_request> n4_asc =
      std::shared_ptr<itti_n4_association_setup_request>(
          new itti_n4_association_setup_request(TASK_SMF_APP, TASK_SMF_N4));

  // n4_asc->trxn_id = smf_n4_inst->generate_trxn_id();
  pfcp::cp_function_features_s cp_function_features;
  cp_function_features      = {};
  cp_function_features.load = 1;
  cp_function_features.ovrl = 1;

  pfcp::node_id_t this_node_id = {};
  if (smf_cfg.get_pfcp_node_id(this_node_id) == RETURNok) {
    n4_asc->pfcp_ies.set(this_node_id);
    pfcp::recovery_time_stamp_t r = {.recovery_time_stamp = (uint32_t) tv_ntp};
    n4_asc->pfcp_ies.set(r);

    n4_asc->pfcp_ies.set(cp_function_features);
    if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
      n4_asc->r_endpoint =
          endpoint(node_id.u1.ipv4_address, pfcp::default_port);
      int ret = itti_inst->send_msg(n4_asc);
      if (RETURNok != ret) {
        Logger::smf_app().error(
            "Could not send ITTI message %s to task TASK_SMF_N4",
            n4_asc.get()->get_msg_name());
      }
    } else {
      Logger::smf_app().warn("Start_association() node_id IPV6, FQDN!");
    }
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n4_session_establishment_response& seresp) {
  std::shared_ptr<smf_context> pc = {};
  if (seid_2_smf_context(seresp.seid, pc)) {
    pc.get()->handle_itti_msg(seresp);
  } else {
    Logger::smf_app().debug(
        "Received N4 Session Establishment Response seid" TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", smf_context not found, discarded!",
        seresp.seid, seresp.trxn_id);
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n4_session_modification_response& smresp) {
  std::shared_ptr<smf_context> pc = {};
  if (seid_2_smf_context(smresp.seid, pc)) {
    pc.get()->handle_itti_msg(smresp);
  } else {
    Logger::smf_app().debug(
        "Received N4 Session Modification Response seid" TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", smf_context not found, discarded!",
        smresp.seid, smresp.trxn_id);
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n4_session_deletion_response& smresp) {
  std::shared_ptr<smf_context> pc = {};
  if (seid_2_smf_context(smresp.seid, pc)) {
    pc.get()->handle_itti_msg(smresp);

    if (pc->get_number_dnn_contexts() == 0) {
      delete_smf_context(pc);
    }
  } else {
    Logger::smf_app().debug(
        "Received N4 Session Deletion Response seid" TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", smf_context not found, discarded!",
        smresp.seid, smresp.trxn_id);
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(
    std::shared_ptr<itti_n4_session_report_request> snr) {
  std::shared_ptr<smf_context> pc = {};
  if (seid_2_smf_context(snr->seid, pc)) {
    pc.get()->handle_itti_msg(snr);
  } else {
    Logger::smf_app().debug(
        "Received N4 Session Report Request seid" TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", smf_context not found, discarded!",
        snr->seid, snr->trxn_id);
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(std::shared_ptr<itti_n4_node_failure> snf) {
  pfcp::node_id_t node_id = snf->node_id;

  for (auto it : scid2smf_context) {
    if (it.second->upf_node_id == node_id) {
      supi64_t supi64 = smf_supi_to_u64(it.second->supi);
      Logger::smf_app().debug(
          "Remove the associated PDU session (SUPI " SUPI_64_FMT
          ", PDU Sessin Id %d)",
          supi64, it.second->pdu_session_id);
      // TODO: remove the session
    }
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(
    itti_n11_n1n2_message_transfer_response_status& m) {
  Logger::smf_app().info("Process N1N2MessageTransfer Response");

  switch (m.procedure_type) {
    case session_management_procedures_type_e::
        PDU_SESSION_ESTABLISHMENT_UE_REQUESTED: {
      // Update PDU Session accordingly
      Logger::smf_app().info("PDU_SESSION_ESTABLISHMENT_UE_REQUESTED");
      pdu_session_status_e status = {
          pdu_session_status_e::PDU_SESSION_INACTIVE};
      upCnx_state_e state = {upCnx_state_e::UPCNX_STATE_DEACTIVATED};

      if ((static_cast<http_response_codes_e>(m.response_code) ==
           http_response_codes_e::HTTP_RESPONSE_CODE_OK) or
          (static_cast<http_response_codes_e>(m.response_code) ==
           http_response_codes_e::HTTP_RESPONSE_CODE_ACCEPTED)) {
        if (m.msg_type == PDU_SESSION_ESTABLISHMENT_REJECT) {
          status = pdu_session_status_e::PDU_SESSION_INACTIVE;
        } else if (m.msg_type == PDU_SESSION_ESTABLISHMENT_ACCEPT) {
          status = pdu_session_status_e::PDU_SESSION_ESTABLISHMENT_PENDING;
          state  = upCnx_state_e::UPCNX_STATE_ACTIVATING;
        }
        update_pdu_session_status(m.scid, status);
        update_pdu_session_upCnx_state(m.scid, state);
        Logger::smf_app().debug(
            "Got successful response from AMF (response code %d), set session "
            "status to %s",
            m.response_code,
            pdu_session_status_e2str.at(static_cast<int>(status)).c_str());
      } else {
        // TODO:
        Logger::smf_app().debug(
            "Got response from AMF (response code %d)", m.response_code);
      }
    } break;

    case session_management_procedures_type_e::
        SERVICE_REQUEST_NETWORK_TRIGGERED: {
      Logger::smf_app().debug(
          "Got response from AMF (response code %d) with cause %s",
          m.response_code, m.cause.c_str());
      if ((static_cast<http_response_codes_e>(m.response_code) !=
           http_response_codes_e::HTTP_RESPONSE_CODE_OK) and
          (static_cast<http_response_codes_e>(m.response_code) !=
           http_response_codes_e::HTTP_RESPONSE_CODE_ACCEPTED)) {
        Logger::smf_app().debug("Send failure indication to UPF");
        // TODO: to be completed
        pfcp::node_id_t up_node_id = {};
        // Get UPF node
        std::shared_ptr<smf_context_ref> scf = {};
        if (smf_app_inst->is_scid_2_smf_context(m.scid)) {
          scf        = scid_2_smf_context(m.scid);
          up_node_id = scf.get()->upf_node_id;
        } else {
          Logger::smf_app().warn(
              "SM Context associated with this id " SCID_FMT " does not exit!",
              m.scid);
          return;
        }

        itti_n4_session_failure_indication* itti_n4 =
            new itti_n4_session_failure_indication(TASK_SMF_APP, TASK_SMF_N4);
        itti_n4->seid    = m.seid;
        itti_n4->trxn_id = m.trxn_id;
        itti_n4->r_endpoint =
            endpoint(up_node_id.u1.ipv4_address, pfcp::default_port);
        std::shared_ptr<itti_n4_session_failure_indication>
            itti_n4_failure_indication =
                std::shared_ptr<itti_n4_session_failure_indication>(itti_n4);

        Logger::smf_app().info(
            "Sending ITTI message %s to task TASK_SMF_N4",
            itti_n4->get_msg_name());
        int ret = itti_inst->send_msg(itti_n4_failure_indication);
        if (RETURNok != ret) {
          Logger::smf_app().error(
              "Could not send ITTI message %s to task TASK_SMF_N4",
              itti_n4->get_msg_name());
          return;
        }
      }
    } break;

    default: {
      Logger::smf_app().warn("Unknown procedure type %d", m.procedure_type);
    }
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n11_update_pdu_session_status& m) {
  Logger::smf_app().info(
      "Set PDU Session Status to %s",
      pdu_session_status_e2str.at(static_cast<int>(m.pdu_session_status))
          .c_str());
  update_pdu_session_status(m.scid, m.pdu_session_status);
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n11_create_sm_context_response& m) {
  Logger::smf_app().debug(
      "PDU Session Create SM Context: Set promise with ID %d to ready", m.pid);
  pdu_session_create_sm_context_response sm_context_response = {};
  std::unique_lock lock(m_sm_context_create_promises);
  if (sm_context_create_promises.count(m.pid) > 0) {
    sm_context_create_promises[m.pid]->set_value(m.res);
    // Remove this promise from list
    sm_context_create_promises.erase(m.pid);
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n11_update_sm_context_response& m) {
  Logger::smf_app().debug(
      "PDU Session Update SM Context: Set promise with ID %d to ready", m.pid);
  pdu_session_update_sm_context_response sm_context_response = {};
  std::unique_lock lock(m_sm_context_update_promises);
  if (sm_context_update_promises.count(m.pid) > 0) {
    sm_context_update_promises[m.pid]->set_value(m.res);
    // Remove this promise from list
    sm_context_update_promises.erase(m.pid);
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n11_release_sm_context_response& m) {
  Logger::smf_app().debug(
      "PDU Session Release SM Context: Set promise with ID %d to ready", m.pid);
  pdu_session_release_sm_context_response sm_context_response = {};
  std::unique_lock lock(m_sm_context_release_promises);
  if (sm_context_release_promises.count(m.pid) > 0) {
    sm_context_release_promises[m.pid]->set_value(m.res);
    // Remove this promise from list
    sm_context_release_promises.erase(m.pid);
  }
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n11_register_nf_instance_response& r) {
  Logger::smf_app().debug("Handle NF Instance Registration response");

  nf_instance_profile = r.profile;
  // Set heartbeat timer
  Logger::smf_app().debug(
      "Set value of NRF Heartbeat timer to %d",
      r.profile.get_nf_heartBeat_timer());
  timer_nrf_heartbeat = itti_inst->timer_setup(
      r.profile.get_nf_heartBeat_timer(), 0, TASK_SMF_APP,
      TASK_SMF_APP_TIMEOUT_NRF_HEARTBEAT,
      0);  // TODO arg2_user

  /*  //Set timer to send NF Deregistration (for testing purpose)
    itti_inst->timer_setup(50, 0,
                               TASK_SMF_APP,
    TASK_SMF_APP_TIMEOUT_NRF_DEREGISTRATION, 0);  // TODO arg2_user
  */
}

//------------------------------------------------------------------------------
void smf_app::handle_itti_msg(itti_n11_update_nf_instance_response& u) {
  Logger::smf_app().debug("NF Update NF response");

  Logger::smf_app().debug(
      "Set NRF Heartbeat timer (%d)",
      nf_instance_profile.get_nf_heartBeat_timer());

  // Set heartbeat timer
  //  timer_nrf_heartbeat = itti_inst->timer_setup(
  //      nf_instance_profile.get_nf_heartBeat_timer(), 0, TASK_SMF_APP,
  //      TASK_SMF_APP_TIMEOUT_NRF_HEARTBEAT, 0); //TODO arg2_user
}

//------------------------------------------------------------------------------
void smf_app::handle_pdu_session_create_sm_context_request(
    std::shared_ptr<itti_n11_create_sm_context_request> smreq) {
  Logger::smf_app().info(
      "Handle a PDU Session Create SM Context Request from an AMF (HTTP "
      "version %d)",
      smreq->http_version);
  // handle PDU Session Create SM Context Request as specified in section 4.3.2
  // 3GPP TS 23.502
  std::string n1_sm_message, n1_sm_message_hex;
  nas_message_t decoded_nas_msg       = {};
  cause_value_5gsm_e cause_n1         = {cause_value_5gsm_e::CAUSE_0_UNKNOWN};
  pdu_session_type_t pdu_session_type = {.pdu_session_type =
                                             PDU_SESSION_TYPE_E_IPV4};

  // Step 1. Decode NAS and get the necessary information
  int decoder_rc = smf_n1::get_instance().decode_n1_sm_container(
      decoded_nas_msg, smreq->req.get_n1_sm_message());

  // Failed to decode, send reply to AMF with PDU Session Establishment Reject
  if (decoder_rc != RETURNok) {
    Logger::smf_app().warn("N1 SM container cannot be decoded correctly!");

    if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
            smreq->req, n1_sm_message,
            cause_value_5gsm_e::CAUSE_95_SEMANTICALLY_INCORRECT_MESSAGE)) {
      smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_message_hex);
      // trigger to send reply to AMF
      trigger_create_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
          PDU_SESSION_APPLICATION_ERROR_N1_SM_ERROR, n1_sm_message_hex,
          smreq->pid);
    } else {
      // trigger to send reply to AMF
      trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
          smreq->pid, N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
    }
    return;
  }

  // Get necessary info from NAS
  xgpp_conv::sm_context_request_from_nas(decoded_nas_msg, smreq->req);

  pdu_session_type.pdu_session_type = smreq->req.get_pdu_session_type();
  // TODO: Support IPv4 only for now
  if (pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV6) {
    cause_n1 = cause_value_5gsm_e::CAUSE_50_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED;
  } else if (
      (pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_ETHERNET) or
      (pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_UNSTRUCTURED)) {
    cause_n1 = cause_value_5gsm_e::CAUSE_28_UNKNOWN_PDU_SESSION_TYPE;
  }

  if ((pdu_session_type.pdu_session_type != PDU_SESSION_TYPE_E_IPV4) and
      (pdu_session_type.pdu_session_type != PDU_SESSION_TYPE_E_IPV4V6)) {
    // PDU Session Establishment Reject
    if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
            smreq->req, n1_sm_message, cause_n1)) {
      smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_message_hex);
      // trigger to send reply to AMF
      trigger_create_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
          PDU_SESSION_APPLICATION_ERROR_PDUTYPE_DENIED, n1_sm_message_hex,
          smreq->pid);
    } else {
      trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
          smreq->pid, N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
    }
    return;
  }

  // Get SUPI, SNSSAI
  supi_t supi             = smreq->req.get_supi();
  std::string supi_prefix = smreq->req.get_supi_prefix();
  supi64_t supi64         = smf_supi_to_u64(supi);
  snssai_t snssai         = smreq->req.get_snssai();
  Logger::smf_app().info(
      "Handle a PDU Session Create SM Context Request message from AMF, "
      "SUPI " SUPI_64_FMT ", SNSSAI SST %d, SD %s",
      supi64, snssai.sST, snssai.sD.c_str());

  // Step 2. Verify Procedure transaction id, pdu session id, message type,
  // request type, etc.
  // Check PTI
  procedure_transaction_id_t pti = {
      .procedure_transaction_id =
          decoded_nas_msg.plain.sm.header.procedure_transaction_identity};
  if ((pti.procedure_transaction_id ==
       PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED) ||
      (pti.procedure_transaction_id > PROCEDURE_TRANSACTION_IDENTITY_LAST)) {
    Logger::smf_app().warn(
        "Invalid PTI value (pti = %d)", pti.procedure_transaction_id);
    // PDU Session Establishment Reject including cause "#81 Invalid PTI value"
    // (section 7.3.1 @3GPP TS 24.501)
    if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
            smreq->req, n1_sm_message,
            cause_value_5gsm_e::CAUSE_81_INVALID_PTI_VALUE)) {
      smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_message_hex);
      // trigger to send reply to AMF
      trigger_create_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
          PDU_SESSION_APPLICATION_ERROR_N1_SM_ERROR, n1_sm_message_hex,
          smreq->pid);
    } else {
      trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
          smreq->pid, N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
    }
    return;
  }

  // Check pdu session id
  pdu_session_id_t pdu_session_id =
      decoded_nas_msg.plain.sm.header.pdu_session_identity;
  if ((pdu_session_id == PDU_SESSION_IDENTITY_UNASSIGNED) ||
      (pdu_session_id > PDU_SESSION_IDENTITY_LAST)) {
    Logger::smf_app().warn("Invalid PDU Session ID value (%d)", pdu_session_id);
    // section 7.3.2@3GPP TS 24.501; NAS N1 SM message: ignore the message
    // trigger to send reply to AMF
    trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_406_NOT_ACCEPTABLE, smreq->pid,
        N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
    return;
  }

  // Check message type
  uint8_t message_type = decoded_nas_msg.plain.sm.header.message_type;
  if (message_type != PDU_SESSION_ESTABLISHMENT_REQUEST) {
    Logger::smf_app().warn(
        "Invalid message type (message type = %d)", message_type);
    // PDU Session Establishment Reject
    //(24.501 (section 7.4)) implementation dependent->do similar to UE:
    // response with a 5GSM STATUS message including cause "#98 message type not
    // compatible with protocol state."
    if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
            smreq->req, n1_sm_message,
            cause_value_5gsm_e::
                CAUSE_98_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE)) {
      smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_message_hex);
      // trigger to send reply to AMF
      trigger_create_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
          PDU_SESSION_APPLICATION_ERROR_N1_SM_ERROR, n1_sm_message_hex,
          smreq->pid);
    } else {
      trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
          smreq->pid, N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
    }
    return;
  }

  // Check request type
  std::string request_type = smreq->req.get_request_type();
  if (request_type.compare("INITIAL_REQUEST") != 0) {
    Logger::smf_app().warn(
        "Invalid request type (request type = %s)", request_type.c_str());
    //"Existing PDU Session", AMF should use PDUSession_UpdateSMContext instead
    //(see step 3, section 4.3.2.2.1 @ 3GPP TS 23.502 v16.0.0) ignore the
    // message
    return;
  }

  // TODO:
  // If no DNN information from UE, set to default value
  std::string dnn = smreq->req.get_dnn();
  if (dnn.length() == 0) {
    dnn == smf_cfg.get_default_dnn();
  }

  // TODO: For the moment, not support PDU session authentication and
  // authorization by the external DN

  // Step 3. check if the DNN requested is valid
  std::string nd_dnn;
  util::dotted_to_string(dnn, nd_dnn);
  dnn = nd_dnn;
  // update DNN
  smreq->req.set_dnn(dnn);
  if (not smf_cfg.is_dotted_dnn_handled(dnn, pdu_session_type)) {
    // Not a valid request...
    Logger::smf_app().warn(
        "Received a PDU Session Create SM Context Request: unknown requested "
        "DNN %s, ignore message!",
        dnn.c_str());
    // PDU Session Establishment Reject
    if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
            smreq->req, n1_sm_message,
            cause_value_5gsm_e::CAUSE_27_MISSING_OR_UNKNOWN_DNN)) {
      smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_message_hex);
      // trigger to send reply to AMF
      trigger_create_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
          PDU_SESSION_APPLICATION_ERROR_DNN_DENIED, n1_sm_message_hex,
          smreq->pid);
    } else {
      trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
          smreq->pid, N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
    }
    return;
  }

  // Step 4. create a context for this supi if not existed, otherwise update
  std::shared_ptr<smf_context> sc = {};
  if (is_supi_2_smf_context(supi64)) {
    Logger::smf_app().debug(
        "Update SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = supi_2_smf_context(supi64);
    sc.get()->set_supi(supi);
  } else {
    Logger::smf_app().debug(
        "Create a new SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = std::shared_ptr<smf_context>(new smf_context());
    sc.get()->set_supi(supi);
    sc.get()->set_supi_prefix(supi_prefix);
    set_supi_2_smf_context(supi64, sc);
  }

  // Step 5. Create/update context with dnn information
  std::shared_ptr<dnn_context> sd = {};

  if (!sc.get()->find_dnn_context(snssai, dnn, sd)) {
    if (nullptr == sd.get()) {
      // create a new one and insert to the list
      Logger::smf_app().debug(
          "Create a DNN context and add to the SMF context");
      sd                   = std::shared_ptr<dnn_context>(new dnn_context(dnn));
      sd.get()->in_use     = true;
      sd.get()->dnn_in_use = dnn;
      sd.get()->nssai      = snssai;
      sc.get()->insert_dnn(sd);
    }
  }

  // Step 6. if colliding with an existing SM context (session is already
  // existed and request type is INITIAL_REQUEST)  Delete the local context
  // (including and any associated resources in the UPF and PCF) and create a
  // new one
  if (is_scid_2_smf_context(supi64, dnn, snssai, pdu_session_id) &&
      (request_type.compare("INITIAL_REQUEST") == 0)) {
    // remove smf_pdu_session (including all flows associated to this session)
    sd.get()->remove_pdu_session(pdu_session_id);
    Logger::smf_app().warn(
        "PDU Session already existed (SUPI " SUPI_64_FMT
        ", DNN %s, NSSAI (sst %d, sd %s), PDU Session ID %d)",
        supi64, dnn.c_str(), snssai.sST, snssai.sD.c_str(), pdu_session_id);
  }

  // Step 7. retrieve Session Management Subscription data from UDM if not
  // available (step 4, section 4.3.2 3GPP TS 23.502)
  std::string dnn_selection_mode = smreq->req.get_dnn_selection_mode();
  // if the Session Management Subscription data is not available, get from
  // configuration file or UDM
  if (not sc.get()->is_dnn_snssai_subscription_data(dnn, snssai)) {
    Logger::smf_app().debug(
        "The Session Management Subscription data is not available");

    session_management_subscription* s =
        new session_management_subscription(snssai);
    std::shared_ptr<session_management_subscription> subscription =
        std::shared_ptr<session_management_subscription>(s);

    if (not use_local_configuration_subscription_data(dnn_selection_mode)) {
      Logger::smf_app().debug(
          "Retrieve Session Management Subscription data from the UDM");
      if (smf_sbi_inst->get_sm_data(supi64, dnn, snssai, subscription)) {
        // update dnn_context with subscription info
        sc.get()->insert_dnn_subscription(snssai, dnn, subscription);
      } else {
        // Cannot retrieve information from UDM, reject PDU session
        // establishment
        Logger::smf_app().warn(
            "Received a PDU Session Create SM Context Request, couldn't "
            "retrieve the Session Management Subscription from UDM, ignore "
            "message!");
        // PDU Session Establishment Reject
        if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
                smreq->req, n1_sm_message,
                cause_value_5gsm_e::
                    CAUSE_29_USER_AUTHENTICATION_OR_AUTHORIZATION_FAILED)) {
          smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_message_hex);
          // trigger to send reply to AMF
          trigger_create_context_error_response(
              http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
              PDU_SESSION_APPLICATION_ERROR_SUBSCRIPTION_DENIED,
              n1_sm_message_hex, smreq->pid);

        } else {
          trigger_http_response(
              http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
              smreq->pid, N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
        }
        return;
      }
    } else {
      // use local configuration
      Logger::smf_app().debug(
          "Retrieve Session Management Subscription data from local "
          "configuration");
      if (get_session_management_subscription_data(
              supi64, dnn, snssai, subscription)) {
        // update dnn_context with subscription info
        sc.get()->insert_dnn_subscription(snssai, dnn, subscription);
      }
    }
  }

  // store PLMN
  sc.get()->set_plmn(smreq->req.get_plmn());

  // Step 8. generate a SMF context Id and store the corresponding information
  // in a map (SM_Context_ID, (supi, dnn, nssai, pdu_session_id))
  scid_t scid = generate_smf_context_ref();
  std::shared_ptr<smf_context_ref> scf =
      std::shared_ptr<smf_context_ref>(new smf_context_ref());
  scf.get()->supi           = supi;
  scf.get()->dnn            = dnn;
  scf.get()->nssai          = snssai;
  scf.get()->pdu_session_id = pdu_session_id;
  set_scid_2_smf_context(scid, scf);
  smreq->set_scid(scid);
  // store scid in the context itself
  sc.get()->set_scid(scid);
  Logger::smf_app().debug("Generated a SMF Context ID " SCID_FMT " ", scid);

  // Step 9. Let the context handle the message
  sc.get()->handle_pdu_session_create_sm_context_request(smreq);
}

//------------------------------------------------------------------------------
void smf_app::handle_pdu_session_update_sm_context_request(
    std::shared_ptr<itti_n11_update_sm_context_request> smreq) {
  // handle PDU Session Update SM Context Request as specified in section 4.3.2
  // 3GPP TS 23.502
  Logger::smf_app().info(
      "Handle a PDU Session Update SM Context Request from an AMF (HTTP "
      "version %d)",
      smreq->http_version);

  // Step 1. get supi, dnn, nssai, pdu_session id from sm_context
  // SM Context ID - uint32_t in our case
  scid_t scid = {};
  try {
    scid = std::stoi(smreq->scid);
  } catch (const std::exception& err) {
    Logger::smf_app().warn(
        "Received a PDU Session Update SM Context Request, couldn't retrieve "
        "the corresponding SMF context, ignore message!");
    // trigger to send reply to AMF
    trigger_update_context_error_response(
        http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND,
        PDU_SESSION_APPLICATION_ERROR_CONTEXT_NOT_FOUND, smreq->pid);

    return;
  }

  std::shared_ptr<smf_context_ref> scf = {};

  if (is_scid_2_smf_context(scid)) {
    scf = scid_2_smf_context(scid);
  } else {
    Logger::smf_app().warn(
        "SM Context associated with this id " SCID_FMT " does not exit!", scid);
    // trigger to send reply to AMF
    trigger_update_context_error_response(
        http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND,
        PDU_SESSION_APPLICATION_ERROR_CONTEXT_NOT_FOUND, smreq->pid);
    return;
  }

  // Step 2. store supi, dnn, nssai  in itti_n11_update_sm_context_request to be
  // processed later on
  supi64_t supi64 = smf_supi_to_u64(scf.get()->supi);
  smreq->req.set_supi(scf.get()->supi);
  smreq->req.set_dnn(scf.get()->dnn);
  smreq->req.set_snssai(scf.get()->nssai);
  smreq->req.set_pdu_session_id(scf.get()->pdu_session_id);

  // Step 2. find the smf context
  std::shared_ptr<smf_context> sc = {};
  if (is_supi_2_smf_context(supi64)) {
    sc = supi_2_smf_context(supi64);
    Logger::smf_app().debug(
        "Retrieve SMF context with SUPI " SUPI_64_FMT "", supi64);
  } else {
    // send PDUSession_SMUpdateContext Response to AMF
    Logger::smf_app().warn(
        "Received PDU Session Update SM Context Request with Supi " SUPI_64_FMT
        "couldn't retrieve the corresponding SMF context, ignore message!",
        supi64);
    // trigger to send reply to AMF
    trigger_update_context_error_response(
        http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND,
        PDU_SESSION_APPLICATION_ERROR_CONTEXT_NOT_FOUND, smreq->pid);

    return;
  }

  // get dnn context
  std::shared_ptr<dnn_context> sd = {};

  if (!sc.get()->find_dnn_context(scf.get()->nssai, scf.get()->dnn, sd)) {
    if (nullptr == sd.get()) {
      Logger::smf_app().warn(
          "Received PDU Session Update SM Context Request, couldn't retrieve "
          "the corresponding SMF context, ignore message!");
      // trigger to send reply to AMF
      trigger_update_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND,
          PDU_SESSION_APPLICATION_ERROR_CONTEXT_NOT_FOUND, smreq->pid);
      return;
    }
  }

  // Step 3. Verify AMF??

  // Step 4. handle the message in smf_context
  sc.get()->handle_pdu_session_update_sm_context_request(smreq);
}
//------------------------------------------------------------------------------
void smf_app::handle_pdu_session_release_sm_context_request(
    std::shared_ptr<itti_n11_release_sm_context_request> smreq) {
  Logger::smf_app().info(
      "Handle a PDU Session Release SM Context Request from an AMF");

  // Step 1. get supi, dnn, nssai, pdu_session id from sm_context
  // SM Context ID - uint32_t in our case
  scid_t scid = {};
  try {
    scid = std::stoi(smreq->scid);
  } catch (const std::exception& err) {
    Logger::smf_app().warn(
        "Received a PDU Session Release SM Context Request, couldn't retrieve "
        "the corresponding SMF context, ignore message!");
    // trigger to send reply to AMF
    trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND, smreq->pid,
        N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE);
    return;
  }

  std::shared_ptr<smf_context_ref> scf = {};

  if (is_scid_2_smf_context(scid)) {
    scf = scid_2_smf_context(scid);
  } else {
    Logger::smf_app().warn(
        "Context associated with this id " SCID_FMT " does not exit!", scid);
    // trigger to send reply to AMF
    trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND, smreq->pid,
        N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE);
    return;
  }

  // Step 2. store supi, dnn, nssai  in itti_n11_update_sm_context_request to be
  // processed later on
  supi64_t supi64 = smf_supi_to_u64(scf.get()->supi);
  smreq->req.set_supi(scf.get()->supi);
  smreq->req.set_dnn(scf.get()->dnn);
  smreq->req.set_snssai(scf.get()->nssai);
  smreq->req.set_pdu_session_id(scf.get()->pdu_session_id);

  // Step 2. find the smf context
  std::shared_ptr<smf_context> sc = {};
  if (is_supi_2_smf_context(supi64)) {
    sc = supi_2_smf_context(supi64);
    Logger::smf_app().debug(
        "Retrieve SMF context with SUPI " SUPI_64_FMT "", supi64);
  } else {
    // send PDUSession_SMReleaseContext Response to AMF
    Logger::smf_app().warn(
        "Received PDU Session Release SM Context Request with Supi " SUPI_64_FMT
        "couldn't retrieve the corresponding SMF context, ignore message!",
        supi64);
    // trigger to send reply to AMF
    trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND, smreq->pid,
        N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE);
    return;
  }

  // get dnn context
  std::shared_ptr<dnn_context> sd = {};

  if (!sc.get()->find_dnn_context(scf.get()->nssai, scf.get()->dnn, sd)) {
    if (nullptr == sd.get()) {
      // Error, DNN context doesn't exist, send PDUSession_SMUpdateContext
      // Response to AMF
      Logger::smf_app().warn(
          "Received PDU Session Release SM Context Request, couldn't retrieve "
          "the corresponding SMF context, ignore message!");
      // trigger to send reply to AMF
      trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND, smreq->pid,
          N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE);
      return;
    }
  }

  // Step 3. handle the message in smf_context
  sc.get()->handle_pdu_session_release_sm_context_request(smreq);
}

//------------------------------------------------------------------------------
void smf_app::trigger_pdu_session_modification(
    const supi_t& supi, const std::string& dnn,
    const pdu_session_id_t pdu_session_id, const snssai_t& snssai,
    const pfcp::qfi_t& qfi, const uint8_t& http_version) {
  // SMF-requested session modification, see section 4.3.3.2@3GPP TS 23.502
  // The SMF may decide to modify PDU Session. This procedure also may be
  // triggered based on locally configured policy or triggered from the (R)AN
  // (see clause 4.2.6 and clause 4.9.1).  It may also be triggered if the UP
  // connection is activated (as described in Service Request procedure) and the
  // SMF has marked that the status of one or more QoS Flows are deleted in the
  // 5GC but not synchronized with  the UE yet.

  std::shared_ptr<itti_nx_trigger_pdu_session_modification> itti_msg =
      std::make_shared<itti_nx_trigger_pdu_session_modification>(
          TASK_SMF_APP, TASK_SMF_SBI);
  itti_msg->http_version = http_version;

  // step 1. collect the necessary information
  itti_msg->msg.set_supi(supi);
  itti_msg->msg.set_dnn(dnn);
  itti_msg->msg.set_pdu_session_id(pdu_session_id);
  itti_msg->msg.set_snssai(snssai);
  itti_msg->msg.add_qfi(qfi);
  supi64_t supi64 = smf_supi_to_u64(supi);

  // Step 2. find the smf context
  std::shared_ptr<smf_context> sc = {};

  if (is_supi_2_smf_context(supi64)) {
    sc = supi_2_smf_context(supi64);
    Logger::smf_app().debug(
        "Retrieve SMF context with SUPI " SUPI_64_FMT "", supi64);
  } else {
    Logger::smf_app().debug(
        "SMF context with SUPI " SUPI_64_FMT "does not exist", supi64);
    return;
  }

  // handle the message in smf_context
  sc.get()->handle_pdu_session_modification_network_requested(itti_msg);
}

//------------------------------------------------------------------------------
evsub_id_t smf_app::handle_event_exposure_subscription(
    std::shared_ptr<itti_sbi_event_exposure_request> msg) {
  Logger::smf_app().info(
      "Handle an Event Exposure Subscription Request from a NF (HTTP version "
      "%d)",
      msg->http_version);

  // Generate a subscription ID Id and store the corresponding information in a
  // map (subscription id, info)
  evsub_id_t evsub_id = generate_ev_subscription_id();

  std::shared_ptr<smf_subscription> ss =
      std::shared_ptr<smf_subscription>(new smf_subscription());
  ss.get()->sub_id = evsub_id;
  if (msg->event_exposure.is_supi_is_set()) {
    supi64_t supi64 = smf_supi_to_u64(msg->event_exposure.get_supi());
    ss.get()->supi  = supi64;
  }
  ss.get()->notif_id  = msg->event_exposure.get_notif_id();
  ss.get()->notif_uri = msg->event_exposure.get_notif_uri();

  std::vector<event_subscription_t> event_subscriptions =
      msg->event_exposure.get_event_subs();
  // store subscription
  for (auto i : event_subscriptions) {
    ss.get()->ev_type = i.smf_event;
    add_event_subscription(evsub_id, i.smf_event, ss);
  }
  return evsub_id;
}

//------------------------------------------------------------------------------
bool smf_app::handle_nf_status_notification(
    std::shared_ptr<itti_sbi_notification_data>& msg,
    oai::smf_server::model::ProblemDetails& problem_details,
    uint8_t& http_code) {
  Logger::smf_app().info(
      "Handle a NF status notification from NRF (HTTP version "
      "%d)",
      msg->http_version);

  data_notification_msg notification_msg = msg.get()->notification_msg;
  std::string event_type;
  notification_msg.get_notification_event_type(event_type);
  if (event_type.compare("NF_REGISTERED") == 0) {
    std::shared_ptr<nf_profile> profile = {};
    notification_msg.get_profile(profile);
    if (profile.get() != nullptr) {
      std::string nf_type = profile.get()->get_nf_type();
      if (nf_type.compare("UPF") == 0) {  // UPF
        upf_info_t upf_info = {};
        std::static_pointer_cast<upf_profile>(profile).get()->get_upf_info(
            upf_info);
        // Verify if the UPF is already exist
        // if not, add to the DB and send Association request
        // UPF N4 ipv4 address
        std::vector<struct in_addr> ipv4_addrs = {};
        profile.get()->get_nf_ipv4_addresses(ipv4_addrs);

        if (ipv4_addrs.size() < 1) {
          Logger::smf_app().debug("No IP Addr found");
          return false;
        }

        bool found = false;
        for (auto node : smf_cfg.upfs) {
          if (node.u1.ipv4_address.s_addr == ipv4_addrs[0].s_addr) {
            found = false;
            break;
          }
        }
        if (!found) {
          // Add a new UPF node
          Logger::smf_app().debug(
              "Add a new UPF node, Ipv4 Addr %s", inet_ntoa(ipv4_addrs[0]));
          pfcp::node_id_t n        = {};
          n.node_id_type           = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
          n.u1.ipv4_address.s_addr = ipv4_addrs[0].s_addr;
          // memcpy(&n.u1.ipv4_address, &ipv4_addrs[0], sizeof(struct in_addr));
          smf_cfg.upfs.push_back(n);
          upf_profile* upf_node_profile =
              dynamic_cast<upf_profile*>(profile.get());
          start_upf_association(n, std::ref(*upf_node_profile));
          // start_upf_association(n,
          // std::static_pointer_cast<upf_profile>(profile));
        } else {
          Logger::smf_app().debug(
              "UPF node already exist (%s)", inet_ntoa(ipv4_addrs[0]));
        }
      }
    } else {
      return false;
    }
  }
  if (event_type.compare("NF_DEREGISTERED") == 0) {
    Logger::smf_app().debug(
        "This event (%s) has not been supported yet!", event_type);
    // TODO: Remove UPF from the list UPFs if received DE-REGISTERED Event
    /*    std::string nf_instance_uri = {};
        notification_msg.get_nf_instance_uri(nf_instance_uri);
        std::vector<std::string> split_result;

        boost::split(
            split_result, nf_instance_uri, boost::is_any_of("/"));
        if (split_result.size() > 0) {
          std::string instance_id = split_result[split_result.size() -1];
          pfcp_associations::get_instance().remove_association(instance_id);
        }
     */
  }

  return true;
}

//------------------------------------------------------------------------------
bool smf_app::is_supi_2_smf_context(const supi64_t& supi) const {
  std::shared_lock lock(m_supi2smf_context);
  return bool{supi2smf_context.count(supi) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<smf_context> smf_app::supi_2_smf_context(
    const supi64_t& supi) const {
  std::shared_lock lock(m_supi2smf_context);
  return supi2smf_context.at(supi);
}

//------------------------------------------------------------------------------
void smf_app::set_supi_2_smf_context(
    const supi64_t& supi, std::shared_ptr<smf_context> sc) {
  std::unique_lock lock(m_supi2smf_context);
  supi2smf_context[supi] = sc;
}

//------------------------------------------------------------------------------
void smf_app::set_scid_2_smf_context(
    const scid_t& id, std::shared_ptr<smf_context_ref> scf) {
  std::unique_lock lock(m_scid2smf_context);
  scid2smf_context[id] = scf;
}

//------------------------------------------------------------------------------
std::shared_ptr<smf_context_ref> smf_app::scid_2_smf_context(
    const scid_t& scid) const {
  std::shared_lock lock(m_scid2smf_context);
  return scid2smf_context.at(scid);
}

//------------------------------------------------------------------------------
bool smf_app::is_scid_2_smf_context(const scid_t& scid) const {
  std::shared_lock lock(m_scid2smf_context);
  return bool{scid2smf_context.count(scid) > 0};
}

//------------------------------------------------------------------------------
bool smf_app::is_scid_2_smf_context(
    const supi64_t& supi, const std::string& dnn, const snssai_t& snssai,
    const pdu_session_id_t& pid) const {
  std::shared_lock lock(m_scid2smf_context);
  for (auto it : scid2smf_context) {
    supi64_t supi64 = smf_supi_to_u64(it.second->supi);
    if ((supi64 == supi) and (it.second->dnn.compare(dnn) == 0) and
        (it.second->nssai == snssai) and (it.second->pdu_session_id == pid))
      return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool smf_app::scid_2_smf_context(
    const scid_t& scid, std::shared_ptr<smf_context_ref>& scf) const {
  std::shared_lock lock(m_scid2smf_context);
  if (scid2smf_context.count(scid) > 0) {
    scf = scid2smf_context.at(scid);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool smf_app::use_local_configuration_subscription_data(
    const std::string& dnn_selection_mode) {
  // TODO: should be implemented
  return smf_cfg.use_local_subscription_info;
}

//------------------------------------------------------------------------------
bool smf_app::is_supi_dnn_snssai_subscription_data(
    const supi_t& supi, const std::string& dnn, const snssai_t& snssai) const {
  // TODO: should be implemented
  return false;  // Session Management Subscription from UDM isn't available
}

//------------------------------------------------------------------------------
bool smf_app::is_create_sm_context_request_valid() const {
  // TODO: should be implemented
  return true;
}

//---------------------------------------------------------------------------------------------
void smf_app::convert_string_2_hex(
    const std::string& input_str, std::string& output_str) {
  Logger::smf_app().debug("Convert string to Hex");
  unsigned char* data = (unsigned char*) malloc(input_str.length() + 1);
  memset(data, 0, input_str.length() + 1);
  memcpy((void*) data, (void*) input_str.c_str(), input_str.length());

#if DEBUG_IS_ON
  Logger::smf_app().debug("Input: ");
  for (int i = 0; i < input_str.length(); i++) {
    printf("%02x ", data[i]);
  }
  printf("\n");
#endif
  char* datahex = (char*) malloc(input_str.length() * 2 + 1);
  memset(datahex, 0, input_str.length() * 2 + 1);

  for (int i = 0; i < input_str.length(); i++)
    sprintf(datahex + i * 2, "%02x", data[i]);

  output_str = reinterpret_cast<char*>(datahex);
  Logger::smf_app().debug("Output: \n %s ", output_str.c_str());

  // free memory
  free_wrapper((void**) &data);
  free_wrapper((void**) &datahex);
}

//---------------------------------------------------------------------------------------------
void smf_app::update_pdu_session_status(
    const scid_t& scid, const pdu_session_status_e& status) {
  Logger::smf_app().info("Update PDU Session Status");

  // get the smf context
  std::shared_ptr<smf_context_ref> scf = {};

  if (is_scid_2_smf_context(scid)) {
    scf = scid_2_smf_context(scid);
  } else {
    Logger::smf_app().warn(
        "Context associated with this id " SCID_FMT " does not exit!", scid);
  }

  supi_t supi                     = scf.get()->supi;
  supi64_t supi64                 = smf_supi_to_u64(supi);
  pdu_session_id_t pdu_session_id = scf.get()->pdu_session_id;

  std::shared_ptr<smf_context> sc = {};

  if (is_supi_2_smf_context(supi64)) {
    sc = supi_2_smf_context(supi64);
    Logger::smf_app().debug(
        "Retrieve SMF context with SUPI " SUPI_64_FMT "", supi64);
  } else {
    Logger::smf_app().warn(
        "Could not retrieve the corresponding SMF context with "
        "Supi " SUPI_64_FMT "!",
        supi64);
  }

  // get dnn context
  std::shared_ptr<dnn_context> sd = {};

  if (!sc.get()->find_dnn_context(scf.get()->nssai, scf.get()->dnn, sd)) {
    if (nullptr == sd.get()) {
      Logger::smf_app().warn(
          "Could not retrieve the corresponding DNN context!");
    }
  }
  // get smd_pdu_session
  std::shared_ptr<smf_pdu_session> sp = {};
  bool find_pdn = sd.get()->find_pdu_session(pdu_session_id, sp);

  if (nullptr == sp.get()) {
    Logger::smf_app().warn(
        "Could not retrieve the corresponding SMF PDU Session context!");
  }
  sp.get()->set_pdu_session_status(status);
  Logger::smf_app().info(
      "Set PDU Session Status to %s",
      pdu_session_status_e2str.at(static_cast<int>(status)).c_str());
}

//---------------------------------------------------------------------------------------------
void smf_app::update_pdu_session_upCnx_state(
    const scid_t& scid, const upCnx_state_e& state) {
  Logger::smf_app().info("Update UpCnx_State");

  // get the smf context
  std::shared_ptr<smf_context_ref> scf = {};

  if (is_scid_2_smf_context(scid)) {
    scf = scid_2_smf_context(scid);
  } else {
    Logger::smf_app().warn(
        "Context associated with this id " SCID_FMT " does not exit!", scid);
  }

  supi_t supi                     = scf.get()->supi;
  supi64_t supi64                 = smf_supi_to_u64(supi);
  pdu_session_id_t pdu_session_id = scf.get()->pdu_session_id;

  std::shared_ptr<smf_context> sc = {};

  if (is_supi_2_smf_context(supi64)) {
    sc = supi_2_smf_context(supi64);
    Logger::smf_app().debug(
        "Retrieve SMF context with SUPI " SUPI_64_FMT "", supi64);
  } else {
    Logger::smf_app().warn(
        "Could not retrieve the corresponding SMF context with "
        "Supi " SUPI_64_FMT "!",
        supi64);
  }

  // get dnn context
  std::shared_ptr<dnn_context> sd = {};

  if (!sc.get()->find_dnn_context(scf.get()->nssai, scf.get()->dnn, sd)) {
    if (nullptr == sd.get()) {
      Logger::smf_app().warn(
          "Could not retrieve the corresponding DNN context!");
    }
  }
  // get smd_pdu_session
  std::shared_ptr<smf_pdu_session> sp = {};
  bool find_pdn = sd.get()->find_pdu_session(pdu_session_id, sp);

  if (nullptr == sp.get()) {
    Logger::smf_app().warn(
        "Could not retrieve the corresponding SMF PDU Session context!");
  }
  sp.get()->set_upCnx_state(state);
  Logger::smf_app().info(
      "Set PDU Session UpCnxState to %s",
      upCnx_state_e2str.at(static_cast<int>(state)).c_str());
}
//---------------------------------------------------------------------------------------------
void smf_app::timer_t3591_timeout(timer_id_t timer_id, uint64_t arg2_user) {
  // TODO: send session modification request again...
}

//---------------------------------------------------------------------------------------------
void smf_app::timer_nrf_heartbeat_timeout(
    timer_id_t timer_id, uint64_t arg2_user) {
  Logger::smf_app().debug("Send ITTI msg to N11 task to trigger NRF Heartbeat");

  std::shared_ptr<itti_n11_update_nf_instance_request> itti_msg =
      std::make_shared<itti_n11_update_nf_instance_request>(
          TASK_SMF_APP, TASK_SMF_SBI);

  oai::smf_server::model::PatchItem patch_item = {};
  //{"op":"replace","path":"/nfStatus", "value": "REGISTERED"}
  patch_item.setOp("replace");
  patch_item.setPath("/nfStatus");
  patch_item.setValue("REGISTERED");
  itti_msg->patch_items.push_back(patch_item);
  itti_msg->smf_instance_id = smf_instance_id;

  int ret = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_SBI",
        itti_msg->get_msg_name());
  } else {
    Logger::smf_app().debug(
        "Set a timer to the next Heart-beat (%d)",
        nf_instance_profile.get_nf_heartBeat_timer());
    timer_nrf_heartbeat = itti_inst->timer_setup(
        nf_instance_profile.get_nf_heartBeat_timer(), 0, TASK_SMF_APP,
        TASK_SMF_APP_TIMEOUT_NRF_HEARTBEAT,
        0);  // TODO arg2_user
  }
}

//---------------------------------------------------------------------------------------------
void smf_app::timer_nrf_deregistration(
    timer_id_t timer_id, uint64_t arg2_user) {
  Logger::smf_app().debug(
      "Send ITTI msg to N11 task to trigger NRF Deregistratino");
  trigger_nf_deregistration();
}

//---------------------------------------------------------------------------------------------
n2_sm_info_type_e smf_app::n2_sm_info_type_str2e(
    const std::string& n2_info_type) const {
  std::size_t number_of_types = n2_sm_info_type_e2str.size();
  for (auto i = 0; i < number_of_types; ++i) {
    if (n2_info_type.compare(n2_sm_info_type_e2str.at(i)) == 0) {
      return static_cast<n2_sm_info_type_e>(i);
    }
  }
  return n2_sm_info_type_e::UNKNOWN_TYPE;
}

//---------------------------------------------------------------------------------------------
bool smf_app::get_session_management_subscription_data(
    const supi64_t& supi, const std::string& dnn, const snssai_t& snssai,
    std::shared_ptr<session_management_subscription> subscription) {
  Logger::smf_app().debug(
      "Get Session Management Subscription from configuration file");

  for (int i = 0; i < smf_cfg.num_session_management_subscription; i++) {
    if ((0 == dnn.compare(smf_cfg.session_management_subscription[i].dnn)) and
        (snssai.sST ==
         smf_cfg.session_management_subscription[i].single_nssai.sST) and
        (0 ==
         snssai.sD.compare(
             smf_cfg.session_management_subscription[i].single_nssai.sD))) {
      std::shared_ptr<dnn_configuration_t> dnn_configuration =
          std::make_shared<dnn_configuration_t>();

      // PDU Session Type
      pdu_session_type_t pdu_session_type(
          pdu_session_type_e::PDU_SESSION_TYPE_E_IPV4);
      Logger::smf_app().debug(
          "Default session type %s",
          smf_cfg.session_management_subscription[i].session_type.c_str());

      std::string session_type =
          smf_cfg.session_management_subscription[i].session_type;
      if (boost::iequals(session_type, "IPv4")) {
        pdu_session_type.pdu_session_type =
            pdu_session_type_e::PDU_SESSION_TYPE_E_IPV4;
      } else if (boost::iequals(session_type, "IPv6")) {
        pdu_session_type.pdu_session_type =
            pdu_session_type_e::PDU_SESSION_TYPE_E_IPV6;
      } else if (boost::iequals(session_type, "IPv4v6")) {
        pdu_session_type.pdu_session_type =
            pdu_session_type_e::PDU_SESSION_TYPE_E_IPV4V6;
      }

      dnn_configuration->pdu_session_types.default_session_type =
          pdu_session_type;

      // Ssc_Mode
      dnn_configuration->ssc_modes.default_ssc_mode.ssc_mode =
          smf_cfg.session_management_subscription[i].ssc_mode;

      // 5gQosProfile
      dnn_configuration->_5g_qos_profile._5qi =
          smf_cfg.session_management_subscription[i].default_qos._5qi;
      dnn_configuration->_5g_qos_profile.arp.priority_level =
          smf_cfg.session_management_subscription[i]
              .default_qos.arp.priority_level;
      dnn_configuration->_5g_qos_profile.arp.preempt_cap =
          smf_cfg.session_management_subscription[i]
              .default_qos.arp.preempt_cap;
      dnn_configuration->_5g_qos_profile.arp.preempt_vuln =
          smf_cfg.session_management_subscription[i]
              .default_qos.arp.preempt_vuln;
      dnn_configuration->_5g_qos_profile.priority_level =
          smf_cfg.session_management_subscription[i].default_qos.priority_level;

      // session_ambr
      dnn_configuration->session_ambr.uplink =
          smf_cfg.session_management_subscription[i].session_ambr.uplink;
      dnn_configuration->session_ambr.downlink =
          smf_cfg.session_management_subscription[i].session_ambr.downlink;
      Logger::smf_app().debug(
          "Session AMBR Uplink %s, Downlink %s",
          dnn_configuration->session_ambr.uplink.c_str(),
          dnn_configuration->session_ambr.downlink.c_str());

      subscription->insert_dnn_configuration(dnn, dnn_configuration);
      return true;
    }
  }
  return false;
}

//---------------------------------------------------------------------------------------------
void smf_app::add_promise(
    uint32_t id,
    boost::shared_ptr<boost::promise<pdu_session_create_sm_context_response>>&
        p) {
  std::unique_lock lock(m_sm_context_create_promises);
  sm_context_create_promises.emplace(id, p);
}

//---------------------------------------------------------------------------------------------
void smf_app::add_promise(
    uint32_t id,
    boost::shared_ptr<boost::promise<pdu_session_update_sm_context_response>>&
        p) {
  std::unique_lock lock(m_sm_context_update_promises);
  sm_context_update_promises.emplace(id, p);
}

//---------------------------------------------------------------------------------------------
void smf_app::add_promise(
    uint32_t id,
    boost::shared_ptr<boost::promise<pdu_session_release_sm_context_response>>&
        p) {
  std::unique_lock lock(m_sm_context_release_promises);
  sm_context_release_promises.emplace(id, p);
}

//---------------------------------------------------------------------------------------------
void smf_app::trigger_create_context_error_response(
    const uint32_t& http_code, const uint8_t& cause,
    const std::string& n1_sm_msg, uint32_t& promise_id) {
  Logger::smf_app().debug(
      "Send ITTI msg to SMF APP to trigger the response of Server");

  oai::smf_server::model::SmContextCreateError sm_context = {};
  oai::smf_server::model::ProblemDetails problem_details  = {};
  oai::smf_server::model::RefToBinaryData refToBinaryData = {};
  Logger::smf_app().warn("Create SmContextCreateError");
  problem_details.setCause(pdu_session_application_error_e2str.at(cause));
  sm_context.setError(problem_details);
  refToBinaryData.setContentId(N1_SM_CONTENT_ID);
  sm_context.setN1SmMsg(refToBinaryData);

  std::shared_ptr<itti_n11_create_sm_context_response> itti_msg =
      std::make_shared<itti_n11_create_sm_context_response>(
          TASK_SMF_SBI, TASK_SMF_APP, promise_id);
  pdu_session_create_sm_context_response sm_context_response = {};
  nlohmann::json json_data                                   = {};
  to_json(json_data, sm_context);
  sm_context_response.set_json_data(json_data);
  sm_context_response.set_json_format("application/problem+json");
  sm_context_response.set_n1_sm_message(n1_sm_msg);
  sm_context_response.set_http_code(http_code);
  itti_msg->res = sm_context_response;
  int ret       = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_APP",
        itti_msg->get_msg_name());
  }
}

//---------------------------------------------------------------------------------------------
void smf_app::trigger_update_context_error_response(
    const uint32_t& http_code, const uint8_t& cause, uint32_t& promise_id) {
  Logger::smf_app().debug(
      "Send ITTI msg to SMF APP to trigger the response of API Server");

  oai::smf_server::model::SmContextUpdateError smContextUpdateError = {};
  oai::smf_server::model::ProblemDetails problem_details            = {};
  problem_details.setCause(pdu_session_application_error_e2str.at(cause));
  smContextUpdateError.setError(problem_details);

  std::shared_ptr<itti_n11_update_sm_context_response> itti_msg =
      std::make_shared<itti_n11_update_sm_context_response>(
          TASK_SMF_SBI, TASK_SMF_APP, promise_id);
  pdu_session_update_sm_context_response sm_context_response = {};
  nlohmann::json json_data                                   = {};
  to_json(json_data, smContextUpdateError);
  sm_context_response.set_json_data(json_data);
  sm_context_response.set_json_format("application/problem+json");
  sm_context_response.set_http_code(http_code);
  itti_msg->res = sm_context_response;
  int ret       = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_APP",
        itti_msg->get_msg_name());
  }
}

//---------------------------------------------------------------------------------------------
void smf_app::trigger_update_context_error_response(
    const uint32_t& http_code, const uint8_t& cause,
    const std::string& n1_sm_msg, uint32_t& promise_id) {
  Logger::smf_app().debug(
      "Send ITTI msg to SMF APP to trigger the response of HTTP Server");

  oai::smf_server::model::SmContextUpdateError smContextUpdateError = {};
  oai::smf_server::model::ProblemDetails problem_details            = {};
  problem_details.setCause(pdu_session_application_error_e2str.at(cause));
  smContextUpdateError.setError(problem_details);

  std::shared_ptr<itti_n11_update_sm_context_response> itti_msg =
      std::make_shared<itti_n11_update_sm_context_response>(
          TASK_SMF_SBI, TASK_SMF_APP, promise_id);
  pdu_session_update_sm_context_response sm_context_response = {};
  nlohmann::json json_data                                   = {};
  to_json(json_data, smContextUpdateError);
  sm_context_response.set_json_data(json_data);
  sm_context_response.set_json_format("application/problem+json");
  sm_context_response.set_n1_sm_message(n1_sm_msg);
  sm_context_response.set_http_code(http_code);
  itti_msg->res = sm_context_response;
  int ret       = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_APP",
        itti_msg->get_msg_name());
  }
}

//---------------------------------------------------------------------------------------------
void smf_app::trigger_http_response(
    const uint32_t& http_code, uint32_t& promise_id, uint8_t msg_type) {
  Logger::smf_app().debug(
      "Send ITTI msg to SMF APP to trigger the response of HTTP Server");
  switch (msg_type) {
    case N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE: {
      std::shared_ptr<itti_n11_release_sm_context_response> itti_msg =
          std::make_shared<itti_n11_release_sm_context_response>(
              TASK_SMF_SBI, TASK_SMF_APP, promise_id);
      pdu_session_release_sm_context_response sm_context_response = {};
      sm_context_response.set_http_code(http_code);
      itti_msg->res = sm_context_response;
      int ret       = itti_inst->send_msg(itti_msg);
      if (RETURNok != ret) {
        Logger::smf_app().error(
            "Could not send ITTI message %s to task TASK_SMF_APP",
            itti_msg->get_msg_name());
      }
    } break;

    case N11_SESSION_CREATE_SM_CONTEXT_RESPONSE: {
      std::shared_ptr<itti_n11_create_sm_context_response> itti_msg =
          std::make_shared<itti_n11_create_sm_context_response>(
              TASK_SMF_SBI, TASK_SMF_APP, promise_id);
      pdu_session_create_sm_context_response sm_context_response = {};
      sm_context_response.set_http_code(http_code);
      itti_msg->res = sm_context_response;
      int ret       = itti_inst->send_msg(itti_msg);
      if (RETURNok != ret) {
        Logger::smf_app().error(
            "Could not send ITTI message %s to task TASK_SMF_APP",
            itti_msg->get_msg_name());
      }
    } break;

    case N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE: {
      std::shared_ptr<itti_n11_update_sm_context_response> itti_msg =
          std::make_shared<itti_n11_update_sm_context_response>(
              TASK_SMF_SBI, TASK_SMF_APP, promise_id);
      pdu_session_update_sm_context_response sm_context_response = {};
      sm_context_response.set_http_code(http_code);
      itti_msg->res = sm_context_response;
      int ret       = itti_inst->send_msg(itti_msg);
      if (RETURNok != ret) {
        Logger::smf_app().error(
            "Could not send ITTI message %s to task TASK_SMF_APP",
            itti_msg->get_msg_name());
      }
    } break;

    default: {
      Logger::smf_app().debug("Unknown message type %d", msg_type);
    }
  }
}

//---------------------------------------------------------------------------------------------
void smf_app::add_event_subscription(
    evsub_id_t sub_id, smf_event_t ev, std::shared_ptr<smf_subscription> ss) {
  Logger::smf_app().debug(
      "Add an Event subscription (Sub ID %d, Event %d)", sub_id, (uint8_t) ev);
  std::unique_lock lock(m_smf_event_subscriptions);
  smf_event_subscriptions.emplace(std::make_pair(sub_id, ev), ss);
}

//---------------------------------------------------------------------------------------------
void smf_app::get_ee_subscriptions(
    smf_event_t ev,
    std::vector<std::shared_ptr<smf_subscription>>& subscriptions) {
  for (auto const& i : smf_event_subscriptions) {
    if ((uint8_t) std::get<1>(i.first) == (uint8_t) ev) {
      Logger::smf_app().debug(
          "Found an event subscription (Event ID %d, Event %d)",
          (uint8_t) std::get<0>(i.first), (uint8_t) ev);
      subscriptions.push_back(i.second);
    }
  }
}

//---------------------------------------------------------------------------------------------
void smf_app::get_ee_subscriptions(
    evsub_id_t sub_id,
    std::vector<std::shared_ptr<smf_subscription>>& subscriptions) {
  for (auto const& i : smf_event_subscriptions) {
    if (i.first.first == sub_id) {
      subscriptions.push_back(i.second);
    }
  }
}

//---------------------------------------------------------------------------------------------
void smf_app::get_ee_subscriptions(
    smf_event_t ev, supi64_t supi, pdu_session_id_t pdu_session_id,
    std::vector<std::shared_ptr<smf_subscription>>& subscriptions) {
  for (auto const& i : smf_event_subscriptions) {
    if ((i.first.second == ev) && (i.second->supi == supi) &&
        (i.second->pdu_session_id == pdu_session_id)) {
      subscriptions.push_back(i.second);
    }
  }
}

//---------------------------------------------------------------------------------------------
void smf_app::generate_smf_profile() {
  // TODO: remove hardcoded values
  // generate UUID
  generate_uuid();
  nf_instance_profile.set_nf_instance_id(smf_instance_id);
  nf_instance_profile.set_nf_instance_name("OAI-SMF");
  nf_instance_profile.set_nf_type("SMF");
  nf_instance_profile.set_nf_status("REGISTERED");
  nf_instance_profile.set_nf_heartBeat_timer(50);
  nf_instance_profile.set_nf_priority(1);
  nf_instance_profile.set_nf_capacity(100);
  nf_instance_profile.add_nf_ipv4_addresses(smf_cfg.sbi.addr4);

  // NF services
  nf_service_t nf_service        = {};
  nf_service.service_instance_id = "nsmf-pdusession";
  nf_service.service_name        = "nsmf-pdusession";
  nf_service_version_t version   = {};
  version.api_version_in_uri     = "v1";
  version.api_full_version       = "1.0.0";  // TODO: to be updated
  nf_service.versions.push_back(version);
  nf_service.scheme            = "http";
  nf_service.nf_service_status = "REGISTERED";
  // IP Endpoint
  ip_endpoint_t endpoint = {};
  std::vector<struct in_addr> addrs;
  nf_instance_profile.get_nf_ipv4_addresses(addrs);
  endpoint.ipv4_address = addrs[0];  // TODO: use first IP ADDR for now
  endpoint.transport    = "TCP";
  endpoint.port         = smf_cfg.sbi.port;
  nf_service.ip_endpoints.push_back(endpoint);

  nf_instance_profile.add_nf_service(nf_service);

  // TODO: custom info

  int i = 0;
  for (auto sms : smf_cfg.session_management_subscription) {
    if (i < smf_cfg.num_session_management_subscription)
      i++;
    else
      break;

    // SNSSAIS
    snssai_t snssai = {};
    snssai.sD       = sms.single_nssai.sD;
    snssai.sST      = sms.single_nssai.sST;
    // Verify if this SNSSAI exist
    std::vector<snssai_t> ss = {};
    nf_instance_profile.get_nf_snssais(ss);
    bool found = false;
    for (auto it : ss) {
      if ((it.sD == snssai.sD) and (it.sST == snssai.sST)) {
        found = true;
        break;
      }
    }
    if (!found) nf_instance_profile.add_snssai(snssai);

    // SMF info
    dnn_smf_info_item_t dnn_item         = {.dnn = sms.dnn};
    snssai_smf_info_item_t smf_info_item = {};
    smf_info_item.dnn_smf_info_list.push_back(dnn_item);
    smf_info_item.snssai.sD  = sms.single_nssai.sD;
    smf_info_item.snssai.sST = sms.single_nssai.sST;
    nf_instance_profile.add_smf_info_item(smf_info_item);
  }

  // Display the profile
  nf_instance_profile.display();
}

//---------------------------------------------------------------------------------------------
void smf_app::register_to_nrf() {
  // Create a NF profile to this instance
  generate_smf_profile();
  // Send request to N11 to send NF registration to NRF
  trigger_nf_registration_request();
}

//------------------------------------------------------------------------------
void smf_app::generate_uuid() {
  smf_instance_id = to_string(boost::uuids::random_generator()());
}

//------------------------------------------------------------------------------
void smf_app::trigger_nf_registration_request() {
  Logger::smf_app().debug(
      "Send ITTI msg to N11 task to trigger the registration request to NRF");

  std::shared_ptr<itti_n11_register_nf_instance_request> itti_msg =
      std::make_shared<itti_n11_register_nf_instance_request>(
          TASK_SMF_APP, TASK_SMF_SBI);
  itti_msg->profile = nf_instance_profile;
  int ret           = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void smf_app::trigger_nf_deregistration() {
  Logger::smf_app().debug(
      "Send ITTI msg to N11 task to trigger the deregistration request to NRF");

  std::shared_ptr<itti_n11_deregister_nf_instance> itti_msg =
      std::make_shared<itti_n11_deregister_nf_instance>(
          TASK_SMF_APP, TASK_SMF_SBI);
  itti_msg->smf_instance_id = smf_instance_id;
  int ret                   = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void smf_app::trigger_upf_status_notification_subscribe() {
  Logger::smf_app().debug(
      "Send ITTI msg to N11 task to subscribe to UPF status notification "
      "from NRF");

  std::shared_ptr<itti_n11_subscribe_upf_status_notify> itti_msg =
      std::make_shared<itti_n11_subscribe_upf_status_notify>(
          TASK_SMF_APP, TASK_SMF_SBI);

  nlohmann::json json_data = {};
  // TODO: remove hardcoded values
  json_data["nfStatusNotificationUri"] =
      std::string(inet_ntoa(*((struct in_addr*) &smf_cfg.sbi.addr4))) + ":" +
      std::to_string(smf_cfg.sbi.port) + "/nsmf-nfstatus-notify/" +
      smf_cfg.sbi_api_version + "/subscriptions";

  json_data["subscrCond"]["NfTypeCond"]["nfType"] = "UPF";
  json_data["reqNotifEvents"]                     = nlohmann::json::array();
  json_data["reqNotifEvents"].push_back("NF_REGISTERED");
  json_data["reqNotifEvents"].push_back("NF_DEREGISTERED");
  json_data["validityTime"] = "20210531T235959";

  std::string url =
      std::string(inet_ntoa(*((struct in_addr*) &smf_cfg.nrf_addr.ipv4_addr))) +
      ":" + std::to_string(smf_cfg.nrf_addr.port) + NNRF_NFM_BASE +
      smf_cfg.nrf_addr.api_version + NNRF_NF_STATUS_SUBSCRIBE_URL;

  itti_msg->url       = url;
  itti_msg->json_data = json_data;
  int ret             = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_SBI",
        itti_msg->get_msg_name());
  }
}
