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

/*! \file smf_pfcp_association.cpp
 \brief
 \author  Lionel GAUTHIER
 \date 2019
 \email: lionel.gauthier@eurecom.fr
 */

#include "smf_pfcp_association.hpp"

#include "common_defs.h"
#include "logger.hpp"
#include "smf_n4.hpp"
#include "smf_procedure.hpp"

using namespace smf;
using namespace std;

extern itti_mw* itti_inst;
extern smf_n4* smf_n4_inst;

//------------------------------------------------------------------------------
void pfcp_association::notify_add_session(const pfcp::fseid_t& cp_fseid) {
  std::unique_lock<std::mutex> l(m_sessions);
  sessions.insert(cp_fseid);
}

//------------------------------------------------------------------------------
bool pfcp_association::has_session(const pfcp::fseid_t& cp_fseid) {
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
  sessions.clear();
}

//------------------------------------------------------------------------------
void pfcp_association::restore_n4_sessions() {
  std::unique_lock<std::mutex> l(m_sessions);
  if (sessions.size()) {
    is_restore_sessions_pending = true;
    n4_session_restore_procedure* restore_proc =
        new n4_session_restore_procedure(sessions);
    restore_proc->run();
  }
}

//------------------------------------------------------------------------------
bool pfcp_associations::add_association(
    pfcp::node_id_t& node_id, pfcp::recovery_time_stamp_t& recovery_time_stamp,
    bool& restore_n4_sessions) {
  std::shared_ptr<pfcp_association> sa =
      std::shared_ptr<pfcp_association>(nullptr);
  if (get_association(node_id, sa)) {
    itti_inst->timer_remove(sa->timer_heartbeat);
    if (sa->recovery_time_stamp == recovery_time_stamp) {
      restore_n4_sessions = false;
    } else {
      restore_n4_sessions = true;
    }
    sa->recovery_time_stamp = recovery_time_stamp;
    sa->function_features   = {};
  } else {
    // Resolve FQDN to get UPF IP address if necessary
    if (node_id.node_id_type == pfcp::NODE_ID_TYPE_FQDN) {
      Logger::smf_app().info("Node ID Type FQDN: %s", node_id.fqdn.c_str());
      struct hostent* record = gethostbyname(node_id.fqdn.c_str());
      if (record == NULL) {
        Logger::smf_app().info(
            "Add association with node (FQDN) %s: cannot resolve the hostname!",
            node_id.fqdn.c_str());
        return false;
      }

      if (record->h_addrtype == AF_INET) {
        in_addr* address        = (struct in_addr*) record->h_addr;
        node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
        node_id.u1.ipv4_address = *address;
        Logger::smf_app().info(
            "Node ID Type FQDN: %s, IPv4 Addr: %s", node_id.fqdn.c_str(),
            inet_ntoa(*address));
      } else if (record->h_addrtype == AF_INET6) {
        // TODO
        Logger::smf_app().info(
            "Node ID Type FQDN: %s. IPv6 Addr, this mode has not been "
            "supported yet!",
            node_id.fqdn.c_str());
        return false;
      } else {
        return false;
      }
    }

    restore_n4_sessions = false;
    pfcp_association* association =
        new pfcp_association(node_id, recovery_time_stamp);
    sa                       = std::shared_ptr<pfcp_association>(association);
    sa->recovery_time_stamp  = recovery_time_stamp;
    std::size_t hash_node_id = std::hash<pfcp::node_id_t>{}(node_id);
    // Associate with UPF profile if exist
    for (std::vector<std::shared_ptr<pfcp_association>>::iterator it =
             pending_associations.begin();
         it < pending_associations.end(); ++it) {
      if (((*it)->node_id == node_id) and ((*it)->is_upf_profile_set())) {
        Logger::smf_app().info("Associate with UPF profile");
        sa->set_upf_node_profile((*it)->get_upf_node_profile());
        break;
      }
    }
    associations.insert((int32_t) hash_node_id, sa);
    trigger_heartbeat_request_procedure(sa);
  }
  return true;
}

