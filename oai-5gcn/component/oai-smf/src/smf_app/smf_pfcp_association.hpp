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

/*! \file smf_pfcp_association.hpp
 \author  Lionel GAUTHIER
 \date 2019
 \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_SMF_PFCP_ASSOCIATION_HPP_SEEN
#define FILE_SMF_PFCP_ASSOCIATION_HPP_SEEN

#include <folly/AtomicHashMap.h>
#include <folly/AtomicLinkedList.h>
#include <mutex>
#include <vector>

#include "3gpp_29.244.h"
#include "itti.hpp"
#include "smf_profile.hpp"

namespace smf {

#define PFCP_ASSOCIATION_HEARTBEAT_INTERVAL_SEC 10
#define PFCP_ASSOCIATION_HEARTBEAT_MAX_RETRIES 2
#define PFCP_ASSOCIATION_GRACEFUL_RELEASE_PERIOD 5

class pfcp_association {
 public:
  pfcp::node_id_t node_id;
  std::size_t hash_node_id;
  pfcp::recovery_time_stamp_t recovery_time_stamp;
  std::pair<bool, pfcp::up_function_features_s> function_features;
  //
  mutable std::mutex m_sessions;
  std::set<pfcp::fseid_t> sessions;
  //
  timer_id_t timer_heartbeat;
  int num_retries_timer_heartbeat;
  uint64_t trxn_id_heartbeat;

  bool is_restore_sessions_pending;

  timer_id_t timer_association;
  timer_id_t timer_graceful_release;

  upf_profile upf_node_profile;
  bool upf_profile_is_set;

  explicit pfcp_association(const pfcp::node_id_t& node_id)
      : node_id(node_id),
        recovery_time_stamp(),
        function_features(),
        m_sessions(),
        sessions(),
        upf_node_profile(),
        upf_profile_is_set(false) {
    hash_node_id                = std::hash<pfcp::node_id_t>{}(node_id);
    timer_heartbeat             = ITTI_INVALID_TIMER_ID;
    num_retries_timer_heartbeat = 0;
    trxn_id_heartbeat           = 0;
    is_restore_sessions_pending = false;
    timer_association           = ITTI_INVALID_TIMER_ID;
    timer_graceful_release      = ITTI_INVALID_TIMER_ID;
  }
  pfcp_association(
      const pfcp::node_id_t& node_id,
      pfcp::recovery_time_stamp_t& recovery_time_stamp)
      : node_id(node_id),
        recovery_time_stamp(recovery_time_stamp),
        function_features(),
        m_sessions(),
        sessions(),
        upf_node_profile(),
        upf_profile_is_set(false) {
    hash_node_id                = std::hash<pfcp::node_id_t>{}(node_id);
    timer_heartbeat             = ITTI_INVALID_TIMER_ID;
    num_retries_timer_heartbeat = 0;
    trxn_id_heartbeat           = 0;
    timer_association           = ITTI_INVALID_TIMER_ID;
    is_restore_sessions_pending = false;
    timer_graceful_release      = ITTI_INVALID_TIMER_ID;
  }
  pfcp_association(
      const pfcp::node_id_t& ni, pfcp::recovery_time_stamp_t& rts,
      pfcp::up_function_features_s& uff)
      : node_id(ni),
        recovery_time_stamp(rts),
        m_sessions(),
        sessions(),
        upf_node_profile(),
        upf_profile_is_set(false) {
    hash_node_id                = std::hash<pfcp::node_id_t>{}(node_id);
    function_features.first     = true;
    function_features.second    = uff;
    timer_heartbeat             = ITTI_INVALID_TIMER_ID;
    num_retries_timer_heartbeat = 0;
    trxn_id_heartbeat           = 0;
    is_restore_sessions_pending = false;
    timer_association           = ITTI_INVALID_TIMER_ID;
    timer_graceful_release      = ITTI_INVALID_TIMER_ID;
  }
  pfcp_association(pfcp_association const& p)
      : node_id(p.node_id),
        hash_node_id(p.hash_node_id),
        recovery_time_stamp(p.recovery_time_stamp),
        function_features(p.function_features),
        timer_heartbeat(p.timer_heartbeat),
        num_retries_timer_heartbeat(p.num_retries_timer_heartbeat),
        trxn_id_heartbeat(p.trxn_id_heartbeat),
        is_restore_sessions_pending(p.is_restore_sessions_pending),
        timer_association(0),
        timer_graceful_release(0),
        upf_node_profile(p.upf_node_profile),
        upf_profile_is_set(p.upf_profile_is_set) {}

  void notify_add_session(const pfcp::fseid_t& cp_fseid);
  bool has_session(const pfcp::fseid_t& cp_fseid);
  void notify_del_session(const pfcp::fseid_t& cp_fseid);
  void del_sessions();
  void restore_n4_sessions();
  void set(const pfcp::up_function_features_s& ff) {
    function_features.first  = true;
    function_features.second = ff;
  };
  void set_upf_node_profile(const upf_profile& profile) {
    upf_node_profile   = profile;
    upf_profile_is_set = true;
  };
  bool is_upf_profile_set() { return upf_profile_is_set; }
  void get_upf_node_profile(upf_profile& profile) const {
    profile = upf_node_profile;
  };
  upf_profile get_upf_node_profile() const { return upf_node_profile; };
};

enum node_selection_criteria_e {
  NODE_SELECTION_CRITERIA_BEST_MAX_HEARBEAT_RTT = 0,
  NODE_SELECTION_CRITERIA_MIN_PFCP_SESSIONS     = 1,
  NODE_SELECTION_CRITERIA_MIN_UP_TIME           = 2,
  NODE_SELECTION_CRITERIA_MAX_AVAILABLE_BW      = 3,
  NODE_SELECTION_CRITERIA_NONE                  = 4
};

#define PFCP_MAX_ASSOCIATIONS 16

class pfcp_associations {
 private:
  std::vector<std::shared_ptr<pfcp_association>> pending_associations;
  folly::AtomicHashMap<int32_t, std::shared_ptr<pfcp_association>> associations;

  pfcp_associations()
      : associations(PFCP_MAX_ASSOCIATIONS), pending_associations(){};
  void trigger_heartbeat_request_procedure(
      std::shared_ptr<pfcp_association>& s);

 public:
  static pfcp_associations& get_instance() {
    static pfcp_associations instance;
    return instance;
  }

  pfcp_associations(pfcp_associations const&) = delete;
  void operator=(pfcp_associations const&) = delete;

  bool add_association(
      pfcp::node_id_t& node_id,
      pfcp::recovery_time_stamp_t& recovery_time_stamp,
      bool& restore_n4_sessions);
  bool add_association(
      pfcp::node_id_t& node_id,
      pfcp::recovery_time_stamp_t& recovery_time_stamp,
      pfcp::up_function_features_s& function_features,
      bool& restore_n4_sessions);
  bool add_association(
      pfcp::node_id_t& node_id,
      pfcp::recovery_time_stamp_t& recovery_time_stamp,
      pfcp::up_function_features_s& function_features,
      pfcp::enterprise_specific_s& enterprise_specific,
      bool& restore_n4_sessions);
  bool update_association(
      pfcp::node_id_t& node_id,
      pfcp::up_function_features_s& function_features);
  bool get_association(
      const pfcp::node_id_t& node_id,
      std::shared_ptr<pfcp_association>& sa) const;
  bool get_association(
      const pfcp::fseid_t& cp_fseid,
      std::shared_ptr<pfcp_association>& sa) const;

  void notify_add_session(
      const pfcp::node_id_t& node_id, const pfcp::fseid_t& cp_fseid);
  void notify_del_session(const pfcp::fseid_t& cp_fseid);

  void restore_n4_sessions(const pfcp::node_id_t& node_id);

  void initiate_heartbeat_request(timer_id_t timer_id, uint64_t arg2_user);
  void timeout_heartbeat_request(timer_id_t timer_id, uint64_t arg2_user);
  void timeout_release_request(timer_id_t timer_id, uint64_t arg2_user);
  void handle_receive_heartbeat_response(const uint64_t trxn_id);

  bool select_up_node(
      pfcp::node_id_t& node_id, const int node_selection_criteria);
  bool select_up_node(
      pfcp::node_id_t& node_id, const snssai_t& snssai, const std::string& dnn);
  bool add_peer_candidate_node(const pfcp::node_id_t& node_id);
  bool add_peer_candidate_node(
      const pfcp::node_id_t& node_id, const upf_profile& profile);
  bool remove_association(const std::string& node_instance_id);
  bool remove_association(const int32_t& hash_node_id);
};
}  // namespace smf

#endif /* FILE_SMF_PFCP_ASSOCIATION_HPP_SEEN */
