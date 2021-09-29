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

#ifndef _INITIALCONTEXTSETUPREQUEST_H_
#define _INITIALCONTEXTSETUPREQUEST_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "AMFName.hpp"
#include "AllowedNssai.hpp"
#include "CoreNetworkAssistanceInformation.hpp"
#include "GUAMI.hpp"
#include "MessageType.hpp"
#include "NAS-PDU.hpp"
#include "NgapIEsStruct.hpp"
#include "PDUSessionResourceSetupListCxtReq.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "SecurityKey.hpp"
#include "UEAggregateMaxBitRate.hpp"
#include "UERadioCapability.hpp"
#include "UESecurityCapabilities.hpp"

extern "C" {
#include "Ngap_InitialContextSetupRequest.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class InitialContextSetupRequestMsg {
 public:
  InitialContextSetupRequestMsg();
  virtual ~InitialContextSetupRequestMsg();

  void setMessageType();
  void setAmfUeNgapId(unsigned long id);  // 40 bits
  void setRanUeNgapId(uint32_t id);       // 32 bits
  void setOldAmfName(const std::string name);

  void setUEAggregateMaxBitRate(long bit_rate_downlink, long bit_rate_uplink);
  void setCoreNetworkAssistanceInfo(
      uint16_t ueIdentityIndexValue /*10bits*/, e_Ngap_PagingDRX ueSpecificDrx,
      uint8_t periodicRegUpdateTimer, bool micoModeInd,
      std::vector<Tai_t> taiListForRRcInactive);
  void setCoreNetworkAssistanceInfo(
      uint16_t ueIdentityIndexValue /*10bits*/, uint8_t periodicRegUpdateTimer,
      bool micoModeInd, std::vector<Tai_t> taiListForRRcInactive);
  void setGuami(Guami_t value);
  void setPduSessionResourceSetupRequestList(
      std::vector<PDUSessionResourceSetupRequestItem_t> list);
  void setAllowedNssai(std::vector<S_Nssai> list);
  void setUESecurityCapability(
      uint16_t NR_EncryptionAlgs, uint16_t NR_IntegrityProtectionAlgs,
      uint16_t E_UTRA_EncryptionAlgs, uint16_t E_UTRA_IntegrityProtectionAlgs);
  void setSecurityKey(uint8_t* key);  // 256bits
  void setNasPdu(uint8_t* nas, size_t sizeofnas);
  int encode2buffer(uint8_t* buf, int buf_size);
  void setUERadioCapability(uint8_t* buf, size_t size);
  // Decapsulation
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  unsigned long getAmfUeNgapId();
  uint32_t getRanUeNgapId();
  bool getOldAmfName(std::string& name);
  bool getUEAggregateMaxBitRate(long& bit_rate_downlink, long& bit_rate_uplink);
  bool getCoreNetworkAssistanceInfo(
      uint16_t& ueIdentityIndexValue /*10bits*/, int& ueSpecificDrx,
      uint8_t& periodicRegUpdateTimer, bool& micoModeInd,
      std::vector<Tai_t>& taiListForRRcInactive);
  bool getGuami(Guami_t& value);
  bool getPduSessionResourceSetupRequestList(
      std::vector<PDUSessionResourceSetupRequestItem_t>& list);
  bool getAllowedNssai(std::vector<S_Nssai>& list);
  bool getUESecurityCapability(
      uint16_t& NR_EncryptionAlgs, uint16_t& NR_IntegrityProtectionAlgs,
      uint16_t& E_UTRA_EncryptionAlgs,
      uint16_t& E_UTRA_IntegrityProtectionAlgs);
  bool getSecurityKey(uint8_t*& key);  // 256bits
  bool getNasPdu(uint8_t*& nas, size_t& sizeofnas);

 private:
  Ngap_NGAP_PDU_t* initialContextSetupRequestPdu;
  Ngap_InitialContextSetupRequest_t* initialContextSetupRequestIEs;

  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  AmfName* oldAmfName;
  UEAggregateMaxBitRate* uEAggregateMaxBitRate;
  CoreNetworkAssistanceInfo* coreNetworkAssistanceInfo;
  GUAMI* guami;
  PDUSessionResourceSetupListCxtReq* pduSessionResourceSetupRequestList;
  AllowedNSSAI* allowedNssai;
  UESecurityCapabilities* uESecurityCapabilities;
  SecurityKey* securityKey;
  NAS_PDU* nasPdu;
  UERadioCapability* ueRadioCapability;
};

}  // namespace ngap
#endif
