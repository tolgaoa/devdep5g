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
 \author
 \date 2020
 \email: contact@openairinterface.org
 */

#include "DeregistrationRequest.hpp"

#include <string>

#include "3gpp_ts24501.hpp"
#include "String2Value.hpp"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
DeregistrationRequest::DeregistrationRequest() {
  plain_header          = NULL;
  ie_deregistrationtype = NULL;
  ie_ngKSI              = NULL;
  ie_5gs_mobility_id    = NULL;
}

//------------------------------------------------------------------------------
DeregistrationRequest::~DeregistrationRequest() {}

//------------------------------------------------------------------------------
void DeregistrationRequest::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type,
      DEREGISTRATION_REQUEST_UE_ORIGINATING);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::setDeregistrationType(uint8_t dereg_type) {
  ie_deregistrationtype = new _5GSDeregistrationType(dereg_type);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::setDeregistrationType(
    _5gs_deregistration_type_t type) {
  ie_deregistrationtype = new _5GSDeregistrationType(type);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::setngKSI(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI = new NasKeySetIdentifier(tsc, key_set_id);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::getDeregistrationType(uint8_t& dereg_type) {
  ie_deregistrationtype->get(dereg_type);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::getDeregistrationType(
    _5gs_deregistration_type_t& type) {
  ie_deregistrationtype->get(type);
}

//------------------------------------------------------------------------------
void DeregistrationRequest::getngKSI(uint8_t& ng_ksi) {
  if (ie_ngKSI) {
    ng_ksi =
        (ie_ngKSI->getTypeOfSecurityContext()) | ie_ngKSI->getasKeyIdentifier();
  } else {
    ng_ksi = 0;
  }
}

//------------------------------------------------------------------------------
void DeregistrationRequest::setSUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, const string routingInd,
    uint8_t protection_sch_id, const string msin) {
  if (protection_sch_id != NULL_SCHEME) {
    Logger::nas_mm().error(
        "encoding suci and supi format for imsi error, please choose right "
        "interface");
    return;
  } else {
    ie_5gs_mobility_id =
        new _5GSMobilityIdentity(mcc, mnc, routingInd, protection_sch_id, msin);
  }
}

//------------------------------------------------------------------------------
void DeregistrationRequest::getMobilityIdentityType(uint8_t& type) {
  if (ie_5gs_mobility_id) {
    type = ie_5gs_mobility_id->gettypeOfIdentity();
  } else {
    type = 0;
  }
}

//------------------------------------------------------------------------------
bool DeregistrationRequest::getSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi) {
  if (ie_5gs_mobility_id) {
    ie_5gs_mobility_id->getSuciWithSupiImsi(imsi);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
std::string DeregistrationRequest::get_5g_guti() {
  if (ie_5gs_mobility_id) {
    nas::_5G_GUTI_t guti;
    ie_5gs_mobility_id->get5GGUTI(guti);
    std::string str;  //= guti.toString();
    return str;
  } else {
    return "error";
  }
}

//------------------------------------------------------------------------------
void DeregistrationRequest::setSUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, const string routingInd,
    uint8_t protection_sch_id, uint8_t hnpki, const string msin) {}

//------------------------------------------------------------------------------
void DeregistrationRequest::set5G_GUTI() {}

//------------------------------------------------------------------------------
void DeregistrationRequest::setIMEI_IMEISV() {}

//------------------------------------------------------------------------------
void DeregistrationRequest::set5G_S_TMSI() {}

//------------------------------------------------------------------------------
int DeregistrationRequest::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding DeregistrationRequest message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!ie_deregistrationtype) {
    Logger::nas_mm().error("Mandatory IE missing Deregistration Type");
    return 0;
  }
  if (!ie_ngKSI) {
    Logger::nas_mm().error("Mandatory IE missing ie_ngKSI");
    return 0;
  }
  if (!ie_5gs_mobility_id) {
    Logger::nas_mm().error("Mandatory IE missing ie_5gs_mobility_id");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (!(ie_deregistrationtype->encode2buffer(
          buf + encoded_size, len - encoded_size))) {
    if (!(ie_ngKSI->encode2buffer(buf + encoded_size, len - encoded_size))) {
      encoded_size += 1;
    } else {
      Logger::nas_mm().error("Encoding IE ie_ngKSI error");
      return 0;
    }
  } else {
    Logger::nas_mm().error("Encoding IE Deregistrationt Type error");
    return 0;
  }
  if (int size = ie_5gs_mobility_id->encode2buffer(
          buf + encoded_size, len - encoded_size)) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding IE ie_5gs_mobility_id  error");
    return 0;
  }

  Logger::nas_mm().debug(
      "Encoded DeregistrationRequest message len (%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int DeregistrationRequest::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding DeregistrationRequest message");
  int decoded_size      = 3;
  plain_header          = header;
  ie_deregistrationtype = new _5GSDeregistrationType();
  decoded_size += ie_deregistrationtype->decodefrombuffer(
      buf + decoded_size, len - decoded_size);
  ie_ngKSI = new NasKeySetIdentifier();
  decoded_size += ie_ngKSI->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false, true);
  decoded_size++;
  ie_5gs_mobility_id = new _5GSMobilityIdentity();
  decoded_size += ie_5gs_mobility_id->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug(
      "Decoded DeregistrationRequest message (len %d)", decoded_size);
  return 1;
}
