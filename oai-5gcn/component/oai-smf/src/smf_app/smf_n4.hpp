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

/*! \file smf_n4.hpp
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr
 */

#ifndef FILE_SMF_N4_HPP_SEEN
#define FILE_SMF_N4_HPP_SEEN

#include <thread>

#include "itti_msg_n4.hpp"
#include "pfcp.hpp"
#include "smf_pfcp_association.hpp"

namespace smf {

#define TASK_SMF_N4_TRIGGER_HEARTBEAT_REQUEST (0)
#define TASK_SMF_N4_TIMEOUT_HEARTBEAT_REQUEST (1)
#define TASK_SMF_N4_TIMEOUT_ASSOCIATION_REQUEST (2)
#define TASK_SMF_N4_TIMEOUT_GRACEFUL_RELEASE_PERIOD (3)
class smf_n4 : public pfcp::pfcp_l4_stack {
 private:
  std::thread::id thread_id;
  std::thread thread;

  uint64_t recovery_time_stamp;  // timestamp in seconds

  pfcp::cp_function_features_t cp_function_features;

 public:
  smf_n4();
  smf_n4(smf_n4 const&) = delete;
  void operator=(smf_n4 const&) = delete;

  void handle_itti_msg(itti_n4_heartbeat_request& s){};
  void handle_itti_msg(itti_n4_heartbeat_response& s){};
  void handle_itti_msg(itti_n4_association_setup_request& s){};
  void handle_itti_msg(itti_n4_association_setup_response& s){};
  void handle_itti_msg(itti_n4_association_update_request& s){};
  void handle_itti_msg(itti_n4_association_update_response& s){};
  void handle_itti_msg(itti_n4_association_release_request& s){};
  void handle_itti_msg(itti_n4_association_release_response& s){};
  void handle_itti_msg(itti_n4_version_not_supported_response& s){};
  void handle_itti_msg(itti_n4_node_report_response& s){};
  void handle_itti_msg(itti_n4_session_set_deletion_request& s){};
  void handle_itti_msg(itti_n4_session_establishment_request& s){};
  void handle_itti_msg(itti_n4_session_modification_request& s){};
  void handle_itti_msg(itti_n4_session_deletion_request& s){};
  void handle_itti_msg(itti_n4_session_report_response& s){};

  void send_n4_msg(itti_n4_heartbeat_request& s){};
  void send_n4_msg(itti_n4_heartbeat_response& s){};
  void send_n4_msg(itti_n4_association_setup_request& s){};
  void send_n4_msg(itti_n4_association_setup_response& s);
  void send_n4_msg(itti_n4_association_update_request& s){};
  void send_n4_msg(itti_n4_association_update_response& s){};
  void send_n4_msg(itti_n4_association_release_request& s){};
  void send_n4_msg(itti_n4_association_release_response& s){};
  void send_n4_msg(itti_n4_version_not_supported_response& s){};
  void send_n4_msg(itti_n4_node_report_request& s){};
  void send_n4_msg(itti_n4_session_set_deletion_response& s){};
  void send_n4_msg(itti_n4_session_establishment_request& s);
  void send_n4_msg(itti_n4_session_modification_request& s);
  void send_n4_msg(itti_n4_session_deletion_request& s);
  void send_n4_msg(itti_n4_session_report_response& s);
  void send_association_setup_request(itti_n4_association_setup_request& i);

  void send_heartbeat_request(std::shared_ptr<pfcp_association>& a);
  void send_heartbeat_response(
      const endpoint& r_endpoint, const uint64_t trxn_id);

  void send_release_request(std::shared_ptr<pfcp_association>& a);

  void handle_receive_pfcp_msg(pfcp::pfcp_msg& msg, const endpoint& r_endpoint);
  void handle_receive(
      char* recv_buffer, const std::size_t bytes_transferred,
      const endpoint& r_endpoint);

  void handle_receive_heartbeat_request(
      pfcp::pfcp_msg& msg, const endpoint& r_endpoint);
  void handle_receive_heartbeat_response(
      pfcp::pfcp_msg& msg, const endpoint& r_endpoint);
  void handle_receive_association_setup_request(
      pfcp::pfcp_msg& msg, const endpoint& r_endpoint);
  void handle_receive_association_setup_response(
      pfcp::pfcp_msg& msg, const endpoint& remote_endpoint);
  void handle_receive_association_update_request(
      pfcp::pfcp_msg& msg, const endpoint& remote_endpoint);
  void handle_receive_association_release_response(
      pfcp::pfcp_msg& msg, const endpoint& remote_endpoint);
  void handle_receive_session_establishment_response(
      pfcp::pfcp_msg& msg, const endpoint& r_endpoint);
  void handle_receive_session_modification_response(
      pfcp::pfcp_msg& msg, const endpoint& r_endpoint);
  void handle_receive_session_deletion_response(
      pfcp::pfcp_msg& msg, const endpoint& r_endpoint);
  void handle_receive_session_report_request(
      pfcp::pfcp_msg& msg, const endpoint& r_endpoint);

  void time_out_itti_event(const uint32_t timer_id);
};
}  // namespace smf
#endif /* FILE_SMF_N4_HPP_SEEN */
