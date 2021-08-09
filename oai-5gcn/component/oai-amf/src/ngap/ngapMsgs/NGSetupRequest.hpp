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

#ifndef _NGSETUPREQUEST_H_
#define _NGSETUPREQUEST_H_

#include "DefaultPagingDRX.hpp"
#include "GlobalRanNodeId.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "RanNodeName.hpp"
#include "SupportedTAList.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_NGSetupRequest.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class NGSetupRequestMsg {
 public:
  NGSetupRequestMsg();
  virtual ~NGSetupRequestMsg();

  // External interfaces
  // Encapsulation
  void setMessageType();  // Initialize the PDU and populate the MessageType;
  void setGlobalRanNodeID(
      const std::string mcc, const std::string mnc,
      Ngap_GlobalRANNodeID_PR ranNodeType, uint32_t ranNodeId);
  void setRanNodeName(const std::string ranNodeName);
  void setSupportedTAList(const std::vector<struct SupportedItem_s> list);
  void setDefaultPagingDRX(e_Ngap_PagingDRX value);
  int encode2buffer(uint8_t* buf, int buf_size);
  // Decapsulation
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  bool getGlobalGnbID(uint32_t& gnbId, std::string& mcc, std::string& mnc);
  bool getRanNodeName(std::string& name);
  bool getSupportedTAList(std::vector<struct SupportedItem_s>& list);
  int getDefaultPagingDRX();

 private:
  Ngap_NGAP_PDU_t* ngSetupRequestPdu;
  Ngap_NGSetupRequest_t* ngSetupRequestIEs;

  /***************** for decoding ****************/
  GlobalRanNodeId* globalRanNodeId;
  RanNodeName* ranNodeName;
  SupportedTAList* supportedTAList;
  DefaultPagingDRX* defaultPagingDrx;
};

}  // namespace ngap

#endif
