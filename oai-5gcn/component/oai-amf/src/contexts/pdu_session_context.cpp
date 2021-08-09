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

/*! \file pdu_session_context.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "pdu_session_context.hpp"

//------------------------------------------------------------------------------
pdu_session_context::pdu_session_context() {
  smf_available    = false;
  isn2sm_avaliable = false;
  isn1sm_avaliable = false;
  ran_ue_ngap_id   = 0;
  amf_ue_ngap_id   = 0;
  req_type         = 0;
  pdu_session_id   = 0;
  // bstring n2sm;
  isn2sm_avaliable = false;
  // bstring n1sm;
  isn1sm_avaliable = false;
  smf_available    = false;
  snssai           = {};
  plmn             = {};
}

//------------------------------------------------------------------------------
pdu_session_context::~pdu_session_context() {}
