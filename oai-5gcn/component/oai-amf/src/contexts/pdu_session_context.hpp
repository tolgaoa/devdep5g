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

/*! \file pdu_session_context.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _PDU_SESSION_CONTEXT_H_
#define _PDU_SESSION_CONTEXT_H_

#include <string>

#include "amf.hpp"
#include "bstrlib.h"

class pdu_session_context {
 public:
  pdu_session_context();
  ~pdu_session_context();

  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  uint8_t req_type;
  uint8_t pdu_session_id;
  bstring n2sm;
  bool isn2sm_avaliable;
  bstring n1sm;
  bool isn1sm_avaliable;
  std::string dnn;
  std::string smf_addr;
  std::string smf_api_version;
  bool smf_available;
  std::string location;
  snssai_t snssai;
  plmn_t plmn;
  std::string smf_context_location;
};
#endif
