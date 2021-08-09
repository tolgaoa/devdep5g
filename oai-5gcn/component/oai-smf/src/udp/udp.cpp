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

/*! \file udp.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "udp.hpp"

#include <cstdlib>

//------------------------------------------------------------------------------
void udp_application::handle_receive(
    char* recv_buffer, const std::size_t bytes_transferred,
    const endpoint& r_endpoint) {
  Logger::udp().warn("Missing implementation of interface udp_application\n");
}

//------------------------------------------------------------------------------
void udp_application::start_receive(
    udp_application* gtp_stack, const util::thread_sched_params& sched_params) {
  Logger::udp().warn("Missing implementation of interface udp_application\n");
}

//------------------------------------------------------------------------------
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
void udp_server::udp_read_loop(const util::thread_sched_params& sched_params) {
  endpoint r_endpoint   = {};
  size_t bytes_received = 0;

  sched_params.apply(TASK_NONE, Logger::udp());

  while (1) {
    r_endpoint.addr_storage_len = sizeof(struct sockaddr_storage);
    if ((bytes_received = recvfrom(
             socket_, recv_buffer_, UDP_RECV_BUFFER_SIZE, 0,
             (struct sockaddr*) &r_endpoint.addr_storage,
             &r_endpoint.addr_storage_len)) > 0) {
      app_->handle_receive(recv_buffer_, bytes_received, r_endpoint);
    } else {
      Logger::udp().error("Recvfrom failed %s\n", strerror(errno));
    }
  }
}
//------------------------------------------------------------------------------
int udp_server::create_socket(
    const struct in_addr& address, const uint16_t port) {
  struct sockaddr_in addr = {};
  int sd                  = 0;

  /*
   * Create UDP socket
   */
  if ((sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    /*
     * Socket creation has failed...
     */
    Logger::udp().error("Socket creation failed (%s)\n", strerror(errno));
    return errno;
  }

  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(port);
  addr.sin_addr.s_addr = address.s_addr;

  std::string ipv4 = conv::toString(address);
  Logger::udp().debug(
      "Creating new listen socket on address %s and port %" PRIu16 " ",
      ipv4.c_str(), port);

  if (bind(sd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in)) < 0) {
    /*
     * Bind failed
     */
    Logger::udp().error(
        "Socket bind failed (%s) for address %s and port %" PRIu16 " ",
        strerror(errno), ipv4.c_str(), port);
    close(sd);
    return errno;
  }
  sa_family = AF_INET;
  return sd;
}
//------------------------------------------------------------------------------
int udp_server::create_socket(
    const struct in6_addr& address, const uint16_t port) {
  struct sockaddr_in6 addr = {};
  int sd                   = 0;

  /*
   * Create UDP socket
   */
  if ((sd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
    /*
     * Socket creation has failed...
     */
    Logger::udp().error("Socket creation failed (%s)\n", strerror(errno));
    return errno;
  }

  addr.sin6_family = AF_INET6;
  addr.sin6_port   = htons(port);
  addr.sin6_addr   = address;

  std::string ipv6 = conv::toString(address);
  Logger::udp().debug(
      "Creating new listen socket on address %s and port %" PRIu16 "\n",
      ipv6.c_str(), port);

  if (bind(sd, (struct sockaddr*) &addr, sizeof(struct sockaddr_in6)) < 0) {
    /*
     * Bind failed
     */
    Logger::udp().error(
        "Socket bind failed (%s) for address %s and port %" PRIu16 "\n",
        strerror(errno), ipv6.c_str(), port);
    close(sd);
    return errno;
  }
  sa_family = AF_INET6;
  return sd;
}
//------------------------------------------------------------------------------
int udp_server::create_socket(const char* address, const uint16_t port_num) {
  unsigned char buf_in_addr[sizeof(struct in6_addr)];
  if (inet_pton(AF_INET, address, buf_in_addr) == 1) {
    struct in_addr addr4 = {};
    memcpy(&addr4, buf_in_addr, sizeof(struct in_addr));
    return create_socket(addr4, port_num);
  } else if (inet_pton(AF_INET6, address, buf_in_addr) == 1) {
    struct in6_addr addr6 = {};
    memcpy(&addr6, buf_in_addr, sizeof(struct in6_addr));
    return create_socket(addr6, port_num);
  } else {
    Logger::udp().error("udp_server::create_socket(%s:%d)", address, port_num);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    throw std::system_error(
        socket_, std::generic_category(), "UDP socket creation failed!");
  }
}
//------------------------------------------------------------------------------
void udp_server::start_receive(
    udp_application* app, const util::thread_sched_params& sched_params) {
  app_ = app;
  Logger::udp().trace("udp_server::start_receive");
  thread_ = std::thread(&udp_server::udp_read_loop, this, sched_params);
  thread_.detach();
}
