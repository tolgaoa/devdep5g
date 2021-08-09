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

#ifndef _AuthenticationRequest_H_
#define _AuthenticationRequest_H_

#include "nas_ie_header.hpp"

namespace nas {

class AuthenticationRequest {
 public:
  AuthenticationRequest();
  ~AuthenticationRequest();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(NasMmPlainHeader* header, uint8_t* buf, int len);
  void setHeader(uint8_t security_header_type);
  void setngKSI(uint8_t tsc, uint8_t key_set_id);
  void setEAP_Message(bstring eap);
  void setABBA(uint8_t length, uint8_t* value);
  void setAuthentication_Parameter_RAND(uint8_t* value);
  void setAuthentication_Parameter_AUTN(uint8_t* value);

 public:
  NasMmPlainHeader* plain_header;
  NasKeySetIdentifier* ie_ngKSI;
  ABBA* ie_abba;
  Authentication_Parameter_RAND* ie_authentication_parameter_rand;
  Authentication_Parameter_AUTN* ie_authentication_parameter_autn;
  EAP_Message* ie_eap_message;
};

}  // namespace nas

#endif
