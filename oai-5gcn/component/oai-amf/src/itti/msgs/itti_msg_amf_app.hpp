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

#ifndef _AMF_APP_ITTI_H_
#define _AMF_APP_ITTI_H_

#include <string>

#include "NgapIEsStruct.hpp"
#include "itti_msg.hpp"
using namespace ngap;
#include "bstrlib.h"

class itti_msg_amf_app : public itti_msg {
 public:
  itti_msg_amf_app(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {}

  itti_msg_amf_app(const itti_msg_amf_app& i) : itti_msg(i) {
    ran_ue_ngap_id = i.ran_ue_ngap_id;
    amf_ue_ngap_id = i.amf_ue_ngap_id;
  }

  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
};

class itti_nas_signalling_establishment_request : public itti_msg_amf_app {
 public:
  itti_nas_signalling_establishment_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(NAS_SIG_ESTAB_REQ, origin, destination) {}
  itti_nas_signalling_establishment_request(
      const itti_nas_signalling_establishment_request& i)
      : itti_msg_amf_app(i) {}
  int rrc_cause;
  int ueCtxReq;
  NrCgi_t cgi;
  Tai_t tai;
  bstring nas_buf;
  bool is_5g_s_tmsi_present;
  std::string _5g_s_tmsi;
};

class itti_n1n2_message_transfer_request : public itti_msg_amf_app {
 public:
  itti_n1n2_message_transfer_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_amf_app(N1N2_MESSAGE_TRANSFER_REQ, origin, destination) {}
  itti_n1n2_message_transfer_request(
      const itti_n1n2_message_transfer_request& i)
      : itti_msg_amf_app(i) {}

  std::string supi;
  bstring n1sm;
  bstring n2sm;
  bool is_n2sm_set;
  bool is_n1sm_set;
  uint8_t pdu_session_id;
  std::string n2sm_info_type;
  // other parameters
};

#endif
