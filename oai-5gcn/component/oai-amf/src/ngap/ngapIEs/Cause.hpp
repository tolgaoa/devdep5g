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

#ifndef _CAUSE_H_
#define _CAUSE_H_

extern "C" {
#include "Ngap_Cause.h"
}

namespace ngap {

class Cause {
 public:
  Cause();
  virtual ~Cause();

  bool encode2Cause(Ngap_Cause_t* cause);
  bool decodefromCause(Ngap_Cause_t* pdu);
  void setChoiceOfCause(Ngap_Cause_PR m_causePresent);
  Ngap_Cause_PR getChoiceOfCause();
  void setValue(long);
  long getValue();

 private:
  long causeValue;
  Ngap_Cause_PR causePresent;
};
}  // namespace ngap

#endif
