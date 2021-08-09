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

/*! \file gtpv2c.hpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/
#ifndef FILE_GTPV2C_HPP_SEEN
#define FILE_GTPV2C_HPP_SEEN

#include "3gpp_29.274.hpp"
#include "endpoint.hpp"
#include "itti.hpp"
#include "udp.hpp"
#include "uint_generator.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "msg_gtpv2c.hpp"

namespace gtpv2c {

static const uint16_t default_port = 2123;

class gtpv2c_procedure {
 public:
  std::shared_ptr<gtpv2c_msg> retry_msg;
  endpoint remote_endpoint;
  teid_t local_teid;  // for peer not responding
  timer_id_t retry_timer_id;
  timer_id_t proc_cleanup_timer_id;
  uint64_t gtpc_tx_id;
  uint8_t initial_msg_type;    // sent or received
  uint8_t triggered_msg_type;  // sent or received
  uint8_t retry_count;
  // Could add customized N3, and customized T3:
  // T3-RESPONSE timer and N3-REQUESTS counter setting is implementation
  // dependent. That is, the timers and counters may be configurable per
  // procedure. Multileg communications (e.g. Create Session Requests and
  // Responses) however require longer timer values and possibly a higher number
  // of retransmission attempts compared to single leg communication.
  gtpv2c_procedure()
      : retry_msg(),
        remote_endpoint(),
        local_teid(0),
        retry_timer_id(0),
        proc_cleanup_timer_id(0),
        gtpc_tx_id(0),
        initial_msg_type(0),
        triggered_msg_type(0),
        retry_count(0) {}

  gtpv2c_procedure(const gtpv2c_procedure& p)
      : retry_msg(p.retry_msg),
        remote_endpoint(p.remote_endpoint),
        local_teid(p.local_teid),
        retry_timer_id(p.retry_timer_id),
        proc_cleanup_timer_id(p.proc_cleanup_timer_id),
        gtpc_tx_id(p.gtpc_tx_id),
        initial_msg_type(p.initial_msg_type),
        triggered_msg_type(p.triggered_msg_type),
        retry_count(p.retry_count) {}
};

enum gtpv2c_transaction_action { DELETE_TX = 0, CONTINUE_TX };

class gtpv2c_stack : public udp_application {
#define GTPV2C_T3_RESPONSE_MS 1000
#define GTPV2C_N3_REQUESTS 3
#define GTPV2C_PROC_TIME_OUT_MS                                                \
  ((GTPV2C_T3_RESPONSE_MS) * (GTPV2C_N3_REQUESTS + 1) + 1000)

 protected:
  uint32_t id;
  udp_server udp_s;
  udp_server udp_s_allocated;

  // seems no need for atomic
  uint32_t seq_num;
  std::mutex m_seq_num;
  uint32_t restart_counter;

  std::map<uint64_t, uint32_t> gtpc_tx_id2seq_num;
  std::map<timer_id_t, uint32_t> proc_cleanup_timers;
  std::map<timer_id_t, uint32_t> msg_out_retry_timers;
  std::map<uint32_t, gtpv2c_procedure> pending_procedures;

  static const char* msg_type2cstr[256];

  uint32_t get_next_seq_num();

  static uint64_t generate_gtpc_tx_id() {
    return util::uint_uid_generator<uint64_t>::get_instance().get_uid();
  }

  static bool check_initial_message_type(const uint8_t initial);
  static bool check_triggered_message_type(
      const uint8_t initial, const uint8_t triggered);
  void start_proc_cleanup_timer(
      gtpv2c_procedure& p, uint32_t time_out_milli_seconds,
      const task_id_t& task_id, const uint32_t& seq_num);
  void start_msg_retry_timer(
      gtpv2c_procedure& p, uint32_t time_out_milli_seconds,
      const task_id_t& task_id, const uint32_t& seq_num);
  void stop_msg_retry_timer(gtpv2c_procedure& p);
  void stop_msg_retry_timer(timer_id_t& t);
  void stop_proc_cleanup_timer(gtpv2c_procedure& p);
  void notify_ul_error(const gtpv2c_procedure& p, const cause_value_e cause);

 public:
  static const uint8_t version = 2;
  gtpv2c_stack(
      const std::string& ip_address, const unsigned short port_num,
      const util::thread_sched_params& sched_param);
  virtual void handle_receive(
      char* recv_buffer, const std::size_t bytes_transferred,
      const endpoint& r_endpoint);
  virtual void notify_ul_error(
      const endpoint& r_endpoint, const teid_t l_teid,
      const cause_value_e cause, const uint64_t gtpc_tx_id);

  void handle_receive_message_cb(
      const gtpv2c_msg& msg, const endpoint& r_endpoint,
      const task_id_t& task_id, bool& error, uint64_t& gtpc_tx_id);

  // Path mangement messages
  virtual uint32_t send_initial_message(
      const endpoint& r_endpoint, const gtpv2c_echo_request& gtp_ies,
      const task_id_t& task_id, const uint64_t gtp_tx_id);
  virtual void send_triggered_message(
      const endpoint& r_endpoint, const gtpv2c_echo_response& gtp_ies,
      const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a = DELETE_TX);

  // Tunnel management messages
  virtual uint32_t send_initial_message(
      const endpoint& r_endpoint, const teid_t r_teid, const teid_t l_teid,
      const gtpv2c_create_session_request& gtp_ies, const task_id_t& task_id,
      const uint64_t gtp_tx_id);
  virtual uint32_t send_initial_message(
      const endpoint& r_endpoint, const teid_t r_teid, const teid_t l_teid,
      const gtpv2c_delete_session_request& gtp_ies, const task_id_t& task_id,
      const uint64_t gtp_tx_id);
  virtual uint32_t send_initial_message(
      const endpoint& r_endpoint, const teid_t r_teid, const teid_t l_teid,
      const gtpv2c_modify_bearer_request& gtp_ies, const task_id_t& task_id,
      const uint64_t gtp_tx_id);
  virtual uint32_t send_initial_message(
      const endpoint& r_endpoint, const teid_t r_teid, const teid_t l_teid,
      const gtpv2c_release_access_bearers_request& gtp_ies,
      const task_id_t& task_id, const uint64_t gtp_tx_id);
  virtual uint32_t send_initial_message(
      const endpoint& r_endpoint, const teid_t r_teid, const teid_t l_teid,
      const gtpv2c_downlink_data_notification& gtp_ies,
      const task_id_t& task_id, const uint64_t gtp_tx_id);

  virtual void send_triggered_message(
      const endpoint& r_endpoint, const teid_t teid,
      const gtpv2c_create_session_response& gtp_ies, const uint64_t gtp_tx_id,
      const gtpv2c_transaction_action& a = DELETE_TX);
  virtual void send_triggered_message(
      const endpoint& r_endpoint, const teid_t teid,
      const gtpv2c_delete_session_response& gtp_ies, const uint64_t gtp_tx_id,
      const gtpv2c_transaction_action& a = DELETE_TX);
  virtual void send_triggered_message(
      const endpoint& r_endpoint, const teid_t teid,
      const gtpv2c_modify_bearer_response& gtp_ies, const uint64_t gtp_tx_id,
      const gtpv2c_transaction_action& a = DELETE_TX);
  virtual void send_triggered_message(
      const endpoint& r_endpoint, const teid_t teid,
      const gtpv2c_release_access_bearers_response& gtp_ies,
      const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a = DELETE_TX);
  virtual void send_triggered_message(
      const endpoint& r_endpoint, const teid_t teid,
      const gtpv2c_downlink_data_notification_acknowledge& gtp_ies,
      const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a = DELETE_TX);

  void time_out_event(
      const uint32_t timer_id, const task_id_t& task_id, bool& error);
};
}  // namespace gtpv2c

#endif /* FILE_GTPV2C_HPP_SEEN */
