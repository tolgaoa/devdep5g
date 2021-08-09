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

/*
 *  itti_msg_nx.hpp
 *
 *  Created on:
 *  Author:
 */

#ifndef ITTI_MSG_NX_HPP_INCLUDED_
#define ITTI_MSG_NX_HPP_INCLUDED_

#include "itti_msg.hpp"
#include "smf_msg.hpp"
#include "pistache/http.h"

class itti_nx_msg : public itti_msg {
 public:
  itti_nx_msg(
      const itti_msg_type_t msg_type, const task_id_t orig,
      const task_id_t dest)
      : itti_msg(msg_type, orig, dest) {}
  itti_nx_msg(const itti_nx_msg& i) : itti_msg(i) {}
  itti_nx_msg(const itti_nx_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_nx_msg(i) {
    origin      = orig;
    destination = dest;
  }
};

//-----------------------------------------------------------------------------
class itti_nx_trigger_pdu_session_modification : public itti_nx_msg {
 public:
  itti_nx_trigger_pdu_session_modification(
      const task_id_t orig, const task_id_t dest)
      : itti_nx_msg(NX_TRIGGER_SESSION_MODIFICATION, orig, dest),
        msg(),
        http_version() {}
  itti_nx_trigger_pdu_session_modification(
      const itti_nx_trigger_pdu_session_modification& i)
      : itti_nx_msg(i), msg(i.msg), http_version(i.http_version) {}
  itti_nx_trigger_pdu_session_modification(
      const itti_nx_trigger_pdu_session_modification& i, const task_id_t orig,
      const task_id_t dest)
      : itti_nx_msg(i, orig, dest), msg(), http_version(i.http_version) {}
  const char* get_msg_name() { return "NX_TRIGGER_PDU_SESSION_MODIFICATION"; };
  smf::pdu_session_modification_network_requested msg;
  uint8_t http_version;
};

#endif /* ITTI_MSG_NX_HPP_INCLUDED_ */
