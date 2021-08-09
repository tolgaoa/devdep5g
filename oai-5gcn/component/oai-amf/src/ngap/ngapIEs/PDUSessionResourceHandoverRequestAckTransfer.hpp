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
#ifndef _PDUSESSIONRESOURCEHANDOVERREQUESTACKTRANSFER_H_
#define _PDUSESSIONRESOURCEHANDOVERREQUESTACKTRANSFER_H_

#include "DLQoSFlowPerTNLInformation.hpp"
#include "NgapIEsStruct.hpp"
#include "QosFlowListWithDataForwarding.hpp"
#include "SecurityResult.hpp"
#include "UPTransportLayerInformation.hpp"
extern "C" {
#include "Ngap_HandoverRequestAcknowledgeTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}
namespace ngap {
class PDUSessionResourceHandoverRequestAckTransfer {
 public:
  PDUSessionResourceHandoverRequestAckTransfer();
  virtual ~PDUSessionResourceHandoverRequestAckTransfer();
  bool decodefromHandoverRequestAckTransfer(uint8_t* buf, int buf_size);
  bool getUpTransportLayerInformation2(GtpTunnel_t*& upTnlInfo);
  bool getqosFlowSetupResponseList(
      std::vector<QosFlowLItemWithDataForwarding_t>& list);

 private:
  Ngap_HandoverRequestAcknowledgeTransfer_t*
      handoverRequestAcknowledegTransferIEs;
  UpTransportLayerInformation* dLForwardingUP_TNLInformation;
  QosFlowListWithDataForwarding* QosFlowSetupResponseList;
};
}  // namespace ngap

#endif
