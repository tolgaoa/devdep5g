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

#ifndef _QOSFLOWTOBEFORWARDEDLIST_H_
#define _QOSFLOWTOBEFORWARDEDLIST_H_

#include "QosFlowToBeForwardedItem.hpp"
extern "C" {
#include "Ngap_QosFlowToBeForwardedList.h"
}
namespace ngap {
class QosFlowToBeForwardedList {
 public:
  QosFlowToBeForwardedList();
  virtual ~QosFlowToBeForwardedList();
  void setQosFlowToBeForwardedItem(
      QosFlowToBeForwardedItem* m_qosflowtobeforwardeditem, int num);
  bool encodeQosFlowToBeForwardedList(
      Ngap_QosFlowToBeForwardedList*& m_ngap_qosflowtobeforwardedlist);

 private:
  QosFlowToBeForwardedItem* Qosflowtobeforwardeditem;
  int numofqosflowtobeforwardeditem;
};
}  // namespace ngap
#endif
