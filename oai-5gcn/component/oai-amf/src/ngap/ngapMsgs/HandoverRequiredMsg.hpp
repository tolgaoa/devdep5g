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

#ifndef _HANDOVERREQUIRED_H_
#define _HANDOVERREQUIRED_H_

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
#include "NgapIEsStruct.hpp"
#include "PDUSessionResourceListHORqd.hpp"
#include "TAI.hpp"
namespace ngap {

class HandoverRequiredMsg {
 public:
  HandoverRequiredMsg();
  virtual ~HandoverRequiredMsg();

  int encode2buffer(uint8_t* buf, int buf_size);
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  unsigned long getAmfUeNgapId();  // return -1;(不存在)
  uint32_t getRanUeNgapId();       // return -1;(不存在)

  Ngap_HandoverType_t getHandoverType();
  Ngap_Cause_PR getChoiceOfCause();
  long getCauseValue();
  OCTET_STRING_t getSourceToTarget_TransparentContainer();
  void getGlobalRanNodeId(GlobalgNBId*& ptr);
  void getTAI(TAI*& ptr);
  bool getPDUSessionResourceList(std::vector<PDUSessionResourceItem_t>& list);
  long getDirectForwardingPathAvailability();

 private:
  Ngap_NGAP_PDU_t* handoverRequiredPdu;
  Ngap_HandoverRequired_t* handoverRequiredIEs;
  /***************** for decoding ****************/
  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  Ngap_HandoverType_t* handovertype;
  Cause* cause;
  Ngap_TargetID_t* targetid;
  Ngap_DirectForwardingPathAvailability_t* directforwardingPathAvailability;
  PDUSessionResourceListHORqd* PDUSessionResourceList;
  Ngap_SourceToTarget_TransparentContainer_t*
      SourceToTarget_TransparentContainer;
};

}  // namespace ngap

#endif
