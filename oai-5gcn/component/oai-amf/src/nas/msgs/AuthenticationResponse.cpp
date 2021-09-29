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

#include "AuthenticationResponse.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationResponse::AuthenticationResponse() {
  plain_header                         = NULL;
  ie_authentication_response_parameter = NULL;
  ie_eap_message                       = NULL;
}

//------------------------------------------------------------------------------
AuthenticationResponse::~AuthenticationResponse() {}

//------------------------------------------------------------------------------
void AuthenticationResponse::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, AUTHENTICATION_RESPONSE);
}

//------------------------------------------------------------------------------
void AuthenticationResponse::setAuthentication_Response_Parameter(
    bstring para) {
  ie_authentication_response_parameter =
      new Authentication_Response_Parameter(0x2D, para);
}

//------------------------------------------------------------------------------
bool AuthenticationResponse::getAuthenticationResponseParameter(bstring& para) {
  if (ie_authentication_response_parameter) {
    ie_authentication_response_parameter->getValue(para);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void AuthenticationResponse::setEAP_Message(bstring eap) {
  ie_eap_message = new EAP_Message(0x78, eap);
}

//------------------------------------------------------------------------------
bool AuthenticationResponse::getEapMessage(bstring& eap) {
  if (ie_eap_message) {
    ie_eap_message->getValue(eap);
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
int AuthenticationResponse::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationResponse message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (!ie_authentication_response_parameter) {
    Logger::nas_mm().warn(
        "IE ie_authentication_response_parameter is not available");
  } else {
    if (int size = ie_authentication_response_parameter->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_authentication_response_parameter error");
      return 0;
    }
  }
  if (!ie_eap_message) {
    Logger::nas_mm().warn("IE ie_eap_message is not available");
  } else {
    if (int size = ie_eap_message->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eap_message error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "Encoded AuthenticationResponse message len (%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int AuthenticationResponse::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding AuthenticationResponse message");
  int decoded_size = 3;
  plain_header     = header;
  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case 0x2D: {
        Logger::nas_mm().debug("Decoding IEI (0x2D)");
        ie_authentication_response_parameter =
            new Authentication_Response_Parameter();
        decoded_size += ie_authentication_response_parameter->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x78: {
        Logger::nas_mm().debug("Decoding IEI (0x78)");
        ie_eap_message = new EAP_Message();
        decoded_size += ie_eap_message->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded AuthenticationResponse message len (%d)", decoded_size);
  return 1;
}
