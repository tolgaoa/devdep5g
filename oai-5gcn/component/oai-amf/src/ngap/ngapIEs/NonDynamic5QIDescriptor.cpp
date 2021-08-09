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

#include "NonDynamic5QIDescriptor.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
NonDynamic5QIDescriptor::NonDynamic5QIDescriptor() {
  fiveQI                 = NULL;
  priorityLevelQos       = NULL;
  averagingWindow        = NULL;
  maximumDataBurstVolume = NULL;
}

//------------------------------------------------------------------------------
NonDynamic5QIDescriptor::~NonDynamic5QIDescriptor() {}

//------------------------------------------------------------------------------
void NonDynamic5QIDescriptor::setNonDynamic5QIDescriptor(
    FiveQI* m_fiveQI, PriorityLevelQos* m_priorityLevelQos,
    AveragingWindow* m_averagingWindow,
    MaximumDataBurstVolume* m_maximumDataBurstVolume) {
  fiveQI = m_fiveQI;
  if (m_priorityLevelQos) {
    priorityLevelQos = m_priorityLevelQos;
  }
  if (m_averagingWindow) {
    averagingWindow = m_averagingWindow;
  }
  if (m_maximumDataBurstVolume) {
    maximumDataBurstVolume = m_maximumDataBurstVolume;
  }
}

//------------------------------------------------------------------------------
bool NonDynamic5QIDescriptor::getNonDynamic5QIDescriptor(
    FiveQI*& m_fiveQI, PriorityLevelQos*& m_priorityLevelQos,
    AveragingWindow*& m_averagingWindow,
    MaximumDataBurstVolume*& m_maximumDataBurstVolume) {
  if (!fiveQI) return false;
  m_fiveQI = fiveQI;
  if (priorityLevelQos) m_priorityLevelQos = priorityLevelQos;
  if (averagingWindow) m_averagingWindow = averagingWindow;
  if (maximumDataBurstVolume) m_maximumDataBurstVolume = maximumDataBurstVolume;

  return true;
}

//------------------------------------------------------------------------------
bool NonDynamic5QIDescriptor::encode2NonDynamic5QIDescriptor(
    Ngap_NonDynamic5QIDescriptor_t* nonDynamic5QIDescriptor) {
  if (!fiveQI->encode2FiveQI(&nonDynamic5QIDescriptor->fiveQI)) return false;
  if (priorityLevelQos) {
    Ngap_PriorityLevelQos_t* plq =
        (Ngap_PriorityLevelQos_t*) calloc(1, sizeof(Ngap_PriorityLevelQos_t));
    if (!plq) return false;
    if (!priorityLevelQos->encode2PriorityLevelQos(plq)) return false;
    nonDynamic5QIDescriptor->priorityLevelQos = plq;
  }
  if (averagingWindow) {
    Ngap_AveragingWindow_t* aw =
        (Ngap_AveragingWindow_t*) calloc(1, sizeof(Ngap_AveragingWindow_t));
    if (!aw) return false;
    if (!averagingWindow->encode2AveragingWindow(aw)) return false;
    nonDynamic5QIDescriptor->averagingWindow = aw;
  }
  if (maximumDataBurstVolume) {
    Ngap_MaximumDataBurstVolume_t* mdbv =
        (Ngap_MaximumDataBurstVolume_t*) calloc(
            1, sizeof(Ngap_MaximumDataBurstVolume_t));
    if (!mdbv) return false;
    if (!maximumDataBurstVolume->encode2MaximumDataBurstVolume(mdbv))
      return false;
    nonDynamic5QIDescriptor->maximumDataBurstVolume = mdbv;
  }

  return true;
}

//------------------------------------------------------------------------------
bool NonDynamic5QIDescriptor::decodefromNonDynamic5QIDescriptor(
    Ngap_NonDynamic5QIDescriptor_t* nonDynamic5QIDescriptor) {
  if (fiveQI == nullptr) fiveQI = new FiveQI();
  if (!fiveQI->decodefromFiveQI(&nonDynamic5QIDescriptor->fiveQI)) return false;
  if (nonDynamic5QIDescriptor->priorityLevelQos) {
    priorityLevelQos = new PriorityLevelQos();
    if (!priorityLevelQos->decodefromPriorityLevelQos(
            nonDynamic5QIDescriptor->priorityLevelQos))
      return false;
  }
  if (nonDynamic5QIDescriptor->averagingWindow) {
    averagingWindow = new AveragingWindow();
    if (!averagingWindow->decodefromAveragingWindow(
            nonDynamic5QIDescriptor->averagingWindow))
      return false;
  }
  if (nonDynamic5QIDescriptor->maximumDataBurstVolume) {
    maximumDataBurstVolume = new MaximumDataBurstVolume();
    if (!maximumDataBurstVolume->decodefromMaximumDataBurstVolume(
            nonDynamic5QIDescriptor->maximumDataBurstVolume))
      return false;
  }

  return true;
}
}  // namespace ngap
