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

/*! \file spgwu_pfcp_association.cpp
   \brief
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/
#include "common_defs.h"
#include "logger.hpp"
#include "pfcp_switch.hpp"
#include "spgwu_pfcp_association.hpp"
#include "spgwu_sx.hpp"

using namespace spgwu;
using namespace std;

extern itti_mw* itti_inst;
extern pfcp_switch* pfcp_switch_inst;
extern spgwu_sx* spgwu_sx_inst;

//------------------------------------------------------------------------------
void pfcp_association::notify_add_session(const pfcp::fseid_t& cp_fseid) {
  std::unique_lock<std::mutex> l(m_sessions);
  sessions.insert(cp_fseid);
}
//------------------------------------------------------------------------------
bool pfcp_association::has_session(const pfcp::fseid_t& cp_fseid) const {
  std::unique_lock<std::mutex> l(m_sessions);
  auto it = sessions.find(cp_fseid);
  if (it != sessions.end()) {
    return true;
  } else {
    return false;
  }
}
//------------------------------------------------------------------------------
void pfcp_association::notify_del_session(const pfcp::fseid_t& cp_fseid) {
  std::unique_lock<std::mutex> l(m_sessions);
  sessions.erase(cp_fseid);
}
//------------------------------------------------------------------------------
void pfcp_association::del_sessions() {
  std::unique_lock<std::mutex> l(m_sessions);
  for (std::set<pfcp::fseid_t>::iterator it = sessions.begin();
       it != sessions.end();) {
    pfcp_switch_inst->remove_pfcp_session(*it);
    sessions.erase(it++);
  }
}
//------------------------------------------------------------------------------
bool pfcp_associations::add_association(
    pfcp::node_id_t& node_id,
    pfcp::recovery_time_stamp_t& recovery_time_stamp) {
  std::shared_ptr<pfcp_association> sa = {};
  if (remove_peer_candidate_node(node_id, sa)) {
    sa->recovery_time_stamp  = recovery_time_stamp;
    sa->function_features    = {};
    std::size_t hash_node_id = std::hash<pfcp::node_id_t>{}(node_id);
    associations.insert((int32_t) hash_node_id, sa);
    trigger_heartbeat_request_procedure(sa);
    return true;
  }
  return false;
}
//------------------------------------------------------------------------------
bool pfcp_associations::add_association(
    pfcp::node_id_t& node_id, pfcp::recovery_time_stamp_t& recovery_time_stamp,
    pfcp::cp_function_features_s& function_features) {
  std::shared_ptr<pfcp_association> sa = {};
  if (remove_peer_candidate_node(node_id, sa)) {
    sa->recovery_time_stamp = recovery_time_stamp;
    sa->set(function_features);
    std::size_t hash_node_id = std::hash<pfcp::node_id_t>{}(node_id);
    associations.insert((int32_t) hash_node_id, sa);
    trigger_heartbeat_request_procedure(sa);
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
bool pfcp_associations::get_association(
    const pfcp::node_id_t& node_id,
    std::shared_ptr<pfcp_association>& sa) const {
  std::size_t hash_node_id = std::hash<pfcp::node_id_t>{}(node_id);
  auto pit                 = associations.find((int32_t) hash_node_id);
  if (pit == associations.end())
    return false;
  else {
    sa = pit->second;
    return true;
  }
}
//------------------------------------------------------------------------------
bool pfcp_associations::get_association(
    const pfcp::fseid_t& cp_fseid,
    std::shared_ptr<pfcp_association>& sa) const {
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;

  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> a = it->second;
    if (it->second->has_session(cp_fseid)) {
      sa = it->second;
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
bool pfcp_associations::remove_peer_candidate_node(
    pfcp::node_id_t& node_id, std::shared_ptr<pfcp_association>& s) {
  for (std::vector<std::shared_ptr<pfcp_association>>::iterator it =
           pending_associations.begin();
       it < pending_associations.end(); ++it) {
    if ((*it)->node_id == node_id) {
      s = *it;
      pending_associations.erase(it);
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
bool pfcp_associations::add_peer_candidate_node(
    const pfcp::node_id_t& node_id) {
  for (std::vector<std::shared_ptr<pfcp_association>>::iterator it =
           pending_associations.begin();
       it < pending_associations.end(); ++it) {
    if ((*it)->node_id == node_id) {
      // TODO purge sessions of this node
      Logger::spgwu_sx().info("TODO purge sessions of this node");
      pending_associations.erase(it);
      break;
    }
  }
  pfcp_association* association = new pfcp_association(node_id);
  std::shared_ptr<pfcp_association> s =
      std::shared_ptr<pfcp_association>(association);
  pending_associations.push_back(s);
  return true;
  // start_timer = itti_inst->timer_setup(0,0, TASK_SPGWU_SX,
  // TASK_MME_S11_TIMEOUT_SEND_GTPU_PING, 0);
}
//------------------------------------------------------------------------------
void pfcp_associations::trigger_heartbeat_request_procedure(
    std::shared_ptr<pfcp_association>& s) {
  s->timer_heartbeat = itti_inst->timer_setup(
      5, 0, TASK_SPGWU_SX, TASK_SPGWU_SX_TRIGGER_HEARTBEAT_REQUEST,
      s->hash_node_id);
}
//------------------------------------------------------------------------------
void pfcp_associations::initiate_heartbeat_request(
    timer_id_t timer_id, uint64_t arg2_user) {
  size_t hash = (size_t) arg2_user;
  for (auto it : associations) {
    if (it.second->hash_node_id == hash) {
      Logger::spgwu_sx().info(
          "PFCP HEARTBEAT PROCEDURE hash %u starting", hash);
      it.second->num_retries_timer_heartbeat = 0;
      spgwu_sx_inst->send_heartbeat_request(it.second);
    }
  }
}
//------------------------------------------------------------------------------
void pfcp_associations::timeout_heartbeat_request(
    timer_id_t timer_id, uint64_t arg2_user) {
  size_t hash = (size_t) arg2_user;
  for (auto it : associations) {
    if (it.second->hash_node_id == hash) {
      Logger::spgwu_sx().info(
          "PFCP HEARTBEAT PROCEDURE hash %u TIMED OUT", hash);
      if (it.second->num_retries_timer_heartbeat <
          PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES) {
        it.second->num_retries_timer_heartbeat++;
        spgwu_sx_inst->send_heartbeat_request(it.second);
      } else {
        it.second->del_sessions();
        pfcp::node_id_t node_id  = it.second->node_id;
        std::size_t hash_node_id = it.second->hash_node_id;
        associations.erase((uint32_t) hash_node_id);
        add_peer_candidate_node(node_id);
        break;
      }
    }
  }
}
//------------------------------------------------------------------------------
void pfcp_associations::handle_receive_heartbeat_response(
    const uint64_t trxn_id) {
  for (auto it : associations) {
    if (it.second->trxn_id_heartbeat == trxn_id) {
      itti_inst->timer_remove(it.second->timer_heartbeat);
      trigger_heartbeat_request_procedure(it.second);
      return;
    }
  }
  Logger::spgwu_sx().info(
      "PFCP HEARTBEAT PROCEDURE trxn_id %d NOT FOUND", trxn_id);
}

//------------------------------------------------------------------------------
void pfcp_associations::notify_add_session(
    const pfcp::node_id_t& node_id, const pfcp::fseid_t& cp_fseid) {
  std::shared_ptr<pfcp_association> sa = {};
  if (get_association(node_id, sa)) {
    sa->notify_add_session(cp_fseid);
  }
}
//------------------------------------------------------------------------------
void pfcp_associations::notify_del_session(const pfcp::fseid_t& cp_fseid) {
  std::shared_ptr<pfcp_association> sa = {};
  if (get_association(cp_fseid, sa)) {
    sa->notify_del_session(cp_fseid);
  }
}
