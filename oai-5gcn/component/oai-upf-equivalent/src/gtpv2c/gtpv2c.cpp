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

/*! \file gtpv2c.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "common_root_types.h"
#include "gtpv2c.hpp"

#include <cstdlib>

using namespace gtpv2c;
using namespace std;

extern itti_mw* itti_inst;

static std::string string_to_hex(const std::string& input) {
  static const char* const lut = "0123456789ABCDEF";
  size_t len                   = input.length();

  std::string output;
  output.reserve(2 * len);
  for (size_t i = 0; i < len; ++i) {
    const unsigned char c = input[i];
    output.push_back(lut[c >> 4]);
    output.push_back(lut[c & 15]);
  }
  return output;
}

//------------------------------------------------------------------------------
gtpv2c_stack::gtpv2c_stack(
    const string& ip_address, const unsigned short port_num,
    const util::thread_sched_params& sched_params)
    : udp_s(udp_server(ip_address.c_str(), port_num)),
      udp_s_allocated(ip_address.c_str(), 0),
      m_seq_num() {
  timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  seq_num = (uint32_t) ts.tv_nsec & 0x7FFFFFFF;

  Logger::gtpv2_c().info(
      "gtpv2c_stack created listening to %s:%d initial seq num %d",
      ip_address.c_str(), port_num, seq_num);

  gtpc_tx_id2seq_num   = {};
  proc_cleanup_timers  = {};
  msg_out_retry_timers = {};
  pending_procedures   = {};

  id              = 0;
  restart_counter = 0;
  udp_s.start_receive(this, sched_params);
  udp_s_allocated.start_receive(this, sched_params);
}
//------------------------------------------------------------------------------
uint32_t gtpv2c_stack::get_next_seq_num() {
  std::unique_lock lock(m_seq_num);
  seq_num++;
  if (seq_num & 0x80000000) {
    seq_num = 0;
  }
  return seq_num;
}
//------------------------------------------------------------------------------
void gtpv2c_stack::handle_receive(
    char* recv_buffer, const std::size_t bytes_transferred,
    const endpoint& r_endpoint) {
  Logger::gtpv2_c().error(
      "TODO implement gtpv2c_stack::handle_receive in derived class");
}
//------------------------------------------------------------------------------
void gtpv2c_stack::notify_ul_error(
    const endpoint& r_endpoint, const teid_t teid, const cause_value_e cause,
    const uint64_t gtpc_tx_id) {
  Logger::gtpv2_c().error(
      "TODO implement gtpv2c_stack::notify_ul_error in derived class");
}
//------------------------------------------------------------------------------
bool gtpv2c_stack::check_initial_message_type(const uint8_t initial) {
  switch (initial) {
    case GTP_ECHO_REQUEST:
    case GTP_CREATE_SESSION_REQUEST:
    case GTP_MODIFY_BEARER_REQUEST:
    case GTP_DELETE_SESSION_REQUEST:
    case GTP_CHANGE_NOTIFICATION_REQUEST:
    case GTP_REMOTE_UE_REPORT_NOTIFICATION:
    case GTP_MODIFY_BEARER_COMMAND:
    case GTP_DELETE_BEARER_COMMAND:
    case GTP_BEARER_RESOURCE_COMMAND:
    case GTP_CREATE_BEARER_REQUEST:
    case GTP_UPDATE_BEARER_REQUEST:
    case GTP_DELETE_BEARER_REQUEST:
    case GTP_DELETE_PDN_CONNECTION_SET_REQUEST:
    case GTP_PGW_DOWNLINK_TRIGGERING_NOTIFICATION:
    case GTP_IDENTIFICATION_REQUEST:
    case GTP_CONTEXT_REQUEST:
    case GTP_FORWARD_RELOCATION_REQUEST:
    case GTP_FORWARD_RELOCATION_COMPLETE_NOTIFICATION:
    case GTP_FORWARD_ACCESS_CONTEXT_NOTIFICATION:
    case GTP_RELOCATION_CANCEL_REQUEST:
    case GTP_DETACH_NOTIFICATION:
    case GTP_ALERT_MME_NOTIFICATION:
    case GTP_UE_ACTIVITY_NOTIFICATION:
    case GTP_UE_REGISTRATION_QUERY_REQUEST:
    case GTP_CREATE_FORWARDING_TUNNEL_REQUEST:
    case GTP_SUSPEND_NOTIFICATION:
    case GTP_RESUME_NOTIFICATION:
    case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST:
    case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST:
    case GTP_RELEASE_ACCESS_BEARERS_REQUEST:
    case GTP_DOWNLINK_DATA_NOTIFICATION:
    case GTP_PGW_RESTART_NOTIFICATION:
    case GTP_UPDATE_PDN_CONNECTION_SET_REQUEST:
    case GTP_MODIFY_ACCESS_BEARERS_REQUEST:
    case GTP_MBMS_SESSION_START_REQUEST:
    case GTP_MBMS_SESSION_UPDATE_REQUEST:
    case GTP_MBMS_SESSION_STOP_REQUEST:
      return true;
      break;
    default:
      return false;
  }
}
//------------------------------------------------------------------------------
bool gtpv2c_stack::check_triggered_message_type(
    const uint8_t initial, const uint8_t triggered) {
  Logger::gtpv2_c().info(
      "check_triggered_message_type GTPV2-C msg type %d/%d", (int) initial,
      (int) triggered);
  switch (initial) {
    case GTP_ECHO_REQUEST:
    case GTP_CREATE_SESSION_REQUEST:
    case GTP_MODIFY_BEARER_REQUEST:
    case GTP_DELETE_SESSION_REQUEST:
    case GTP_CHANGE_NOTIFICATION_REQUEST:
    case GTP_REMOTE_UE_REPORT_NOTIFICATION:
    case GTP_MODIFY_BEARER_COMMAND:
    case GTP_DELETE_BEARER_COMMAND:
    case GTP_BEARER_RESOURCE_COMMAND:
    case GTP_CREATE_BEARER_REQUEST:
    case GTP_UPDATE_BEARER_REQUEST:
    case GTP_DELETE_BEARER_REQUEST:
    case GTP_DELETE_PDN_CONNECTION_SET_REQUEST:
    case GTP_PGW_DOWNLINK_TRIGGERING_NOTIFICATION:
    case GTP_IDENTIFICATION_REQUEST:
    case GTP_CONTEXT_REQUEST:
    case GTP_FORWARD_RELOCATION_REQUEST:
    case GTP_FORWARD_RELOCATION_COMPLETE_NOTIFICATION:
    case GTP_FORWARD_ACCESS_CONTEXT_NOTIFICATION:
    case GTP_RELOCATION_CANCEL_REQUEST:
    case GTP_DETACH_NOTIFICATION:
    case GTP_ALERT_MME_NOTIFICATION:
    case GTP_UE_ACTIVITY_NOTIFICATION:
    case GTP_UE_REGISTRATION_QUERY_REQUEST:
    case GTP_CREATE_FORWARDING_TUNNEL_REQUEST:
    case GTP_SUSPEND_NOTIFICATION:
    case GTP_RESUME_NOTIFICATION:
    case GTP_CREATE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST:
    case GTP_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST:
    case GTP_RELEASE_ACCESS_BEARERS_REQUEST:
    case GTP_DOWNLINK_DATA_NOTIFICATION:
    case GTP_PGW_RESTART_NOTIFICATION:
    case GTP_UPDATE_PDN_CONNECTION_SET_REQUEST:
    case GTP_MODIFY_ACCESS_BEARERS_REQUEST:
    case GTP_MBMS_SESSION_START_REQUEST:
    case GTP_MBMS_SESSION_UPDATE_REQUEST:
    case GTP_MBMS_SESSION_STOP_REQUEST:
      if (triggered == (initial + 1)) return true;
      break;
    default:
      if (triggered == GTP_VERSION_NOT_SUPPORTED_INDICATION) return true;
  }
  switch (triggered) {
    case GTP_CREATE_BEARER_REQUEST:
    case GTP_UPDATE_BEARER_REQUEST:
    case GTP_DELETE_BEARER_REQUEST:
      // TODO refine this
      return true;
      break;
    default:
      return false;
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::start_msg_retry_timer(
    gtpv2c_procedure& p, uint32_t time_out_milli_seconds,
    const task_id_t& task_id, const uint32_t& seq_num) {
  if (!p.retry_timer_id) {
    p.retry_timer_id = itti_inst->timer_setup(
        time_out_milli_seconds / 1000, time_out_milli_seconds % 1000, task_id);
    msg_out_retry_timers.insert(
        std::pair<timer_id_t, uint32_t>(p.retry_timer_id, seq_num));
#if TRACE_IS_ON
    Logger::gtpv2_c().trace(
        "Started Msg retry timer %d, proc " PROC_ID_FMT ", seq %d",
        p.retry_timer_id, p.gtpc_tx_id, seq_num);
#endif
  } else {
    Logger::gtpv2_c().error(
        "Try to overwrite Msg retry timer %d, proc " PROC_ID_FMT ", seq %d!",
        p.retry_timer_id, p.gtpc_tx_id, seq_num);
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::stop_msg_retry_timer(gtpv2c_procedure& p) {
  if (p.retry_timer_id) {
    itti_inst->timer_remove(p.retry_timer_id);
    msg_out_retry_timers.erase(p.retry_timer_id);
#if TRACE_IS_ON
    Logger::gtpv2_c().trace(
        "Stopped Msg retry timer %d, proc " PROC_ID_FMT ", seq %d",
        p.retry_timer_id, p.gtpc_tx_id, p.retry_msg->get_sequence_number());
#endif
    p.retry_timer_id = 0;
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::stop_msg_retry_timer(timer_id_t& t) {
  itti_inst->timer_remove(t);
  msg_out_retry_timers.erase(t);
#if TRACE_IS_ON
  Logger::gtpv2_c().trace("Stopped Msg retry timer %d", t);
#endif
}
//------------------------------------------------------------------------------
void gtpv2c_stack::start_proc_cleanup_timer(
    gtpv2c_procedure& p, uint32_t time_out_milli_seconds,
    const task_id_t& task_id, const uint32_t& seq_num) {
  if (!p.proc_cleanup_timer_id) {
    p.proc_cleanup_timer_id = itti_inst->timer_setup(
        time_out_milli_seconds / 1000, time_out_milli_seconds % 1000, task_id);
    proc_cleanup_timers.insert(
        std::pair<timer_id_t, uint32_t>(p.proc_cleanup_timer_id, seq_num));
#if TRACE_IS_ON
    Logger::gtpv2_c().trace(
        "Started proc cleanup timer %d, proc " PROC_ID_FMT " t-out %" PRIu32
        " ms",
        p.proc_cleanup_timer_id, p.gtpc_tx_id, time_out_milli_seconds);
#endif
  } else {
    Logger::gtpv2_c().error(
        "Try to overwrite proc cleanup timer %d, proc " PROC_ID_FMT
        " t-out %" PRIu32 " ms",
        p.proc_cleanup_timer_id, p.gtpc_tx_id, time_out_milli_seconds);
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::stop_proc_cleanup_timer(gtpv2c_procedure& p) {
  itti_inst->timer_remove(p.proc_cleanup_timer_id);
#if TRACE_IS_ON
  Logger::gtpv2_c().trace(
      "Stopped proc cleanup timer %d, proc " PROC_ID_FMT "",
      p.proc_cleanup_timer_id, p.gtpc_tx_id);
#endif
  msg_out_retry_timers.erase(p.proc_cleanup_timer_id);
  p.proc_cleanup_timer_id = 0;
}
//------------------------------------------------------------------------------
void gtpv2c_stack::handle_receive_message_cb(
    const gtpv2c_msg& msg, const endpoint& r_endpoint, const task_id_t& task_id,
    bool& error, uint64_t& gtpc_tx_id) {
  gtpc_tx_id = 0;
  error      = true;
  std::map<uint32_t, gtpv2c_procedure>::iterator it;
  it = pending_procedures.find(msg.get_sequence_number());
  if (it == pending_procedures.end()) {
    if (gtpv2c_stack::check_initial_message_type(msg.get_message_type())) {
      gtpv2c_procedure proc = {};
      proc.gtpc_tx_id       = generate_gtpc_tx_id();
      proc.initial_msg_type = msg.get_message_type();
      // TODO later 13.3 Detection and handling of requests which have timed out
      // at the originating entity if (msg_has_timestamp()) {
      // start_proc_cleanup_timer(proc, (N3+1) x T3, task_id,
      // msg.get_sequence_number()); } else
      start_proc_cleanup_timer(
          proc, GTPV2C_PROC_TIME_OUT_MS, task_id, msg.get_sequence_number());
      pending_procedures.insert(std::pair<uint32_t, gtpv2c_procedure>(
          msg.get_sequence_number(), proc));
      gtpc_tx_id2seq_num.insert(std::pair<uint64_t, uint32_t>(
          proc.gtpc_tx_id, msg.get_sequence_number()));
      error      = false;
      gtpc_tx_id = proc.gtpc_tx_id;
      Logger::gtpv2_c().info(
          "Received Initial GTPV2-C msg type %d, seq %d, proc " PROC_ID_FMT "",
          msg.get_message_type(), msg.get_sequence_number(), proc.gtpc_tx_id);
    } else {
      Logger::gtpv2_c().info(
          "Failed to check Initial message type, Silently discarding GTPV2-C "
          "msg type %d, seq %d",
          msg.get_message_type(), msg.get_sequence_number());
      error = true;
    }
    return;
  } else {
    //    Logger::gtpv2_c().info( "gtpv2c_procedure retry_timer_id        %d",
    //    it->second.retry_timer_id); Logger::gtpv2_c().info( "gtpv2c_procedure
    //    proc_cleanup_timer_id %d", it->second.proc_cleanup_timer_id);
    //    Logger::gtpv2_c().info( "gtpv2c_procedure gtpc_tx_id            %ld",
    //    it->second.gtpc_tx_id); Logger::gtpv2_c().info( "gtpv2c_procedure
    //    initial_msg_type      %d", it->second.initial_msg_type);
    //    Logger::gtpv2_c().info( "gtpv2c_procedure triggered_msg_type    %d",
    //    it->second.triggered_msg_type); Logger::gtpv2_c().info(
    //    "gtpv2c_procedure retry_count           %d", it->second.retry_count);

    uint8_t check_initial_msg_type = it->second.triggered_msg_type;
    if (!it->second.triggered_msg_type) {
      check_initial_msg_type = it->second.initial_msg_type;
    }
    if (gtpv2c_stack::check_triggered_message_type(
            check_initial_msg_type, msg.get_message_type())) {
      if (!it->second.triggered_msg_type) {
        it->second.triggered_msg_type = msg.get_message_type();
      }
      error      = false;
      gtpc_tx_id = it->second.gtpc_tx_id;
      if (it->second.retry_timer_id) {
        stop_msg_retry_timer(it->second);
      }
      Logger::gtpv2_c().info(
          "Received Triggered GTPV2-C msg type %d, seq %d, proc " PROC_ID_FMT
          "",
          msg.get_message_type(), msg.get_sequence_number(), gtpc_tx_id);
    } else {
      Logger::gtpv2_c().info(
          "Failed to check Triggered message type, Silently discarding GTPV2-C "
          "msg type %d, seq %d",
          msg.get_message_type(), msg.get_sequence_number());
      error = true;
    }
  }
}

//------------------------------------------------------------------------------
uint32_t gtpv2c_stack::send_initial_message(
    const endpoint& dest, const gtpv2c_echo_request& gtp_ies,
    const task_id_t& task_id, const uint64_t gtp_tx_id) {
  std::ostringstream oss(std::ostringstream::binary);
  gtpv2c_msg msg(gtp_ies);
  msg.set_sequence_number(get_next_seq_num());
  msg.dump_to(oss);
  // std::cout << string_to_hex(oss.str()) << std::endl;
  // std::cout << std::hex << "msg length 0x" << msg.get_message_length() <<
  // "msg seqnum 0x" << msg.get_sequence_number() << std::endl;
  std::string bstream = oss.str();

  Logger::gtpv2_c().trace(
      "Sending %s, seq %d, proc " PROC_ID_FMT " ", gtp_ies.get_msg_name(),
      msg.get_sequence_number(), gtp_tx_id);
  gtpv2c_procedure proc = {};
  proc.initial_msg_type = msg.get_message_type();
  proc.gtpc_tx_id       = gtp_tx_id;
  proc.retry_msg        = std::make_shared<gtpv2c_msg>(msg);
  proc.remote_endpoint  = dest;
  start_msg_retry_timer(
      proc, GTPV2C_T3_RESPONSE_MS, task_id, msg.get_sequence_number());
  start_proc_cleanup_timer(
      proc, GTPV2C_PROC_TIME_OUT_MS, task_id, msg.get_sequence_number());
  pending_procedures.insert(
      std::pair<uint32_t, gtpv2c_procedure>(msg.get_sequence_number(), proc));
  gtpc_tx_id2seq_num.insert(std::pair<uint64_t, uint32_t>(
      proc.gtpc_tx_id, msg.get_sequence_number()));

  udp_s_allocated.async_send_to(
      reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), dest);
  return msg.get_sequence_number();
}
//------------------------------------------------------------------------------
uint32_t gtpv2c_stack::send_initial_message(
    const endpoint& dest, const teid_t r_teid, const teid_t l_teid,
    const gtpv2c_create_session_request& gtp_ies, const task_id_t& task_id,
    const uint64_t gtp_tx_id) {
  std::ostringstream oss(std::ostringstream::binary);
  gtpv2c_msg msg(gtp_ies);
  msg.set_teid(r_teid);
  msg.set_sequence_number(get_next_seq_num());
  msg.dump_to(oss);
  // std::cout << string_to_hex(oss.str()) << std::endl;
  // std::cout << std::hex << "msg length 0x" << msg.get_message_length() <<
  // "msg seqnum 0x" << msg.get_sequence_number() << std::endl;
  std::string bstream = oss.str();

  Logger::gtpv2_c().trace(
      "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
      gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
      gtp_tx_id);
  gtpv2c_procedure proc = {};
  proc.initial_msg_type = msg.get_message_type();
  proc.gtpc_tx_id       = gtp_tx_id;
  proc.local_teid       = l_teid;
  proc.retry_msg        = std::make_shared<gtpv2c_msg>(msg);
  proc.remote_endpoint  = dest;
  start_msg_retry_timer(
      proc, GTPV2C_T3_RESPONSE_MS, task_id, msg.get_sequence_number());
  start_proc_cleanup_timer(
      proc, GTPV2C_PROC_TIME_OUT_MS, task_id, msg.get_sequence_number());
  pending_procedures.insert(
      std::pair<uint32_t, gtpv2c_procedure>(msg.get_sequence_number(), proc));
  gtpc_tx_id2seq_num.insert(std::pair<uint64_t, uint32_t>(
      proc.gtpc_tx_id, msg.get_sequence_number()));

  udp_s_allocated.async_send_to(
      reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), dest);
  return msg.get_sequence_number();
}
//------------------------------------------------------------------------------
uint32_t gtpv2c_stack::send_initial_message(
    const endpoint& dest, const teid_t r_teid, const teid_t l_teid,
    const gtpv2c_delete_session_request& gtp_ies, const task_id_t& task_id,
    const uint64_t gtp_tx_id) {
  std::ostringstream oss(std::ostringstream::binary);
  gtpv2c_msg msg(gtp_ies);
  msg.set_teid(r_teid);
  msg.set_sequence_number(get_next_seq_num());
  msg.dump_to(oss);
  std::string bstream = oss.str();

  Logger::gtpv2_c().trace(
      "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
      gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
      gtp_tx_id);
  gtpv2c_procedure proc = {};
  proc.initial_msg_type = msg.get_message_type();
  proc.gtpc_tx_id       = gtp_tx_id;
  proc.local_teid       = l_teid;
  proc.retry_msg        = std::make_shared<gtpv2c_msg>(msg);
  proc.remote_endpoint  = dest;
  start_msg_retry_timer(
      proc, GTPV2C_T3_RESPONSE_MS, task_id, msg.get_sequence_number());
  start_proc_cleanup_timer(
      proc, GTPV2C_PROC_TIME_OUT_MS, task_id, msg.get_sequence_number());
  pending_procedures.insert(
      std::pair<uint32_t, gtpv2c_procedure>(msg.get_sequence_number(), proc));
  gtpc_tx_id2seq_num.insert(std::pair<uint64_t, uint32_t>(
      proc.gtpc_tx_id, msg.get_sequence_number()));

  udp_s_allocated.async_send_to(
      reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), dest);
  return msg.get_sequence_number();
}
//------------------------------------------------------------------------------
uint32_t gtpv2c_stack::send_initial_message(
    const endpoint& dest, const teid_t r_teid, const teid_t l_teid,
    const gtpv2c_modify_bearer_request& gtp_ies, const task_id_t& task_id,
    const uint64_t gtp_tx_id) {
  std::ostringstream oss(std::ostringstream::binary);
  gtpv2c_msg msg(gtp_ies);
  msg.set_teid(r_teid);
  msg.set_sequence_number(get_next_seq_num());
  msg.dump_to(oss);
  std::string bstream = oss.str();

  Logger::gtpv2_c().trace(
      "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
      gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
      gtp_tx_id);
  gtpv2c_procedure proc = {};
  proc.initial_msg_type = msg.get_message_type();
  proc.gtpc_tx_id       = gtp_tx_id;
  proc.local_teid       = l_teid;
  proc.retry_msg        = std::make_shared<gtpv2c_msg>(msg);
  proc.remote_endpoint  = dest;
  start_msg_retry_timer(
      proc, GTPV2C_T3_RESPONSE_MS, task_id, msg.get_sequence_number());
  start_proc_cleanup_timer(
      proc, GTPV2C_PROC_TIME_OUT_MS, task_id, msg.get_sequence_number());
  pending_procedures.insert(
      std::pair<uint32_t, gtpv2c_procedure>(msg.get_sequence_number(), proc));
  gtpc_tx_id2seq_num.insert(std::pair<uint64_t, uint32_t>(
      proc.gtpc_tx_id, msg.get_sequence_number()));

  udp_s_allocated.async_send_to(
      reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), dest);
  return msg.get_sequence_number();
}
//------------------------------------------------------------------------------
uint32_t gtpv2c_stack::send_initial_message(
    const endpoint& dest, const teid_t r_teid, const teid_t l_teid,
    const gtpv2c_release_access_bearers_request& gtp_ies,
    const task_id_t& task_id, const uint64_t gtp_tx_id) {
  std::ostringstream oss(std::ostringstream::binary);
  gtpv2c_msg msg(gtp_ies);
  msg.set_teid(r_teid);
  msg.set_sequence_number(get_next_seq_num());
  msg.dump_to(oss);
  std::string bstream = oss.str();

  Logger::gtpv2_c().trace(
      "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
      gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
      gtp_tx_id);
  gtpv2c_procedure proc = {};
  proc.initial_msg_type = msg.get_message_type();
  proc.gtpc_tx_id       = gtp_tx_id;
  proc.local_teid       = l_teid;
  proc.retry_msg        = std::make_shared<gtpv2c_msg>(msg);
  proc.remote_endpoint  = dest;
  start_msg_retry_timer(
      proc, GTPV2C_T3_RESPONSE_MS, task_id, msg.get_sequence_number());
  start_proc_cleanup_timer(
      proc, GTPV2C_PROC_TIME_OUT_MS, task_id, msg.get_sequence_number());
  pending_procedures.insert(
      std::pair<uint32_t, gtpv2c_procedure>(msg.get_sequence_number(), proc));
  gtpc_tx_id2seq_num.insert(std::pair<uint64_t, uint32_t>(
      proc.gtpc_tx_id, msg.get_sequence_number()));

  udp_s_allocated.async_send_to(
      reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), dest);
  return msg.get_sequence_number();
}
//------------------------------------------------------------------------------
uint32_t gtpv2c_stack::send_initial_message(
    const endpoint& dest, const teid_t r_teid, const teid_t l_teid,
    const gtpv2c_downlink_data_notification& gtp_ies, const task_id_t& task_id,
    const uint64_t gtp_tx_id) {
  std::ostringstream oss(std::ostringstream::binary);
  gtpv2c_msg msg(gtp_ies);
  msg.set_teid(r_teid);
  msg.set_sequence_number(get_next_seq_num());
  msg.dump_to(oss);
  std::string bstream = oss.str();

  Logger::gtpv2_c().trace(
      "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
      gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
      gtp_tx_id);
  gtpv2c_procedure proc = {};
  proc.initial_msg_type = msg.get_message_type();
  proc.gtpc_tx_id       = gtp_tx_id;
  proc.local_teid       = l_teid;
  proc.retry_msg        = std::make_shared<gtpv2c_msg>(msg);
  proc.remote_endpoint  = dest;
  start_msg_retry_timer(
      proc, GTPV2C_T3_RESPONSE_MS, task_id, msg.get_sequence_number());
  start_proc_cleanup_timer(
      proc, GTPV2C_PROC_TIME_OUT_MS, task_id, msg.get_sequence_number());
  pending_procedures.insert(
      std::pair<uint32_t, gtpv2c_procedure>(msg.get_sequence_number(), proc));
  gtpc_tx_id2seq_num.insert(std::pair<uint64_t, uint32_t>(
      proc.gtpc_tx_id, msg.get_sequence_number()));

  udp_s_allocated.async_send_to(
      reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), dest);
  return msg.get_sequence_number();
}
//------------------------------------------------------------------------------
void gtpv2c_stack::send_triggered_message(
    const endpoint& dest, const gtpv2c_echo_response& gtp_ies,
    const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a) {
  std::map<uint64_t, uint32_t>::iterator it;
  it = gtpc_tx_id2seq_num.find(gtp_tx_id);
  if (it != gtpc_tx_id2seq_num.end()) {
    std::ostringstream oss(std::ostringstream::binary);
    gtpv2c_msg msg(gtp_ies);
    msg.set_sequence_number(it->second);
    msg.dump_to(oss);
    std::string bstream = oss.str();
    Logger::gtpv2_c().trace(
        "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
        gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
        gtp_tx_id);
    udp_s.async_send_to(
        reinterpret_cast<const char*>(bstream.c_str()), bstream.length(), dest);

    if (a == DELETE_TX) {
      std::map<uint32_t, gtpv2c_procedure>::iterator it_proc =
          pending_procedures.find(it->second);
      if (it_proc != pending_procedures.end()) {
        stop_proc_cleanup_timer(it_proc->second);
        pending_procedures.erase(it_proc);
      }
      gtpc_tx_id2seq_num.erase(it);
    }
  } else {
    Logger::gtpv2_c().error(
        "Sending %s, gtp_tx_id " PROC_ID_FMT " proc not found, discarded!",
        gtp_ies.get_msg_name(), gtp_tx_id);
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::send_triggered_message(
    const endpoint& r_endpoint, const teid_t r_teid,
    const gtpv2c_create_session_response& gtp_ies, const uint64_t gtp_tx_id,
    const gtpv2c_transaction_action& a) {
  std::map<uint64_t, uint32_t>::iterator it;
  it = gtpc_tx_id2seq_num.find(gtp_tx_id);
  if (it != gtpc_tx_id2seq_num.end()) {
    std::ostringstream oss(std::ostringstream::binary);
    gtpv2c_msg msg(gtp_ies);
    msg.set_teid(r_teid);
    msg.set_sequence_number(it->second);
    msg.dump_to(oss);
    std::string bstream = oss.str();
    Logger::gtpv2_c().trace(
        "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
        gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
        gtp_tx_id);
    udp_s.async_send_to(
        reinterpret_cast<const char*>(bstream.c_str()), bstream.length(),
        r_endpoint);

    if (a == DELETE_TX) {
      std::map<uint32_t, gtpv2c_procedure>::iterator it_proc =
          pending_procedures.find(it->second);
      if (it_proc != pending_procedures.end()) {
        stop_proc_cleanup_timer(it_proc->second);
        pending_procedures.erase(it_proc);
      }
      gtpc_tx_id2seq_num.erase(it);
    }
  } else {
    Logger::gtpv2_c().error(
        "Sending %s, gtp_tx_id " PROC_ID_FMT " proc not found, discarded!",
        gtp_ies.get_msg_name(), gtp_tx_id);
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::send_triggered_message(
    const endpoint& r_endpoint, const teid_t r_teid,
    const gtpv2c_delete_session_response& gtp_ies, const uint64_t gtp_tx_id,
    const gtpv2c_transaction_action& a) {
  std::map<uint64_t, uint32_t>::iterator it;
  it = gtpc_tx_id2seq_num.find(gtp_tx_id);
  if (it != gtpc_tx_id2seq_num.end()) {
    std::ostringstream oss(std::ostringstream::binary);
    gtpv2c_msg msg(gtp_ies);
    msg.set_teid(r_teid);
    msg.set_sequence_number(it->second);
    msg.dump_to(oss);
    std::string bstream = oss.str();
    Logger::gtpv2_c().trace(
        "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
        gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
        gtp_tx_id);
    udp_s.async_send_to(
        reinterpret_cast<const char*>(bstream.c_str()), bstream.length(),
        r_endpoint);

    if (a == DELETE_TX) {
      std::map<uint32_t, gtpv2c_procedure>::iterator it_proc =
          pending_procedures.find(it->second);
      if (it_proc != pending_procedures.end()) {
        stop_proc_cleanup_timer(it_proc->second);
        pending_procedures.erase(it_proc);
      }
      gtpc_tx_id2seq_num.erase(it);
    }
  } else {
    Logger::gtpv2_c().error(
        "Sending %s, gtp_tx_id " PROC_ID_FMT " proc not found, discarded!",
        gtp_ies.get_msg_name(), gtp_tx_id);
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::send_triggered_message(
    const endpoint& r_endpoint, const teid_t r_teid,
    const gtpv2c_modify_bearer_response& gtp_ies, const uint64_t gtp_tx_id,
    const gtpv2c_transaction_action& a) {
  std::map<uint64_t, uint32_t>::iterator it;
  it = gtpc_tx_id2seq_num.find(gtp_tx_id);
  if (it != gtpc_tx_id2seq_num.end()) {
    std::ostringstream oss(std::ostringstream::binary);
    gtpv2c_msg msg(gtp_ies);
    msg.set_teid(r_teid);
    msg.set_sequence_number(it->second);
    msg.dump_to(oss);
    std::string bstream = oss.str();
    Logger::gtpv2_c().trace(
        "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
        gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
        gtp_tx_id);
    udp_s.async_send_to(
        reinterpret_cast<const char*>(bstream.c_str()), bstream.length(),
        r_endpoint);

    if (a == DELETE_TX) {
      std::map<uint32_t, gtpv2c_procedure>::iterator it_proc =
          pending_procedures.find(it->second);
      if (it_proc != pending_procedures.end()) {
        stop_proc_cleanup_timer(it_proc->second);
        pending_procedures.erase(it_proc);
      }
      gtpc_tx_id2seq_num.erase(it);
    }
  } else {
    Logger::gtpv2_c().error(
        "Sending %s, gtp_tx_id " PROC_ID_FMT " proc not found, discarded!",
        gtp_ies.get_msg_name(), gtp_tx_id);
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::send_triggered_message(
    const endpoint& r_endpoint, const teid_t r_teid,
    const gtpv2c_release_access_bearers_response& gtp_ies,
    const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a) {
  std::map<uint64_t, uint32_t>::iterator it;
  it = gtpc_tx_id2seq_num.find(gtp_tx_id);
  if (it != gtpc_tx_id2seq_num.end()) {
    std::ostringstream oss(std::ostringstream::binary);
    gtpv2c_msg msg(gtp_ies);
    msg.set_teid(r_teid);
    msg.set_sequence_number(it->second);
    msg.dump_to(oss);
    std::string bstream = oss.str();
    Logger::gtpv2_c().trace(
        "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
        gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
        gtp_tx_id);
    udp_s.async_send_to(
        reinterpret_cast<const char*>(bstream.c_str()), bstream.length(),
        r_endpoint);

    if (a == DELETE_TX) {
      std::map<uint32_t, gtpv2c_procedure>::iterator it_proc =
          pending_procedures.find(it->second);
      if (it_proc != pending_procedures.end()) {
        stop_proc_cleanup_timer(it_proc->second);
        pending_procedures.erase(it_proc);
      }
      gtpc_tx_id2seq_num.erase(it);
    }
  } else {
    Logger::gtpv2_c().error(
        "Sending %s, gtp_tx_id " PROC_ID_FMT " proc not found, discarded!",
        gtp_ies.get_msg_name(), gtp_tx_id);
  }
}
//------------------------------------------------------------------------------
void gtpv2c_stack::send_triggered_message(
    const endpoint& r_endpoint, const teid_t r_teid,
    const gtpv2c_downlink_data_notification_acknowledge& gtp_ies,
    const uint64_t gtp_tx_id, const gtpv2c_transaction_action& a) {
  std::map<uint64_t, uint32_t>::iterator it;
  it = gtpc_tx_id2seq_num.find(gtp_tx_id);
  if (it != gtpc_tx_id2seq_num.end()) {
    std::ostringstream oss(std::ostringstream::binary);
    gtpv2c_msg msg(gtp_ies);
    msg.set_teid(r_teid);
    msg.set_sequence_number(it->second);
    msg.dump_to(oss);
    std::string bstream = oss.str();
    Logger::gtpv2_c().trace(
        "Sending %s, seq %d, teid " TEID_FMT ", proc " PROC_ID_FMT "",
        gtp_ies.get_msg_name(), msg.get_sequence_number(), msg.get_teid(),
        gtp_tx_id);
    udp_s.async_send_to(
        reinterpret_cast<const char*>(bstream.c_str()), bstream.length(),
        r_endpoint);

    if (a == DELETE_TX) {
      std::map<uint32_t, gtpv2c_procedure>::iterator it_proc =
          pending_procedures.find(it->second);
      if (it_proc != pending_procedures.end()) {
        stop_proc_cleanup_timer(it_proc->second);
        pending_procedures.erase(it_proc);
      }
      gtpc_tx_id2seq_num.erase(it);
    }
  } else {
    Logger::gtpv2_c().error(
        "Sending %s, gtp_tx_id " PROC_ID_FMT " proc not found, discarded!",
        gtp_ies.get_msg_name(), gtp_tx_id);
  }
}

//------------------------------------------------------------------------------
void gtpv2c_stack::time_out_event(
    const uint32_t timer_id, const task_id_t& task_id, bool& handled) {
  handled = false;
  std::map<timer_id_t, uint32_t>::iterator it =
      msg_out_retry_timers.find(timer_id);
  if (it != msg_out_retry_timers.end()) {
    std::map<uint32_t, gtpv2c_procedure>::iterator it_proc =
        pending_procedures.find(it->second);
    msg_out_retry_timers.erase(it);
    handled = true;
    if (it_proc != pending_procedures.end()) {
      if (it_proc->second.retry_count < GTPV2C_N3_REQUESTS) {
        it_proc->second.retry_count++;
        it_proc->second.retry_timer_id = 0;
        start_msg_retry_timer(
            it_proc->second, GTPV2C_T3_RESPONSE_MS, task_id,
            it_proc->second.retry_msg->get_sequence_number());
        // send again message
        Logger::gtpv2_c().trace(
            "Retry %d Sending msg type %d, seq %d", it_proc->second.retry_count,
            it_proc->second.retry_msg->get_message_type(),
            it_proc->second.retry_msg->get_sequence_number());
        std::ostringstream oss(std::ostringstream::binary);
        it_proc->second.retry_msg->dump_to(oss);
        std::string bstream = oss.str();
        udp_s.async_send_to(
            reinterpret_cast<const char*>(bstream.c_str()), bstream.length(),
            it_proc->second.remote_endpoint);
      } else {
        // abort procedure
        notify_ul_error(
            it_proc->second.remote_endpoint, it_proc->second.local_teid,
            cause_value_e::REMOTE_PEER_NOT_RESPONDING,
            it_proc->second.gtpc_tx_id);
        handled                               = true;
        it_proc->second.proc_cleanup_timer_id = 0;
        Logger::gtpv2_c().trace(
            "Delete proc " PROC_ID_FMT " Retry %d seq %d timer id %u",
            it_proc->second.gtpc_tx_id, it_proc->second.retry_count,
            it_proc->first, timer_id);
        pending_procedures.erase(it_proc);
      }
    }
  } else {
    it = proc_cleanup_timers.find(timer_id);
    if (it != proc_cleanup_timers.end()) {
      std::map<uint32_t, gtpv2c_procedure>::iterator it_proc =
          pending_procedures.find(it->second);
      proc_cleanup_timers.erase(it);
      handled = true;
      if (it_proc != pending_procedures.end()) {
        it_proc->second.proc_cleanup_timer_id = 0;
        Logger::gtpv2_c().trace(
            "Delete proc " PROC_ID_FMT " Retry %d seq %d timer id %u",
            it_proc->second.gtpc_tx_id, it_proc->second.retry_count,
            it_proc->first, timer_id);
        pending_procedures.erase(it_proc);
      }
    }
  }
}
