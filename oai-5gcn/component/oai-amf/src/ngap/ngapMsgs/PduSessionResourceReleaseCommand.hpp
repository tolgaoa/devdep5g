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
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _PDU_SESSION_RESOURCE_RELEASE_COMMAND_H_
#define _PDU_SESSION_RESOURCE_RELEASE_COMMAND_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "MessageType.hpp"
#include "NAS-PDU.hpp"
#include "NgapIEsStruct.hpp"
#include "PDUSessionResourceToReleaseListRelCmd.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "RANPagingPriority.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class PduSessionResourceReleaseCommandMsg {
 public:
  PduSessionResourceReleaseCommandMsg();
  virtual ~PduSessionResourceReleaseCommandMsg();

  void setMessageType();
  void setAmfUeNgapId(unsigned long id);  // 40 bits
  void setRanUeNgapId(uint32_t id);       // 32 bits
  void setRanPagingPriority(uint8_t priority);
  void setNasPdu(uint8_t* nas, size_t sizeofnas);
  void setPduSessionResourceToReleaseList(
      std::vector<PDUSessionResourceToReleaseItem_t> list);

  unsigned long getAmfUeNgapId();
  uint32_t getRanUeNgapId();
  int getRanPagingPriority();
  bool getNasPdu(uint8_t*& nas, size_t& sizeofnas);
  bool getPduSessionResourceToReleaseList(
      std::vector<PDUSessionResourceToReleaseItem_t>& list);

  int encode2buffer(uint8_t* buf, int buf_size);
  void encode2buffer_new(char* buf, int& encoded_size);
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);

 private:
  Ngap_NGAP_PDU_t* pduSessionResourceReleaseCommandPdu;
  Ngap_PDUSessionResourceReleaseCommand_t*
      pduSessionResourceReleaseCommandIEs;  // store list of IEs

  // section 9.2.1.3 PDU Session Resource Release Command (3GPP TS 38.413
  // V16.0.0 (2019-12))
  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  RANPagingPriority* ranPagingPriority;
  NAS_PDU* nasPdu;
  PDUSessionResourceToReleaseListRelCmd* pduSessionResourceToReleaseList;
};

}  // namespace ngap
#endif
