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

#ifndef _ULNASTransport_H_
#define _ULNASTransport_H_

#include "nas_ie_header.hpp"

namespace nas {

class ULNASTransport {
 public:
  ULNASTransport();
  ~ULNASTransport();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(NasMmPlainHeader* header, uint8_t* buf, int len);
  void setHeader(uint8_t security_header_type);
  void setPayload_Container_Type(uint8_t value);
  void setPayload_Container(std::vector<PayloadContainerEntry> content);
  void setPDU_Session_Identity_2(uint8_t value);
  void setOLD_PDU_Session_Identity_2(uint8_t value);
  void setRequest_Type(uint8_t value);
  void setS_NSSAI(SNSSAI_s snssai);
  void setDNN(bstring dnn);
  void setAdditional_Information(uint8_t _length, uint8_t value);
  void setMA_PDU_Session_Information(uint8_t value);
  void setRelease_Assistance_Indication(uint8_t value);

  uint8_t getPayloadContainerType();
  bool getPayloadContainer(std::vector<PayloadContainerEntry>& content);
  bool getPayloadContainer(bstring& content);
  uint8_t getPduSessionId();
  uint8_t getOldPduSessionId();
  uint8_t getRequestType();
  bool getSnssai(SNSSAI_s& snssai);
  bool getDnn(bstring& dnn);

 public:
  NasMmPlainHeader* plain_header;
  Payload_Container_Type* ie_payload_container_type;
  Payload_Container* ie_payload_container;
  PDU_Session_Identity_2* ie_pdu_session_identity_2;
  PDU_Session_Identity_2* ie_old_pdu_session_identity_2;
  Request_Type* ie_request_type;
  S_NSSAI* ie_s_nssai;
  DNN* ie_dnn;
  Additional_Information* ie_additional_information;
  MA_PDU_Session_Information* ie_ma_pdu_session_information;
  Release_Assistance_Indication* ie_release_assistance_indication;
};

}  // namespace nas

#endif
