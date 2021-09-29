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

#include "SupportedTAList.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
SupportedTAList::SupportedTAList() {
  supportedTaItem         = NULL;
  numberOfSupportedTaItem = 0;
}

//------------------------------------------------------------------------------
SupportedTAList::~SupportedTAList() {}

//------------------------------------------------------------------------------
bool SupportedTAList::encode2SupportedTAList(
    Ngap_SupportedTAList_t* supportedTAList) {
  cout << "SupportedTAList::numberOfSupportedTaItem	"
       << numberOfSupportedTaItem << endl;
  for (int i = 0; i < numberOfSupportedTaItem; i++) {
    Ngap_SupportedTAItem_t* ta =
        (Ngap_SupportedTAItem_t*) calloc(1, sizeof(Ngap_SupportedTAItem_t));
    if (!supportedTaItem[i].encode2SupportedTaItem(ta)) return false;
    if (ASN_SEQUENCE_ADD(&supportedTAList->list, ta) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool SupportedTAList::decodefromSupportedTAList(Ngap_SupportedTAList_t* pdu) {
  numberOfSupportedTaItem = pdu->list.count;
  supportedTaItem         = new SupportedTaItem[numberOfSupportedTaItem]();
  for (int i = 0; i < numberOfSupportedTaItem; i++) {
    if (!supportedTaItem[i].decodefromSupportedTaItem(pdu->list.array[i]))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void SupportedTAList::setSupportedTaItems(
    SupportedTaItem* m_supportedTaItem, int numOfItem) {
  supportedTaItem         = m_supportedTaItem;
  numberOfSupportedTaItem = numOfItem;
}

//------------------------------------------------------------------------------
void SupportedTAList::getSupportedTaItems(
    SupportedTaItem*& m_supportedTaItem, int& numOfItem) {
  m_supportedTaItem = supportedTaItem;
  numOfItem         = numberOfSupportedTaItem;
}

}  // namespace ngap
