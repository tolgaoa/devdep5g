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

#include "ServedGUAMIList.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
ServedGUAMIList::ServedGUAMIList() {
  servedGUAMIItem         = nullptr;
  numberOfservedGUAMIItem = 0;
}

//------------------------------------------------------------------------------
ServedGUAMIList::~ServedGUAMIList() {}

//------------------------------------------------------------------------------
void ServedGUAMIList::addServedGUAMIItems(
    ServedGUAMIItem* m_supportedGuamiItem, int numOfItem) {
  servedGUAMIItem         = m_supportedGuamiItem;
  numberOfservedGUAMIItem = numOfItem;
}

//------------------------------------------------------------------------------
bool ServedGUAMIList::encode2ServedGUAMIList(
    Ngap_ServedGUAMIList_t* servedGUAMIList) {
  cout << "ServedGUAMIList::numberOfservedGUAMIItem	("
       << numberOfservedGUAMIItem << ")" << endl;
  for (int i = 0; i < numberOfservedGUAMIItem; i++) {
    Ngap_ServedGUAMIItem* guamiItem =
        (Ngap_ServedGUAMIItem*) calloc(1, sizeof(Ngap_ServedGUAMIItem));
    if (!guamiItem) return false;
    if (!servedGUAMIItem[i].encode2ServedGUAMIItem(guamiItem)) return false;
    if (ASN_SEQUENCE_ADD(&servedGUAMIList->list, guamiItem) != 0) return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool ServedGUAMIList::decodefromServedGUAMIList(Ngap_ServedGUAMIList_t* pdu) {
  numberOfservedGUAMIItem = pdu->list.count;
  if (servedGUAMIItem == nullptr)
    servedGUAMIItem = new ServedGUAMIItem[numberOfservedGUAMIItem]();
  for (int i = 0; i < numberOfservedGUAMIItem; i++) {
    if (!servedGUAMIItem[i].decodefromServedGUAMIItem(pdu->list.array[i]))
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
void ServedGUAMIList::getServedGUAMIItems(
    ServedGUAMIItem*& m_supportedGuamiItem, int& numOfItem) {
  m_supportedGuamiItem = servedGUAMIItem;
  numOfItem            = numberOfservedGUAMIItem;
}

}  // namespace ngap
