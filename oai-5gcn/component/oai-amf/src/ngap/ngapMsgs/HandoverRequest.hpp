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

#ifndef _HANDOVERREQUEST_H_
#define _HANDOVERREQUEST_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "Cause.hpp"
#include "DefaultPagingDRX.hpp"
#include "GlobalRanNodeId.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "RanNodeName.hpp"
#include "SupportedTAList.hpp"
#include "UEAggregateMaxBitRate.hpp"
extern "C" {
#include "Ngap_AllowedNSSAI-Item.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_NGSetupRequest.h"
#include "Ngap_ProtocolIE-Field.h"
}

#include "GUAMI.hpp"
#include "MobilityRestrictionList.hpp"
#include "PDUSessionResourceSetupListHOReq.hpp"
#include "S-NSSAI.hpp"
#include "SecurityKey.hpp"
#include "UESecurityCapabilities.hpp"
namespace ngap {

class HandoverRequest {
 public:
  HandoverRequest();
  virtual ~HandoverRequest();

  void setMessageType();  // Initialize the PDU and populate the MessageType;
  void setAmfUeNgapId(unsigned long id);  // 40 bits
  void setHandoverType(long type);
  void setCause(Ngap_Cause_PR m_causePresent, long value);
  void setUEAggregateMaximumBitRate(
      long bit_rate_downlink, long bit_rate_uplink);
  void setUESecurityCapabilities(
      uint16_t m_NR_EncryptionAlgs, uint16_t m_NR_IntegrityProtectionAlgs,
      uint16_t m_E_UTRA_EncryptionAlgs,
      uint16_t m_E_UTRA_IntegrityProtectionAlgs);
  void setSecurityContext(long count, uint8_t* buffer);

  void setPduSessionResourceSetupList(
      std::vector<PDUSessionResourceSetupRequestItem_t> list);

  void setSourceToTarget_TransparentContainer(OCTET_STRING_t sourceTotarget);

  void setAllowedNSSAI(std::vector<S_NSSAI> list);
  void setGUAMI(
      PlmnId* m_plmnId, AMFRegionID* m_aMFRegionID, AMFSetID* m_aMFSetID,
      AMFPointer* m_aMFPointer);
  void setMobilityRestrictionList(PlmnId* m_plmnId);
  int encode2buffer(uint8_t* buf, int buf_size);
  // bool decodefrompdu(Ngap_NGAP_PDU_t *ngap_msg_pdu);
  unsigned long getAmfUeNgapId();  // return -1;

 private:
  Ngap_NGAP_PDU_t* handoverRequestPdu;
  Ngap_HandoverRequest_t* handoverRequestIEs;
  /***************** for decoding ****************/
  AMF_UE_NGAP_ID* amfUeNgapId;
  Ngap_HandoverType_t* handovertype;
  Cause* cause;
  UEAggregateMaxBitRate* ueAggregateMaximumBitRate;
  // Core Network Assistance Information for RRC INACTIVE
  UESecurityCapabilities* ueSecurityCapabilities;
  Ngap_SecurityContext_t* SecurityContext;
  // New Security Context Indicator
  // NASC
  PDUSessionResourceSetupListHOReq* PDUSessionResourceSetupList;
  Ngap_AllowedNSSAI_t* allowedNSSAI;
  // Trace Activation
  // Masked IMEISV
  Ngap_SourceToTarget_TransparentContainer_t*
      SourceToTarget_TransparentContainer;
  // Mobility Restriction List
  // Location Reporting Request Type
  // RRC Inactive Transition Report Request
  GUAMI* guami;
  MobilityRestrictionList* mobilityrestrictionlist;
  // Redirection for Voice EPS Fallback
  // CN Assisted RAN Parameters Tuning
};

}  // namespace ngap

#endif
