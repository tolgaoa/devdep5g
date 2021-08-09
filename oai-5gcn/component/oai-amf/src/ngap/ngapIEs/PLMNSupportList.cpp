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

#include "PLMNSupportList.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PLMNSupportList::PLMNSupportList() {
  plmnsupportItemItem         = nullptr;
  numberOfplmnsupportItemItem = 0;
}

//------------------------------------------------------------------------------
PLMNSupportList::~PLMNSupportList() {}

//------------------------------------------------------------------------------
void PLMNSupportList::addPLMNSupportItems(
    PLMNSupportItem* m_plmnsupportItemItem, int numOfItem) {
  plmnsupportItemItem         = m_plmnsupportItemItem;
  numberOfplmnsupportItemItem = numOfItem;
}

//------------------------------------------------------------------------------
bool PLMNSupportList::encode2PLMNSupportList(
    Ngap_PLMNSupportList_t* plmnsupportList) {
  cout << "PLMNSupportList::numberOfplmnsupportItemItem	("
       << numberOfplmnsupportItemItem << ")" << endl;
  for (int i = 0; i < numberOfplmnsupportItemItem; i++) {
    Ngap_PLMNSupportItem_t* supportItem =
        (Ngap_PLMNSupportItem_t*) calloc(1, sizeof(Ngap_PLMNSupportItem_t));
    if (!supportItem) return false;
    if (!plmnsupportItemItem[i].encode2PLMNSupportItem(supportItem))
      return false;
    if (ASN_SEQUENCE_ADD(&plmnsupportList->list, supportItem) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool PLMNSupportList::decodefromPLMNSupportList(
    Ngap_PLMNSupportList_t* plmnsupportList) {
  numberOfplmnsupportItemItem = plmnsupportList->list.count;
  plmnsupportItemItem = new PLMNSupportItem[numberOfplmnsupportItemItem]();
  for (int i = 0; i < numberOfplmnsupportItemItem; i++) {
    if (!plmnsupportItemItem[i].decodefromPLMNSupportItem(
            plmnsupportList->list.array[i]))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void PLMNSupportList::getPLMNSupportItems(
    PLMNSupportItem*& m_plmnsupportItemItem, int& numOfItem) {
  m_plmnsupportItemItem = plmnsupportItemItem;
  numOfItem             = numberOfplmnsupportItemItem;
}

}  // namespace ngap
