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

#ifndef _SecurityModeCommand_H_
#define _SecurityModeCommand_H_

#include "nas_ie_header.hpp"

namespace nas {

class SecurityModeCommand {
 public:
  SecurityModeCommand();
  ~SecurityModeCommand();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(NasMmPlainHeader* header, uint8_t* buf, int len);
  void setHeader(uint8_t security_header_type);
  void setNAS_Security_Algorithms(uint8_t ciphering, uint8_t integrity);
  void setngKSI(uint8_t tsc, uint8_t key_set_id);
  void setUE_Security_Capability(uint8_t g_EASel, uint8_t g_IASel);
  void setUE_Security_Capability(
      uint8_t g_EASel, uint8_t g_IASel, uint8_t EEASel, uint8_t EIASel);
  void setIMEISV_Request(uint8_t value);
  void setEPS_NAS_Security_Algorithms(uint8_t ciphering, uint8_t integrity);
  void setAdditional_5G_Security_Information(bool rinmr, bool hdp);
  void setEAP_Message(bstring eap);
  void setABBA(uint8_t length, uint8_t* value);
  void setS1_UE_Security_Capability(uint8_t g_EEASel, uint8_t g_EIASel);

 public:
  NasMmPlainHeader* plain_header;
  NAS_Security_Algorithms* ie_selected_nas_security_algorithms;
  NasKeySetIdentifier* ie_ngKSI;
  UESecurityCapability* ie_ue_security_capability;
  IMEISV_Request* ie_imeisv_request;
  EPS_NAS_Security_Algorithms* ie_eps_nas_security_algorithms;
  Additional_5G_Security_Information* ie_additional_5G_security_information;
  EAP_Message* ie_eap_message;
  ABBA* ie_abba;
  S1_UE_Security_Capability* ie_s1_ue_security_capability;
};

}  // namespace nas

#endif
