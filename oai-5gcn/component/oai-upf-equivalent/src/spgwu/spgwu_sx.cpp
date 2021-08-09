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

/*! \file spgwu_sx.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "common_defs.h"
#include "itti.hpp"
#include "logger.hpp"
#include "spgwu_config.hpp"
#include "spgwu_pfcp_association.hpp"
#include "spgwu_sx.hpp"

#include <chrono>
#include <ctime>
#include <stdexcept>

using namespace pfcp;
using namespace spgwu;
using namespace std;

extern itti_mw* itti_inst;
extern spgwu_config spgwu_cfg;
extern spgwu_sx* spgwu_sx_inst;

void spgwu_sx_task(void*);

//------------------------------------------------------------------------------

void spgwu_sx_task(void* args_p) {
  const task_id_t task_id = TASK_SPGWU_SX;

  const util::thread_sched_params* const sched_params =
      (const util::thread_sched_params* const) args_p;
  sched_params->apply(task_id, Logger::spgwu_sx());

  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case SXAB_HEARTBEAT_REQUEST:
        if (itti_sxab_heartbeat_request* m =
                dynamic_cast<itti_sxab_heartbeat_request*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_HEARTBEAT_RESPONSE:
        if (itti_sxab_heartbeat_response* m =
                dynamic_cast<itti_sxab_heartbeat_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_ASSOCIATION_SETUP_REQUEST:
        if (itti_sxab_association_setup_request* m =
                dynamic_cast<itti_sxab_association_setup_request*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_ASSOCIATION_SETUP_RESPONSE:
        if (itti_sxab_association_setup_response* m =
                dynamic_cast<itti_sxab_association_setup_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_ASSOCIATION_UPDATE_REQUEST:
        if (itti_sxab_association_update_request* m =
                dynamic_cast<itti_sxab_association_update_request*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_ASSOCIATION_UPDATE_RESPONSE:
        if (itti_sxab_association_update_response* m =
                dynamic_cast<itti_sxab_association_update_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_ASSOCIATION_RELEASE_REQUEST:
        if (itti_sxab_association_release_request* m =
                dynamic_cast<itti_sxab_association_release_request*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_ASSOCIATION_RELEASE_RESPONSE:
        if (itti_sxab_association_release_response* m =
                dynamic_cast<itti_sxab_association_release_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_VERSION_NOT_SUPPORTED_RESPONSE:
        if (itti_sxab_version_not_supported_response* m =
                dynamic_cast<itti_sxab_version_not_supported_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_NODE_REPORT_RESPONSE:
        if (itti_sxab_node_report_response* m =
                dynamic_cast<itti_sxab_node_report_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_SESSION_SET_DELETION_REQUEST:
        if (itti_sxab_session_set_deletion_request* m =
                dynamic_cast<itti_sxab_session_set_deletion_request*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_SESSION_ESTABLISHMENT_RESPONSE:
        if (itti_sxab_session_establishment_response* m =
                dynamic_cast<itti_sxab_session_establishment_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_SESSION_MODIFICATION_RESPONSE:
        if (itti_sxab_session_modification_response* m =
                dynamic_cast<itti_sxab_session_modification_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_SESSION_DELETION_RESPONSE:
        if (itti_sxab_session_deletion_response* m =
                dynamic_cast<itti_sxab_session_deletion_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case SXAB_SESSION_REPORT_RESPONSE:
        if (itti_sxab_session_report_response* m =
                dynamic_cast<itti_sxab_session_report_response*>(msg)) {
          spgwu_sx_inst->handle_itti_msg(ref(*m));
        }
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::spgwu_sx().info("TIME-OUT event timer id %d", to->timer_id);
          switch (to->arg1_user) {
            case TASK_SPGWU_SX_TRIGGER_HEARTBEAT_REQUEST:
              pfcp_associations::get_instance().initiate_heartbeat_request(
                  to->timer_id, to->arg2_user);
              break;
            case TASK_SPGWU_SX_TIMEOUT_HEARTBEAT_REQUEST:
              pfcp_associations::get_instance().timeout_heartbeat_request(
                  to->timer_id, to->arg2_user);
              break;
            case TASK_SPGWU_SX_TIMEOUT_ASSOCIATION_REQUEST:
              // TODO
              // spgwu_sx_inst->time_out_event_association_request(to->timer_id,
              // to->arg1_user, to->arg2_user);
              break;
            default:;
          }
        }
        break;
      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::spgwu_sx().info("Received terminate message");
          return;
        }
        break;
      default:
        Logger::spgwu_sx().info("no handler for msg type %d", msg->msg_type);
    }

  } while (true);
}

//------------------------------------------------------------------------------
spgwu_sx::spgwu_sx()
    : pfcp_l4_stack(
          std::string(inet_ntoa(spgwu_cfg.sx.addr4)), spgwu_cfg.sx.port,
          spgwu_cfg.sx.thread_rd_sched_params) {
  Logger::spgwu_sx().startup("Starting...");

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
  recovery_time_stamp  = ellapsed;

  // TODO load from config when features available ?
  up_function_features       = {};
  up_function_features.bucp  = 0;
  up_function_features.ddnd  = 0;
  up_function_features.dlbd  = 0;
  up_function_features.trst  = 0;
  up_function_features.ftup  = 1;
  up_function_features.pfdm  = 0;
  up_function_features.heeu  = 0;
  up_function_features.treu  = 0;
  up_function_features.empu  = 0;
  up_function_features.pdiu  = 0;
  up_function_features.udbc  = 0;
  up_function_features.quoac = 0;
  up_function_features.trace = 0;
  up_function_features.frrt  = 0;

  if (itti_inst->create_task(
          TASK_SPGWU_SX, spgwu_sx_task, &spgwu_cfg.itti.sx_sched_params)) {
    Logger::spgwu_sx().error("Cannot create task TASK_SPGWU_SX");
    throw std::runtime_error("Cannot create task TASK_SPGWU_SX");
  }

  for (std::vector<pfcp::node_id_t>::const_iterator it =
           spgwu_cfg.spgwcs.begin();
       it != spgwu_cfg.spgwcs.end(); ++it) {
    start_association(*it);
  }
  Logger::spgwu_sx().startup("Started");
}

//------------------------------------------------------------------------------
void spgwu_sx::handle_receive_heartbeat_request(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                               = true;
  uint64_t trxn_id                         = 0;
  pfcp_heartbeat_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SPGWU_SX, error, trxn_id);
  if (!error) {
    if (not msg_ies_container.recovery_time_stamp.first) {
      // Should be detected by lower layers
      Logger::spgwu_sx().warn(
          "Received SX HEARTBEAT REQUEST without recovery time stamp IE!, "
          "ignore message");
      return;
    }
    Logger::spgwu_sx().info("Received SX HEARTBEAT REQUEST");
    send_heartbeat_response(remote_endpoint, trxn_id);
  }
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_receive_heartbeat_response(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                = true;
  uint64_t trxn_id                          = 0;
  pfcp_heartbeat_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SPGWU_SX, error, trxn_id);
  if (!error) {
    if (not msg_ies_container.recovery_time_stamp.first) {
      // Should be detected by lower layers
      Logger::spgwu_sx().warn(
          "Received SX HEARTBEAT RESPONSE without recovery time stamp IE!, "
          "ignore message");
      return;
    }
    Logger::spgwu_sx().info("Received SX HEARTBEAT RESPONSE");
    pfcp_associations::get_instance().handle_receive_heartbeat_response(
        trxn_id);
  }
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_receive_association_setup_response(
    pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                        = true;
  uint64_t trxn_id                                  = 0;
  pfcp_association_setup_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SPGWU_SX, error, trxn_id);
  if (!error) {
    if (not msg_ies_container.node_id.first) {
      // Should be detected by lower layers
      Logger::spgwu_sx().warn(
          "Received SX ASSOCIATION SETUP RESPONSE without node id IE!, ignore "
          "message");
      return;
    }
    if (not msg_ies_container.recovery_time_stamp.first) {
      // Should be detected by lower layers
      Logger::spgwu_sx().warn(
          "Received SX ASSOCIATION SETUP RESPONSE without recovery time stamp "
          "IE!, ignore message");
      return;
    }
    Logger::spgwu_sx().info("Received SX ASSOCIATION SETUP RESPONSE");
    if (msg_ies_container.cp_function_features.first) {
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second,
          msg_ies_container.cp_function_features.second);
    } else {
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second);
    }
  }
  // else ignore ?
}

void spgwu_sx::handle_receive_association_setup_request(
    pfcp::pfcp_msg& msg, const endpoint& remote_endpoint) {
Logger::spgwu_sx().info("Handle SX ASSOCIATION SETUP REQUEST");
  bool error                                       = true;
  uint64_t trxn_id                                 = 0;
  pfcp_association_setup_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SPGWU_SX, error, trxn_id);
  if (!error) {
    if (not msg_ies_container.node_id.first) {
      // Should be detected by lower layers
      Logger::spgwu_sx().warn(
          "Received SX ASSOCIATION SETUP REQUEST without node id IE!, ignore "
          "message");
      return;
    }
    if (not msg_ies_container.recovery_time_stamp.first) {
      // Should be detected by lower layers
      Logger::spgwu_sx().warn(
          "Received SX ASSOCIATION SETUP REQUEST without recovery time stamp "
          "IE!, ignore message");
      return;
    }
    // bool restore_n4_sessions = false;
    if (msg_ies_container.cp_function_features.first) {
      // Should be detected by lower layers
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second,
          msg_ies_container.cp_function_features.second);
    } else {
      pfcp_associations::get_instance().add_association(
          msg_ies_container.node_id.second,
          msg_ies_container.recovery_time_stamp.second);
    }

    // always yes (for the time being)
    itti_sxab_association_setup_response a(TASK_SPGWU_SX, TASK_SPGWU_SX);
    a.trxn_id           = trxn_id;
    pfcp::cause_t cause = {.cause_value = pfcp::CAUSE_VALUE_REQUEST_ACCEPTED};
    a.pfcp_ies.set(cause);
    pfcp::node_id_t node_id = {};
    if (spgwu_cfg.get_pfcp_node_id(node_id) == RETURNok) {
      a.pfcp_ies.set(node_id);
      pfcp::recovery_time_stamp_t r = {.recovery_time_stamp =
                                           (uint32_t) recovery_time_stamp};
      a.pfcp_ies.set(r);
      a.pfcp_ies.set(up_function_features);
      if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
        a.r_endpoint = remote_endpoint;
	send_sx_msg(a);
      } else {
        Logger::spgwu_sx().warn(
            "Received SX ASSOCIATION SETUP REQUEST node_id IPV6, FQDN!, "
            "ignore message");
        return;
      }
    } else {
      Logger::spgwu_sx().warn(
          "Received SX ASSOCIATION SETUP REQUEST could not set node id!, "
          "ignore message");
      return;
    }
  }
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_receive_session_establishment_request(
    pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                           = true;
  uint64_t trxn_id                                     = 0;
  pfcp_session_establishment_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SPGWU_SX, error, trxn_id);
  if (!error) {
    itti_sxab_session_establishment_request* itti_msg =
        new itti_sxab_session_establishment_request(
            TASK_SPGWU_SX, TASK_SPGWU_APP);
    itti_msg->pfcp_ies   = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->trxn_id    = trxn_id;
    itti_msg->seid       = msg.get_seid();
    std::shared_ptr<itti_sxab_session_establishment_request> i =
        std::shared_ptr<itti_sxab_session_establishment_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::spgwu_sx().error(
          "Could not send ITTI message %s to task TASK_SPGWU_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_receive_session_modification_request(
    pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                          = true;
  uint64_t trxn_id                                    = 0;
  pfcp_session_modification_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SPGWU_SX, error, trxn_id);
  if (!error) {
    itti_sxab_session_modification_request* itti_msg =
        new itti_sxab_session_modification_request(
            TASK_SPGWU_SX, TASK_SPGWU_APP);
    itti_msg->pfcp_ies   = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->trxn_id    = trxn_id;
    itti_msg->seid       = msg.get_seid();
    std::shared_ptr<itti_sxab_session_modification_request> i =
        std::shared_ptr<itti_sxab_session_modification_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::spgwu_sx().error(
          "Could not send ITTI message %s to task TASK_SPGWU_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_receive_session_deletion_request(
    pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                      = true;
  uint64_t trxn_id                                = 0;
  pfcp_session_deletion_request msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SPGWU_SX, error, trxn_id);
  if (!error) {
    itti_sxab_session_deletion_request* itti_msg =
        new itti_sxab_session_deletion_request(TASK_SPGWU_SX, TASK_SPGWU_APP);
    itti_msg->pfcp_ies   = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->trxn_id    = trxn_id;
    itti_msg->seid       = msg.get_seid();
    std::shared_ptr<itti_sxab_session_deletion_request> i =
        std::shared_ptr<itti_sxab_session_deletion_request>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::spgwu_sx().error(
          "Could not send ITTI message %s to task TASK_SPGWU_APP",
          i->get_msg_name());
    }
  }
  // else ignore
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_receive_session_report_response(
    pfcp_msg& msg, const endpoint& remote_endpoint) {
  bool error                                     = true;
  uint64_t trxn_id                               = 0;
  pfcp_session_report_response msg_ies_container = {};
  msg.to_core_type(msg_ies_container);

  handle_receive_message_cb(
      msg, remote_endpoint, TASK_SPGWU_SX, error, trxn_id);
  if (!error) {
    itti_sxab_session_report_response* itti_msg =
        new itti_sxab_session_report_response(TASK_SPGWU_SX, TASK_SPGWU_APP);
    itti_msg->pfcp_ies   = msg_ies_container;
    itti_msg->r_endpoint = remote_endpoint;
    itti_msg->trxn_id    = trxn_id;
    itti_msg->seid       = msg.get_seid();
    std::shared_ptr<itti_sxab_session_report_response> i =
        std::shared_ptr<itti_sxab_session_report_response>(itti_msg);
    int ret = itti_inst->send_msg(i);
    if (RETURNok != ret) {
      Logger::spgwu_sx().error(
          "Could not send ITTI message %s to task TASK_SPGWU_APP",
          i->get_msg_name());
    }
  }
  // else ignore ?
}

//------------------------------------------------------------------------------
void spgwu_sx::handle_receive_pfcp_msg(
    pfcp_msg& msg, const endpoint& remote_endpoint) {
  // Logger::spgwu_sx().trace( "handle_receive_pfcp_msg msg type %d length %d",
  // msg.get_message_type(), msg.get_message_length());
  switch (msg.get_message_type()) {
    case PFCP_ASSOCIATION_SETUP_RESPONSE:
      handle_receive_association_setup_response(msg, remote_endpoint);
      break;

    case PFCP_SESSION_ESTABLISHMENT_REQUEST:
      handle_receive_session_establishment_request(msg, remote_endpoint);
      break;

    case PFCP_SESSION_MODIFICATION_REQUEST:
      handle_receive_session_modification_request(msg, remote_endpoint);
      break;

    case PFCP_SESSION_DELETION_REQUEST:
      handle_receive_session_deletion_request(msg, remote_endpoint);
      break;

    case PFCP_SESSION_REPORT_RESPONSE:
      handle_receive_session_report_response(msg, remote_endpoint);
      break;

    case PFCP_HEARTBEAT_REQUEST:
      handle_receive_heartbeat_request(msg, remote_endpoint);
      break;

    case PFCP_HEARTBEAT_RESPONSE:
      handle_receive_heartbeat_response(msg, remote_endpoint);
      break;

    case PFCP_ASSOCIATION_SETUP_REQUEST:
      handle_receive_association_setup_request(msg, remote_endpoint);
      break;

    case PFCP_PFCP_PFD_MANAGEMENT_REQUEST:
    case PFCP_PFCP_PFD_MANAGEMENT_RESPONSE:
      //    case PFCP_ASSOCIATION_SETUP_REQUEST:
    case PFCP_ASSOCIATION_UPDATE_REQUEST:
    case PFCP_ASSOCIATION_UPDATE_RESPONSE:
    case PFCP_ASSOCIATION_RELEASE_REQUEST:
    case PFCP_ASSOCIATION_RELEASE_RESPONSE:
    case PFCP_VERSION_NOT_SUPPORTED_RESPONSE:
    case PFCP_NODE_REPORT_REQUEST:
    case PFCP_NODE_REPORT_RESPONSE:
    case PFCP_SESSION_SET_DELETION_REQUEST:
    case PFCP_SESSION_SET_DELETION_RESPONSE:
    case PFCP_SESSION_ESTABLISHMENT_RESPONSE:
    case PFCP_SESSION_MODIFICATION_RESPONSE:
    case PFCP_SESSION_DELETION_RESPONSE:
    case PFCP_SESSION_REPORT_REQUEST:
      Logger::spgwu_sx().info(
          "handle_receive_pfcp_msg msg %d length %d, not handled, discarded!",
          msg.get_message_type(), msg.get_message_length());
      break;
    default:
      Logger::spgwu_sx().info(
          "unhandled msg %d length %d, unknown, discarded!",
          msg.get_message_type(), msg.get_message_length());
  }
}

//------------------------------------------------------------------------------
void spgwu_sx::handle_itti_msg(itti_sxab_session_establishment_response& msg) {
  send_sx_msg(msg);
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_itti_msg(itti_sxab_session_modification_response& msg) {
  send_sx_msg(msg);
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_itti_msg(itti_sxab_session_deletion_response& msg) {
  send_sx_msg(msg);
}
//------------------------------------------------------------------------------
void spgwu_sx::send_sx_msg(itti_sxab_association_setup_request& i) {
  send_request(i.r_endpoint, i.pfcp_ies, TASK_SPGWU_SX, i.trxn_id);
}
//------------------------------------------------------------------------------
void spgwu_sx::send_sx_msg(itti_sxab_association_setup_response& i) {
  send_response(i.r_endpoint, i.pfcp_ies, i.trxn_id);
}
//------------------------------------------------------------------------------
void spgwu_sx::send_sx_msg(itti_sxab_session_establishment_response& i) {
  send_response(i.r_endpoint, i.seid, i.pfcp_ies, i.trxn_id);
}
//------------------------------------------------------------------------------
void spgwu_sx::send_sx_msg(itti_sxab_session_modification_response& i) {
  send_response(i.r_endpoint, i.seid, i.pfcp_ies, i.trxn_id);
}
//------------------------------------------------------------------------------
void spgwu_sx::send_sx_msg(itti_sxab_session_deletion_response& i) {
  send_response(i.r_endpoint, i.seid, i.pfcp_ies, i.trxn_id);
}
//------------------------------------------------------------------------------
void spgwu_sx::send_sx_msg(itti_sxab_session_report_request& i) {
  send_request(i.r_endpoint, i.seid, i.pfcp_ies, TASK_SPGWU_SX, i.trxn_id);
}
//------------------------------------------------------------------------------
void spgwu_sx::start_association(const pfcp::node_id_t& node_id) {
  pfcp_associations::get_instance().add_peer_candidate_node(node_id);

  itti_sxab_association_setup_request a(TASK_SPGWU_SX, TASK_SPGWU_SX);
  a.trxn_id = generate_trxn_id();

  pfcp::node_id_t this_node_id = {};
  if (spgwu_cfg.get_pfcp_node_id(this_node_id) == RETURNok) {
    a.pfcp_ies.set(this_node_id);
    pfcp::recovery_time_stamp_t r = {.recovery_time_stamp =
                                         (uint32_t) recovery_time_stamp};
    a.pfcp_ies.set(r);
    a.pfcp_ies.set(up_function_features);
    if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
      // a.l_endpoint =
      // endpoint(boost::asio::ip::address_v4(spgwu_cfg.sx.addr4), 0);
      a.r_endpoint = endpoint(node_id.u1.ipv4_address, pfcp::default_port);
      send_sx_msg(a);
    } else {
      Logger::spgwu_sx().warn("TODO start_association() node_id IPV6, FQDN!");
    }
  }
}
//------------------------------------------------------------------------------
void spgwu_sx::send_sx_msg(
    const pfcp::fseid_t& cp_fseid, const pfcp::pfcp_session_report_request& s) {
  itti_sxab_session_report_request isrr(TASK_SPGWU_SX, TASK_SPGWU_SX);
  isrr.trxn_id  = generate_trxn_id();
  isrr.pfcp_ies = s;
  isrr.seid     = cp_fseid.seid;

  std::shared_ptr<pfcp_association> sa = {};
  if (pfcp_associations::get_instance().get_association(cp_fseid, sa)) {
    const pfcp::node_id_t& peer_node_id = sa->peer_node_id();
    if (peer_node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
      // a.l_endpoint =
      // endpoint(boost::asio::ip::address_v4(spgwu_cfg.sx.addr4), 0);
      isrr.r_endpoint =
          endpoint(peer_node_id.u1.ipv4_address, pfcp::default_port);
      send_sx_msg(isrr);
    } else {
      Logger::spgwu_sx().warn("TODO start_association() node_id IPV6, FQDN!");
    }
  } else {
    Logger::spgwu_sx().warn(
        "Could not send PFCP_SESSION_REPORT_REQUEST, cause association not "
        "found for cp_fseid");
  }
}
//------------------------------------------------------------------------------
void spgwu_sx::send_heartbeat_request(std::shared_ptr<pfcp_association>& a) {
  pfcp::pfcp_heartbeat_request h = {};
  pfcp::recovery_time_stamp_t r  = {.recovery_time_stamp =
                                       (uint32_t) recovery_time_stamp};
  h.set(r);

  pfcp::node_id_t& node_id = a->node_id;
  if (node_id.node_id_type == pfcp::NODE_ID_TYPE_IPV4_ADDRESS) {
    a->timer_heartbeat = itti_inst->timer_setup(
        5, 0, TASK_SPGWU_SX, TASK_SPGWU_SX_TIMEOUT_HEARTBEAT_REQUEST,
        a->hash_node_id);

    endpoint r_endpoint = endpoint(node_id.u1.ipv4_address, pfcp::default_port);
    a->trxn_id_heartbeat = generate_trxn_id();
    send_request(r_endpoint, h, TASK_SPGWU_SX, a->trxn_id_heartbeat);

  } else {
    Logger::spgwu_sx().warn(
        "TODO send_heartbeat_request() node_id IPV6, FQDN!");
  }
}
//------------------------------------------------------------------------------
void spgwu_sx::send_heartbeat_response(
    const endpoint& r_endpoint, const uint64_t trxn_id) {
  pfcp::pfcp_heartbeat_response h = {};
  pfcp::recovery_time_stamp_t r   = {.recovery_time_stamp =
                                       (uint32_t) recovery_time_stamp};
  h.set(r);
  send_response(r_endpoint, h, trxn_id);
}
//------------------------------------------------------------------------------
void spgwu_sx::handle_receive(
    char* recv_buffer, const std::size_t bytes_transferred,
    const endpoint& remote_endpoint) {
  Logger::spgwu_sx().info("handle_receive(%d bytes)", bytes_transferred);
  // std::cout << string_to_hex(recv_buffer, bytes_transferred) << std::endl;
  std::istringstream iss(std::istringstream::binary);
  iss.rdbuf()->pubsetbuf(recv_buffer, bytes_transferred);
  pfcp_msg msg    = {};
  msg.remote_port = remote_endpoint.port();
  try {
    msg.load_from(iss);
    handle_receive_pfcp_msg(msg, remote_endpoint);
  } catch (pfcp_exception& e) {
    Logger::spgwu_sx().info("handle_receive exception %s", e.what());
  }
}
//------------------------------------------------------------------------------
void spgwu_sx::time_out_itti_event(const uint32_t timer_id) {
  bool handled = false;
  time_out_event(timer_id, TASK_SPGWU_SX, handled);
  if (!handled) {
    Logger::spgwu_sx().error("Timer %d not Found", timer_id);
  }
}
