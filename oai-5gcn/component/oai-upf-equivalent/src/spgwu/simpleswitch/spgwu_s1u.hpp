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

/*! \file spgwu_s1u.hpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#ifndef FILE_SGWU_S1U_HPP_SEEN
#define FILE_SGWU_S1U_HPP_SEEN

#include "endpoint.hpp"
#include "gtpv1u.hpp"
#include "itti_msg_s1u.hpp"
#include "msg_gtpv1u.hpp"

#include <linux/ip.h>
#include <linux/ipv6.h>
#include <netinet/in.h>
#include <thread>

namespace spgwu {

class spgwu_s1u : public gtpv1u::gtpu_l4_stack {
 private:
  std::thread::id thread_id;
  std::thread thread;

  void handle_receive_gtpv1u_msg(
      gtpv1u::gtpv1u_msg& msg, const endpoint& r_endpoint);
  void handle_receive_echo_request(
      gtpv1u::gtpv1u_msg& msg, const endpoint& r_endpoint);

 public:
  spgwu_s1u();
  spgwu_s1u(spgwu_s1u const&) = delete;
  void operator=(spgwu_s1u const&) = delete;

  // void handle_itti_msg (itti_s1u_echo_request& s) {};
  void handle_itti_msg(std::shared_ptr<itti_s1u_echo_response> m);
  void handle_itti_msg(std::shared_ptr<itti_s1u_error_indication> m);
  // void handle_itti_msg (itti_s1u_supported_extension_headers_notification& s)
  // {}; void handle_itti_msg (itti_s1u_end_marker& s) {};

  // void send_msg (itti_s1u_echo_request& s) {};
  // void send_msg (itti_s1u_echo_response& s);
  // void send_msg (itti_s1u_error_indication& s) {};
  // void send_msg (itti_s1u_supported_extension_headers_notification& s) {};
  // void send_msg (itti_s1u_end_marker& s) {};

  void handle_receive_s1u_msg(
      gtpv1u::gtpv1u_msg& msg, const endpoint& r_endpoint);
  void handle_receive(
      char* recv_buffer, const std::size_t bytes_transferred,
      const endpoint& r_endpoint);

  void send_g_pdu(
      const struct in_addr& peer_addr, const uint16_t peer_udp_port,
      const uint32_t tunnel_id, const char* send_buffer,
      const ssize_t num_bytes, uint8_t  qfi);
  void send_g_pdu(
      const struct in6_addr& peer_addr, const uint16_t peer_udp_port,
      const uint32_t tunnel_id, const char* send_buffer,
      const ssize_t num_bytes);

  void time_out_itti_event(const uint32_t timer_id);
  void report_error_indication(
      const endpoint& r_endpoint, const uint32_t tunnel_id);
};
}  // namespace spgwu
#endif /* FILE_SGWU_S1U_HPP_SEEN */
