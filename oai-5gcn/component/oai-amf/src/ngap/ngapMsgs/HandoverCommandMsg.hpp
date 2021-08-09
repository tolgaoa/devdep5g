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

#ifndef _HANDOVERCOMMAND_H_
#define _HANDOVERCOMMAND_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "Cause.hpp"
#include "DefaultPagingDRX.hpp"
#include "GlobalRanNodeId.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "RanNodeName.hpp"
#include "SupportedTAList.hpp"
extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_NGSetupRequest.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

typedef struct {
  uint8_t pduSessionId;
  OCTET_STRING_t HandoverCommandTransfer;
} PDUSessionResourceHandoverItem_t;

class HandoverCommandMsg {
 public:
  HandoverCommandMsg();
  virtual ~HandoverCommandMsg();

  void setMessageType();  // Initialize the PDU and populate the MessageType;
  void setAmfUeNgapId(unsigned long id);  // 40 bits
  void setRanUeNgapId(uint32_t id);       // 32 bits

  void setHandoverType(long type);
  void setPduSessionResourceHandoverList(
      std::vector<PDUSessionResourceHandoverItem_t> list);
  void setTargetToSource_TransparentContainer(OCTET_STRING_t targetTosource);

  int encode2buffer(uint8_t* buf, int buf_size);
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  unsigned long getAmfUeNgapId();  // return -1;(不存在)
  uint32_t getRanUeNgapId();       // return -1;(不存在)
  /*void getHandoverType(Ngap_HandoverType_t &handovertype);
  void getCause(Cause cause);
  void getTargetID(Ngap_TargetID_t targetID);
  void
  getDirectForwardingPathAvailability(Ngap_DirectForwardingPathAvailability_t
  directpathavailable); void
  getPDUSessionResourceList(Ngap_PDUSessionResourceHandoverList_t
  pdusessionresourcelist); void getSourceToTargetTransparentContainer();*/
 private:
  Ngap_NGAP_PDU_t* handoverCommandPdu;
  Ngap_HandoverCommand_t* handoverCommandIEs;
  /***************** for decoding ****************/
  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  Ngap_HandoverType_t* ngap_handovertype;
  Ngap_NASSecurityParametersFromNGRAN_t* NASSecurityParametersFromNGRAN;
  Ngap_PDUSessionResourceHandoverList_t* PDUSessionResourceHandoverList;
  Ngap_PDUSessionResourceToReleaseListHOCmd_t*
      PDUSessionResourceToReleaseListHOCmd;
  Ngap_TargetToSource_TransparentContainer_t*
      TargetToSource_TransparentContainer;
  Ngap_CriticalityDiagnostics_t* CriticalityDiagnostics;
};

}  // namespace ngap

#endif
