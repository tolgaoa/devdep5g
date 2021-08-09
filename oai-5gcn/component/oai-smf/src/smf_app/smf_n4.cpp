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

/*! \file smf_n4.cpp
 \brief
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr
 */

#include "smf_n4.hpp"

#include <chrono>
#include <ctime>
#include <stdexcept>

#include "common_defs.h"
#include "itti.hpp"
#include "logger.hpp"
#include "smf_config.hpp"

using namespace pfcp;
using namespace smf;
using namespace std;

extern itti_mw* itti_inst;
extern smf_config smf_cfg;
extern smf_n4* smf_n4_inst;

void smf_n4_task(void*);

//------------------------------------------------------------------------------
void smf_n4_task(void* args_p) {
  const task_id_t task_id = TASK_SMF_N4;
  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case N4_HEARTBEAT_REQUEST:
        if (itti_n4_heartbeat_request* m =
                dynamic_cast<itti_n4_heartbeat_request*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_HEARTBEAT_RESPONSE:
        if (itti_n4_heartbeat_response* m =
                dynamic_cast<itti_n4_heartbeat_response*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_ASSOCIATION_SETUP_REQUEST:
        if (itti_n4_association_setup_request* m =
                dynamic_cast<itti_n4_association_setup_request*>(msg)) {
          // m->trxn_id = smf_n4_inst->generate_trxn_id();
          smf_n4_inst->send_association_setup_request(ref(*m));
          // smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_ASSOCIATION_SETUP_RESPONSE:
        if (itti_n4_association_setup_response* m =
                dynamic_cast<itti_n4_association_setup_response*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_ASSOCIATION_UPDATE_REQUEST:
        if (itti_n4_association_update_request* m =
                dynamic_cast<itti_n4_association_update_request*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_ASSOCIATION_UPDATE_RESPONSE:
        if (itti_n4_association_update_response* m =
                dynamic_cast<itti_n4_association_update_response*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_ASSOCIATION_RELEASE_REQUEST:
        if (itti_n4_association_release_request* m =
                dynamic_cast<itti_n4_association_release_request*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_ASSOCIATION_RELEASE_RESPONSE:
        if (itti_n4_association_release_response* m =
                dynamic_cast<itti_n4_association_release_response*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_VERSION_NOT_SUPPORTED_RESPONSE:
        if (itti_n4_version_not_supported_response* m =
                dynamic_cast<itti_n4_version_not_supported_response*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_NODE_REPORT_RESPONSE:
        if (itti_n4_node_report_response* m =
                dynamic_cast<itti_n4_node_report_response*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_SESSION_SET_DELETION_REQUEST:
        if (itti_n4_session_set_deletion_request* m =
                dynamic_cast<itti_n4_session_set_deletion_request*>(msg)) {
          smf_n4_inst->handle_itti_msg(ref(*m));
        }
        break;

      case N4_SESSION_ESTABLISHMENT_REQUEST:
        if (itti_n4_session_establishment_request* m =
                dynamic_cast<itti_n4_session_establishment_request*>(msg)) {
          smf_n4_inst->send_n4_msg(ref(*m));
        }
        break;

      case N4_SESSION_MODIFICATION_REQUEST:
        if (itti_n4_session_modification_request* m =
                dynamic_cast<itti_n4_session_modification_request*>(msg)) {
          smf_n4_inst->send_n4_msg(ref(*m));
        }
        break;

      case N4_SESSION_DELETION_REQUEST:
        if (itti_n4_session_deletion_request* m =
                dynamic_cast<itti_n4_session_deletion_request*>(msg)) {
          smf_n4_inst->send_n4_msg(ref(*m));
        }
        break;

      case N4_SESSION_REPORT_RESPONSE:
        if (itti_n4_session_report_response* m =
                dynamic_cast<itti_n4_session_report_response*>(msg)) {
          smf_n4_inst->send_n4_msg(ref(*m));
        }
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::smf_n4().info("TIME-OUT event timer id %d", to->timer_id);
          switch (to->arg1_user) {
            case TASK_SMF_N4_TRIGGER_HEARTBEAT_REQUEST:
              pfcp_associations::get_instance().initiate_heartbeat_request(
                  to->timer_id, to->arg2_user);
              break;
            case TASK_SMF_N4_TIMEOUT_HEARTBEAT_REQUEST:
              pfcp_associations::get_instance().timeout_heartbeat_request(
                  to->timer_id, to->arg2_user);
              break;
            case TASK_SMF_N4_TIMEOUT_GRACEFUL_RELEASE_PERIOD:
              pfcp_associations::get_instance().timeout_release_request(
                  to->timer_id, to->arg2_user);
              break;
            default:;
          }
        }
        break;
      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::smf_n4().info("Received terminate message");
          return;
        }
        break;

      case HEALTH_PING:
        break;

      default:
        Logger::smf_n4().info("no handler for msg type %d", msg->msg_type);
    }

  } while (true);
}

//------------------------------------------------------------------------------
smf_n4::smf_n4()
    : pfcp_l4_stack(
          string(inet_ntoa(smf_cfg.n4.addr4)), smf_cfg.n4.port,
          smf_cfg.n4.thread_rd_sched_params) {
  Logger::smf_n4().startup("Starting...");
  // TODO  refine this, look at RFC5905
  std::tm tm_epoch       = {0};          // Feb 8th, 2036
  tm_epoch.tm_year       = 2036 - 1900;  // years count from 1900
  tm_epoch.tm_mon        = 2 - 1;        // months count from January=0
  tm_epoch.tm_mday       = 8;            // days count from 1
  std::time_t time_epoch = std::mktime(&tm_epoch);
  std::chrono::time_point<std::chrono::system_clock> now =
      std::chrono::system_clock::now();
  std::time_t now_c    = std::chrono::system_clock::to_time_t(now);
  std::time_t ellapsed = now_c - time_epoch;
  // recovery_time_stamp = ellapsed;

  // test new way to calculate recovery_time_stamp
  std::time_t time_epoch_ntp = std::time(nullptr);
  uint64_t tv_ntp            = time_epoch_ntp + SECONDS_SINCE_FIRST_EPOCH;
  recovery_time_stamp        = tv_ntp;

  // TODO may load this from config
  cp_function_features      = {};
  cp_function_features.ovrl = 0;
  cp_function_features.load = 0;

  if (itti_inst->create_task(TASK_SMF_N4, smf_n4_task, nullptr)) {
    Logger::smf_n4().error("Cannot create task TASK_SMF_N4");
    throw std::runtime_error("Cannot create task TASK_SMF_N4");
  }
  Logger::smf_n4().startup("Started");
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_pfcp_msg(
    pfcp_msg& msg, const endpoint& remote_endpoint) {
  Logger::smf_n4().trace(
      "handle_receive_pfcp_msg msg type %d length %d", msg.get_message_type(),
      msg.get_message_length());
  switch (msg.get_message_type()) {
    case PFCP_ASSOCIATION_SETUP_REQUEST:
      handle_receive_association_setup_request(msg, remote_endpoint);
      break;
    case PFCP_HEARTBEAT_REQUEST:
      handle_receive_heartbeat_request(msg, remote_endpoint);
      break;
    case PFCP_HEARTBEAT_RESPONSE:
      handle_receive_heartbeat_response(msg, remote_endpoint);
      break;
    case PFCP_SESSION_ESTABLISHMENT_RESPONSE:
      handle_receive_session_establishment_response(msg, remote_endpoint);
      break;
    case PFCP_SESSION_MODIFICATION_RESPONSE:
      handle_receive_session_modification_response(msg, remote_endpoint);
      break;
    case PFCP_SESSION_DELETION_RESPONSE:
      handle_receive_session_deletion_response(msg, remote_endpoint);
      break;
    case PFCP_SESSION_REPORT_REQUEST:
      handle_receive_session_report_request(msg, remote_endpoint);
      break;
    case PFCP_PFCP_PFD_MANAGEMENT_REQUEST:
    case PFCP_PFCP_PFD_MANAGEMENT_RESPONSE:
    case PFCP_ASSOCIATION_SETUP_RESPONSE:
      handle_receive_association_setup_response(msg, remote_endpoint);
      break;
    case PFCP_ASSOCIATION_UPDATE_REQUEST:
      handle_receive_association_update_request(msg, remote_endpoint);
      break;
    case PFCP_ASSOCIATION_UPDATE_RESPONSE:
    case PFCP_ASSOCIATION_RELEASE_REQUEST:
    case PFCP_ASSOCIATION_RELEASE_RESPONSE:
      handle_receive_association_release_response(msg, remote_endpoint);
      break;
    case PFCP_VERSION_NOT_SUPPORTED_RESPONSE:
    case PFCP_NODE_REPORT_REQUEST:
    case PFCP_NODE_REPORT_RESPONSE:
    case PFCP_SESSION_SET_DELETION_REQUEST:
    case PFCP_SESSION_SET_DELETION_RESPONSE:
    case PFCP_SESSION_ESTABLISHMENT_REQUEST:
    case PFCP_SESSION_MODIFICATION_REQUEST:
    case PFCP_SESSION_DELETION_REQUEST:
    case PFCP_SESSION_REPORT_RESPONSE:
      Logger::smf_n4().info(
          "handle_receive_pfcp_msg msg %d length %d, not handled, discarded!",
          msg.get_message_type(), msg.get_message_length());
      break;
    default:
      Logger::smf_n4().info(
          "handle_receive_pfcp_msg msg %d length %d, unknown, discarded!",
          msg.get_message_type(), msg.get_message_length());
  }
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_heartbeat_request(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                               = true;
  uint64_t trxn_id                         = 0;
  pfcp_heartbeat_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (!error) {
    if (not msg_ies_container.recovery_time_stamp.first) {
      // Should be detected by lower layers
      Logger::smf_n4().warn(
          "Received N4 HEARTBEAT REQUEST without recovery time stamp IE!, "
          "ignore message");
      return;
    }
    send_heartbeat_response(remote_endpoint, trxn_id);
  }
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_heartbeat_response(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                = true;
  uint64_t trxn_id                          = 0;
  pfcp_heartbeat_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (!error) {
    if (not msg_ies_container.recovery_time_stamp.first) {
      // Should be detected by lower layers
      Logger::smf_n4().warn(
          "Received N4 HEARTBEAT REQUEST without recovery time stamp IE!, "
          "ignore message");
      return;
    }
    pfcp_associations::get_instance().handle_receive_heartbeat_response(
        trxn_id);
  }
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_association_setup_request(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                       = true;
  uint64_t trxn_id                                 = 0;
  pfcp_association_setup_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (!error) {
    if (not msg_ies_container.node_id.first) {
      // Should be detected by lower layers
      Logger::smf_n4().warn(
          "Received N4 ASSOCIATION SETUP REQUEST without node id IE!, ignore "
          "message");
      return;
    }
    if (not msg_ies_container.recovery_time_stamp.first) {
      // Should be detected by lower layers
      Logger::smf_n4().warn(
          "Received N4 ASSOCIATION SETUP REQUEST without recovery time stamp "
          "IE!, ignore message");
      return;
    }
    bool restore_n4_sessions = false;
    if (msg_ies_container.up_function_features.first) {
      // Should be detected by lower layers
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second,
          msg_ies_container.up_function_features.second, restore_n4_sessions);
    } else if (msg_ies_container.enterprise_specific.first)  {
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second,
          msg_ies_container.up_function_features.second,
          msg_ies_container.enterprise_specific.second, restore_n4_sessions);
    } else {
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second, restore_n4_sessions);
    }

    // always yes (for the time being)
    itti_n4_association_setup_response a(TASK_SMF_N4, TASK_SMF_N4);
    a.trxn_id           = trxn_id;
    pfcp::cause_t cause = {.cause_value = pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};
    a.pfcp_ies.set(cause);
    pfcp::node_id_t node_id = {};
    if (smf_cfg.get_pfcp_node_id(node_id) == RETURNok) {
      a.pfcp_ies.set(node_id);
      pfcp::recovery_time_stamp_t r = {.recovery_time_stamp =
                                           (uint32_t) recovery_time_stamp};
      a.pfcp_ies.set(r);
      a.pfcp_ies.set(cp_function_features);
      if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
        a.r_endpoint = remote_endpoint;
        send_n4_msg(a);
      } else {
        Logger::smf_n4().warn(
            "Received N4 ASSOCIATION SETUP REQUEST node_id IPV6, FQDN!, "
            "ignore message");
        return;
      }
    } else {
      Logger::smf_n4().warn(
          "Received N4 ASSOCIATION SETUP REQUEST could not set node id!, "
          "ignore message");
      return;
    }

    if (restore_n4_sessions) {
      pfcp_associations::get_instance().restore_n4_sessions(
          msg_ies_container.node_id.second);
    }
  }
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_association_setup_response(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  // TODO: To be completed
  Logger::smf_n4().info("Received N4 ASSOCIATION SETUP RESPONSE from an UPF");
  bool error                                        = true;
  uint64_t trxn_id                                  = 0;
  pfcp_association_setup_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (!error) {
    if (not msg_ies_container.node_id.first) {
      // Should be detected by lower layers
      Logger::smf_n4().warn(
          "Received N4 ASSOCIATION SETUP RESPONSE without node id IE!, ignore "
          "message");
      return;
    }
    if (not msg_ies_container.recovery_time_stamp.first) {
      // Should be detected by lower layers
      Logger::smf_n4().warn(
          "Received N4 ASSOCIATION SETUP RESPONSE without recovery time stamp "
          "IE!, ignore message");
      return;
    }
    Logger::smf_n4().info("Received N4 ASSOCIATION SETUP RESPONSE");
    bool restore_n4_sessions = false;
    if (msg_ies_container.up_function_features.first) {
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second,
          msg_ies_container.up_function_features.second, restore_n4_sessions);
    } else if (msg_ies_container.enterprise_specific.first)  {
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second,
          msg_ies_container.up_function_features.second,
          msg_ies_container.enterprise_specific.second, restore_n4_sessions);
    } else {
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second, restore_n4_sessions);
    }
  }
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_association_update_request(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                        = true;
  uint64_t trxn_id                                  = 0;
  pfcp_association_update_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);
  pfcp::cause_t cause = {.cause_value = pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};
  uint32_t graceful_release_period = PFCP_ASSOCIATION_GRACEFUL_RELEASE_PERIOD;
  pfcp::node_id_t node_id          = {};

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (error) {
    Logger::smf_n4().warn(
        "Received N4 ASSOCIATION UPDATE REQUEST, error in "
        "handle_receive_message_cb!");
    return;
  }

  if (not msg_ies_container.node_id.first) {
    Logger::smf_n4().warn(
        "Received N4 ASSOCIATION UPDATE REQUEST without node id IE!, ignore "
        "message");
    cause.cause_value = pfcp::CAUSE_VALUE_MANDATORY_IE_MISSING;
  }

  if (smf_cfg.get_pfcp_node_id(node_id) != RETURNok) {
    Logger::smf_n4().warn(
        "Received N4 ASSOCIATION UPDATE REQUEST could not set node id!, ignore "
        "message");
    cause.cause_value = pfcp::CAUSE_VALUE_NO_ESTABLISHED_PFCP_ASSOCIATION;
  }

  if (cause.cause_value == pfcp::CAUSE_VALUE_REQUEST_ACCEPTED) {
    if (msg_ies_container.up_function_features.first) {
      // Update features and set cause accordingly
      if (pfcp_associations::get_instance().update_association(
              node_id, msg_ies_container.up_function_features.second))
        cause.cause_value = pfcp::CAUSE_VALUE_REQUEST_ACCEPTED;
      else
        cause.cause_value = pfcp::CAUSE_VALUE_REQUEST_REJECTED;
    }

    // If the UP function has requested to release the PFCP association
    if (msg_ies_container.pfcp_association_release_request.first) {
      // Graceful Release Period
      if (msg_ies_container.graceful_release_period.first) {
        // TODO:
        // max (PFCP_ASSOCIATION_GRACEFUL_RELEASE_PERIOD,
        // graceful_release_period)
      }

      // Schedule PFCP Association Release Request to release the PFCP
      // association
      std::shared_ptr<pfcp_association> sa =
          std::shared_ptr<pfcp_association>(nullptr);
      if (pfcp_associations::get_instance().get_association(node_id, sa)) {
        sa->timer_graceful_release = itti_inst->timer_setup(
            graceful_release_period, 0, TASK_SMF_N4,
            TASK_SMF_N4_TIMEOUT_GRACEFUL_RELEASE_PERIOD, sa->hash_node_id);
      }
    }

    if (msg_ies_container.user_plane_ip_resource_information.first) {
      // TODO:
    }
  }

  // send an PFCP Association Update Response
  itti_n4_association_update_response a(TASK_SMF_N4, TASK_SMF_N4);
  a.trxn_id = trxn_id;
  a.pfcp_ies.set(cause);
  a.pfcp_ies.set(node_id);
  if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
    a.r_endpoint = remote_endpoint;
    send_n4_msg(a);
  } else {
    Logger::smf_n4().warn(
        "Received N4 ASSOCIATION UPDATE REQUEST TODO node_id IPV6, FQDN!, "
        "ignore message");
    return;
  }
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_association_release_response(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  // TODO: To be completed
  Logger::smf_n4().info("Received N4 ASSOCIATION RELEASE RESPONSE from an UPF");
  bool error                                          = true;
  uint64_t trxn_id                                    = 0;
  pfcp_association_release_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (error) {
    Logger::smf_n4().warn(
        "Received N4 ASSOCIATION RELEASE RESPONSE, error in "
        "handle_receive_message_cb!");
    return;
  }

  if (not msg_ies_container.node_id.first) {
    Logger::smf_n4().warn(
        "Received N4 ASSOCIATION RELEASE RESPONSE without node id IE!, ignore "
        "message");
    return;
  }

  pfcp::node_id_t node_id = {};
  if (smf_cfg.get_pfcp_node_id(node_id) != RETURNok) {
    Logger::smf_n4().warn(
        "Received N4 ASSOCIATION RELEASE RESPONSE with an invalid node ID!, "
        "ignore message");
    return;
  }

  if (not msg_ies_container.cause.first) {
    Logger::smf_n4().warn(
        "Received N4 ASSOCIATION RELEASE RESPONSE without cause IE!, ignore "
        "message");
  } else {
    if (msg_ies_container.cause.second.cause_value ==
        pfcp::CAUSE_VALUE_REQUEST_ACCEPTED) {
      std::shared_ptr<pfcp_association> sa =
          std::shared_ptr<pfcp_association>(nullptr);
      if (pfcp_associations::get_instance().get_association(node_id, sa)) {
        // Delete locally all the PFCP sessions related to that PFCP association
        sa->del_sessions();
      }
    }
  }
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_session_establishment_response(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                            = true;
  uint64_t trxn_id                                      = 0;
  pfcp_session_establishment_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (!error) {
    itti_n4_session_establishment_response* itti_msg =
        new itti_n4_session_establishment_response(TASK_SMF_N4, TASK_SMF_APP);
    itti_msg->pfcp_ies   = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->trxn_id    = trxn_id;
    itti_msg->seid       = msg.get_seid();
    std::shared_ptr<itti_n4_session_establishment_response> i =
        std::shared_ptr<itti_n4_session_establishment_response>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::smf_n4().error(
          "Could not send ITTI message %s to task TASK_SMF_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_session_modification_response(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                           = true;
  uint64_t trxn_id                                     = 0;
  pfcp_session_modification_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (!error) {
    itti_n4_session_modification_response* itti_msg =
        new itti_n4_session_modification_response(TASK_SMF_N4, TASK_SMF_APP);
    itti_msg->pfcp_ies   = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->trxn_id    = trxn_id;
    itti_msg->seid       = msg.get_seid();
    std::shared_ptr<itti_n4_session_modification_response> i =
        std::shared_ptr<itti_n4_session_modification_response>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::smf_n4().error(
          "Could not send ITTI message %s to task TASK_SMF_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_session_deletion_response(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                       = true;
  uint64_t trxn_id                                 = 0;
  pfcp_session_deletion_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (!error) {
    itti_n4_session_deletion_response* itti_msg =
        new itti_n4_session_deletion_response(TASK_SMF_N4, TASK_SMF_APP);
    itti_msg->pfcp_ies   = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->trxn_id    = trxn_id;
    itti_msg->seid       = msg.get_seid();
    std::shared_ptr<itti_n4_session_deletion_response> i =
        std::shared_ptr<itti_n4_session_deletion_response>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::smf_n4().error(
          "Could not send ITTI message %s to task TASK_SMF_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive_session_report_request(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                    = true;
  uint64_t trxn_id                              = 0;
  pfcp_session_report_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(msg, remote_endpoint, TASK_SMF_N4, error, trxn_id);
  if (!error) {
    itti_n4_session_report_request* itti_msg =
        new itti_n4_session_report_request(TASK_SMF_N4, TASK_SMF_APP);
    itti_msg->pfcp_ies   = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->trxn_id    = trxn_id;
    itti_msg->seid       = msg.get_seid();
    std::shared_ptr<itti_n4_session_report_request> i =
        std::shared_ptr<itti_n4_session_report_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::smf_n4().error(
          "Could not send ITTI message %s to task TASK_SMF_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}

//------------------------------------------------------------------------------
void smf_n4::send_n4_msg(itti_n4_association_setup_response& i) {
  send_response(i.r_endpoint, i.pfcp_ies, i.trxn_id);
}

//------------------------------------------------------------------------------
void smf_n4::send_n4_msg(itti_n4_session_report_response& i) {
  send_response(i.r_endpoint, i.seid, i.pfcp_ies, i.trxn_id);
}

//------------------------------------------------------------------------------
void smf_n4::send_heartbeat_request(std::shared_ptr<pfcp_association>& a) {
  std::time_t time_epoch = std::time(nullptr);
  uint64_t tv_ntp        = time_epoch + SECONDS_SINCE_FIRST_EPOCH;

  pfcp::pfcp_heartbeat_request h = {};
  pfcp::recovery_time_stamp_t r  = {.recovery_time_stamp = (uint32_t) tv_ntp};
  h.set(r);

  pfcp::node_id_t& node_id = a->node_id;
  if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
    a->timer_heartbeat = itti_inst->timer_setup(
        5, 0, TASK_SMF_N4, TASK_SMF_N4_TIMEOUT_HEARTBEAT_REQUEST,
        a->hash_node_id);

    endpoint r_endpoint = endpoint(node_id.u1.ipv4_address, pfcp::default_port);
    a->trxn_id_heartbeat = generate_trxn_id();
    send_request(r_endpoint, h, TASK_SMF_N4, a->trxn_id_heartbeat);

  } else {
    Logger::smf_n4().warn("TODO send_heartbeat_request() node_id IPV6, FQDN!");
  }
}

//------------------------------------------------------------------------------
void smf_n4::send_heartbeat_response(
    const endpoint& r_endpoint, const uint64_t trxn_id) {
  pfcp::pfcp_heartbeat_response h = {};
  pfcp::recovery_time_stamp_t r   = {.recovery_time_stamp =
                                       (uint32_t) recovery_time_stamp};
  h.set(r);
  send_response(r_endpoint, h, trxn_id);
}

//------------------------------------------------------------------------------
void smf_n4::send_n4_msg(itti_n4_session_establishment_request& i) {
  send_request(i.r_endpoint, i.seid, i.pfcp_ies, TASK_SMF_N4, i.trxn_id);
}

//------------------------------------------------------------------------------
void smf_n4::send_n4_msg(itti_n4_session_modification_request& i) {
  send_request(i.r_endpoint, i.seid, i.pfcp_ies, TASK_SMF_N4, i.trxn_id);
}

//------------------------------------------------------------------------------
void smf_n4::send_n4_msg(itti_n4_session_deletion_request& i) {
  send_request(i.r_endpoint, i.seid, i.pfcp_ies, TASK_SMF_N4, i.trxn_id);
}

//------------------------------------------------------------------------------
void smf_n4::handle_receive(
    char* recv_buffer, const std::size_t bytes_transferred,
    const endpoint& remote_endpoint) {
  Logger::smf_n4().info("handle_receive(%d bytes)", bytes_transferred);
  // std::cout << string_to_hex(recv_buffer, bytes_transferred) << std::endl;
  std::istringstream iss(std::istringstream::binary);
  iss.rdbuf()->pubsetbuf(recv_buffer, bytes_transferred);
  pfcp_msg msg    = {};
  msg.remote_port = remote_endpoint.port();
  try {
    msg.load_from(iss);
    handle_receive_pfcp_msg(msg, remote_endpoint);
  } catch (pfcp_exception& e) {
    Logger::smf_n4().info("handle_receive exception %s", e.what());
  }
}

//------------------------------------------------------------------------------
void smf_n4::time_out_itti_event(const uint32_t timer_id) {
  bool handled = false;
  time_out_event(timer_id, TASK_SMF_N4, handled);
  if (!handled) {
    Logger::smf_n4().error("Timer %d not Found", timer_id);
  }
}

//------------------------------------------------------------------------------
void smf_n4::send_association_setup_request(
    itti_n4_association_setup_request& i) {
  i.trxn_id = generate_trxn_id();
  send_request(i.r_endpoint, i.pfcp_ies, TASK_SMF_N4, i.trxn_id);
}

//------------------------------------------------------------------------------
void smf_n4::send_release_request(std::shared_ptr<pfcp_association>& a) {
  Logger::smf_n4().debug("Send N4 ASSOCIATION RELEASE REQUEST");
  pfcp::pfcp_association_release_request release_request = {};

  pfcp::node_id_t& node_id = a->node_id;
  if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
    endpoint r_endpoint = endpoint(node_id.u1.ipv4_address, pfcp::default_port);
    uint64_t trxn_id    = generate_trxn_id();
    release_request.set(node_id);
    send_request(r_endpoint, release_request, TASK_SMF_N4, trxn_id);
  } else {
    Logger::smf_n4().warn("TODO send_release_request() node_id IPV6, FQDN!");
  }
}
