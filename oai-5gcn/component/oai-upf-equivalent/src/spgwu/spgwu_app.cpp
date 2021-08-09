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

/*! \file spgwu_app.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#include "conversions.hpp"
#include "itti.hpp"
#include "logger.hpp"
#include "pfcp_switch.hpp"
#include "spgwu_app.hpp"
#include "spgwu_config.hpp"
#include "spgwu_s1u.hpp"
#include "spgwu_sx.hpp"
#include "spgwu_nrf.hpp"

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <stdexcept>

using namespace pfcp;
using namespace spgwu;
using namespace std;

// C includes

spgwu_sx* spgwu_sx_inst   = nullptr;
spgwu_s1u* spgwu_s1u_inst = nullptr;
spgwu_nrf* spgwu_nrf_inst = nullptr;

extern itti_mw* itti_inst;
extern pfcp_switch* pfcp_switch_inst;
extern spgwu_app* spgwu_app_inst;
extern spgwu_config spgwu_cfg;

void spgwu_app_task(void*);

//------------------------------------------------------------------------------
void spgwu_app_task(void* args_p) {
  const task_id_t task_id = TASK_SPGWU_APP;

  const util::thread_sched_params* const sched_params =
      (const util::thread_sched_params* const) args_p;

  sched_params->apply(task_id, Logger::spgwu_app());

  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case S1U_ECHO_REQUEST:
        spgwu_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_s1u_echo_request>(shared_msg));
        break;

      case SXAB_SESSION_ESTABLISHMENT_REQUEST:
        spgwu_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_sxab_session_establishment_request>(
                shared_msg));
        break;

      case SXAB_SESSION_MODIFICATION_REQUEST:
        spgwu_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_sxab_session_modification_request>(
                shared_msg));
        break;

      case SXAB_SESSION_DELETION_REQUEST:
        spgwu_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_sxab_session_deletion_request>(
                shared_msg));
        break;

      case SXAB_SESSION_REPORT_RESPONSE:
        spgwu_app_inst->handle_itti_msg(
            std::static_pointer_cast<itti_sxab_session_report_response>(
                shared_msg));
        break;

      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          switch (to->arg1_user) {
            case TASK_SPGWU_PFCP_SWITCH_MIN_COMMIT_INTERVAL:
              pfcp_switch_inst->time_out_min_commit_interval(to->timer_id);
              break;
            case TASK_SPGWU_PFCP_SWITCH_MAX_COMMIT_INTERVAL:
              pfcp_switch_inst->time_out_max_commit_interval(to->timer_id);
              break;
            default:;
          }
        }
        break;

      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::spgwu_app().info("Received terminate message");
          return;
        }
        break;

      case HEALTH_PING:
        break;

      default:
        Logger::spgwu_app().info(
            "no handler for ITTI msg type %d", msg->msg_type);
    }
  } while (true);
}

//------------------------------------------------------------------------------
spgwu_app::spgwu_app(const std::string& config_file) {
  Logger::spgwu_app().startup("Starting...");
  spgwu_cfg.execute();

  if (itti_inst->create_task(
          TASK_SPGWU_APP, spgwu_app_task,
          &spgwu_cfg.itti.spgwu_app_sched_params)) {
    Logger::spgwu_app().error("Cannot create task TASK_SPGWU_APP");
    throw std::runtime_error("Cannot create task TASK_SPGWU_APP");
  }
  try {
    spgwu_sx_inst = new spgwu_sx();
  } catch (std::exception& e) {
    Logger::spgwu_app().error("Cannot create SPGWU_SX: %s", e.what());
    throw;
  }
  try {
    spgwu_s1u_inst = new spgwu_s1u();
  } catch (std::exception& e) {
    Logger::spgwu_app().error("Cannot create SPGWU_S1U: %s", e.what());
    throw;
  }
  try {
    pfcp_switch_inst = new pfcp_switch();
  } catch (std::exception& e) {
    Logger::spgwu_app().error("Cannot create PFCP_SWITCH: %s", e.what());
    throw;
  }
  try {
    if (spgwu_cfg.register_nrf) spgwu_nrf_inst = new spgwu_nrf();
  } catch (std::exception& e) {
    Logger::spgwu_app().error("Cannot create SPGWU_NRF: %s", e.what());
    throw;
  }
  Logger::spgwu_app().startup("Started");
}

//------------------------------------------------------------------------------
spgwu_app::~spgwu_app() {
  if (spgwu_sx_inst) delete spgwu_sx_inst;
}
//------------------------------------------------------------------------------
void spgwu_app::handle_itti_msg(std::shared_ptr<itti_s1u_echo_request> m) {
  Logger::spgwu_app().debug("Received %s ", m->get_msg_name());
  itti_s1u_echo_response* s1u_resp =
      new itti_s1u_echo_response(TASK_SPGWU_APP, TASK_SPGWU_S1U);

  // May insert a call to a function here(throttle for example)
  s1u_resp->gtp_ies.r_endpoint      = m->gtp_ies.r_endpoint;
  s1u_resp->gtp_ies.teid            = m->gtp_ies.teid;
  s1u_resp->gtp_ies.sequence_number = m->gtp_ies.sequence_number;

  std::shared_ptr<itti_s1u_echo_response> msg =
      std::shared_ptr<itti_s1u_echo_response>(s1u_resp);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::spgwu_app().error(
        "Could not send ITTI message %s to task TASK_SPGWU_S1U",
        s1u_resp->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void spgwu_app::handle_itti_msg(
    std::shared_ptr<itti_sxab_session_establishment_request> m) {
  Logger::spgwu_app().info(
      "Received SXAB_SESSION_ESTABLISHMENT_REQUEST seid " SEID_FMT " ",
      m->seid);
  itti_sxab_session_establishment_response* sx_resp =
      new itti_sxab_session_establishment_response(
          TASK_SPGWU_APP, TASK_SPGWU_SX);
  pfcp_switch_inst->handle_pfcp_session_establishment_request(m, sx_resp);

  pfcp::node_id_t node_id = {};
  spgwu_cfg.get_pfcp_node_id(node_id);
  sx_resp->pfcp_ies.set(node_id);

  sx_resp->trxn_id = m->trxn_id;
  sx_resp->seid    = m->pfcp_ies.cp_fseid.second
                      .seid;  // Mandatory IE, but... may be bad to do this
  sx_resp->r_endpoint = m->r_endpoint;
  sx_resp->l_endpoint = m->l_endpoint;
  std::shared_ptr<itti_sxab_session_establishment_response> msg =
      std::shared_ptr<itti_sxab_session_establishment_response>(sx_resp);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::spgwu_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_SX",
        sx_resp->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void spgwu_app::handle_itti_msg(
    std::shared_ptr<itti_sxab_session_modification_request> m) {
  Logger::spgwu_app().info(
      "Received SXAB_SESSION_MODIFICATION_REQUEST seid " SEID_FMT " ", m->seid);
  itti_sxab_session_modification_response* sx_resp =
      new itti_sxab_session_modification_response(
          TASK_SPGWU_APP, TASK_SPGWU_SX);
  pfcp_switch_inst->handle_pfcp_session_modification_request(m, sx_resp);

  sx_resp->trxn_id    = m->trxn_id;
  sx_resp->r_endpoint = m->r_endpoint;
  sx_resp->l_endpoint = m->l_endpoint;
  std::shared_ptr<itti_sxab_session_modification_response> msg =
      std::shared_ptr<itti_sxab_session_modification_response>(sx_resp);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::spgwu_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_SX",
        sx_resp->get_msg_name());
  }
}
//------------------------------------------------------------------------------
void spgwu_app::handle_itti_msg(
    std::shared_ptr<itti_sxab_session_deletion_request> m) {
  Logger::spgwu_app().info(
      "Received SXAB_SESSION_DELETION_REQUEST seid " SEID_FMT " ", m->seid);
  itti_sxab_session_deletion_response* sx_resp =
      new itti_sxab_session_deletion_response(TASK_SPGWU_APP, TASK_SPGWU_SX);
  pfcp_switch_inst->handle_pfcp_session_deletion_request(m, sx_resp);

  sx_resp->trxn_id    = m->trxn_id;
  sx_resp->r_endpoint = m->r_endpoint;
  sx_resp->l_endpoint = m->l_endpoint;
  std::shared_ptr<itti_sxab_session_deletion_response> msg =
      std::shared_ptr<itti_sxab_session_deletion_response>(sx_resp);
  int ret = itti_inst->send_msg(msg);
  if (RETURNok != ret) {
    Logger::spgwu_app().error(
        "Could not send ITTI message %s to task TASK_PGWC_SX",
        sx_resp->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void spgwu_app::handle_itti_msg(
    std::shared_ptr<itti_sxab_session_report_response> m) {
  Logger::spgwu_app().info(
      "Received SXAB_SESSION_REPORT_RESPONSE seid " SEID_FMT " ", m->seid);
}
