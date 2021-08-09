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

#ifndef _UERADIOCAPABILITYINFOINDICATION_H_
#define _UERADIOCAPABILITYINFOINDICATION_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "MessageType.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "UERadioCapability.hpp"
#include "UERadioCapabilityForPaging.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_UERadioCapabilityInfoIndication.h"
}

namespace ngap {

class UeRadioCapabilityInfoIndicationMsg {
 public:
  UeRadioCapabilityInfoIndicationMsg();
  virtual ~UeRadioCapabilityInfoIndicationMsg();

  void setMessageType();
  void setAmfUeNgapId(unsigned long id);  // 40 bits
  void setRanUeNgapId(uint32_t id);       // 32 bits
  void setUERadioCapability(uint8_t* buf, size_t size);
  void setUERadioCapabilityForPaging(
      uint8_t* nr, size_t sizeofnr, uint8_t* eutra, size_t sizeofeutra);
  int encode2buffer(uint8_t* buf, int buf_size);
  // Decapsulation
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  bool getAmfUeNgapId(unsigned long& id);
  bool getRanUeNgapId(uint32_t& id);
  bool getUERadioCapability(uint8_t*& buf, size_t& size);
  bool getUERadioCapabilityForPaging(
      uint8_t*& nr, size_t& sizeofnr, uint8_t*& eutra, size_t& sizeofeutra);

 private:
  Ngap_NGAP_PDU_t* ueRadioCapabilityInfoIndicationPdu;
  Ngap_UERadioCapabilityInfoIndication_t* ueRadioCapabilityInfoIndicationIEs;

  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  UERadioCapability* ueRadioCapability;
  UERadioCapabilityForPaging* ueRadioCapabilityForPaging;
};

}  // namespace ngap
#endif