//------------------------------------------------------------------------------
bool pfcp_associations::add_association(
    pfcp::node_id_t& node_id, pfcp::recovery_time_stamp_t& recovery_time_stamp,
    pfcp::up_function_features_s& function_features,
    bool& restore_n4_sessions) {
  std::shared_ptr<pfcp_association> sa =
      std::shared_ptr<pfcp_association>(nullptr);
  if (get_association(node_id, sa)) {
    itti_inst->timer_remove(sa->timer_heartbeat);
    if (sa->recovery_time_stamp == recovery_time_stamp) {
      restore_n4_sessions = false;
    } else {
      restore_n4_sessions = true;
    }
    sa->recovery_time_stamp      = recovery_time_stamp;
    sa->function_features.first  = true;
    sa->function_features.second = function_features;
  } else {
    if (node_id.node_id_type == pfcp::NODE_ID_TYPE_FQDN) {
      Logger::smf_app().info("Node ID Type FQDN: %s", node_id.fqdn.c_str());
      struct hostent* record = gethostbyname(node_id.fqdn.c_str());
      if (record == NULL) {
        Logger::smf_app().info(
            "Add association with node (FQDN) %s: cannot resolve the hostname!",
            node_id.fqdn.c_str());
        return false;
      }
      if (record->h_addrtype == AF_INET) {
        in_addr* address        = (struct in_addr*) record->h_addr;
        node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
        node_id.u1.ipv4_address = *address;
        Logger::smf_app().info(
            "Node ID Type FQDN: %s, IPv4 Addr: %s", node_id.fqdn.c_str(),
            inet_ntoa(*address));
      } else if (record->h_addrtype == AF_INET6) {
        // TODO
        Logger::smf_app().info(
            "Node ID Type FQDN: %s. IPv6 Addr, this mode has not been "
            "supported yet!",
            node_id.fqdn.c_str());
        return false;
      } else {
        return false;
      }
    }

    restore_n4_sessions = false;
    pfcp_association* association =
        new pfcp_association(node_id, recovery_time_stamp, function_features);
    sa                      = std::shared_ptr<pfcp_association>(association);
    sa->recovery_time_stamp = recovery_time_stamp;
    sa->function_features.first  = true;
    sa->function_features.second = function_features;
    std::size_t hash_node_id     = std::hash<pfcp::node_id_t>{}(node_id);
    // Associate with UPF profile if exist
    for (std::vector<std::shared_ptr<pfcp_association>>::iterator it =
             pending_associations.begin();
         it < pending_associations.end(); ++it) {
      if (((*it)->node_id == node_id) and ((*it)->is_upf_profile_set())) {
        Logger::smf_app().info("Associate with UPF profile");
        sa->set_upf_node_profile((*it)->get_upf_node_profile());
        break;
      }
    }
    associations.insert((int32_t) hash_node_id, sa);
    trigger_heartbeat_request_procedure(sa);
  }
  return true;
}
//------------------------------------------------------------------------------
bool pfcp_associations::add_association(
    pfcp::node_id_t& node_id, pfcp::recovery_time_stamp_t& recovery_time_stamp,
    pfcp::up_function_features_s& function_features,
    pfcp::enterprise_specific_s& enterprise_specific,
    bool& restore_n4_sessions) {
  std::shared_ptr<pfcp_association> sa =
      std::shared_ptr<pfcp_association>(nullptr);
  if (get_association(node_id, sa)) {
    itti_inst->timer_remove(sa->timer_heartbeat);
    if (sa->recovery_time_stamp == recovery_time_stamp) {
      restore_n4_sessions = false;
    } else {
      restore_n4_sessions = true;
    }
    sa->recovery_time_stamp      = recovery_time_stamp;
    sa->function_features.first  = true;
    sa->function_features.second = function_features;
  } else {
    restore_n4_sessions = false;
    pfcp_association* association =
        new pfcp_association(node_id, recovery_time_stamp, function_features);
    sa                      = std::shared_ptr<pfcp_association>(association);
    sa->recovery_time_stamp = recovery_time_stamp;
    sa->function_features.first  = true;
    sa->function_features.second = function_features;
    std::size_t hash_node_id     = std::hash<pfcp::node_id_t>{}(node_id);
    associations.insert((int32_t) hash_node_id, sa);
    trigger_heartbeat_request_procedure(sa);
  }
  return true;
}

