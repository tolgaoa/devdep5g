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

#ifndef _PDUSESSIONRESOURCESETUPREQUESTTRANSFER_H_
#define _PDUSESSIONRESOURCESETUPREQUESTTRANSFER_H_

#include <vector>

#include "DataForwardingNotPossible.hpp"
#include "NetworkInstance.hpp"
#include "NgapIEsStruct.hpp"
#include "PDUSessionType.hpp"
#include "PduSessionAggregateMaximumBitRate.hpp"
#include "QosFlowSetupRequestList.hpp"
#include "SecurityIndication.hpp"
#include "UPTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupRequestTransfer.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class PduSessionResourceSetupRequestTransferIE {
 public:
  PduSessionResourceSetupRequestTransferIE();
  virtual ~PduSessionResourceSetupRequestTransferIE();

  void setPduSessionAggregateMaximumBitRate(
      long bit_rate_downlink,
      long bit_rate_uplink);                                  // O
  void setUL_NG_U_UP_TNL_Information(GtpTunnel_t upTnlInfo);  // M
  // void
  // setAdditionalUL_NG_U_UP_TNL_Information(std::vector<GtpTunnel>list);//O
  void setDataForwardingNotPossible();                 // O
  void setPduSessionType(e_Ngap_PDUSessionType type);  // M
  void setSecurityIndication(
      e_Ngap_IntegrityProtectionIndication integrity_protection,
      e_Ngap_ConfidentialityProtectionIndication confidentiality_protection,
      e_Ngap_MaximumIntegrityProtectedDataRate maxIntProtDataRate);
  void setSecurityIndication(
      e_Ngap_IntegrityProtectionIndication integrity_protection,
      e_Ngap_ConfidentialityProtectionIndication confidentiality_protection);
  void setNetworkInstance(long value);
  void setQosFlowSetupRequestList(std::vector<QosFlowSetupReq_t> list);
  int encode2buffer(uint8_t* buf, int buf_size);
  // Decapsulation
  bool decodefromIE(uint8_t* buf, int buf_size);
  bool getPduSessionAggregateMaximumBitRate(
      long& bit_rate_downlink,
      long& bit_rate_uplink);                                  // O
  bool getUL_NG_U_UP_TNL_Information(GtpTunnel_t& upTnlInfo);  // M
  // bool
  // getAdditionalUL_NG_U_UP_TNL_Information(std::vector<GtpTunnel>&list);//O
  bool getDataForwardingNotPossible();  // O
  bool getPduSessionType(long& type);   // M
  bool getSecurityIndication(
      long& integrity_protection, long& confidentiality_protection,
      long& maxIntProtDataRate);
  bool getNetworkInstance(long& value);
  bool getQosFlowSetupRequestList(std::vector<QosFlowSetupReq_t>& list);

 private:
  Ngap_PDUSessionResourceSetupRequestTransfer_t*
      pduSessionResourceSetupRequestTransferIEs;

  PduSessionAggregateMaximumBitRate* pduSessionAggregateMaximumBitRateIE;
  UpTransportLayerInformation* upTransportLayerInformation;
  DataForwardingNotPossible* dataForwardingNotPossible;
  PDUSessionType* pduSessionType;
  SecurityIndication* securityIndication;
  NetworkInstance* networkInstance;
  QosFlowSetupRequestList* qosFlowSetupRequestList;
};

}  // namespace ngap
#endif
