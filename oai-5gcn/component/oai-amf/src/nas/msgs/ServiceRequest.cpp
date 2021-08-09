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

#include "ServiceRequest.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
ServiceRequest::ServiceRequest() {
  plain_header                  = nullptr;
  ie_ngKSI                      = nullptr;
  ie_service_type               = nullptr;
  ie_5g_s_tmsi                  = nullptr;
  ie_uplink_data_status         = nullptr;
  ie_PDU_session_status         = nullptr;
  ie_allowed_PDU_session_status = nullptr;
  ie_nas_message_container      = nullptr;
}

//------------------------------------------------------------------------------
ServiceRequest::~ServiceRequest() {}

//------------------------------------------------------------------------------
void ServiceRequest::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, SERVICE_REQUEST);
}

//------------------------------------------------------------------------------
void ServiceRequest::setngKSI(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI = new NasKeySetIdentifier(0x00, tsc, key_set_id);
}

//------------------------------------------------------------------------------
void ServiceRequest::setServiceType(uint8_t stp) {
  ie_service_type = new ServiceType(0x00, stp);
}

//------------------------------------------------------------------------------
void ServiceRequest::set5G_S_TMSI(
    uint16_t amfSetId, uint8_t amfPointer, string tmsi) {
  ie_5g_s_tmsi = new _5GSMobilityIdentity(0x00, amfSetId, amfPointer, tmsi);
}

//------------------------------------------------------------------------------
void ServiceRequest::setUplink_data_status(uint16_t value) {
  ie_uplink_data_status = new UplinkDataStatus(0x40, value);
}

//------------------------------------------------------------------------------
void ServiceRequest::setPDU_session_status(uint16_t value) {
  ie_PDU_session_status = new PDU_Session_Status(0x50, value);
}

//------------------------------------------------------------------------------
void ServiceRequest::setAllowed_PDU_Session_Status(uint16_t value) {
  ie_allowed_PDU_session_status = new Allowed_PDU_Session_Status(0x25, value);
}

//------------------------------------------------------------------------------
void ServiceRequest::setNAS_Message_Container(bstring value) {
  ie_nas_message_container = new NAS_Message_Container(0x71, value);
}

