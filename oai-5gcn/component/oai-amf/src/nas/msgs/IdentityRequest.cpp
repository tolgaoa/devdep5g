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

#include "IdentityRequest.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
IdentityRequest::IdentityRequest() {
  Logger::nas_mm().debug("initiating class IdentityRequest");
  plain_header       = NULL;
  _5gs_identity_type = NULL;
}

//------------------------------------------------------------------------------
IdentityRequest::~IdentityRequest() {}

//------------------------------------------------------------------------------
void IdentityRequest::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, IDENTITY_REQUEST);
}

//------------------------------------------------------------------------------
void IdentityRequest::set_5GS_Identity_Type(uint8_t value) {
  _5gs_identity_type = new _5GS_Identity_Type(0x00, value);
}

//------------------------------------------------------------------------------
int IdentityRequest::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding IdentityRequest message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (!_5gs_identity_type) {
    Logger::nas_mm().warn("IE _5gs_identity_type is not available");
  } else {
    if (int size = _5gs_identity_type->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding _5gs_identity_type  error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "encoded IdentityRequest message len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int IdentityRequest::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("decoding IdentityRequest message");
  int decoded_size   = 3;
  plain_header       = header;
  _5gs_identity_type = new _5GS_Identity_Type();
  decoded_size += _5gs_identity_type->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug("decoded_size(%d)", decoded_size);
  Logger::nas_mm().debug(
      "decoded IdentityRequest message len(%d)", decoded_size);
  return 1;
}
