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

#include "SecurityModeCommand.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
SecurityModeCommand::SecurityModeCommand() {
  plain_header                          = NULL;
  ie_eap_message                        = NULL;
  ie_selected_nas_security_algorithms   = NULL;
  ie_ngKSI                              = NULL;
  ie_ue_security_capability             = NULL;
  ie_imeisv_request                     = NULL;
  ie_eps_nas_security_algorithms        = NULL;
  ie_additional_5G_security_information = NULL;
  ie_abba                               = NULL;
  ie_s1_ue_security_capability          = NULL;
}

//------------------------------------------------------------------------------
SecurityModeCommand::~SecurityModeCommand() {}

//------------------------------------------------------------------------------
void SecurityModeCommand::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, SECURITY_MODE_COMMAND);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setNAS_Security_Algorithms(
    uint8_t ciphering, uint8_t integrity) {
  ie_selected_nas_security_algorithms =
      new NAS_Security_Algorithms(ciphering, integrity);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setngKSI(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI = new NasKeySetIdentifier(0x00, tsc, key_set_id);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setUE_Security_Capability(
    uint8_t g_EASel, uint8_t g_IASel) {
  ie_ue_security_capability = new UESecurityCapability(0x00, g_EASel, g_IASel);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setUE_Security_Capability(
    uint8_t g_EASel, uint8_t g_IASel, uint8_t EEASel, uint8_t EIASel) {
  ie_ue_security_capability =
      new UESecurityCapability(0x00, g_EASel, g_IASel, EEASel, EIASel);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setIMEISV_Request(uint8_t value) {
  ie_imeisv_request = new IMEISV_Request(0x0E, value);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setEPS_NAS_Security_Algorithms(
    uint8_t ciphering, uint8_t integrity) {
  ie_eps_nas_security_algorithms =
      new EPS_NAS_Security_Algorithms(0x57, ciphering, integrity);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setAdditional_5G_Security_Information(
    bool rinmr, bool hdp) {
  ie_additional_5G_security_information =
      new Additional_5G_Security_Information(0x36, rinmr, hdp);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setEAP_Message(bstring eap) {
  ie_eap_message = new EAP_Message(0x78, eap);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setABBA(uint8_t length, uint8_t* value) {
  ie_abba = new ABBA(0x38, length, value);
}

//------------------------------------------------------------------------------
void SecurityModeCommand::setS1_UE_Security_Capability(
    uint8_t g_EEASel, uint8_t g_EIASel) {
  ie_s1_ue_security_capability =
      new S1_UE_Security_Capability(0x19, g_EEASel, g_EIASel);
}

//------------------------------------------------------------------------------
int SecurityModeCommand::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding SecurityModeCommand message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (!ie_selected_nas_security_algorithms) {
    Logger::nas_mm().warn(
        "IE ie_selected_nas_security_algorithms is not available");
  } else {
    if (int size = ie_selected_nas_security_algorithms->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_selected_nas_security_algorithms error");
      return 0;
    }
  }
  if (!ie_ngKSI) {
    Logger::nas_mm().warn("IE ie_ngKSI is not available");
  } else {
    if (int size =
            ie_ngKSI->encode2buffer(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_ngKSI error");
      return 0;
    }
  }
  if (!ie_ue_security_capability) {
    Logger::nas_mm().warn("IE ie_ue_security_capability is not available");
  } else {
    if (int size = ie_ue_security_capability->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_ue_security_capability error");
      return 0;
    }
  }
  if (!ie_imeisv_request) {
    Logger::nas_mm().warn("IE ie_imeisv_request is not available");
  } else {
    if (int size = ie_imeisv_request->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_imeisv_request error");
      return 0;
    }
  }
  if (!ie_eps_nas_security_algorithms) {
    Logger::nas_mm().warn("IE ie_eps_nas_security_algorithms is not available");
  } else {
    if (int size = ie_eps_nas_security_algorithms->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_eps_nas_security_algorithms error");
      return 0;
    }
  }
  if (!ie_additional_5G_security_information) {
    Logger::nas_mm().warn(
        "IE ie_additional_5G_security_information is not available");
  } else {
    if (int size = ie_additional_5G_security_information->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error(
          "Encoding ie_additional_5G_security_information error");
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
      Logger::nas_mm().error("encoding ie_eap_message error");
      return 0;
    }
  }
  if (!ie_abba) {
    Logger::nas_mm().warn("IE ie_abba is not available");
  } else {
    if (int size =
            ie_abba->encode2buffer(buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_abba error");
      return 0;
    }
  }
  if (!ie_s1_ue_security_capability) {
    Logger::nas_mm().warn("IE ie_s1_ue_security_capability is not available");
  } else {
    if (int size = ie_s1_ue_security_capability->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_s1_ue_security_capability error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "Encoded SecurityModeCommand message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int SecurityModeCommand::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding SecurityModeCommand message");
  int decoded_size                    = 3;
  plain_header                        = header;
  ie_selected_nas_security_algorithms = new NAS_Security_Algorithms();
  decoded_size += ie_selected_nas_security_algorithms->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  ie_ngKSI = new NasKeySetIdentifier();
  decoded_size += ie_ngKSI->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false, false);
  ie_ue_security_capability = new UESecurityCapability();
  decoded_size += ie_ue_security_capability->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  Logger::nas_mm().debug("Decoded_size (%d)", decoded_size);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI (0x%x)", octet);
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case 0xE: {
        Logger::nas_mm().debug("Decoding IEI (0xE)");
        ie_imeisv_request = new IMEISV_Request();
        decoded_size += ie_imeisv_request->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
    switch (octet) {
      case 0x57: {
        Logger::nas_mm().debug("decoding IEI (0x57)");
        ie_eps_nas_security_algorithms = new EPS_NAS_Security_Algorithms();
        decoded_size += ie_eps_nas_security_algorithms->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x36: {
        Logger::nas_mm().debug("decoding IEI (0x36)");
        ie_additional_5G_security_information =
            new Additional_5G_Security_Information();
        decoded_size += ie_additional_5G_security_information->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x78: {
        Logger::nas_mm().debug("decoding IEI (0x78)");
        ie_eap_message = new EAP_Message();
        decoded_size += ie_eap_message->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x38: {
        Logger::nas_mm().debug("decoding IEI (0x38)");
        ie_abba = new ABBA();
        decoded_size += ie_abba->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x19: {
        Logger::nas_mm().debug("decoding IEI (0x19)");
        ie_s1_ue_security_capability = new S1_UE_Security_Capability();
        decoded_size += ie_s1_ue_security_capability->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded SecurityModeCommand message len (%d)", decoded_size);
  return 1;
}
