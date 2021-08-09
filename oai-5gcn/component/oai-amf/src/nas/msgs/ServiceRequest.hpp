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

#ifndef _SERVICE_REQUEST_H_
#define _SERVICE_REQUEST_H_

#include <string>

#include "bstrlib.h"
#include "nas_ie_header.hpp"

using namespace std;

namespace nas {

class ServiceRequest {
 public:
  ServiceRequest();
  ~ServiceRequest();

 public:
  void setHeader(uint8_t security_header_type);
  void setngKSI(uint8_t tsc, uint8_t key_set_id);
  void setServiceType(uint8_t stp);
  void set5G_S_TMSI(uint16_t amfSetId, uint8_t amfPointer, string tmsi);
  void setUplink_data_status(uint16_t value);
  void setPDU_session_status(uint16_t value);
  void setAllowed_PDU_Session_Status(uint16_t value);
  void setNAS_Message_Container(bstring value);
  int encode2buffer(uint8_t* buf, int len);

 public:
  int decodefrombuffer(NasMmPlainHeader* header, uint8_t* buf, int len);
  uint8_t getngKSI();
  uint8_t getServiceType();
  bool get5G_S_TMSI(uint16_t& amfSetId, uint8_t& amfPointer, string& tmsi);
  uint16_t getUplinkDataStatus();
  uint16_t getPduSessionStatus();
  uint16_t getAllowedPduSessionStatus();
  bool getNasMessageContainer(bstring& nas);

 private:
  NasMmPlainHeader* plain_header;
  NasKeySetIdentifier* ie_ngKSI;
  ServiceType* ie_service_type;
  _5GSMobilityIdentity* ie_5g_s_tmsi;
  UplinkDataStatus* ie_uplink_data_status;
  PDU_Session_Status* ie_PDU_session_status;
  Allowed_PDU_Session_Status* ie_allowed_PDU_session_status;
  NAS_Message_Container* ie_nas_message_container;
};

}  // namespace nas

#endif
