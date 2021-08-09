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

#include "UEIdentityIndexValue.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
UEIdentityIndexValue::UEIdentityIndexValue() {
  indexLength10 = 0;
}

//------------------------------------------------------------------------------
UEIdentityIndexValue::~UEIdentityIndexValue() {}

//------------------------------------------------------------------------------
void UEIdentityIndexValue::setUEIdentityIndexValue(
    uint16_t m_indexLength10 /*10bits*/) {
  indexLength10 = m_indexLength10;
}

//------------------------------------------------------------------------------
bool UEIdentityIndexValue::encode2UEIdentityIndexValue(
    Ngap_UEIdentityIndexValue_t* ueIdentityIndexValue) {
  ueIdentityIndexValue->present = Ngap_UEIdentityIndexValue_PR_indexLength10;
  ueIdentityIndexValue->choice.indexLength10.size        = sizeof(uint16_t);
  ueIdentityIndexValue->choice.indexLength10.bits_unused = 6;
  ueIdentityIndexValue->choice.indexLength10.buf =
      (uint8_t*) calloc(1, ueIdentityIndexValue->choice.indexLength10.size);
  if (!ueIdentityIndexValue->choice.indexLength10.buf) return false;
  ueIdentityIndexValue->choice.indexLength10.buf[0] =
      (indexLength10 >> 8) & 0x03;
  ueIdentityIndexValue->choice.indexLength10.buf[1] = indexLength10 & 0xff;

  return true;
}

//------------------------------------------------------------------------------
bool UEIdentityIndexValue::decodefromUEIdentityIndexValue(
    Ngap_UEIdentityIndexValue_t* ueIdentityIndexValue) {
  if (ueIdentityIndexValue->present !=
      Ngap_UEIdentityIndexValue_PR_indexLength10)
    return false;
  if (!ueIdentityIndexValue->choice.indexLength10.buf) return false;
  indexLength10 = ueIdentityIndexValue->choice.indexLength10.buf[0];
  indexLength10 = indexLength10 << 8;
  indexLength10 |= ueIdentityIndexValue->choice.indexLength10.buf[1];

  return true;
}

//------------------------------------------------------------------------------
void UEIdentityIndexValue::getUEIdentityIndexValue(
    uint16_t& m_indexLength10 /*10bits*/) {
  m_indexLength10 = indexLength10;
}

}  // namespace ngap
