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

/*! \file
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "RAN-UE-NGAP-ID.hpp"
//#include "String2Value.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
RAN_UE_NGAP_ID::RAN_UE_NGAP_ID() {
  ranUeNgapId = 0;
}

//------------------------------------------------------------------------------
RAN_UE_NGAP_ID::~RAN_UE_NGAP_ID() {}

//------------------------------------------------------------------------------
void RAN_UE_NGAP_ID::setRanUeNgapId(uint32_t m_ranuengapid) {
  ranUeNgapId = m_ranuengapid;
}

//------------------------------------------------------------------------------
uint32_t RAN_UE_NGAP_ID::getRanUeNgapId() {
  return ranUeNgapId;
}

//------------------------------------------------------------------------------
bool RAN_UE_NGAP_ID::encode2RAN_UE_NGAP_ID(Ngap_RAN_UE_NGAP_ID_t& ranuengapid) {
  ranuengapid = ranUeNgapId;
  return true;
}

//------------------------------------------------------------------------------
bool RAN_UE_NGAP_ID::decodefromRAN_UE_NGAP_ID(
    Ngap_RAN_UE_NGAP_ID_t& ranuengapid) {
  ranUeNgapId = ranuengapid;
  cout << "[LibNGAP]Received RanUeNgapId " << ranUeNgapId << endl;
  return true;
}
}  // namespace ngap
