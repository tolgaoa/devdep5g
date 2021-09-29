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

#ifndef _NONDYNAMIC5QIDESCRIPTOR_H_
#define _NONDYNAMIC5QIDESCRIPTOR_H_

#include "AveragingWindow.hpp"
#include "FiveQI.hpp"
#include "MaximumDataBurstVolume.hpp"
#include "PriorityLevelQos.hpp"

extern "C" {
#include "Ngap_NonDynamic5QIDescriptor.h"
}

namespace ngap {

class NonDynamic5QIDescriptor {
 public:
  NonDynamic5QIDescriptor();
  virtual ~NonDynamic5QIDescriptor();

  void setNonDynamic5QIDescriptor(
      FiveQI* m_fiveQI, PriorityLevelQos* m_priorityLevelQos,
      AveragingWindow* m_averagingWindow,
      MaximumDataBurstVolume* m_maximumDataBurstVolume);
  bool getNonDynamic5QIDescriptor(
      FiveQI*& m_fiveQI, PriorityLevelQos*& m_priorityLevelQos,
      AveragingWindow*& m_averagingWindow,
      MaximumDataBurstVolume*& m_maximumDataBurstVolume);

  bool encode2NonDynamic5QIDescriptor(Ngap_NonDynamic5QIDescriptor_t*);
  bool decodefromNonDynamic5QIDescriptor(Ngap_NonDynamic5QIDescriptor_t*);

 private:
  FiveQI* fiveQI;
  PriorityLevelQos* priorityLevelQos;             /* OPTIONAL */
  AveragingWindow* averagingWindow;               /* OPTIONAL */
  MaximumDataBurstVolume* maximumDataBurstVolume; /* OPTIONAL */
};
}  // namespace ngap

#endif
