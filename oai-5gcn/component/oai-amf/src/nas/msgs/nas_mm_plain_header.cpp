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

#include "nas_mm_plain_header.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
void NasMmPlainHeader::setEpdIE(const uint8_t epd) {
  ie_epd.setValue(epd);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::getEpdIE() {
  return ie_epd.getValue();
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::setSecurityHeaderTypeIE(const uint8_t type) {
  ie_secu_header_type.setValue(type);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::getSecurityHeaderTypeIE() {
  return ie_secu_header_type.getValue();
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::setMessageTypeIE(const uint8_t type) {
  ie_msg_type.setValue(type);
}

//------------------------------------------------------------------------------
uint8_t NasMmPlainHeader::getMessageType() {
  return msg_type;
}

//------------------------------------------------------------------------------
void NasMmPlainHeader::setHeader(
    uint8_t epd_, uint8_t security_header_type, uint8_t msg_type_) {
  epd              = epd_;
  secu_header_type = security_header_type;
  msg_type         = msg_type_;
}

//------------------------------------------------------------------------------
int NasMmPlainHeader::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding NasMmPlainHeader");
  if (len < 3) {
    Logger::nas_mm().error("buffer length is less than 3 octets");
    return 0;
  } else {
    *(buf++) = epd;
    *(buf++) = secu_header_type;
    *(buf++) = msg_type;
    Logger::nas_mm().debug("Encoded NasMmPlainHeader (len 3 octets)");
    return 3;
  }
}

//------------------------------------------------------------------------------
int NasMmPlainHeader::decodefrombuffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("decoding NasMmPlainHeader");
  if (len < 3) {
    Logger::nas_mm().error(
        "[decoding nas mm header error][buffer length is less than 3 octets]");
    return 0;
  } else {
    epd              = *(buf++);
    secu_header_type = *(buf++);
    msg_type         = *(buf++);
  }
  Logger::nas_mm().debug("decoded NasMmPlainHeader len(3 octets)");
  return 3;
}
