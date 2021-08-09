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

#ifndef _PDUSESSIONRESOURCESETUPRESPONSETRANSFER_H_
#define _PDUSESSIONRESOURCESETUPRESPONSETRANSFER_H_

#include "DLQoSFlowPerTNLInformation.hpp"
#include "NgapIEsStruct.hpp"
#include "SecurityResult.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupResponseTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class PduSessionResourceSetupResponseTransferIE {
 public:
  PduSessionResourceSetupResponseTransferIE();
  virtual ~PduSessionResourceSetupResponseTransferIE();

  void setDLQoSFlowPerTNLInformation(
      GtpTunnel_t uptlinfo, std::vector<AssociatedQosFlow_t> list);
  void setAdditionalDLQoSFlowPerTNLInformation(
      GtpTunnel_t uptlinfo, std::vector<AssociatedQosFlow_t> list);
  void setSecurityResult(
      e_Ngap_IntegrityProtectionResult integrityProtectionResult,
      e_Ngap_ConfidentialityProtectionResult confidentialityProtectionResult);
  // caus待续

  int encode2buffer(uint8_t* buf, int buf_size);
  // Decapsulation
  bool decodefromIE(uint8_t* buf, int buf_size);
  bool getDLQoSFlowPerTNLInformation(
      GtpTunnel_t& uptlinfo, std::vector<AssociatedQosFlow_t>& list);
  bool getAdditionalDLQoSFlowPerTNLInformation(
      GtpTunnel_t& uptlinfo, std::vector<AssociatedQosFlow_t>& list);
  bool getSecurityResult(
      long& integrityProtectionResult, long& confidentialityProtectionResult);
  // caus待续

 private:
  Ngap_PDUSessionResourceSetupResponseTransfer_t*
      pduSessionResourceSetupResponseTransferIEs;

  DLQoSFlowPerTNLInformation* dlQoSFlowPerTNLInformation;
  DLQoSFlowPerTNLInformation* additionalDLQoSFlowPerTNLInformation;
  SecurityResult* securityResult;
};

}  // namespace ngap
#endif
