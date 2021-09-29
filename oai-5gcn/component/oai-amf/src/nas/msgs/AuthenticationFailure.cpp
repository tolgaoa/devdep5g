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

#include "AuthenticationFailure.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
AuthenticationFailure::AuthenticationFailure() {
  plain_header                        = NULL;
  ie_5gmm_cause                       = NULL;
  ie_authentication_failure_parameter = NULL;
}

//------------------------------------------------------------------------------
AuthenticationFailure::~AuthenticationFailure() {}

//------------------------------------------------------------------------------
void AuthenticationFailure::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, AUTHENTICATION_FAILURE);
}

//------------------------------------------------------------------------------
void AuthenticationFailure::set_5GMM_Cause(uint8_t value) {
  ie_5gmm_cause = new _5GMM_Cause(0x00, value);
}

//------------------------------------------------------------------------------
uint8_t AuthenticationFailure::get5GMmCause() {
  if (ie_5gmm_cause) {
    return ie_5gmm_cause->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void AuthenticationFailure::setAuthentication_Failure_Parameter(bstring auts) {
  ie_authentication_failure_parameter =
      new Authentication_Failure_Parameter(0x30, auts);
}
bool AuthenticationFailure::getAutsInAuthFailPara(bstring& auts) {
  if (ie_authentication_failure_parameter) {
    ie_authentication_failure_parameter->getValue(auts);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
int AuthenticationFailure::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding AuthenticationFailure message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (!ie_5gmm_cause) {
    Logger::nas_mm().warn("IE ie_5gmm_cause is not available");
  } else {
    if (int size = ie_5gmm_cause->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_5gmm_cause error");
    }
  }
  if (!ie_authentication_failure_parameter) {
    Logger::nas_mm().warn(
        "IE ie_authentication_failure_parameter is not available");
  } else {
    if (int size = ie_authentication_failure_parameter->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_authentication_failure_parameter error");
      return 0;
    }
  }

  Logger::nas_mm().debug(
      "Encoded AuthenticationFailure message len (%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int AuthenticationFailure::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding AuthenticationFailure message");
  int decoded_size = 3;
  plain_header     = header;
  ie_5gmm_cause    = new _5GMM_Cause();
  decoded_size += ie_5gmm_cause->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case 0x30: {
        Logger::nas_mm().debug("Decoding IEI (0x30)");
        ie_authentication_failure_parameter =
            new Authentication_Failure_Parameter();
        decoded_size += ie_authentication_failure_parameter->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded AuthenticationFailure message len (%d)", decoded_size);
  return 1;
}
