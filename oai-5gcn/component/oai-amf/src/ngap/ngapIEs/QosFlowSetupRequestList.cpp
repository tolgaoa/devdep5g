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

#include "QosFlowSetupRequestList.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
QosFlowSetupRequestList::QosFlowSetupRequestList() {
  items      = NULL;
  numofitems = 0;
}

//------------------------------------------------------------------------------
QosFlowSetupRequestList::~QosFlowSetupRequestList() {}

//------------------------------------------------------------------------------
void QosFlowSetupRequestList::setQosFlowSetupRequestList(
    QosFlowSetupRequestItem* m_items, int m_numofitems) {
  items      = m_items;
  numofitems = m_numofitems;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestList::getQosFlowSetupRequestList(
    QosFlowSetupRequestItem*& m_items, int& m_numofitems) {
  m_items      = items;
  m_numofitems = numofitems;

  if (!items) return false;
  if (!numofitems) return false;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestList::encode2QosFlowSetupRequestList(
    Ngap_QosFlowSetupRequestList_t* qosFlowSetupRequestList) {
  cout << "QosFlowSetupRequestList::numOfQosFlowSetupRequestItem	"
       << numofitems << endl;
  for (int i = 0; i < numofitems; i++) {
    Ngap_QosFlowSetupRequestItem_t* item =
        (Ngap_QosFlowSetupRequestItem_t*) calloc(
            1, sizeof(Ngap_QosFlowSetupRequestItem_t));
    if (!item) return false;
    if (!items[i].encode2QosFlowSetupRequestItem(item)) return false;
    if (ASN_SEQUENCE_ADD(&qosFlowSetupRequestList->list, item) != 0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestList::decodefromQosFlowSetupRequestList(
    Ngap_QosFlowSetupRequestList_t* qosFlowSetupRequestList) {
  numofitems = qosFlowSetupRequestList->list.count;
  items      = new QosFlowSetupRequestItem[numofitems]();
  for (int i = 0; i < numofitems; i++) {
    if (!items[i].decodefromQosFlowSetupRequestItem(
            qosFlowSetupRequestList->list.array[i]))
      return false;
  }
  return true;
}

}  // namespace ngap
