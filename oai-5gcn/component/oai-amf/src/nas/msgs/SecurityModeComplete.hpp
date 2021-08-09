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

#ifndef _SecurityModeComplete_H_
#define _SecurityModeComplete_H_

#include "nas_ie_header.hpp"

namespace nas {

class SecurityModeComplete {
 public:
  SecurityModeComplete();
  ~SecurityModeComplete();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(NasMmPlainHeader* header, uint8_t* buf, int len);
  void setHeader(uint8_t security_header_type);

  void setIMEISV(IMEISV_t imeisv);
  void setNAS_Message_Container(bstring value);
  void setNON_IMEISV(IMEISV_t imeisv);

  bool getIMEISV(IMEISV_t& imeisv);
  bool getNasMessageContainer(bstring& nas);
  bool getNON_IMEISV(IMEISV_t& imeisv);

 public:
  NasMmPlainHeader* plain_header;
  _5GSMobilityIdentity* ie_imeisv;
  NAS_Message_Container* ie_nas_message_container;
  _5GSMobilityIdentity* ie_non_imeisvpei;
};

}  // namespace nas

#endif
