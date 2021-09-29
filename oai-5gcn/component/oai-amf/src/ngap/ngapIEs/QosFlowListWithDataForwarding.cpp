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
 \author  niuxiansheng-niu, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */
#include "QosFlowListWithDataForwarding.hpp"

#include <iostream>
using namespace std;
namespace ngap {
QosFlowListWithDataForwarding::QosFlowListWithDataForwarding() {
  qosFlowItemWithDataForwarding      = NULL;
  numofQosFlowItemWithDataForwarding = 0;
}
QosFlowListWithDataForwarding::~QosFlowListWithDataForwarding() {}

bool QosFlowListWithDataForwarding::decodeFormQosFlowListWithDataForwarding(
    Ngap_QosFlowSetupResponseListHOReqAck_t qosFlowSetupResponseList) {
  numofQosFlowItemWithDataForwarding = qosFlowSetupResponseList.list.count;
  qosFlowItemWithDataForwarding =
      new QosFlowItemWithDataForWarding[numofQosFlowItemWithDataForwarding]();
  for (int i = 0; i < numofQosFlowItemWithDataForwarding; i++) {
    if (!qosFlowItemWithDataForwarding[i]
             .decodeformQosFlowItemWithDataForWarding(
                 qosFlowSetupResponseList.list.array[i]))
      return false;
  }
  return true;
}
bool QosFlowListWithDataForwarding::getQosFlowListWithDataForwarding(
    QosFlowItemWithDataForWarding*& m_QosFlowItemWithDataForwarding, int& num) {
  m_QosFlowItemWithDataForwarding = qosFlowItemWithDataForwarding;
  num                             = numofQosFlowItemWithDataForwarding;
  return true;
}
}  // namespace ngap
