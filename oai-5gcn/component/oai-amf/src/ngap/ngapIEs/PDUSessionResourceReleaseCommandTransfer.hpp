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

#ifndef _PDUSESSIONRESOURCERELEASECOMMANDTRANSFER_H_
#define _PDUSESSIONRESOURCERELEASECOMMANDTRANSFER_H_

#include "Cause.hpp"
#include "NgapIEsStruct.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceReleaseCommandTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class PDUSessionResourceReleaseCommandTransfer {
 public:
  PDUSessionResourceReleaseCommandTransfer();
  virtual ~PDUSessionResourceReleaseCommandTransfer();

  void setCauseRadioNetwork(e_Ngap_CauseRadioNetwork cause_value);
  void setCauseTransport(e_Ngap_CauseTransport cause_value);
  void setCauseNas(e_Ngap_CauseNas cause_value);
  void setCauseProtocol(e_Ngap_CauseProtocol cause_value);
  void setCauseMisc(e_Ngap_CauseMisc cause_value);
  int encode2buffer(uint8_t* buf, int buf_size);
  // Decapsulation
  bool decodefromIE(uint8_t* buf, int buf_size);
  long getChoiceOfCause();
  long getCause();

 private:
  Ngap_PDUSessionResourceReleaseCommandTransfer_t*
      pduSessionResourceReleaseCommandTransferIEs;

  Cause* causeValue;
};

}  // namespace ngap
#endif
