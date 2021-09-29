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
#include "PDUSessionResourceHandoverRequestAckTransfer.hpp"

extern "C" {
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

#include <iostream>
using namespace std;

namespace ngap {
PDUSessionResourceHandoverRequestAckTransfer::
    PDUSessionResourceHandoverRequestAckTransfer() {
  handoverRequestAcknowledegTransferIEs =
      (Ngap_HandoverRequestAcknowledgeTransfer_t*) calloc(
          1, sizeof(Ngap_HandoverRequestAcknowledgeTransfer_t));
  dLForwardingUP_TNLInformation = NULL;
  QosFlowSetupResponseList      = NULL;
}
PDUSessionResourceHandoverRequestAckTransfer::
    ~PDUSessionResourceHandoverRequestAckTransfer() {}
bool PDUSessionResourceHandoverRequestAckTransfer::
    decodefromHandoverRequestAckTransfer(uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_HandoverRequestAcknowledgeTransfer,
      (void**) &handoverRequestAcknowledegTransferIEs, buf, buf_size);
  if (rc.code == RC_OK) {
    cout << "Decoded handoverRequestAcknowledegTransfer successfully" << endl;
  } else if (rc.code == RC_WMORE) {
    cout << "More data expected, call again" << endl;
    return false;
  } else {
    cout << "Failure to decode handoverRequestAcknowledegTransfer data" << endl;
    // return false;
  }
  cout << "rc.consumed to decode = " << rc.consumed << endl;
  cout << endl;
  dLForwardingUP_TNLInformation = new UpTransportLayerInformation();
  if (!dLForwardingUP_TNLInformation->decodefromUpTransportLayerInformation(
          *handoverRequestAcknowledegTransferIEs
               ->dLForwardingUP_TNLInformation)) {
    cout << "decoded ngap DL_NGU_UP_TNLInformation IE error" << endl;
    return false;
  }
  QosFlowSetupResponseList = new QosFlowListWithDataForwarding();
  if (!QosFlowSetupResponseList->decodeFormQosFlowListWithDataForwarding(
          handoverRequestAcknowledegTransferIEs->qosFlowSetupResponseList)) {
    cout << "decoded ngap QosFlowSetupResponseList IE error" << endl;
    return false;
  }
  return true;
}
bool PDUSessionResourceHandoverRequestAckTransfer::
    getUpTransportLayerInformation2(GtpTunnel_t*& upTnlInfo) {
  if (!dLForwardingUP_TNLInformation->decodefromUpTransportLayerInformation(
          *handoverRequestAcknowledegTransferIEs
               ->dLForwardingUP_TNLInformation))
    return false;
  TransportLayerAddress* m_transportLayerAddress;
  GtpTeid* m_gtpTeid;
  if (!dLForwardingUP_TNLInformation->getUpTransportLayerInformation(
          m_transportLayerAddress, m_gtpTeid))
    return false;
  if (!m_transportLayerAddress->getTransportLayerAddress(upTnlInfo->ip_address))
    return false;
  if (!m_gtpTeid->getGtpTeid(upTnlInfo->gtp_teid)) return false;
  return true;
}
bool PDUSessionResourceHandoverRequestAckTransfer::getqosFlowSetupResponseList(
    std::vector<QosFlowLItemWithDataForwarding_t>& list) {
  if (!QosFlowSetupResponseList) return false;
  QosFlowItemWithDataForWarding* m_qosflowitemwithdataforwarding;
  int num = 0;
  if (QosFlowSetupResponseList->getQosFlowListWithDataForwarding(
          m_qosflowitemwithdataforwarding, num)) {
    cout << "successful decode QosFlowList" << endl;
  }
  for (int i = 0; i < num; i++) {
    QosFlowLItemWithDataForwarding_t response;
    m_qosflowitemwithdataforwarding[i].getQosFlowItemWithDataForWarding(
        response.qosFlowIdentifier);
    list.push_back(response);
  }
  return true;
}
}  // namespace ngap
