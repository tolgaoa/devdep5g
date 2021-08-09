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

#ifndef _HANDOVERREQUESTACK_H_
#define _HANDOVERREQUESTACK_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "Cause.hpp"
#include "DefaultPagingDRX.hpp"
#include "GlobalRanNodeId.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "PDUSessionResourceAdmittedItem.hpp"
#include "PDUSessionResourceAdmittedList.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "RanNodeName.hpp"
#include "SupportedTAList.hpp"
extern "C" {
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Ngap_HandoverRequestAcknowledgeTransfer.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_NGSetupRequest.h"
#include "Ngap_PDUSessionResourceAdmittedItem.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class HandoverRequestAck {
 public:
  HandoverRequestAck();
  virtual ~HandoverRequestAck();

  int encode2buffer(uint8_t* buf, int buf_size);
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);

  unsigned long getAmfUeNgapId();  // return -1;
  uint32_t getRanUeNgapId();       // return -1;

  void setMessageType();  // Initialize the PDU and populate the MessageType;
  OCTET_STRING_t getTargetToSource_TransparentContainer();
  bool getPDUSessionResourceAdmittedList(
      std::vector<PDUSessionResourceAdmittedItem_t>& list);

 private:
  Ngap_NGAP_PDU_t* handoverRequestAckPdu;
  Ngap_HandoverRequestAcknowledge_t* handoverRequestAckIEs;
  /***************** for decoding ****************/
  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  Ngap_HandoverType_t* handovertype;

  PDUSessionResourceAdmittedList* pduSessionResourceAdmittedList;
  Ngap_PDUSessionResourceFailedToSetupListHOAck_t*
      PDUSessionResourceFailedToSetupList;
  Ngap_TargetToSource_TransparentContainer_t*
      TargetToSource_TransparentContainer;
  Ngap_CriticalityDiagnostics_t* CriticalityDiagnostics;
};

}  // namespace ngap

#endif
