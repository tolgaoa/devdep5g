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

#include "PDUSessionResourceHandoverCommandTransfer.hpp"
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
PDUSessionResourceHandoverCommandTransfer::
    PDUSessionResourceHandoverCommandTransfer() {
  handovercommandtransferIE = (Ngap_HandoverCommandTransfer_t*) calloc(
      1, sizeof(Ngap_HandoverCommandTransfer_t));
  dLForwardingUP_TNLInformation = NULL;
  qosFlowToBeForwardedList      = NULL;
}
PDUSessionResourceHandoverCommandTransfer::
    ~PDUSessionResourceHandoverCommandTransfer() {}
void PDUSessionResourceHandoverCommandTransfer::setQosFlowToBeForwardedList(
    std::vector<QosFlowToBeForwardedItem_t> list) {
  if (!qosFlowToBeForwardedList)
    qosFlowToBeForwardedList = new QosFlowToBeForwardedList();
  QosFlowToBeForwardedItem* m_item =
      new QosFlowToBeForwardedItem[list.size()]();
  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier* m_qosFlowIdentifier = new QosFlowIdentifier();
    m_qosFlowIdentifier->setQosFlowIdentifier(list[i].QFI);
    m_item[i].setQosFlowIdentifier(m_qosFlowIdentifier);
  }
  qosFlowToBeForwardedList->setQosFlowToBeForwardedItem(m_item, list.size());
  int ret = qosFlowToBeForwardedList->encodeQosFlowToBeForwardedList(
      handovercommandtransferIE->qosFlowToBeForwardedList);
  cout << "QFI in list "
       << handovercommandtransferIE->qosFlowToBeForwardedList->list.count
       << endl;
  if (handovercommandtransferIE->qosFlowToBeForwardedList->list.array) {
    cout << "array OK" << endl;
    if (handovercommandtransferIE->qosFlowToBeForwardedList->list.array[0]) {
      cout << "QFI in list "
           << handovercommandtransferIE->qosFlowToBeForwardedList->list
                  .array[0]
                  ->qosFlowIdentifier
           << endl;
    } else {
      cout << "error1" << endl;
    }
  } else {
    cout << "error2" << endl;
  }

  if (!ret) {
    cout << "encode QosFlowToBeForwardedList IE error" << endl;
    return;
  }
}
void PDUSessionResourceHandoverCommandTransfer::setUPTransportLayerInformation(
    GtpTunnel_t uptlinfo) {
  if (!dLForwardingUP_TNLInformation) {
    dLForwardingUP_TNLInformation = new UpTransportLayerInformation();
  }
  TransportLayerAddress* m_transportLayerAddress = new TransportLayerAddress();
  GtpTeid* m_gtpTeid                             = new GtpTeid();
  m_transportLayerAddress->setTransportLayerAddress(uptlinfo.ip_address);
  m_gtpTeid->setGtpTeid(uptlinfo.gtp_teid);
  dLForwardingUP_TNLInformation->setUpTransportLayerInformation(
      m_transportLayerAddress, m_gtpTeid);
  Ngap_UPTransportLayerInformation* test =
      (Ngap_UPTransportLayerInformation*) calloc(
          1, sizeof(Ngap_UPTransportLayerInformation));
  handovercommandtransferIE->dLForwardingUP_TNLInformation = test;
  int ret =
      dLForwardingUP_TNLInformation->encode2UpTransportLayerInformation(*test);
  if (!ret) {
    cout << "encode dLForwardingUP_TNLInformation IE error";
    return;
  }
}
int PDUSessionResourceHandoverCommandTransfer::
    encodePDUSessionResourceHandoverCommandTransfer(
        uint8_t* buf, int buf_size) {
  asn_fprint(
      stderr, &asn_DEF_Ngap_HandoverCommandTransfer, handovercommandtransferIE);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_HandoverCommandTransfer, NULL, handovercommandtransferIE,
      buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

}  // namespace ngap
