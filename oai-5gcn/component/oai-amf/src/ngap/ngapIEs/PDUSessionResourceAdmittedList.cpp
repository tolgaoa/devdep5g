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

#include "PDUSessionResourceAdmittedList.hpp"

#include <iostream>
using namespace std;

namespace ngap {
PDUSessionResourceAdmittedList::PDUSessionResourceAdmittedList() {
  pduSessionResourceAdmittedItem   = NULL;
  numofSessionResourceAdmittedItem = 0;
}
PDUSessionResourceAdmittedList::~PDUSessionResourceAdmittedList() {}

void PDUSessionResourceAdmittedList::setPDUSessionResourceAdmittedList(
    PDUSessionResourceAdmittedItem* m_pduSessionResourceAdmittedItem, int num) {
  pduSessionResourceAdmittedItem   = m_pduSessionResourceAdmittedItem;
  numofSessionResourceAdmittedItem = num;
}
bool PDUSessionResourceAdmittedList::encode2PDUSessionResourceAdmittedList(
    Ngap_PDUSessionResourceAdmittedList_t* pduSessionResourceAdmittedList) {
  for (int i = 0; i < numofSessionResourceAdmittedItem; i++) {
    Ngap_PDUSessionResourceAdmittedItem_t* response =
        (Ngap_PDUSessionResourceAdmittedItem_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceAdmittedItem_t));
    if (!response) return false;
    if (!pduSessionResourceAdmittedItem[i]
             .encode2PDUSessionResourceAdmittedItem(response))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceAdmittedList->list, response) != 0)
      return false;
  }

  return true;
}
bool PDUSessionResourceAdmittedList::decodefromPDUSessionResourceAdmittedList(
    Ngap_PDUSessionResourceAdmittedList_t* pduSessionResourceAdmittedList) {
  numofSessionResourceAdmittedItem = pduSessionResourceAdmittedList->list.count;
  pduSessionResourceAdmittedItem =
      new PDUSessionResourceAdmittedItem[numofSessionResourceAdmittedItem]();
  for (int i = 0; i < numofSessionResourceAdmittedItem; i++) {
    if (!pduSessionResourceAdmittedItem[i]
             .decodefromPDUSessionResourceAdmittedItem(
                 pduSessionResourceAdmittedList->list.array[i]))
      return false;
  }

  return true;
}
void PDUSessionResourceAdmittedList::getPDUSessionResourceAdmittedList(
    PDUSessionResourceAdmittedItem*& m_pduSessionResourceAdmittedItem,
    int& num) {
  m_pduSessionResourceAdmittedItem = pduSessionResourceAdmittedItem;
  num                              = numofSessionResourceAdmittedItem;
}

}  // namespace ngap
