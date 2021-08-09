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

#include "AssociatedQosFlowItem.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
AssociatedQosFlowItem::AssociatedQosFlowItem() {
  qosFlowIdentifier        = NULL;
  qosFlowMappingIndication = -1;
}

//------------------------------------------------------------------------------
AssociatedQosFlowItem::~AssociatedQosFlowItem() {}

//------------------------------------------------------------------------------
void AssociatedQosFlowItem::setAssociatedQosFlowItem(
    e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication
        m_qosFlowMappingIndication,
    QosFlowIdentifier* m_qosFlowIdentifier) {
  qosFlowMappingIndication = m_qosFlowMappingIndication;
  qosFlowIdentifier        = m_qosFlowIdentifier;
}

//------------------------------------------------------------------------------
void AssociatedQosFlowItem::setAssociatedQosFlowItem(
    QosFlowIdentifier* m_qosFlowIdentifier) {
  qosFlowIdentifier = m_qosFlowIdentifier;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowItem::getAssociatedQosFlowItem(
    long& m_qosFlowMappingIndication, QosFlowIdentifier*& m_qosFlowIdentifier) {
  m_qosFlowMappingIndication = qosFlowMappingIndication;
  m_qosFlowIdentifier        = qosFlowIdentifier;

  return true;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowItem::encode2AssociatedQosFlowItem(
    Ngap_AssociatedQosFlowItem_t* associatedQosFlowItem) {
  if (qosFlowMappingIndication >= 0) {
    associatedQosFlowItem->qosFlowMappingIndication =
        (long*) calloc(1, sizeof(long));
    *associatedQosFlowItem->qosFlowMappingIndication = qosFlowMappingIndication;
  }
  if (!qosFlowIdentifier->encode2QosFlowIdentifier(
          &associatedQosFlowItem->qosFlowIdentifier))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool AssociatedQosFlowItem::decodefromAssociatedQosFlowItem(
    Ngap_AssociatedQosFlowItem_t* associatedQosFlowItem) {
  if (qosFlowIdentifier == nullptr) qosFlowIdentifier = new QosFlowIdentifier();
  if (!qosFlowIdentifier->decodefromQosFlowIdentifier(
          &associatedQosFlowItem->qosFlowIdentifier))
    return false;

  if (associatedQosFlowItem->qosFlowMappingIndication) {
    qosFlowMappingIndication = *associatedQosFlowItem->qosFlowMappingIndication;
  }

  return true;
}

}  // namespace ngap
