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

#ifndef _QOSCHARACTERISTICS_H_
#define _QOSCHARACTERISTICS_H_

#include "Dynamic5QIDescriptor.hpp"
#include "NonDynamic5QIDescriptor.hpp"

extern "C" {
#include "Ngap_QosCharacteristics.h"
}

namespace ngap {

class QosCharacteristics {
 public:
  QosCharacteristics();
  virtual ~QosCharacteristics();

  void setQosCharacteristics(
      NonDynamic5QIDescriptor* m_nonDynamic5QIDescriptor);
  void setQosCharacteristics(Dynamic5QIDescriptor* m_dynamic5QIDescriptor);
  int QosCharacteristicsPresent();
  bool getQosCharacteristics(
      NonDynamic5QIDescriptor*& m_nonDynamic5QIDescriptor);
  bool getQosCharacteristics(Dynamic5QIDescriptor*& m_dynamic5QIDescriptor);
  bool encode2QosCharacteristics(Ngap_QosCharacteristics_t*);
  bool decodefromQosCharacteristics(Ngap_QosCharacteristics_t*);

 private:
  NonDynamic5QIDescriptor* nonDynamic5QIDescriptor;
  Dynamic5QIDescriptor* dynamic5QIDescriptor;
};
}  // namespace ngap

#endif
