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

#include "AssociatedQosFlowList.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
AssociatedQosFlowList::AssociatedQosFlowList() {
  associatedQosFlowItem      = NULL;
  numofassociatedQosFlowItem = 0;
}

//------------------------------------------------------------------------------
AssociatedQosFlowList::~AssociatedQosFlowList() {}

//------------------------------------------------------------------------------
void AssociatedQosFlowList::setAssociatedQosFlowList(
    AssociatedQosFlowItem* m_associatedQosFlowItem, int numofitem) {
  associatedQosFlowItem      = m_associatedQosFlowItem;
  numofassociatedQosFlowItem = numofitem;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowList::getAssociatedQosFlowList(
    AssociatedQosFlowItem*& m_associatedQosFlowItem, int& numofitem) {
  m_associatedQosFlowItem = associatedQosFlowItem;
  numofitem               = numofassociatedQosFlowItem;

  return true;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowList::encode2AssociatedQosFlowList(
    Ngap_AssociatedQosFlowList_t& associatedQosFlowList) {
  for (int i = 0; i < numofassociatedQosFlowItem; i++) {
    Ngap_AssociatedQosFlowItem_t* ie = (Ngap_AssociatedQosFlowItem_t*) calloc(
        1, sizeof(Ngap_AssociatedQosFlowItem_t));
    if (!ie) return false;
    if (!associatedQosFlowItem[i].encode2AssociatedQosFlowItem(ie))
      return false;
    if (ASN_SEQUENCE_ADD(&associatedQosFlowList.list, ie) != 0) return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowList::decodefromAssociatedQosFlowList(
    Ngap_AssociatedQosFlowList_t& associatedQosFlowList) {
  numofassociatedQosFlowItem = associatedQosFlowList.list.count;
  associatedQosFlowItem =
      new AssociatedQosFlowItem[numofassociatedQosFlowItem]();
  for (int i = 0; i < numofassociatedQosFlowItem; i++) {
    if (!associatedQosFlowItem[i].decodefromAssociatedQosFlowItem(
            associatedQosFlowList.list.array[i]))
      return false;
  }

  return true;
}

}  // namespace ngap
