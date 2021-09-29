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

#ifndef _NAS_MM_PLAIN_HEADER_H_
#define _NAS_MM_PLAIN_HEADER_H_

#include "ExtendedProtocolDiscriminator.hpp"
#include "NasMessageType.hpp"
#include "SecurityHeaderType.hpp"

namespace nas {

class NasMmPlainHeader {
 public:
  void setHeader(uint8_t epd_, uint8_t security_header_type, uint8_t msg_type_);
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len);
  void setEpdIE(const uint8_t epd);
  void setSecurityHeaderTypeIE(const uint8_t type);
  void setMessageTypeIE(const uint8_t type);
  uint8_t getEpdIE();
  uint8_t getSecurityHeaderTypeIE();
  uint8_t getMessageType();

 private:
  ExtendedProtocolDiscriminator ie_epd;
  SecurityHeaderType ie_secu_header_type;
  NasMessageType ie_msg_type;
  uint8_t epd;
  uint8_t secu_header_type;
  uint8_t msg_type;
};

}  // namespace nas

#endif
