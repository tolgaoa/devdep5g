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

#ifndef _ITTI_AMF_N1_H_
#define _ITTI_AMF_N1_H_

#include "bstrlib.h"
#include "itti_msg.hpp"

class itti_msg_n1 : public itti_msg {
 public:
  itti_msg_n1(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {}
  itti_msg_n1(const itti_msg_n1& i) : itti_msg(i) {
    is_nas_signalling_estab_req = i.is_nas_signalling_estab_req;
    ran_ue_ngap_id              = i.ran_ue_ngap_id;
  }

 public:
  bool is_nas_signalling_estab_req;
  long amf_ue_ngap_id;
  uint32_t ran_ue_ngap_id;
};

class itti_uplink_nas_data_ind : public itti_msg_n1 {
 public:
  itti_uplink_nas_data_ind(const task_id_t origin, const task_id_t destination)
      : itti_msg_n1(UL_NAS_DATA_IND, origin, destination) {}
  itti_uplink_nas_data_ind(const itti_uplink_nas_data_ind& i)
      : itti_msg_n1(i) {}

 public:
  bstring nas_msg;
  std::string mcc;
  std::string mnc;
  bool is_guti_valid;
  std::string guti;
};

class itti_downlink_nas_transfer : public itti_msg_n1 {
 public:
  itti_downlink_nas_transfer(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n1(DOWNLINK_NAS_TRANSFER, origin, destination) {}
  itti_downlink_nas_transfer(const itti_downlink_nas_transfer& i)
      : itti_msg_n1(i) {}

 public:
  bstring dl_nas;
  bstring n2sm;
  bool is_n2sm_set;
  uint8_t pdu_session_id;
  std::string n2sm_info_type;
};

#endif
