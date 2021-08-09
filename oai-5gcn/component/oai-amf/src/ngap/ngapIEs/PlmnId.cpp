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

#include "PlmnId.hpp"

#include "String2Value.hpp"

extern "C" {
#include <math.h>
}
#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PlmnId::PlmnId() {
  mcc_digit1 = 0;
  mcc_digit2 = 0;
  mcc_digit3 = 0;

  mnc_digit1 = 0;
  mnc_digit2 = 0;
  mnc_digit3 = 0;
}

//------------------------------------------------------------------------------
PlmnId::~PlmnId() {}

//------------------------------------------------------------------------------
void PlmnId::setMccMnc(const std::string mcc, const std::string mnc) {
  int mccValue = fromString<int>(mcc);
  int mncValue = fromString<int>(mnc);

  mcc_digit1 = mccValue / 100;
  mcc_digit2 = (mccValue - mcc_digit1 * 100) / 10;
  mcc_digit3 = mccValue % 10;

  if (mncValue > 99) {
    mnc_digit3 = mncValue / 100;
  } else {
    mnc_digit3 = 0xf;
  }
  mnc_digit1 = (uint8_t) floor((double) (mncValue % 100) / 10);
  mnc_digit2 = mncValue % 10;
}

//------------------------------------------------------------------------------
void PlmnId::getMcc(std::string& mcc) {
  int m_mcc = mcc_digit1 * 100 + mcc_digit2 * 10 + mcc_digit3;
  mcc       = to_string(m_mcc);
}

//------------------------------------------------------------------------------
void PlmnId::getMnc(std::string& mnc) {
  int m_mnc = 0;
  if (mnc_digit3 == 0xf) {
    m_mnc = mnc_digit1 * 10 + mnc_digit2;
    if (mnc_digit1 == 0) {
      mnc = "0" + to_string(m_mnc);
      return;
    }
  } else {
    m_mnc = mnc_digit3 * 100 + mnc_digit1 * 10 + mnc_digit2;
  }
  mnc = to_string(m_mnc);
}

//------------------------------------------------------------------------------
bool PlmnId::encode2octetstring(Ngap_PLMNIdentity_t& plmn) {
  plmn.size = 3;  // OCTET_STRING(SIZE(3))  9.3.3.5, 3gpp ts 38.413 V15.4.0
  uint8_t* buffer = (uint8_t*) calloc(1, 3 * sizeof(uint8_t));
  if (!buffer) return false;

  buffer[0] = 0x00 | ((mcc_digit2 & 0x0f) << 4) | (mcc_digit1 & 0x0f);
  buffer[1] = 0x00 | ((mnc_digit3 & 0x0f) << 4) | (mcc_digit3 & 0x0f);
  buffer[2] = 0x00 | ((mnc_digit2 & 0x0f) << 4) | (mnc_digit1 & 0x0f);
  plmn.buf  = buffer;

  return true;
}

//------------------------------------------------------------------------------
bool PlmnId::decodefromoctetstring(Ngap_PLMNIdentity_t& plmn) {
  if (!plmn.buf) return false;
  mcc_digit1 = plmn.buf[0] & 0x0f;
  mcc_digit2 = plmn.buf[0] >> 4;

  mcc_digit3 = plmn.buf[1] & 0x0f;
  mnc_digit3 = plmn.buf[1] >> 4;
  mnc_digit1 = plmn.buf[2] & 0x0f;
  mnc_digit2 = plmn.buf[2] >> 4;

  return true;
}

}  // namespace ngap
