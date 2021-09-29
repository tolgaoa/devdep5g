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

#ifndef _PDUSessionResourceHandoverCommandTransfer_H_
#define _PDUSessionResourceHandoverCommandTransfer_H_

#include "NgapIEsStruct.hpp"
#include "QosFlowToBeForwardedList.hpp"
#include "UPTransportLayerInformation.hpp"
extern "C" {
#include "Ngap_HandoverCommandTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}
namespace ngap {
class PDUSessionResourceHandoverCommandTransfer {
 private:
  /* data */
  Ngap_HandoverCommandTransfer_t* handovercommandtransferIE;
  UpTransportLayerInformation* dLForwardingUP_TNLInformation;
  QosFlowToBeForwardedList* qosFlowToBeForwardedList;

 public:
  PDUSessionResourceHandoverCommandTransfer(/* args */);
  virtual ~PDUSessionResourceHandoverCommandTransfer();
  void setUPTransportLayerInformation(GtpTunnel_t uptlinfo);
  void setQosFlowToBeForwardedList(
      std::vector<QosFlowToBeForwardedItem_t> list);
  int encodePDUSessionResourceHandoverCommandTransfer(
      uint8_t* buf, int buf_size);
};

}  // namespace ngap
#endif
