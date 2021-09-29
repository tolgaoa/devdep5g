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

#ifndef _UPLINKNASTRANSPORT_H_
#define _UPLINKNASTRANSPORT_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "MessageType.hpp"
#include "NAS-PDU.hpp"
#include "NgapIEsStruct.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "UserLocationInformation.hpp"

extern "C" {
#include "Ngap_InitialUEMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class UplinkNASTransportMsg {
 public:
  UplinkNASTransportMsg();
  virtual ~UplinkNASTransportMsg();

  void setMessageType();

  void setAmfUeNgapId(unsigned long id);  // 40 bits
  void setRanUeNgapId(uint32_t id);       // 32 bits
  void setNasPdu(uint8_t* nas, size_t sizeofnas);
  void setUserLocationInfoNR(struct NrCgi_s cig, struct Tai_s tai);
  int encode2buffer(uint8_t* buf, int buf_size);
  // Decapsulation
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  unsigned long getAmfUeNgapId();
  uint32_t getRanUeNgapId();
  bool getNasPdu(uint8_t*& nas, size_t& sizeofnas);
  bool getUserLocationInfoNR(struct NrCgi_s& cig, struct Tai_s& tai);

 private:
  Ngap_NGAP_PDU_t* uplinkNASTransportPdu;
  Ngap_UplinkNASTransport_t* uplinkNASTransportIEs;

  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  NAS_PDU* nasPdu;
  UserLocationInformation* userLocationInformation;
};

}  // namespace ngap
#endif
