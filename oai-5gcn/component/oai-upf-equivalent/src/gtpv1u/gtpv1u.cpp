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
/*! \file gtpv1u.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "common_root_types.h"
#include "conversions.hpp"
#include "gtpu.h"
#include "gtpv1u.hpp"

#include <cstdlib>
#include <sched.h>

using namespace gtpv1u;
using namespace std;

extern itti_mw* itti_inst;

////------------------------------------------------------------------------------
// void udp_server::handle_receive(const int& error, std::size_t
// bytes_transferred)
//{
//  if (!error) {
//    Logger::udp().trace( "udp_server::handle_receive on %s:%d from %s:%d",
//        socket_.local_endpoint().address().to_string().c_str(),
//        socket_.local_endpoint().port(),
//        remote_endpoint_.address().to_string().c_str(),
//        remote_endpoint_.port());
//    if (app_) {
//      app_->handle_receive(recv_buffer_.data(), bytes_transferred,
//      remote_endpoint_);
//    } else {
//      Logger::udp().error( "No upper layer configured for handling UDP
//      packet");
//    }
//    start_receive(app_);
//  } else {
//    Logger::udp().error( "udp_server::handle_receive err=%s/%d: %s",
//    error.category().name(), error.value(), error.message());
//  }
//}

//------------------------------------------------------------------------------
gtpu_l4_stack::gtpu_l4_stack(
    const struct in_addr& address, const uint16_t port_num,
    const util::thread_sched_params& sched_params)
    : udp_s(udp_server(address, port_num)) {
  Logger::gtpv1_u().info(
      "gtpu_l4_stack created listening to %s:%d",
      conv::toString(address).c_str(), port_num);

  id = 0;
  srand(time(NULL));
  seq_num         = rand() & 0x7FFFFFFF;
  restart_counter = 0;
  udp_s.start_receive(this, sched_params);
}
//------------------------------------------------------------------------------
gtpu_l4_stack::gtpu_l4_stack(
    const struct in6_addr& address, const uint16_t port_num,
    const util::thread_sched_params& sched_params)
    : udp_s(udp_server(address, port_num)) {
  Logger::gtpv1_u().info(
      "gtpu_l4_stack created listening to %s:%d",
      conv::toString(address).c_str(), port_num);

  id = 0;
  srand(time(NULL));
  seq_num         = rand() & 0x7FFFFFFF;
  restart_counter = 0;
  udp_s.start_receive(this, sched_params);
}
//------------------------------------------------------------------------------
gtpu_l4_stack::gtpu_l4_stack(
    char* address, const uint16_t port_num,
    const util::thread_sched_params& sched_params)
    : udp_s(udp_server(address, port_num)) {
  Logger::gtpv1_u().info(
      "gtpu_l4_stack created listening to %s:%d", address, port_num);

  id = 0;
  srand(time(NULL));
  seq_num         = rand() & 0x7FFFFFFF;
  restart_counter = 0;
  udp_s.start_receive(this, sched_params);
}

//------------------------------------------------------------------------------
uint32_t gtpu_l4_stack::get_next_seq_num() {
  seq_num++;
  if (seq_num & 0x80000000) {
    seq_num = 0;
  }
  return seq_num;
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::handle_receive(
    char* recv_buffer, const std::size_t bytes_transferred,
    const endpoint& r_endpoint) {
  Logger::gtpv1_u().error("TODO implement in derived class");
}
//------------------------------------------------------------------------------
bool gtpu_l4_stack::check_initial_message_type(const uint8_t initial) {
  switch (initial) {
    case GTPU_ECHO_REQUEST:
    case GTPU_END_MARKER:
      return true;
      break;
    default:
      return false;
  }
}
//------------------------------------------------------------------------------
bool gtpu_l4_stack::check_triggered_message_type(
    const uint8_t initial, const uint8_t triggered) {
  Logger::gtpv1_u().info(
      "check_triggered_message_type GTPV1-U msg type %d/%d", (int) initial,
      (int) triggered);
  switch (initial) {
    case GTPU_ECHO_REQUEST:
      if (triggered == GTPU_ECHO_RESPONSE) return true;
      return false;
      break;
    case GTPU_ERROR_INDICATION:
    case GTPU_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION:
      return true;
      break;
    default:
      return false;
  }
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::handle_receive_message_cb(
    const gtpv1u_msg& msg, const struct sockaddr_storage& r_endpoint,
    const socklen_t& r_endpoint_addr_len, const task_id_t& task_id, bool& error,
    uint64_t& gtpc_tx_id) {}

//------------------------------------------------------------------------------
void gtpu_l4_stack::send_g_pdu(
    const struct sockaddr_in& peer_addr, const teid_t teid, const char* payload,
    const ssize_t payload_len, uint8_t qfi) {
/*    struct gtpuhdr* gtpuhdr = reinterpret_cast<struct gtpuhdr*>(
      reinterpret_cast<uintptr_t>(payload) -
      (uintptr_t) sizeof(struct gtpuhdr));
  gtpuhdr->spare          = 0;
  gtpuhdr->e              = 0;
  gtpuhdr->s              = 0;
  gtpuhdr->pn             = 0;
  gtpuhdr->pt             = 1;
  gtpuhdr->version        = 1;
  gtpuhdr->message_type   = GTPU_G_PDU;
  gtpuhdr->message_length = htobe16(payload_len);
  gtpuhdr->teid           = htobe32(teid);
  udp_s.async_send_to(
      reinterpret_cast<const char*>(gtpuhdr),
      payload_len + sizeof(struct gtpuhdr), peer_addr);*/
  
 // test this for multiple extension header
   struct gtpu_ext_hdr* gtpu_ext_hdr = reinterpret_cast<struct gtpu_ext_hdr*>(
     reinterpret_cast<uintptr_t>(payload) -
       (uintptr_t) sizeof(struct gtpu_ext_hdr));
  gtpu_ext_hdr->gtpu_hdr.spare          = 0;
