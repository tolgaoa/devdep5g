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

#include "IdentityResponse.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
IdentityResponse::IdentityResponse() {
  Logger::nas_mm().debug("initiating class IdentityResponse");
  plain_header   = NULL;
  ie_mobility_id = NULL;
}

//------------------------------------------------------------------------------
IdentityResponse::~IdentityResponse() {}

//------------------------------------------------------------------------------
void IdentityResponse::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, IDENTITY_RESPONSE);
}

//------------------------------------------------------------------------------
void IdentityResponse::setSUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, const string routingInd,
    uint8_t protection_sch_id, const string msin) {
  if (protection_sch_id != NULL_SCHEME) {
    Logger::nas_mm().error(
        "encoding suci and supi format for imsi error, please choose right "
        "interface");
    return;
  } else {
    ie_mobility_id =
        new _5GSMobilityIdentity(mcc, mnc, routingInd, protection_sch_id, msin);
  }
}

//------------------------------------------------------------------------------
void IdentityResponse::setSUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, const string routingInd,
    uint8_t protection_sch_id, uint8_t hnpki, const string msin) {}

//------------------------------------------------------------------------------
void IdentityResponse::set5G_GUTI() {}

//------------------------------------------------------------------------------
void IdentityResponse::setIMEI_IMEISV() {}

//------------------------------------------------------------------------------
void IdentityResponse::set5G_S_TMSI() {}

//------------------------------------------------------------------------------
int IdentityResponse::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding IdentityResponse message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (!ie_mobility_id) {
    Logger::nas_mm().warn("IE ie_mobility_id is not available");
  } else {
    if (int size = ie_mobility_id->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_mobility_id  error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "encoded IdentityResponse message len(%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int IdentityResponse::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("decoding IdentityResponse message");
  int decoded_size = 3;
  plain_header     = header;
  ie_mobility_id   = new _5GSMobilityIdentity();
  decoded_size += ie_mobility_id->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug("decoded_size(%d)", decoded_size);
  Logger::nas_mm().debug(
      "decoded IdentityResponse message len(%d)", decoded_size);
  return true;
}
