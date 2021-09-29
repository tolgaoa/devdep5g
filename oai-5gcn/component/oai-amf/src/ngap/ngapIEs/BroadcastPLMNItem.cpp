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

#include "BroadcastPLMNItem.hpp"

#include "PlmnId.hpp"
#include "S-NSSAI.hpp"

extern "C" {
#include "Ngap_BroadcastPLMNList.h"
#include "Ngap_SliceSupportItem.h"
}

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
BroadcastPLMNItem::BroadcastPLMNItem() {
  plmn        = NULL;
  snssai      = NULL;
  numOfSnssai = 0;
}

//------------------------------------------------------------------------------
BroadcastPLMNItem::~BroadcastPLMNItem() {}

//------------------------------------------------------------------------------
void BroadcastPLMNItem::setPlmnSliceSupportList(
    PlmnId* m_plmn, S_NSSAI* m_snssai, int num) {
  plmn        = m_plmn;
  snssai      = m_snssai;
  numOfSnssai = num;
}

//------------------------------------------------------------------------------
void BroadcastPLMNItem::getPlmnSliceSupportList(
    PlmnId*& m_plmn, S_NSSAI*& m_snssai, int& snssainum) {
  m_plmn    = plmn;
  m_snssai  = snssai;
  snssainum = numOfSnssai;
}

//------------------------------------------------------------------------------
bool BroadcastPLMNItem::encode2BroadcastPLMNItem(
    Ngap_BroadcastPLMNItem_t* plmnItem) {
  if (!plmn->encode2octetstring(plmnItem->pLMNIdentity)) return false;
  cout << "BroadcastPLMNItem::numOfSnssai	" << numOfSnssai << endl;
  for (int i = 0; i < numOfSnssai; i++) {
    Ngap_SliceSupportItem_t* slice =
        (Ngap_SliceSupportItem_t*) calloc(1, sizeof(Ngap_SliceSupportItem_t));
    if (!slice) return false;
    if (!snssai[i].encode2S_NSSAI(&slice->s_NSSAI)) return false;
    if (ASN_SEQUENCE_ADD(&plmnItem->tAISliceSupportList.list, slice) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool BroadcastPLMNItem::decodefromBroadcastPLMNItem(
    Ngap_BroadcastPLMNItem_t* pdu) {
  if (plmn == nullptr) plmn = new PlmnId();
  if (!plmn->decodefromoctetstring(pdu->pLMNIdentity)) return false;
  numOfSnssai = pdu->tAISliceSupportList.list.count;
  snssai      = new S_NSSAI[numOfSnssai]();
  for (int i = 0; i < numOfSnssai; i++) {
    if (!snssai[i].decodefromS_NSSAI(
            &pdu->tAISliceSupportList.list.array[i]->s_NSSAI))
      return false;
  }
  return true;
}

}  // namespace ngap
