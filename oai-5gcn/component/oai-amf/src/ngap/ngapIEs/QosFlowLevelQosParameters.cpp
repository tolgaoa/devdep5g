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

#include "QosFlowLevelQosParameters.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
QosFlowLevelQosParameters::QosFlowLevelQosParameters() {
  qosCharacteristics             = NULL;
  allocationAndRetentionPriority = NULL;
  gBR_QosInformation             = NULL;
  reflectiveQosAttribute         = NULL;
  additionalQosFlowInformation   = NULL;
}

//------------------------------------------------------------------------------
QosFlowLevelQosParameters::~QosFlowLevelQosParameters() {}

//------------------------------------------------------------------------------
void QosFlowLevelQosParameters::setQosFlowLevelQosParameters(
    QosCharacteristics* m_qosCharacteristics,
    AllocationAndRetentionPriority* m_allocationAndRetentionPriority,
    GBR_QosInformation* m_gBR_QosInformation,
    ReflectiveQosAttribute* m_reflectiveQosAttribute,
    AdditionalQosFlowInformation* m_additionalQosFlowInformation) {
  qosCharacteristics             = m_qosCharacteristics;
  allocationAndRetentionPriority = m_allocationAndRetentionPriority;
  gBR_QosInformation             = m_gBR_QosInformation;
  reflectiveQosAttribute         = m_reflectiveQosAttribute;
  additionalQosFlowInformation   = m_additionalQosFlowInformation;
}

//------------------------------------------------------------------------------
bool QosFlowLevelQosParameters::getQosFlowLevelQosParameters(
    QosCharacteristics*& m_qosCharacteristics,
    AllocationAndRetentionPriority*& m_allocationAndRetentionPriority,
    GBR_QosInformation*& m_gBR_QosInformation,
    ReflectiveQosAttribute*& m_reflectiveQosAttribute,
    AdditionalQosFlowInformation*& m_additionalQosFlowInformation) {
  m_qosCharacteristics             = qosCharacteristics;
  m_allocationAndRetentionPriority = allocationAndRetentionPriority;
  m_gBR_QosInformation             = gBR_QosInformation;
  m_reflectiveQosAttribute         = reflectiveQosAttribute;
  m_additionalQosFlowInformation   = additionalQosFlowInformation;

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowLevelQosParameters::encode2QosFlowLevelQosParameters(
    Ngap_QosFlowLevelQosParameters_t* qosFlowLevelQosParameters) {
  if (!qosCharacteristics->encode2QosCharacteristics(
          &qosFlowLevelQosParameters->qosCharacteristics))
    return false;
  if (!allocationAndRetentionPriority->encode2AllocationAndRetentionPriority(
          &qosFlowLevelQosParameters->allocationAndRetentionPriority))
    return false;

  if (gBR_QosInformation) {
    Ngap_GBR_QosInformation_t* gbrQosInfo = (Ngap_GBR_QosInformation_t*) calloc(
        1, sizeof(Ngap_GBR_QosInformation_t));
    if (!gbrQosInfo) return false;
    if (!gBR_QosInformation->encode2GBR_QosInformation(gbrQosInfo))
      return false;
    qosFlowLevelQosParameters->gBR_QosInformation = gbrQosInfo;
  }
  if (reflectiveQosAttribute) {
    Ngap_ReflectiveQosAttribute_t* reflective =
        (Ngap_ReflectiveQosAttribute_t*) calloc(
            1, sizeof(Ngap_ReflectiveQosAttribute_t));
    if (!reflective) return false;
    if (!reflectiveQosAttribute->encode2ReflectiveQosAttribute(reflective))
      return false;
    qosFlowLevelQosParameters->reflectiveQosAttribute = reflective;
  }
  if (additionalQosFlowInformation) {
    Ngap_AdditionalQosFlowInformation_t* additional =
        (Ngap_AdditionalQosFlowInformation_t*) calloc(
            1, sizeof(Ngap_AdditionalQosFlowInformation_t));
    if (!additional) return false;
    if (!additionalQosFlowInformation->encode2AdditionalQosFlowInformation(
            additional))
      return false;
    qosFlowLevelQosParameters->additionalQosFlowInformation = additional;
  }

  return true;
}

//------------------------------------------------------------------------------
bool QosFlowLevelQosParameters::decodefromQosFlowLevelQosParameters(
    Ngap_QosFlowLevelQosParameters_t* qosFlowLevelQosParameters) {
  if (qosCharacteristics == nullptr)
    qosCharacteristics = new QosCharacteristics();
  if (allocationAndRetentionPriority == nullptr)
    allocationAndRetentionPriority = new AllocationAndRetentionPriority();

  if (!qosCharacteristics->decodefromQosCharacteristics(
          &qosFlowLevelQosParameters->qosCharacteristics))
    return false;
  if (!allocationAndRetentionPriority->decodefromAllocationAndRetentionPriority(
          &qosFlowLevelQosParameters->allocationAndRetentionPriority))
    return false;

  if (qosFlowLevelQosParameters->gBR_QosInformation) {
    gBR_QosInformation = new GBR_QosInformation();
    if (!gBR_QosInformation->decodefromGBR_QosInformation(
            qosFlowLevelQosParameters->gBR_QosInformation))
      return false;
  }
  if (qosFlowLevelQosParameters->reflectiveQosAttribute) {
    reflectiveQosAttribute = new ReflectiveQosAttribute();
    if (!reflectiveQosAttribute->decodefromReflectiveQosAttribute(
            qosFlowLevelQosParameters->reflectiveQosAttribute))
      return false;
  }
  if (qosFlowLevelQosParameters->additionalQosFlowInformation) {
    additionalQosFlowInformation = new AdditionalQosFlowInformation();
    if (!additionalQosFlowInformation->decodefromAdditionalQosFlowInformation(
            qosFlowLevelQosParameters->additionalQosFlowInformation))
      return false;
  }

  return true;
}
}  // namespace ngap
