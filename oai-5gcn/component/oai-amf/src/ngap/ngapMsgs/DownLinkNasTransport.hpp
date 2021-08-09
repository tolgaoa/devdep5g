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

#ifndef _DOWNLINKNASTRANSPORT_H_
#define _DOWNLINKNASTRANSPORT_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "AMFName.hpp"
#include "IndexToRFSP.hpp"
#include "MessageType.hpp"
#include "NAS-PDU.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "RANPagingPriority.hpp"

extern "C" {
#include "Ngap_DownlinkNASTransport.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class DownLinkNasTransportMsg {
 public:
  DownLinkNasTransportMsg();
  virtual ~DownLinkNasTransportMsg();

  void setMessageType();
  void setAmfUeNgapId(unsigned long id);  // 40 bits
  void setRanUeNgapId(uint32_t id);       // 32 bits
  void setOldAmfName(const std::string name);
  void setRanPagingPriority(uint8_t pagingPriority);  // 1~256
  void setNasPdu(uint8_t* nas, size_t sizeofnas);
  void setIndex2Rat_Frequency_SelectionPriority(uint8_t value);  // 1~256
  int encode2buffer(uint8_t* buf, int buf_size);
  // Decapsulation
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  unsigned long getAmfUeNgapId();
  uint32_t getRanUeNgapId();
  bool getOldAmfName(std::string& name);
  uint8_t getRanPagingPriority();
  bool getNasPdu(uint8_t*& nas, size_t& sizeofnas);
  uint8_t getIndex2Rat_Frequency_SelectionPriority();

 private:
  Ngap_NGAP_PDU_t* downLinkNasTransportPdu;
  Ngap_DownlinkNASTransport_t* downLinkNasTransportIEs;

  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  AmfName* oldAmfName;
  RANPagingPriority* ranPagingPriority;
  NAS_PDU* nasPdu;
  IndexToRFSP* indexToRFSP;
};

}  // namespace ngap
#endif
