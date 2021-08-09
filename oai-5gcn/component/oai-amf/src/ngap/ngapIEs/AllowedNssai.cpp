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

#include "AllowedNssai.hpp"

extern "C" {
#include "Ngap_AllowedNSSAI-Item.h"
// #include "Ngap_SliceSupportItem.h"
}

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
AllowedNSSAI::AllowedNSSAI() {
  snssai      = NULL;
  numofSnssai = 0;
}

//------------------------------------------------------------------------------
AllowedNSSAI::~AllowedNSSAI() {}

//------------------------------------------------------------------------------
void AllowedNSSAI::setAllowedNSSAI(S_NSSAI* m_snssai, int m_numofsnssai) {
  snssai      = m_snssai;
  numofSnssai = m_numofsnssai;
}

//------------------------------------------------------------------------------
bool AllowedNSSAI::getAllowedNSSAI(S_NSSAI*& m_snssai, int& m_numofsnssai) {
  m_snssai      = snssai;
  m_numofsnssai = numofSnssai;

  if (!snssai) return false;
  if (!numofSnssai) return false;

  return true;
}

//------------------------------------------------------------------------------
bool AllowedNSSAI::encode2AllowedNSSAI(Ngap_AllowedNSSAI_t* allowedNssaiList) {
  cout << "AllowedNSSAI::numOfSnssai	" << numofSnssai << endl;
  for (int i = 0; i < numofSnssai; i++) {
    Ngap_AllowedNSSAI_Item_t* allowednssaiitem =
        (Ngap_AllowedNSSAI_Item_t*) calloc(1, sizeof(Ngap_AllowedNSSAI_Item_t));
    if (!allowednssaiitem) return false;
    if (!snssai[i].encode2S_NSSAI(&allowednssaiitem->s_NSSAI)) return false;
    if (ASN_SEQUENCE_ADD(&allowedNssaiList->list, allowednssaiitem) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool AllowedNSSAI::decodefromAllowedNSSAI(
    Ngap_AllowedNSSAI_t* allowedNssaiList) {
  numofSnssai = allowedNssaiList->list.count;
  snssai      = new S_NSSAI[numofSnssai]();
  for (int i = 0; i < numofSnssai; i++) {
    if (!snssai[i].decodefromS_NSSAI(&allowedNssaiList->list.array[i]->s_NSSAI))
      return false;
  }
  return true;
}

}  // namespace ngap
