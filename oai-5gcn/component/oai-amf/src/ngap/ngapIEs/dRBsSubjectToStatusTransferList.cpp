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

#include "dRBsSubjectToStatusTransferList.hpp"

#include <iostream>
#include <vector>
using namespace std;
namespace ngap {
dRBSubjectList::dRBSubjectList() {
  drbsubjectitem = NULL;
  numofitem      = 0;
}
dRBSubjectList::~dRBSubjectList() {}
void dRBSubjectList::setdRBSubjectItem(dRBSubjectItem* m_item, int num) {
  drbsubjectitem = m_item;
  numofitem      = num;
}
void dRBSubjectList::getdRBSubjectItem(dRBSubjectItem*& m_item, int& num) {
  m_item = drbsubjectitem;
  num    = numofitem;
}
bool dRBSubjectList::encodefromdRBSubjectlist(
    Ngap_DRBsSubjectToStatusTransferList_t& DRBsSubjectToStatusTransferList) {
  for (int i = 0; i < numofitem; i++) {
    Ngap_DRBsSubjectToStatusTransferItem_t* ie =
        (Ngap_DRBsSubjectToStatusTransferItem_t*) calloc(
            1, sizeof(Ngap_DRBsSubjectToStatusTransferItem_t));
    if (!ie) return false;
    if (!drbsubjectitem[i].encodedRBSubjectItem(ie)) {
      cout << "encodefromdRBSubjectlist error" << endl;
      return false;
    }
    if (ASN_SEQUENCE_ADD(&DRBsSubjectToStatusTransferList.list, ie) != 0) {
      cout
          << "ASN_SEQUENCE_ADD(&DRBsSubjectToStatusTransferList.list, ie) error"
          << endl;
      return false;
    }
  }
  cout << "encodefromdRBSubjectlist successfully" << endl;
  return true;
}
bool dRBSubjectList::decodefromdRBSubjectlist(
    Ngap_DRBsSubjectToStatusTransferList_t& DRBsSubjectToStatusTransferList) {
  numofitem      = DRBsSubjectToStatusTransferList.list.count;
  drbsubjectitem = new dRBSubjectItem[numofitem]();
  for (int i = 0; i < numofitem; i++) {
    if (!drbsubjectitem[i].decodefromdRBSubjectItem(
            DRBsSubjectToStatusTransferList.list.array[i])) {
      cout << "decodefromdRBSubjectlist error" << endl;
      return false;
    }
  }
  cout << "decodefromdRBSubjectlist successfully" << endl;
  return true;
}
}  // namespace ngap
