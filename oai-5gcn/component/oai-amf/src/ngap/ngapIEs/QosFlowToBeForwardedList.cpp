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

#include "QosFlowToBeForwardedList.hpp"

#include <iostream>
using namespace std;
namespace ngap {
QosFlowToBeForwardedList::QosFlowToBeForwardedList() {
  Qosflowtobeforwardeditem      = NULL;
  numofqosflowtobeforwardeditem = 0;
}
QosFlowToBeForwardedList::~QosFlowToBeForwardedList() {}
void QosFlowToBeForwardedList::setQosFlowToBeForwardedItem(
    QosFlowToBeForwardedItem* m_qosflowtobeforwardeditem, int num) {
  Qosflowtobeforwardeditem      = m_qosflowtobeforwardeditem;
  numofqosflowtobeforwardeditem = num;
}
bool QosFlowToBeForwardedList::encodeQosFlowToBeForwardedList(
    Ngap_QosFlowToBeForwardedList*& m_ngap_qosflowtobeforwardedlist) {
  m_ngap_qosflowtobeforwardedlist = (Ngap_QosFlowToBeForwardedList_t*) calloc(
      1, sizeof(Ngap_QosFlowToBeForwardedList_t));
  for (int i = 0; i < numofqosflowtobeforwardeditem; i++) {
    cout << "encoding list..." << endl;
    Ngap_QosFlowToBeForwardedItem_t* response =
        (Ngap_QosFlowToBeForwardedItem_t*) calloc(
            1, sizeof(Ngap_QosFlowToBeForwardedItem_t));
    if (!response) return false;
    if (!Qosflowtobeforwardeditem[i].encodeQosFlowToBeForwardedItem(response)) {
      cout << "encode QosFlowTowardedItem error" << endl;
      return false;
    }
    cout << "QFI is " << response->qosFlowIdentifier << endl;
    if (ASN_SEQUENCE_ADD(&m_ngap_qosflowtobeforwardedlist->list, response) !=
        0) {
      cout << "encode QosFlowTowardedList error" << endl;
      return false;
    }
  }
  return true;
}
}  // namespace ngap
