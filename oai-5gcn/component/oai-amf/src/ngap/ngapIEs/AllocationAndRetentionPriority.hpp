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

#ifndef _ALLOCATIONANDRETENTIONPRIORITY_H_
#define _ALLOCATIONANDRETENTIONPRIORITY_H_

#include "Pre-emptionCapability.hpp"
#include "Pre-emptionVulnerability.hpp"
#include "PriorityLevelARP.hpp"

extern "C" {
#include "Ngap_AllocationAndRetentionPriority.h"
}

namespace ngap {

class AllocationAndRetentionPriority {
 public:
  AllocationAndRetentionPriority();
  virtual ~AllocationAndRetentionPriority();

  void setAllocationAndRetentionPriority(
      PriorityLevelARP* m_priorityLevelARP,
      Pre_emptionCapability* m_pre_emptionCapability,
      Pre_emptionVulnerability* m_pre_emptionVulnerability);
  bool getAllocationAndRetentionPriority(
      PriorityLevelARP*& m_priorityLevelARP,
      Pre_emptionCapability*& m_pre_emptionCapability,
      Pre_emptionVulnerability*& m_pre_emptionVulnerability);

  bool encode2AllocationAndRetentionPriority(
      Ngap_AllocationAndRetentionPriority_t*);
  bool decodefromAllocationAndRetentionPriority(
      Ngap_AllocationAndRetentionPriority_t*);

 private:
  PriorityLevelARP* priorityLevelARP;
  Pre_emptionCapability* pre_emptionCapability;
  Pre_emptionVulnerability* pre_emptionVulnerability;
};
}  // namespace ngap

#endif
