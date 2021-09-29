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

#include "QosFlowSetupRequestItem.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
QosFlowSetupRequestItem::QosFlowSetupRequestItem() {
  qosFlowIdentifier         = NULL;
  qosFlowLevelQosParameters = NULL;
}

//------------------------------------------------------------------------------
QosFlowSetupRequestItem::~QosFlowSetupRequestItem() {}

//------------------------------------------------------------------------------
void QosFlowSetupRequestItem::setQosFlowSetupRequestItem(
    QosFlowIdentifier* m_qosFlowIdentifier,
    QosFlowLevelQosParameters* m_qosFlowLevelQosParameters) {
  qosFlowIdentifier         = m_qosFlowIdentifier;
  qosFlowLevelQosParameters = m_qosFlowLevelQosParameters;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestItem::getQosFlowSetupRequestItem(
    QosFlowIdentifier*& m_qosFlowIdentifier,
    QosFlowLevelQosParameters*& m_qosFlowLevelQosParameters) {
  m_qosFlowIdentifier         = qosFlowIdentifier;
  m_qosFlowLevelQosParameters = qosFlowLevelQosParameters;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestItem::encode2QosFlowSetupRequestItem(
    Ngap_QosFlowSetupRequestItem_t* qosFlowSetupRequestItem) {
  if (!qosFlowIdentifier->encode2QosFlowIdentifier(
          &qosFlowSetupRequestItem->qosFlowIdentifier))
    return false;
  if (!qosFlowLevelQosParameters->encode2QosFlowLevelQosParameters(
          &qosFlowSetupRequestItem->qosFlowLevelQosParameters))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowSetupRequestItem::decodefromQosFlowSetupRequestItem(
    Ngap_QosFlowSetupRequestItem_t* qosFlowSetupRequestItem) {
  if (qosFlowIdentifier == nullptr) qosFlowIdentifier = new QosFlowIdentifier();
  if (qosFlowLevelQosParameters == nullptr)
    qosFlowLevelQosParameters = new QosFlowLevelQosParameters();

  if (!qosFlowIdentifier->decodefromQosFlowIdentifier(
          &qosFlowSetupRequestItem->qosFlowIdentifier))
    return false;
  if (!qosFlowLevelQosParameters->decodefromQosFlowLevelQosParameters(
          &qosFlowSetupRequestItem->qosFlowLevelQosParameters))
    return false;

  return true;
}
}  // namespace ngap
