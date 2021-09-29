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

#include "DLQoSFlowPerTNLInformation.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
DLQoSFlowPerTNLInformation::DLQoSFlowPerTNLInformation() {
  uPTransportLayerInformation = NULL;
  associatedQosFlowList       = NULL;
}

//------------------------------------------------------------------------------
DLQoSFlowPerTNLInformation::~DLQoSFlowPerTNLInformation() {}

//------------------------------------------------------------------------------
void DLQoSFlowPerTNLInformation::setDLQoSFlowPerTNLInformation(
    UpTransportLayerInformation* m_uPTransportLayerInformation,
    AssociatedQosFlowList* m_associatedQosFlowList) {
  uPTransportLayerInformation = m_uPTransportLayerInformation;
  associatedQosFlowList       = m_associatedQosFlowList;
}

//------------------------------------------------------------------------------
bool DLQoSFlowPerTNLInformation::getDLQoSFlowPerTNLInformation(
    UpTransportLayerInformation*& m_uPTransportLayerInformation,
    AssociatedQosFlowList*& m_associatedQosFlowList) {
  m_uPTransportLayerInformation = uPTransportLayerInformation;
  m_associatedQosFlowList       = associatedQosFlowList;

  return true;
}

//------------------------------------------------------------------------------
bool DLQoSFlowPerTNLInformation::encode2DLQoSFlowPerTNLInformation(
    Ngap_QosFlowPerTNLInformation_t* qosFlowPerTNLInformation) {
  if (!uPTransportLayerInformation->encode2UpTransportLayerInformation(
          qosFlowPerTNLInformation->uPTransportLayerInformation))
    return false;
  if (!associatedQosFlowList->encode2AssociatedQosFlowList(
          qosFlowPerTNLInformation->associatedQosFlowList))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool DLQoSFlowPerTNLInformation::decodefromDLQoSFlowPerTNLInformation(
    Ngap_QosFlowPerTNLInformation_t* qosFlowPerTNLInformation) {
  if (uPTransportLayerInformation == nullptr)
    uPTransportLayerInformation = new UpTransportLayerInformation();
  if (associatedQosFlowList == nullptr)
    associatedQosFlowList = new AssociatedQosFlowList();
  if (!uPTransportLayerInformation->decodefromUpTransportLayerInformation(
          qosFlowPerTNLInformation->uPTransportLayerInformation))
    return false;
  if (!associatedQosFlowList->decodefromAssociatedQosFlowList(
          qosFlowPerTNLInformation->associatedQosFlowList))
    return false;

  return true;
}

}  // namespace ngap
