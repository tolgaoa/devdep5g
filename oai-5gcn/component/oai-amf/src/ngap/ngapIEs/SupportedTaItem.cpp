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

#include "SupportedTaItem.hpp"

#include "BroadcastPLMNItem.hpp"
#include "String2Value.hpp"

extern "C" {
#include "Ngap_BroadcastPLMNItem.h"
#include "Ngap_SupportedTAItem.h"
}

#include <string>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
SupportedTaItem::SupportedTaItem() {
  tac                   = NULL;
  broadcastPLMNItem     = NULL;
  numberOfBroadcastItem = 0;
}

//------------------------------------------------------------------------------
SupportedTaItem::~SupportedTaItem() {}

//------------------------------------------------------------------------------
void SupportedTaItem::setTac(TAC* m_tac) {
  tac = m_tac;
}

//------------------------------------------------------------------------------
void SupportedTaItem::getTac(TAC*& m_tac) {
  m_tac = tac;
}

//------------------------------------------------------------------------------
void SupportedTaItem::setBroadcastPlmnList(
    BroadcastPLMNItem* m_broadcastPLMNItem, int numOfItem) {
  numberOfBroadcastItem = numOfItem;
  broadcastPLMNItem     = m_broadcastPLMNItem;
}

//------------------------------------------------------------------------------
void SupportedTaItem::getBroadcastPlmnList(
    BroadcastPLMNItem*& m_broadcastPLMNItem, int& numOfItem) {
  numOfItem           = numberOfBroadcastItem;
  m_broadcastPLMNItem = broadcastPLMNItem;
}

//------------------------------------------------------------------------------
bool SupportedTaItem::encode2SupportedTaItem(Ngap_SupportedTAItem_t* ta) {
  if (!tac->encode2octetstring(ta->tAC)) return false;
  cout << "SupportedTaItem::numberOfBroadcastItem	"
       << numberOfBroadcastItem << endl;
  for (int i = 0; i < numberOfBroadcastItem; i++) {
    Ngap_BroadcastPLMNItem_t* plmnItem =
        (Ngap_BroadcastPLMNItem*) calloc(1, sizeof(Ngap_BroadcastPLMNItem));
    if (!broadcastPLMNItem[i].encode2BroadcastPLMNItem(plmnItem)) return false;
    if (ASN_SEQUENCE_ADD(&ta->broadcastPLMNList.list, plmnItem) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool SupportedTaItem::decodefromSupportedTaItem(Ngap_SupportedTAItem_t* pdu) {
  if (tac == nullptr) tac = new TAC();
  if (!tac->decodefromoctetstring(pdu->tAC)) return false;
  numberOfBroadcastItem = pdu->broadcastPLMNList.list.count;
  broadcastPLMNItem     = new BroadcastPLMNItem[numberOfBroadcastItem]();
  for (int i = 0; i < numberOfBroadcastItem; i++) {
    if (!broadcastPLMNItem[i].decodefromBroadcastPLMNItem(
            pdu->broadcastPLMNList.list.array[i]))
      return false;
  }
  return true;
}

}  // namespace ngap
