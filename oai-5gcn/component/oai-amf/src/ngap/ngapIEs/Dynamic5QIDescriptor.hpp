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

#ifndef _DYNAMIC5QIDESCRIPTOR_H_
#define _DYNAMIC5QIDESCRIPTOR_H_

#include "AveragingWindow.hpp"
#include "DelayCritical.hpp"
#include "FiveQI.hpp"
#include "MaximumDataBurstVolume.hpp"
#include "PacketDelayBudget.hpp"
#include "PacketErrorRate.hpp"
#include "PriorityLevelQos.hpp"

extern "C" {
#include "Ngap_Dynamic5QIDescriptor.h"
}

namespace ngap {

class Dynamic5QIDescriptor {
 public:
  Dynamic5QIDescriptor();
  virtual ~Dynamic5QIDescriptor();

  void setDynamic5QIDescriptor(
      PriorityLevelQos* m_priorityLevelQos,
      PacketDelayBudget* m_packetDelayBudget,
      PacketErrorRate* m_packetErrorRate, FiveQI* m_fiveQI,
      DelayCritical* m_delayCritical, AveragingWindow* m_averagingWindow,
      MaximumDataBurstVolume* m_maximumDataBurstVolume);
  bool getDynamic5QIDescriptor(
      PriorityLevelQos*& m_priorityLevelQos,
      PacketDelayBudget*& m_packetDelayBudget,
      PacketErrorRate*& m_packetErrorRate, FiveQI*& m_fiveQI,
      DelayCritical*& m_delayCritical, AveragingWindow*& m_averagingWindow,
      MaximumDataBurstVolume*& m_maximumDataBurstVolume);

  bool encode2Dynamic5QIDescriptor(Ngap_Dynamic5QIDescriptor_t*);
  bool decodefromDynamic5QIDescriptor(Ngap_Dynamic5QIDescriptor_t*);

 private:
  PriorityLevelQos* priorityLevelQos;
  PacketDelayBudget* packetDelayBudget;
  PacketErrorRate* packetErrorRate;

  FiveQI* fiveQI;                                 /* OPTIONAL */
  DelayCritical* delayCritical;                   /* OPTIONAL */
  AveragingWindow* averagingWindow;               /* OPTIONAL */
  MaximumDataBurstVolume* maximumDataBurstVolume; /* OPTIONAL */
};
}  // namespace ngap

#endif