//------------------------------------------------------------------------------
bool pfcp_associations::update_association(
    pfcp::node_id_t& node_id, pfcp::up_function_features_s& function_features) {
  std::shared_ptr<pfcp_association> sa =
      std::shared_ptr<pfcp_association>(nullptr);
  if (get_association(node_id, sa)) {
    sa->function_features.first  = true;
    sa->function_features.second = function_features;
  } else {
    return false;
  }
  return true;
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
void pfcp_associations::restore_n4_sessions(const pfcp::node_id_t& node_id) {
  std::shared_ptr<pfcp_association> sa = {};
  if (get_association(node_id, sa)) {
    sa->restore_n4_sessions();
  }
}

//------------------------------------------------------------------------------
void pfcp_associations::trigger_heartbeat_request_procedure(
    std::shared_ptr<pfcp_association>& s) {
  s->timer_heartbeat = itti_inst->timer_setup(
      PFCP_ASSOCIATION_HEARTBEAT_INTERVAL_SEC, 0, TASK_SMF_N4,
      TASK_SMF_N4_TRIGGER_HEARTBEAT_REQUEST, s->hash_node_id);
}

//------------------------------------------------------------------------------
void pfcp_associations::initiate_heartbeat_request(
    timer_id_t timer_id, uint64_t arg2_user) {
  size_t hash_node_id = (size_t) arg2_user;
  auto pit            = associations.find((int32_t) hash_node_id);
  if (pit == associations.end())
    return;
  else {
    Logger::smf_n4().info(
        "PFCP HEARTBEAT PROCEDURE hash %u starting", hash_node_id);
    pit->second->num_retries_timer_heartbeat = 0;
    smf_n4_inst->send_heartbeat_request(pit->second);
  }
}

//------------------------------------------------------------------------------
void pfcp_associations::timeout_heartbeat_request(
    timer_id_t timer_id, uint64_t arg2_user) {
  size_t hash_node_id = (size_t) arg2_user;
  auto pit            = associations.find((int32_t) hash_node_id);
  if (pit == associations.end())
    return;
  else {
    if (pit->second->num_retries_timer_heartbeat <
        PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES) {
      Logger::smf_n4().info(
          "PFCP HEARTBEAT PROCEDURE hash %u TIMED OUT (retrie %d)",
          hash_node_id, pit->second->num_retries_timer_heartbeat);
      pit->second->num_retries_timer_heartbeat++;
      smf_n4_inst->send_heartbeat_request(pit->second);
    } else {
      Logger::smf_n4().warn(
          "PFCP HEARTBEAT PROCEDURE FAILED after %d retries, remove the "
          "association with this UPF",
          PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES);
      // Related session contexts and PFCP associations become invalid and may
      // be deleted-> Send request to SMF App to remove all associated sessions
      // and notify AMF accordingly
      std::shared_ptr<itti_n4_node_failure> itti_msg =
          std::make_shared<itti_n4_node_failure>(TASK_SMF_N4, TASK_SMF_APP);
      itti_msg->node_id = pit->second->node_id;
      int ret           = itti_inst->send_msg(itti_msg);
      if (RETURNok != ret) {
        Logger::smf_n4().error(
            "Could not send ITTI message %s to task TASK_SMF_APP",
            itti_msg->get_msg_name());
      }

      // Remove UPF from the associations
      remove_association(hash_node_id);
    }
  }
}

//------------------------------------------------------------------------------
void pfcp_associations::timeout_release_request(
    timer_id_t timer_id, uint64_t arg2_user) {
  size_t hash_node_id = (size_t) arg2_user;
  auto pit            = associations.find((int32_t) hash_node_id);
  if (pit == associations.end())
    return;
  else {
    Logger::smf_n4().info("PFCP RELEASE REQUEST hash %u", hash_node_id);
    smf_n4_inst->send_release_request(pit->second);
  }
}

//------------------------------------------------------------------------------
void pfcp_associations::handle_receive_heartbeat_response(
    const uint64_t trxn_id) {
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;

  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> a = it->second;
    if (it->second->trxn_id_heartbeat == trxn_id) {
      itti_inst->timer_remove(it->second->timer_heartbeat);
      trigger_heartbeat_request_procedure(it->second);
      return;
    }
  }
}

