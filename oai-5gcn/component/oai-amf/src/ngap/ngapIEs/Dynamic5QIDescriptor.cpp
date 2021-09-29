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

#include "Dynamic5QIDescriptor.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
Dynamic5QIDescriptor::Dynamic5QIDescriptor() {
  priorityLevelQos  = NULL;
  packetDelayBudget = NULL;
  packetErrorRate   = NULL;

  fiveQI                 = NULL;
  delayCritical          = NULL;
  averagingWindow        = NULL;
  maximumDataBurstVolume = NULL;
}

//------------------------------------------------------------------------------
Dynamic5QIDescriptor::~Dynamic5QIDescriptor() {}

//------------------------------------------------------------------------------
void Dynamic5QIDescriptor::setDynamic5QIDescriptor(
    PriorityLevelQos* m_priorityLevelQos,
    PacketDelayBudget* m_packetDelayBudget, PacketErrorRate* m_packetErrorRate,
    FiveQI* m_fiveQI, DelayCritical* m_delayCritical,
    AveragingWindow* m_averagingWindow,
    MaximumDataBurstVolume* m_maximumDataBurstVolume) {
  priorityLevelQos  = m_priorityLevelQos;
  packetDelayBudget = m_packetDelayBudget;
  packetErrorRate   = m_packetErrorRate;

  if (m_fiveQI) {
    fiveQI = m_fiveQI;
  }
  if (m_delayCritical) {
    delayCritical = m_delayCritical;
  }
  if (m_averagingWindow) {
    averagingWindow = m_averagingWindow;
  }
  if (m_maximumDataBurstVolume) {
    maximumDataBurstVolume = m_maximumDataBurstVolume;
  }
}

//------------------------------------------------------------------------------
bool Dynamic5QIDescriptor::getDynamic5QIDescriptor(
    PriorityLevelQos*& m_priorityLevelQos,
    PacketDelayBudget*& m_packetDelayBudget,
    PacketErrorRate*& m_packetErrorRate, FiveQI*& m_fiveQI,
    DelayCritical*& m_delayCritical, AveragingWindow*& m_averagingWindow,
    MaximumDataBurstVolume*& m_maximumDataBurstVolume) {
  if (!priorityLevelQos) return false;
  m_priorityLevelQos = priorityLevelQos;
  if (!packetDelayBudget) return false;
  m_packetDelayBudget = packetDelayBudget;
  if (!packetErrorRate) return false;
  m_packetErrorRate = packetErrorRate;

  if (fiveQI) m_fiveQI = fiveQI;
  if (delayCritical) m_delayCritical = delayCritical;
  if (averagingWindow) m_averagingWindow = averagingWindow;
  if (maximumDataBurstVolume) m_maximumDataBurstVolume = maximumDataBurstVolume;

  return true;
}

//------------------------------------------------------------------------------
bool Dynamic5QIDescriptor::encode2Dynamic5QIDescriptor(
    Ngap_Dynamic5QIDescriptor_t* dynamic5QIDescriptor) {
  if (!priorityLevelQos->encode2PriorityLevelQos(
          &dynamic5QIDescriptor->priorityLevelQos))
    return false;
  if (!packetDelayBudget->encode2PacketDelayBudget(
          &dynamic5QIDescriptor->packetDelayBudget))
    return false;
  if (!packetErrorRate->encode2PacketErrorRate(
          &dynamic5QIDescriptor->packetErrorRate))
    return false;

  if (fiveQI) {
    Ngap_FiveQI_t* fq = (Ngap_FiveQI_t*) calloc(1, sizeof(Ngap_FiveQI_t));
    if (!fq) return false;
    if (!fiveQI->encode2FiveQI(fq)) return false;
    dynamic5QIDescriptor->fiveQI = fq;
  }
  if (delayCritical) {
    Ngap_DelayCritical_t* dc =
        (Ngap_DelayCritical_t*) calloc(1, sizeof(Ngap_DelayCritical_t));
    if (!dc) return false;
    if (!delayCritical->encode2DelayCritical(dc)) return false;
    dynamic5QIDescriptor->delayCritical = dc;
  }
  if (averagingWindow) {
    Ngap_AveragingWindow_t* aw =
        (Ngap_AveragingWindow_t*) calloc(1, sizeof(Ngap_AveragingWindow_t));
    if (!aw) return false;
    if (!averagingWindow->encode2AveragingWindow(aw)) return false;
    dynamic5QIDescriptor->averagingWindow = aw;
  }
  if (maximumDataBurstVolume) {
    Ngap_MaximumDataBurstVolume_t* mdbv =
        (Ngap_MaximumDataBurstVolume_t*) calloc(
            1, sizeof(Ngap_MaximumDataBurstVolume_t));
    if (!mdbv) return false;
    if (!maximumDataBurstVolume->encode2MaximumDataBurstVolume(mdbv))
      return false;
    dynamic5QIDescriptor->maximumDataBurstVolume = mdbv;
  }

  return true;
}

//------------------------------------------------------------------------------
bool Dynamic5QIDescriptor::decodefromDynamic5QIDescriptor(
    Ngap_Dynamic5QIDescriptor_t* dynamic5QIDescriptor) {
  if (priorityLevelQos == nullptr) priorityLevelQos = new PriorityLevelQos();
  if (packetDelayBudget == nullptr) packetDelayBudget = new PacketDelayBudget();
  if (packetErrorRate == nullptr) packetErrorRate = new PacketErrorRate();
  if (!priorityLevelQos->decodefromPriorityLevelQos(
          &dynamic5QIDescriptor->priorityLevelQos))
    return false;
  if (!packetDelayBudget->decodefromPacketDelayBudget(
          &dynamic5QIDescriptor->packetDelayBudget))
    return false;
  if (!packetErrorRate->decodefromPacketErrorRate(
          &dynamic5QIDescriptor->packetErrorRate))
    return false;

  if (dynamic5QIDescriptor->fiveQI) {
    fiveQI = new FiveQI();
    if (!fiveQI->decodefromFiveQI(dynamic5QIDescriptor->fiveQI)) return false;
  }
  if (dynamic5QIDescriptor->delayCritical) {
    delayCritical = new DelayCritical();
    if (!delayCritical->decodefromDelayCritical(
            dynamic5QIDescriptor->delayCritical))
      return false;
  }
  if (dynamic5QIDescriptor->averagingWindow) {
    averagingWindow = new AveragingWindow();
    if (!averagingWindow->decodefromAveragingWindow(
            dynamic5QIDescriptor->averagingWindow))
      return false;
  }
  if (dynamic5QIDescriptor->maximumDataBurstVolume) {
    maximumDataBurstVolume = new MaximumDataBurstVolume();
    if (!maximumDataBurstVolume->decodefromMaximumDataBurstVolume(
            dynamic5QIDescriptor->maximumDataBurstVolume))
      return false;
  }

  return true;
}
}  // namespace ngap
