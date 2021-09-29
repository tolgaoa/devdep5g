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

#include "SecurityModeComplete.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
SecurityModeComplete::SecurityModeComplete() {
  Logger::nas_mm().debug("initiating class SecurityModeComplete");
  plain_header             = NULL;
  ie_imeisv                = NULL;
  ie_nas_message_container = NULL;
  ie_non_imeisvpei         = NULL;
};

//------------------------------------------------------------------------------
SecurityModeComplete::~SecurityModeComplete() {}

//------------------------------------------------------------------------------
void SecurityModeComplete::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, SECURITY_MODE_COMPLETE);
}

//------------------------------------------------------------------------------
void SecurityModeComplete::setIMEISV(IMEISV_t imeisv) {
  ie_imeisv = new _5GSMobilityIdentity();
  ie_imeisv->setIEI(0x77);
  ie_imeisv->setIMEISV(imeisv);
}

//------------------------------------------------------------------------------
void SecurityModeComplete::setNAS_Message_Container(bstring value) {
  ie_nas_message_container = new NAS_Message_Container(0x71, value);
}

//------------------------------------------------------------------------------
void SecurityModeComplete::setNON_IMEISV(IMEISV_t imeisv) {
  ie_non_imeisvpei = new _5GSMobilityIdentity();
  ie_non_imeisvpei->setIEI(0x78);
  ie_non_imeisvpei->setIMEISV(imeisv);
}

//------------------------------------------------------------------------------
bool SecurityModeComplete::getIMEISV(IMEISV_t& imeisv) {
  if (ie_imeisv) {
    ie_imeisv->getIMEISV(imeisv);
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
bool SecurityModeComplete::getNasMessageContainer(bstring& nas) {
  if (ie_nas_message_container) {
    ie_nas_message_container->getValue(nas);
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
bool SecurityModeComplete::getNON_IMEISV(IMEISV_t& imeisv) {
  if (ie_non_imeisvpei) {
    ie_non_imeisvpei->getIMEISV(imeisv);
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
int SecurityModeComplete::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding SecurityModeComplete message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (!ie_imeisv) {
    Logger::nas_mm().warn("IE ie_imeisv is not available");
  } else {
    if (int size =
            ie_imeisv->encode2buffer(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_imeisv error");
      return 0;
    }
  }
  if (!ie_nas_message_container) {
    Logger::nas_mm().warn("IE ie_nas_message_container is not available");
  } else {
    if (int size = ie_nas_message_container->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_nas_message_container error");
      return 0;
    }
  }
  if (!ie_non_imeisvpei) {
    Logger::nas_mm().warn("IE ie_non_imeisvpei is not available");
  } else {
    if (int size = ie_non_imeisvpei->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding IE ie_non_imeisvpei error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "Encoded SecurityModeComplete message len (%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int SecurityModeComplete::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding SecurityModeComplete message");
  int decoded_size = 3;
  plain_header     = header;
  Logger::nas_mm().debug("decoded_size(%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case 0x77: {
        Logger::nas_mm().debug("Decoding IEI (0x77)");
        ie_imeisv = new _5GSMobilityIdentity();
        decoded_size += ie_imeisv->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x71: {
        Logger::nas_mm().debug("Decoding IEI (0x71)");
        ie_nas_message_container = new NAS_Message_Container();
        decoded_size += ie_nas_message_container->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x78: {
        Logger::nas_mm().debug("Decoding IEI (0x78)");
        ie_non_imeisvpei = new _5GSMobilityIdentity();
        decoded_size += ie_non_imeisvpei->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded SecurityModeComplete message len (%d)", decoded_size);
  return 1;
}
