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

#ifndef _NGSETUPRESPONSE_H_
#define _NGSETUPRESPONSE_H_

#include "AMFName.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "PLMNSupportList.hpp"
#include "RelativeAMFCapacity.hpp"
#include "ServedGUAMIList.hpp"
//#include "CriticalityDiagnostics.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_NGSetupResponse.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class NGSetupResponseMsg {
 public:
  NGSetupResponseMsg();
  virtual ~NGSetupResponseMsg();

  // External interfaces
  // Encapsulation
  void setMessageType();  // Initialize the PDU and populate the MessageType;
  void setAMFName(const std::string name);
  void setGUAMIList(std::vector<struct GuamiItem_s> list);
  void setRelativeAmfCapacity(long capacity);
  void setPlmnSupportList(std::vector<PlmnSliceSupport_t> list);
  int encode2buffer(uint8_t* buf, int buf_size);
  // Decapsulation
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  bool getAMFName(std::string& name);
  bool getGUAMIList(std::vector<struct GuamiItem_s>& list);
  long getRelativeAmfCapacity();
  bool getPlmnSupportList(std::vector<PlmnSliceSupport_t>& list);

 private:
  Ngap_NGAP_PDU_t* ngSetupResponsePdu;
  Ngap_NGSetupResponse_t* ngSetupResponsIEs;

  /***************** for decoding ****************/

  AmfName* amfName;
  ServedGUAMIList* servedGUAMIList;
  RelativeAMFCapacity* relativeAmfCapacity;
  PLMNSupportList* plmnSupportList;
  // CriticalityDiagnostics *criticalityDiagnostics;
};

}  // namespace ngap
#endif
