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

#include "AllocationAndRetentionPriority.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
AllocationAndRetentionPriority::AllocationAndRetentionPriority() {
  priorityLevelARP         = nullptr;
  pre_emptionCapability    = nullptr;
  pre_emptionVulnerability = nullptr;
}

//------------------------------------------------------------------------------
AllocationAndRetentionPriority::~AllocationAndRetentionPriority() {}

//------------------------------------------------------------------------------
void AllocationAndRetentionPriority::setAllocationAndRetentionPriority(
    PriorityLevelARP* m_priorityLevelARP,
    Pre_emptionCapability* m_pre_emptionCapability,
    Pre_emptionVulnerability* m_pre_emptionVulnerability) {
  priorityLevelARP         = m_priorityLevelARP;
  pre_emptionCapability    = m_pre_emptionCapability;
  pre_emptionVulnerability = m_pre_emptionVulnerability;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::getAllocationAndRetentionPriority(
    PriorityLevelARP*& m_priorityLevelARP,
    Pre_emptionCapability*& m_pre_emptionCapability,
    Pre_emptionVulnerability*& m_pre_emptionVulnerability) {
  if (!priorityLevelARP) return false;
  m_priorityLevelARP = priorityLevelARP;
  if (!pre_emptionCapability) return false;
  m_pre_emptionCapability = pre_emptionCapability;
  if (!pre_emptionVulnerability) return false;
  m_pre_emptionVulnerability = pre_emptionVulnerability;

  return true;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::encode2AllocationAndRetentionPriority(
    Ngap_AllocationAndRetentionPriority_t* allocationAndRetentionPriority) {
  if (!priorityLevelARP->encode2PriorityLevelARP(
          &allocationAndRetentionPriority->priorityLevelARP))
    return false;
  if (!pre_emptionCapability->encode2Pre_emptionCapability(
          &allocationAndRetentionPriority->pre_emptionCapability))
    return false;
  if (!pre_emptionVulnerability->encode2Pre_emptionVulnerability(
          &allocationAndRetentionPriority->pre_emptionVulnerability))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool AllocationAndRetentionPriority::decodefromAllocationAndRetentionPriority(
    Ngap_AllocationAndRetentionPriority_t* allocationAndRetentionPriority) {
  if (priorityLevelARP == nullptr) priorityLevelARP = new PriorityLevelARP();
  if (pre_emptionCapability == nullptr)
    pre_emptionCapability = new Pre_emptionCapability();
  if (pre_emptionVulnerability == nullptr)
    pre_emptionVulnerability = new Pre_emptionVulnerability();
  if (!priorityLevelARP->decodefromPriorityLevelARP(
          &allocationAndRetentionPriority->priorityLevelARP))
    return false;
  if (!pre_emptionCapability->decodefromPre_emptionCapability(
          &allocationAndRetentionPriority->pre_emptionCapability))
    return false;
  if (!pre_emptionVulnerability->decodefromPre_emptionVulnerability(
          &allocationAndRetentionPriority->pre_emptionVulnerability))
    return false;

  return true;
}
}  // namespace ngap