//  gtpuhdr->e              = 0;
  gtpu_ext_hdr->gtpu_hdr.e              = 1;
  gtpu_ext_hdr->gtpu_hdr.s              = 0;
  gtpu_ext_hdr->gtpu_hdr.pn             = 0;
  gtpu_ext_hdr->gtpu_hdr.pt             = 1;
  gtpu_ext_hdr->gtpu_hdr.version        = 1;
  gtpu_ext_hdr->gtpu_hdr.message_type   = GTPU_G_PDU;
//  gtpuhdr->message_length = htobe16(payload_len);
  gtpu_ext_hdr->gtpu_hdr.message_length = htobe16(payload_len + 8);
  gtpu_ext_hdr->gtpu_hdr.teid           = htobe32(teid);

  gtpu_ext_hdr->gtpu_hdr.pdu_number     = 0x00;
  gtpu_ext_hdr->gtpu_hdr.sequence       = 0x00;
  gtpu_ext_hdr->gtpu_hdr.next_ext_type  = GTPU_PDU_SESSION_CONTAINER;
  gtpu_ext_hdr->message_length          = 0x01;
  gtpu_ext_hdr->pdu_type                = GTPU_DL_PDU_SESSION_INFORMATION;
  gtpu_ext_hdr->qfi                     = qfi; // Taken from uplink PDR
  gtpu_ext_hdr->next_ext_type           = GTPU_NO_MORE_EXTENSION_HEADER;

  udp_s.async_send_to(
      reinterpret_cast<const char*>(gtpu_ext_hdr),
//      payload_len + sizeof(struct gtpuhdr), peer_addr);
      payload_len + sizeof(struct gtpu_ext_hdr), peer_addr);
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::send_g_pdu(
    const struct sockaddr_in6& peer_addr, const teid_t teid,
    const char* payload, const ssize_t payload_len) {
  struct gtpuhdr* gtpuhdr = reinterpret_cast<struct gtpuhdr*>(
      reinterpret_cast<uintptr_t>(payload) -
      (uintptr_t) sizeof(struct gtpuhdr));
  gtpuhdr->spare          = 0;
  gtpuhdr->e              = 0;
  gtpuhdr->s              = 0;
  gtpuhdr->pn             = 0;
  gtpuhdr->pt             = 1;
  gtpuhdr->version        = 1;
  gtpuhdr->message_type   = GTPU_G_PDU;
  gtpuhdr->message_length = htobe16(payload_len);
  gtpuhdr->teid           = htobe32(teid);
  udp_s.async_send_to(
      reinterpret_cast<const char*>(gtpuhdr),
      payload_len + sizeof(struct gtpuhdr), peer_addr);
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::send_response(const gtpv1u_echo_response& gtp_ies) {
  std::ostringstream oss(std::ostringstream::binary);
  gtpv1u_msg msg(gtp_ies);
  uint32_t teid = UNASSIGNED_TEID;
  if (gtp_ies.get_teid(teid)) {
    msg.set_teid(teid);
  }
  uint16_t sn = 0;
  if (gtp_ies.get_sequence_number(sn)) {
    msg.set_sequence_number(sn);
  }
  msg.dump_to(oss);
  std::string bstream = oss.str();

  udp_s.async_send_to(
      reinterpret_cast<const char*>(bstream.c_str()), bstream.length(),
      gtp_ies.r_endpoint);
}
//------------------------------------------------------------------------------
void gtpu_l4_stack::send_indication(const gtpv1u_error_indication& gtp_ies) {
  std::ostringstream oss(std::ostringstream::binary);
  gtpv1u_msg msg(gtp_ies);
  uint32_t teid = UNASSIGNED_TEID;
  if (gtp_ies.get_teid(teid)) {
    msg.set_teid(teid);
  }
  uint16_t sn = 0;
  if (gtp_ies.get_sequence_number(sn)) {
    msg.set_sequence_number(sn);
  }
  msg.dump_to(oss);
  std::string bstream = oss.str();

  udp_s.async_send_to(
      reinterpret_cast<const char*>(bstream.c_str()), bstream.length(),
      gtp_ies.r_endpoint);
}
