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

#include "AMF-UE-NGAP-ID.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
AMF_UE_NGAP_ID::AMF_UE_NGAP_ID() {
  amfUeNgapId = 0;
}

//------------------------------------------------------------------------------
AMF_UE_NGAP_ID::~AMF_UE_NGAP_ID() {}

//------------------------------------------------------------------------------
void AMF_UE_NGAP_ID::setAMF_UE_NGAP_ID(unsigned long m_amfUeNgapId) {
  amfUeNgapId = m_amfUeNgapId;
}

//------------------------------------------------------------------------------
unsigned long AMF_UE_NGAP_ID::getAMF_UE_NGAP_ID() {
  return amfUeNgapId;
}

//------------------------------------------------------------------------------
bool AMF_UE_NGAP_ID::encode2AMF_UE_NGAP_ID(Ngap_AMF_UE_NGAP_ID_t& amfuengapid) {
  amfuengapid.size = 5;
  amfuengapid.buf  = (uint8_t*) calloc(1, amfuengapid.size);
  if (!amfuengapid.buf) return false;

  for (int i = 0; i < amfuengapid.size; i++) {
    amfuengapid.buf[i] = (amfUeNgapId & (0xff00000000 >> i * 8)) >>
                         ((amfuengapid.size - i - 1) * 8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool AMF_UE_NGAP_ID::decodefromAMF_UE_NGAP_ID(
    Ngap_AMF_UE_NGAP_ID_t& amfuengapid) {
  if (!amfuengapid.buf) return false;

  amfUeNgapId = 0;
  for (int i = 0; i < amfuengapid.size; i++) {
    amfUeNgapId = amfUeNgapId << 8;
    amfUeNgapId |= amfuengapid.buf[i];
  }

  return true;
}
}  // namespace ngap