//------------------------------------------------------------------------------
bool pfcp_associations::select_up_node(
    pfcp::node_id_t& node_id, const int node_selection_criteria) {
  node_id = {};
  if (associations.empty()) {
    return false;
  }
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;
  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> a = it->second;
    // TODO
    switch (node_selection_criteria) {
      case NODE_SELECTION_CRITERIA_BEST_MAX_HEARBEAT_RTT:
      case NODE_SELECTION_CRITERIA_MIN_PFCP_SESSIONS:
      case NODE_SELECTION_CRITERIA_MIN_UP_TIME:
      case NODE_SELECTION_CRITERIA_MAX_AVAILABLE_BW:
      case NODE_SELECTION_CRITERIA_NONE:
      default:
        node_id = it->second->node_id;
        return true;
        break;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
bool pfcp_associations::select_up_node(
    pfcp::node_id_t& node_id, const snssai_t& snssai, const std::string& dnn) {
  node_id = {};
  if (associations.empty()) {
    return false;
  }
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>>::iterator it;
  FOR_EACH(it, associations) {
    std::shared_ptr<pfcp_association> a = it->second;
    // get the first node id if there's no upf profile (get UPFs from conf file)
    if (!a->upf_profile_is_set) {
      node_id = it->second->node_id;
      Logger::smf_app().info(
          "Could not found UPF profile, select the first available UPF");
      return true;
    }
    // else, verify that UPF belongs to the same slice and supports this dnn
    std::vector<snssai_t> snssais = {};
    upf_info_t upf_info           = {};
    a->upf_node_profile.get_upf_info(upf_info);
    for (auto ui : upf_info.snssai_upf_info_list) {
      if (ui.snssai == snssai) {
        for (auto d : ui.dnn_upf_info_list) {
          if (d.dnn.compare(dnn) == 0) {
            node_id = it->second->node_id;
            Logger::smf_app().info(
                "Select the UPF for the corresponding DNN %s, NSSSAI (SD: %s, "
                "SST: %d) ",
                d.dnn.c_str(), snssai.sD, snssai.sST);
            return true;
          }
        }
      }
    }
  }
  return false;
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

//------------------------------------------------------------------------------
bool pfcp_associations::add_peer_candidate_node(
    const pfcp::node_id_t& node_id) {
  for (std::vector<std::shared_ptr<pfcp_association>>::iterator it =
           pending_associations.begin();
       it < pending_associations.end(); ++it) {
    if ((*it)->node_id == node_id) {
      // TODO purge sessions of this node
      Logger::smf_app().info("TODO purge sessions of this node");
      pending_associations.erase(it);
      break;
    }
  }
  pfcp_association* association = new pfcp_association(node_id);
  std::shared_ptr<pfcp_association> s =
      std::shared_ptr<pfcp_association>(association);
  pending_associations.push_back(s);
  return true;
}

//------------------------------------------------------------------------------
bool pfcp_associations::add_peer_candidate_node(
    const pfcp::node_id_t& node_id, const upf_profile& profile) {
  for (std::vector<std::shared_ptr<pfcp_association>>::iterator it =
           pending_associations.begin();
       it < pending_associations.end(); ++it) {
    if ((*it)->node_id == node_id) {
      // TODO purge sessions of this node
      Logger::smf_app().info("TODO purge sessions of this node");
      pending_associations.erase(it);
      break;
    }
  }
  pfcp_association* association = new pfcp_association(node_id);
  std::shared_ptr<pfcp_association> s =
      std::shared_ptr<pfcp_association>(association);
  s->set_upf_node_profile(profile);
  pending_associations.push_back(s);
  return true;
}

//------------------------------------------------------------------------------
bool pfcp_associations::remove_association(
    const std::string& node_instance_id) {
  // TODO
  return true;
}

//------------------------------------------------------------------------------
bool pfcp_associations::remove_association(const int32_t& hash_node_id) {
  if (associations.count(hash_node_id) > 0) {
    associations.erase(hash_node_id);
    return true;
  }
  return false;
}