//------------------------------------------------------------------------------
int ServiceRequest::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding ServiceRequest message...");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!ie_ngKSI) {
    Logger::nas_mm().error("Mandatory IE missing ie_ngKSI");
    return 0;
  }
  if (!ie_service_type) {
    Logger::nas_mm().error("Mandatory IE missing ie_service_type");
    return 0;
  }
  if (!ie_5g_s_tmsi) {
    Logger::nas_mm().error("Mandatory IE missing ie_5g_s_tmsi");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (ie_ngKSI->encode2buffer(buf + encoded_size, len - encoded_size) != -1) {
    if (ie_service_type->encode2buffer(
            buf + encoded_size, len - encoded_size) != -1) {
      encoded_size++;
    } else {
      Logger::nas_mm().error("Encoding ie_service_type error");
      return 0;
    }
  } else {
    Logger::nas_mm().error("Encoding ie_ngKSI error");
    return 0;
  }
  int size =
      ie_5g_s_tmsi->encode2buffer(buf + encoded_size, len - encoded_size);
  if (size != 0) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("Encoding IE ie_5g_s_tmsi error");
    return 0;
  }
  if (!ie_uplink_data_status) {
    Logger::nas_mm().warn("IE ie_uplink_data_status is not available");
  } else {
    size = ie_uplink_data_status->encode2buffer(
        buf + encoded_size, len - encoded_size);
    if (size != 0) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_uplink_data_status error");
      return 0;
    }
  }
  if (!ie_PDU_session_status) {
    Logger::nas_mm().warn("IE ie_PDU_session_status is not available");
  } else {
    size = ie_PDU_session_status->encode2buffer(
        buf + encoded_size, len - encoded_size);
    if (size != 0) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_PDU_session_status error");
      return 0;
    }
  }
  if (!ie_allowed_PDU_session_status) {
    Logger::nas_mm().warn("IE ie_allowed_PDU_session_status is not available");
  } else {
    size = ie_allowed_PDU_session_status->encode2buffer(
        buf + encoded_size, len - encoded_size);
    if (size != 0) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_allowed_PDU_session_status error");
      return 0;
    }
  }
  if (!ie_nas_message_container) {
    Logger::nas_mm().warn("IE ie_nas_message_container is not available");
  } else {
    size = ie_nas_message_container->encode2buffer(
        buf + encoded_size, len - encoded_size);
    if (size != 0) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_nas_message_container error");
      return 0;
    }
  }
  Logger::nas_mm().debug("Encoded ServiceRequest message (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int ServiceRequest::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding ServiceRequest message");
  int decoded_size = 3;
  plain_header     = header;
  ie_ngKSI         = new NasKeySetIdentifier();
  decoded_size += ie_ngKSI->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false, false);
  ie_service_type = new ServiceType();
  decoded_size += ie_service_type->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false, true);
  decoded_size++;
  ie_5g_s_tmsi = new _5GSMobilityIdentity();
  decoded_size += ie_5g_s_tmsi->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First optional IE (0x%x)", octet);
  while ((octet != 0x0)) {
    switch (octet) {
      case 0x40: {
        Logger::nas_mm().debug("Decoding ie_uplink_data_status (IEI: 0x40)");
        ie_uplink_data_status = new UplinkDataStatus();
        decoded_size += ie_uplink_data_status->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x50: {
        Logger::nas_mm().debug("Decoding ie_PDU_session_status (IEI: 0x50)");
        ie_PDU_session_status = new PDU_Session_Status();
        decoded_size += ie_PDU_session_status->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x25: {
        Logger::nas_mm().debug(
            "Decoding ie_allowed_PDU_session_status (IEI: 0x25)");
        ie_allowed_PDU_session_status = new Allowed_PDU_Session_Status();
        decoded_size += ie_allowed_PDU_session_status->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x71: {
        Logger::nas_mm().debug("Decoding ie_nas_message_container(IEI: 0x71)");
        ie_nas_message_container = new NAS_Message_Container();
        decoded_size += ie_nas_message_container->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded ServiceRequest message len (%d)", decoded_size);
  return decoded_size;
}

//------------------------------------------------------------------------------
uint8_t ServiceRequest::getngKSI() {
  if (ie_ngKSI) {
    uint8_t a = 0;
    a = (ie_ngKSI->getTypeOfSecurityContext()) | ie_ngKSI->getasKeyIdentifier();
    return a;
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------------
uint16_t ServiceRequest::getUplinkDataStatus() {
  if (ie_uplink_data_status) {
    return ie_uplink_data_status->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
uint16_t ServiceRequest::getPduSessionStatus() {
  if (ie_PDU_session_status) {
    return ie_PDU_session_status->getValue();
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------------
uint16_t ServiceRequest::getAllowedPduSessionStatus() {
  if (ie_allowed_PDU_session_status) {
    return ie_allowed_PDU_session_status->getValue();
  } else {
    return 0;
  }
}

//------------------------------------------------------------------------------
bool ServiceRequest::getNasMessageContainer(bstring& nas) {
  if (ie_nas_message_container) {
    ie_nas_message_container->getValue(nas);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
uint8_t ServiceRequest::getServiceType() {
  if (ie_service_type)
    return ie_service_type->getValue();
  else
    return 0;
}

//------------------------------------------------------------------------------
bool ServiceRequest::get5G_S_TMSI(
    uint16_t& amfSetId, uint8_t& amfPointer, string& tmsi) {
  if (ie_5g_s_tmsi)
    return ie_5g_s_tmsi->get5G_S_TMSI(amfSetId, amfPointer, tmsi);
  else
    return false;
}
