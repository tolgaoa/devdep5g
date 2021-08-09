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

/*! \file amf_app.cpp
 \brief
 \author  Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#include "amf_app.hpp"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "DLNASTransport.hpp"
#include "amf_config.hpp"
#include "amf_n1.hpp"
#include "amf_n11.hpp"
#include "amf_n2.hpp"
#include "amf_statistics.hpp"
#include "itti.hpp"
#include "ngap_app.hpp"

using namespace ngap;
using namespace nas;
using namespace amf_application;
using namespace config;

extern void print_buffer(
    const std::string app, const std::string commit, uint8_t* buf, int len);
extern amf_app* amf_app_inst;
extern itti_mw* itti_inst;
amf_n2* amf_n2_inst   = nullptr;
amf_n1* amf_n1_inst   = nullptr;
amf_n11* amf_n11_inst = nullptr;
extern amf_config amf_cfg;
extern statistics stacs;

void amf_app_task(void*);
uint32_t golbal_tmsi = 1;

//------------------------------------------------------------------------------
amf_app::amf_app(const amf_config& amf_cfg) {
  Logger::amf_app().startup("Creating AMF application functionality layer");
  if (itti_inst->create_task(TASK_AMF_APP, amf_app_task, nullptr)) {
    Logger::amf_app().error("Cannot create task TASK_AMF_APP");
    throw std::runtime_error("Cannot create task TASK_AMF_APP");
  }

  try {
    amf_n1_inst = new amf_n1();
    amf_n2_inst =
        new amf_n2(std::string(inet_ntoa(amf_cfg.n2.addr4)), amf_cfg.n2.port);
    amf_n11_inst = new amf_n11();
  } catch (std::exception& e) {
    Logger::amf_app().error("Cannot create AMF APP: %s", e.what());
    throw;
  }

  // Register to NRF
  if (amf_cfg.enable_nf_registration) register_to_nrf();

  timer_id_t tid = itti_inst->timer_setup(
      amf_cfg.statistics_interval, 0, TASK_AMF_APP,
      TASK_AMF_APP_PERIODIC_STATISTICS, 0);
  Logger::amf_app().startup("Started timer (%d)", tid);
}

//------------------------------------------------------------------------------
void amf_app::allRegistredModulesInit(const amf_modules& modules) {
  Logger::amf_app().info("Initiating all registered modules");
}

//------------------------------------------------------------------------------
void amf_app_task(void*) {
  const task_id_t task_id = TASK_AMF_APP;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    timer_id_t tid;
    switch (msg->msg_type) {
      case NAS_SIG_ESTAB_REQ: {
        Logger::amf_app().debug("Received NAS_SIG_ESTAB_REQ");
        itti_nas_signalling_establishment_request* m =
            dynamic_cast<itti_nas_signalling_establishment_request*>(msg);
        amf_app_inst->handle_itti_message(ref(*m));
      } break;

      case N1N2_MESSAGE_TRANSFER_REQ: {
        Logger::amf_app().debug("Received N1N2_MESSAGE_TRANSFER_REQ");
        itti_n1n2_message_transfer_request* m =
            dynamic_cast<itti_n1n2_message_transfer_request*>(msg);
        amf_app_inst->handle_itti_message(ref(*m));
      } break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          switch (to->arg1_user) {
            case TASK_AMF_APP_PERIODIC_STATISTICS:
              tid = itti_inst->timer_setup(
                  amf_cfg.statistics_interval, 0, TASK_AMF_APP,
                  TASK_AMF_APP_PERIODIC_STATISTICS, 0);
              stacs.display();
              break;
            default:
              Logger::amf_app().info(
                  "No handler for timer(%d) with arg1_user(%d) ", to->timer_id,
                  to->arg1_user);
          }
        }
        break;
      default:
        Logger::amf_app().info("no handler for msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
long amf_app::generate_amf_ue_ngap_id() {
  long tmp = 0;
  tmp      = __sync_fetch_and_add(&amf_app_ue_ngap_id_generator, 1);
  return tmp & 0xffffffffff;
}

//------------------------------------------------------------------------------
bool amf_app::is_amf_ue_id_2_ue_context(const long& amf_ue_ngap_id) const {
  std::shared_lock lock(m_amf_ue_ngap_id2ue_ctx);
  return bool{amf_ue_ngap_id2ue_ctx.count(amf_ue_ngap_id) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<ue_context> amf_app::amf_ue_id_2_ue_context(
    const long& amf_ue_ngap_id) const {
  std::shared_lock lock(m_amf_ue_ngap_id2ue_ctx);
  return amf_ue_ngap_id2ue_ctx.at(amf_ue_ngap_id);
}

//------------------------------------------------------------------------------
void amf_app::set_amf_ue_ngap_id_2_ue_context(
    const long& amf_ue_ngap_id, std::shared_ptr<ue_context> uc) {
  std::unique_lock lock(m_amf_ue_ngap_id2ue_ctx);
  amf_ue_ngap_id2ue_ctx[amf_ue_ngap_id] = uc;
}

//------------------------------------------------------------------------------
bool amf_app::is_ran_amf_id_2_ue_context(const string& ue_context_key) const {
  std::shared_lock lock(m_ue_ctx_key);
  return bool{ue_ctx_key.count(ue_context_key) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<ue_context> amf_app::ran_amf_id_2_ue_context(
    const string& ue_context_key) const {
  std::shared_lock lock(m_ue_ctx_key);
  return ue_ctx_key.at(ue_context_key);
}

//------------------------------------------------------------------------------
void amf_app::set_ran_amf_id_2_ue_context(
    const string& ue_context_key, std::shared_ptr<ue_context> uc) {
  std::unique_lock lock(m_ue_ctx_key);
  ue_ctx_key[ue_context_key] = uc;
}

//------------------------------------------------------------------------------
bool amf_app::is_supi_2_ue_context(const string& supi) const {
  std::shared_lock lock(m_supi2ue_ctx);
  return bool{supi2ue_ctx.count(supi) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<ue_context> amf_app::supi_2_ue_context(
    const string& supi) const {
  std::shared_lock lock(m_supi2ue_ctx);
  return supi2ue_ctx.at(supi);
}

//------------------------------------------------------------------------------
void amf_app::set_supi_2_ue_context(
    const string& supi, std::shared_ptr<ue_context>& uc) {
  std::unique_lock lock(m_supi2ue_ctx);
  supi2ue_ctx[supi] = uc;
}

bool amf_app::find_pdu_session_context(
    const string& supi, const std::uint8_t pdu_session_id,
    std::shared_ptr<pdu_session_context>& psc) {
  if (!is_supi_2_ue_context(supi)) return false;
  std::shared_ptr<ue_context> uc = {};
  uc                             = supi_2_ue_context(supi);
  if (!uc.get()->find_pdu_session_context(pdu_session_id, psc)) return false;
  return true;
}

bool amf_app::get_pdu_sessions_context(
    const string& supi,
    std::vector<std::shared_ptr<pdu_session_context>>& sessions_ctx) {
  if (!is_supi_2_ue_context(supi)) return false;
  std::shared_ptr<ue_context> uc = {};
  uc                             = supi_2_ue_context(supi);
  if (!uc.get()->get_pdu_sessions_context(sessions_ctx)) return false;
  return true;
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_n1n2_message_transfer_request& itti_msg) {
  // Encode DL NAS TRANSPORT message(NAS message)
  DLNASTransport* dl = new DLNASTransport();
  dl->setHeader(PLAIN_5GS_MSG);
  dl->setPayload_Container_Type(N1_SM_INFORMATION);
  dl->setPayload_Container(
      (uint8_t*) bdata(itti_msg.n1sm), blength(itti_msg.n1sm));
  dl->setPDUSessionId(itti_msg.pdu_session_id);

  uint8_t nas[1024];
  int encoded_size = dl->encode2buffer(nas, 1024);
  print_buffer("amf_app", "n1n2 transfer", nas, encoded_size);
  bstring dl_nas = blk2bstr(nas, encoded_size);

  itti_downlink_nas_transfer* dl_msg =
      new itti_downlink_nas_transfer(TASK_AMF_APP, TASK_AMF_N1);
  dl_msg->dl_nas = dl_nas;
  if (!itti_msg.is_n2sm_set) {
    dl_msg->is_n2sm_set = false;
  } else {
    dl_msg->n2sm           = itti_msg.n2sm;
    dl_msg->pdu_session_id = itti_msg.pdu_session_id;
    dl_msg->is_n2sm_set    = true;
    dl_msg->n2sm_info_type = itti_msg.n2sm_info_type;
  }
  dl_msg->amf_ue_ngap_id = amf_n1_inst->supi2amfId.at(itti_msg.supi);
  dl_msg->ran_ue_ngap_id = amf_n1_inst->supi2ranId.at(itti_msg.supi);
  std::shared_ptr<itti_downlink_nas_transfer> i =
      std::shared_ptr<itti_downlink_nas_transfer>(dl_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N1",
        i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::handle_itti_message(
    itti_nas_signalling_establishment_request& itti_msg) {
  // 1. Generate amf_ue_ngap_id
  // 2. Create UE Context and store related information information
  // 3. Send nas-pdu to task_amf_n1

  long amf_ue_ngap_id = 0;
  std::shared_ptr<ue_context> uc;

  // Check UE Context with 5g-s-tmsi
  if ((amf_ue_ngap_id = itti_msg.amf_ue_ngap_id) == -1) {
    amf_ue_ngap_id = generate_amf_ue_ngap_id();
  }

  string ue_context_key = "app_ue_ranid_" + to_string(itti_msg.ran_ue_ngap_id) +
                          ":amfid_" + to_string(amf_ue_ngap_id);
  if (!is_ran_amf_id_2_ue_context(ue_context_key)) {
    Logger::amf_app().debug(
        "No existing UE Context, Create a new one with ran_amf_id %s",
        ue_context_key.c_str());
    uc = std::shared_ptr<ue_context>(new ue_context());
    set_ran_amf_id_2_ue_context(ue_context_key, uc);
  }

  // Update AMF UE NGAP ID
  std::shared_ptr<ue_ngap_context> unc = {};
  if (!amf_n2_inst->is_ran_ue_id_2_ue_ngap_context(itti_msg.ran_ue_ngap_id)) {
    Logger::amf_n1().error(
        "Could not find UE NGAP Context with ran_ue_ngap_id (0x%x)",
        itti_msg.ran_ue_ngap_id);
  } else {
    unc = amf_n2_inst->ran_ue_id_2_ue_ngap_context(itti_msg.ran_ue_ngap_id);
    unc.get()->amf_ue_ngap_id = amf_ue_ngap_id;
  }

  if (uc.get() == nullptr) {
    Logger::amf_app().error(
        "Failed to create ue_context with ran_amf_id %s",
        ue_context_key.c_str());
  } else {
    uc.get()->cgi = itti_msg.cgi;
    uc.get()->tai = itti_msg.tai;
    if (itti_msg.rrc_cause != -1)
      uc.get()->rrc_estb_cause =
          (e_Ngap_RRCEstablishmentCause) itti_msg.rrc_cause;
    if (itti_msg.ueCtxReq == -1)
      uc.get()->isUeContextRequest = false;
    else
      uc.get()->isUeContextRequest = true;
    uc.get()->ran_ue_ngap_id = itti_msg.ran_ue_ngap_id;
    uc.get()->amf_ue_ngap_id = amf_ue_ngap_id;

    std::string guti;
    bool is_guti_valid = false;
    if (itti_msg.is_5g_s_tmsi_present) {
      guti = itti_msg.tai.mcc + itti_msg.tai.mnc + amf_cfg.guami.regionID +
             itti_msg._5g_s_tmsi;
      is_guti_valid = true;
      Logger::amf_app().debug("Receiving GUTI %s", guti.c_str());
    }

    itti_uplink_nas_data_ind* itti_n1_msg =
        new itti_uplink_nas_data_ind(TASK_AMF_APP, TASK_AMF_N1);
    itti_n1_msg->amf_ue_ngap_id              = amf_ue_ngap_id;
    itti_n1_msg->ran_ue_ngap_id              = itti_msg.ran_ue_ngap_id;
    itti_n1_msg->is_nas_signalling_estab_req = true;
    itti_n1_msg->nas_msg                     = itti_msg.nas_buf;
    itti_n1_msg->mcc                         = itti_msg.tai.mcc;
    itti_n1_msg->mnc                         = itti_msg.tai.mnc;
    itti_n1_msg->is_guti_valid               = is_guti_valid;
    if (is_guti_valid) {
      itti_n1_msg->guti = guti;
    }
    std::shared_ptr<itti_uplink_nas_data_ind> i =
        std::shared_ptr<itti_uplink_nas_data_ind>(itti_n1_msg);
    int ret = itti_inst->send_msg(i);
    if (0 != ret) {
      Logger::amf_app().error(
          "Could not send ITTI message %s to task TASK_AMF_N1",
          i->get_msg_name());
    }
  }
}

// AMF Client response handlers
//------------------------------------------------------------------------------
void amf_app::handle_post_sm_context_response_error_400() {
  Logger::amf_app().error("Post SM context response error 400");
}

//------------------------------------------------------------------------------
bool amf_app::generate_5g_guti(
    uint32_t ranid, long amfid, string& mcc, string& mnc, uint32_t& tmsi) {
  string ue_context_key =
      "app_ue_ranid_" + to_string(ranid) + ":amfid_" + to_string(amfid);
  if (!is_ran_amf_id_2_ue_context(ue_context_key)) {
    Logger::amf_app().error(
        "No UE context for ran_amf_id %s, exit", ue_context_key.c_str());
    return false;
  }
  std::shared_ptr<ue_context> uc;
  uc   = ran_amf_id_2_ue_context(ue_context_key);
  mcc  = uc.get()->tai.mcc;
  mnc  = uc.get()->tai.mnc;
  tmsi = golbal_tmsi;
  golbal_tmsi++;
  return true;
}

//------------------------------------------------------------------------------
void amf_app::generate_uuid() {
  amf_instance_id = to_string(boost::uuids::random_generator()());
}

//---------------------------------------------------------------------------------------------
void amf_app::generate_amf_profile() {
  // generate UUID
  generate_uuid();
  nf_instance_profile.set_nf_instance_id(amf_instance_id);
  nf_instance_profile.set_nf_instance_name("OAI-AMF");
  nf_instance_profile.set_nf_type("AMF");
  nf_instance_profile.set_nf_status("REGISTERED");
  nf_instance_profile.set_nf_heartBeat_timer(50);
  nf_instance_profile.set_nf_priority(1);
  nf_instance_profile.set_nf_capacity(100);
  nf_instance_profile.add_nf_ipv4_addresses(amf_cfg.n11.addr4);

  // NF services
  nf_service_t nf_service        = {};
  nf_service.service_instance_id = "namf_communication";
  nf_service.service_name        = "namf_communication";
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
  for (auto a : addrs) {
    endpoint.ipv4_addresses.push_back(a);
  }
  endpoint.transport = "TCP";
  endpoint.port      = amf_cfg.n11.port;
  nf_service.ip_endpoints.push_back(endpoint);

  nf_instance_profile.add_nf_service(nf_service);

  // TODO: custom info
  // AMF info
  amf_info_t info    = {};
  info.amf_region_id = amf_cfg.guami.regionID;
  info.amf_set_id    = amf_cfg.guami.AmfSetID;
  for (auto g : amf_cfg.guami_list) {
    guami_5g_t guami = {};
    guami.amf_id =
        g.regionID + ":" + g.AmfSetID + ":" + g.AmfPointer;  // TODO verify??
    guami.plmn.mcc = g.mcc;
    guami.plmn.mnc = g.mnc;
    info.guami_list.push_back(guami);
  }

  nf_instance_profile.set_amf_info(info);

  // Display the profile
  nf_instance_profile.display();
}

//---------------------------------------------------------------------------------------------
void amf_app::register_to_nrf() {
  // create a NF profile to this instance
  generate_amf_profile();
  // send request to N11 to send NF registration to NRF
  trigger_nf_registration_request();
}

//------------------------------------------------------------------------------
void amf_app::trigger_nf_registration_request() {
  Logger::amf_app().debug(
      "Send ITTI msg to N11 task to trigger the registration request to NRF");

  std::shared_ptr<itti_n11_register_nf_instance_request> itti_msg =
      std::make_shared<itti_n11_register_nf_instance_request>(
          TASK_AMF_APP, TASK_AMF_N11);
  itti_msg->profile = nf_instance_profile;
  int ret           = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N11",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_app::trigger_nf_deregistration() {
  Logger::amf_app().debug(
      "Send ITTI msg to N11 task to trigger the deregistration request to NRF");

  std::shared_ptr<itti_n11_deregister_nf_instance> itti_msg =
      std::make_shared<itti_n11_deregister_nf_instance>(
          TASK_AMF_APP, TASK_AMF_N11);
  itti_msg->amf_instance_id = amf_instance_id;
  int ret                   = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::amf_app().error(
        "Could not send ITTI message %s to task TASK_AMF_N11",
        itti_msg->get_msg_name());
  }
}
