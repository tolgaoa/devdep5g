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

/*! \file sctp_server.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _SCTP_SERVER_H_
#define _SCTP_SERVER_H_

#include <thread>

#include "endpoint.hpp"
extern "C" {
#include <netinet/in.h>
#include <netinet/sctp.h>

#include "bstrlib.h"
}
#include <iostream>
#include <vector>

#define SCTP_RECV_BUFFER_SIZE 2048
#define SCTP_RC_ERROR -1
#define SCTP_RC_NORMAL_READ 0
#define SCTP_RC_DISCONNECT 1

namespace sctp {

typedef uint16_t sctp_stream_id_t;
typedef uint32_t sctp_assoc_id_t;

typedef struct sctp_association_s {
  struct sctp_association_s* next_assoc;  ///< Next association in the list
  struct sctp_association_s*
      previous_assoc;  ///< Previous association in the list
  int sd;              ///< Socket descriptor
  uint32_t ppid;       ///< Payload protocol Identifier
  uint16_t
      instreams;  ///< Number of input streams negociated for this connection
  uint16_t
      outstreams;  ///< Number of output strams negotiated for this connection
  sctp_assoc_id_t assoc_id;  ///< SCTP association id for the connection
  uint32_t messages_recv;    ///< Number of messages received on this connection
  uint32_t messages_sent;    ///< Number of messages sent on this connection

  struct sockaddr* peer_addresses;  ///< A list of peer addresses
  int nb_peer_addresses;
} sctp_association_t;

typedef struct sctp_descriptor_s {
  // List of connected peers
  struct sctp_association_s* available_connections_head;
  struct sctp_association_s* available_connections_tail;
  uint32_t number_of_connections;
  uint16_t nb_instreams;
  uint16_t nb_outstreams;
} sctp_descriptor_t;

class sctp_application {
 public:
  virtual void handle_receive(
      bstring payload, sctp_assoc_id_t assoc_id, sctp_stream_id_t stream,
      sctp_stream_id_t instreams, sctp_stream_id_t outstreams) = 0;
  virtual void handle_sctp_new_association(
      sctp_assoc_id_t assoc_id, sctp_stream_id_t instreams,
      sctp_stream_id_t outstreams)                            = 0;
  virtual void handle_sctp_shutdown(sctp_assoc_id_t assoc_id) = 0;
  virtual uint32_t getPpid()                                  = 0;
};

class sctp_server {
 public:
  sctp_server(const char* address, const uint16_t port_num);
  virtual ~sctp_server();
  int create_socket(const char* address, const uint16_t port_num);
  void start_receive(sctp_application* app);
  int sctp_send_msg(
      sctp_assoc_id_t sctp_assoc_id, sctp_stream_id_t stream, bstring* payload);

 private:
  static void* sctp_receiver_thread(void* arg);
  int getSocket();
  int sctp_read_from_socket(int sd, uint32_t m_ppid);
  int handle_assoc_change(
      int sd, uint32_t ppid, struct sctp_assoc_change* assoc_change);
  int sctp_handle_com_down(sctp_assoc_id_t assoc_id);
  int sctp_handle_reset(const sctp_assoc_id_t assoc_id);
  sctp_association_t* add_new_association(
      int sd, uint32_t ppid, struct sctp_assoc_change* sctp_assoc_changed);
  int sctp_get_localaddresses(
      int sock, struct sockaddr** local_addr, int* nb_local_addresses);
  int sctp_get_peeraddresses(
      int sock, struct sockaddr** remote_addr, int* nb_remote_addresses);
  sctp_association_t* sctp_is_assoc_in_list(sctp_assoc_id_t assoc_id);

  int socket_;
  sctp_application* app_;
  pthread_t thread_;
  sctp_descriptor_t sctp_desc;
  struct sockaddr_in serverAddr_;
  struct sctp_event_subscribe events_;
  std::vector<sctp_association_t*> sctp_ctx;
};

}  // namespace sctp

#endif
