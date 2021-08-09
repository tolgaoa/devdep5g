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

#ifndef _QOSFLOWLEVELQOSPARAMETERS_H_
#define _QOSFLOWLEVELQOSPARAMETERS_H_

#include "AdditionalQosFlowInformation.hpp"
#include "AllocationAndRetentionPriority.hpp"
#include "GBR-QosInformation.hpp"
#include "QosCharacteristics.hpp"
#include "ReflectiveQosAttribute.hpp"

extern "C" {
#include "Ngap_QosFlowLevelQosParameters.h"
}

namespace ngap {

class QosFlowLevelQosParameters {
 public:
  QosFlowLevelQosParameters();
  virtual ~QosFlowLevelQosParameters();

  void setQosFlowLevelQosParameters(
      QosCharacteristics* m_qosCharacteristics,
      AllocationAndRetentionPriority* m_allocationAndRetentionPriority,
      GBR_QosInformation* m_gBR_QosInformation,
      ReflectiveQosAttribute* m_reflectiveQosAttribute,
      AdditionalQosFlowInformation* m_additionalQosFlowInformation);
  bool getQosFlowLevelQosParameters(
      QosCharacteristics*& m_qosCharacteristics,
      AllocationAndRetentionPriority*& m_allocationAndRetentionPriority,
      GBR_QosInformation*& m_gBR_QosInformation,
      ReflectiveQosAttribute*& m_reflectiveQosAttribute,
      AdditionalQosFlowInformation*& m_additionalQosFlowInformation);

  bool encode2QosFlowLevelQosParameters(Ngap_QosFlowLevelQosParameters_t*);
  bool decodefromQosFlowLevelQosParameters(Ngap_QosFlowLevelQosParameters_t*);

 private:
  QosCharacteristics* qosCharacteristics;
  AllocationAndRetentionPriority* allocationAndRetentionPriority;
  GBR_QosInformation* gBR_QosInformation;                     /* OPTIONAL */
  ReflectiveQosAttribute* reflectiveQosAttribute;             /* OPTIONAL */
  AdditionalQosFlowInformation* additionalQosFlowInformation; /* OPTIONAL */
};

}  // namespace ngap

#endif
