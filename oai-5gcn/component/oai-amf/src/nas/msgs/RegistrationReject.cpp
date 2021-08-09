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

#include "RegistrationReject.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
RegistrationReject::RegistrationReject() {
  Logger::nas_mm().debug("initiating class RegistrationReject");
  plain_header      = NULL;
  ie_5gmm_cause     = NULL;
  ie_T3346_value    = NULL;
  ie_T3502_value    = NULL;
  ie_eap_message    = NULL;
  ie_rejected_nssai = NULL;
}

//------------------------------------------------------------------------------
RegistrationReject::~RegistrationReject() {}

//------------------------------------------------------------------------------
void RegistrationReject::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, REGISTRATION_REJECT);
}

//------------------------------------------------------------------------------
void RegistrationReject::set_5GMM_Cause(uint8_t value) {
  ie_5gmm_cause = new _5GMM_Cause(0x00, value);
}

//------------------------------------------------------------------------------
void RegistrationReject::setGPRS_Timer_2_3346(uint8_t value) {
  ie_T3346_value = new GPRS_Timer_2(0x5f, value);
}

//------------------------------------------------------------------------------
void RegistrationReject::setGPRS_Timer_2_3502(uint8_t value) {
  ie_T3502_value = new GPRS_Timer_2(0x16, value);
}

//------------------------------------------------------------------------------
void RegistrationReject::setEAP_Message(bstring eap) {
  ie_eap_message = new EAP_Message(0x78, eap);
}

//------------------------------------------------------------------------------
void RegistrationReject::setRejected_NSSAI(uint8_t cause, uint8_t value) {
  ie_rejected_nssai = new Rejected_NSSAI(0x69, cause, value);
}

//------------------------------------------------------------------------------
int RegistrationReject::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding RegistrationReject message");
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
  if (!ie_T3346_value) {
    Logger::nas_mm().warn("IE ie_T3346_value is not available");
  } else {
    if (int size = ie_T3346_value->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3346_value error");
      return 0;
    }
  }
  if (!ie_T3502_value) {
    Logger::nas_mm().warn("IE ie_T3502_value is not available");
  } else {
    if (int size = ie_T3502_value->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_T3502_value error");
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
  if (!ie_rejected_nssai) {
    Logger::nas_mm().warn("IE ie_rejected_nssai is not available");
  } else {
    if (int size = ie_rejected_nssai->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("Encoding ie_rejected_nssai error");
    }
  }
  Logger::nas_mm().debug(
      "Encoded RegistrationReject message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int RegistrationReject::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationReject message");
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
      case 0x5F: {
        Logger::nas_mm().debug("Decoding IEI (0x5F)");
        ie_T3346_value = new GPRS_Timer_2();
        decoded_size += ie_T3346_value->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x16: {
        Logger::nas_mm().debug("Decoding iei(0x16)");
        ie_T3502_value = new GPRS_Timer_2();
        decoded_size += ie_T3502_value->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x78: {
        Logger::nas_mm().debug("Decoding iei(0x78)");
        ie_eap_message = new EAP_Message();
        decoded_size += ie_eap_message->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
      case 0x69: {
        Logger::nas_mm().debug("Decoding IEI (0x69)");
        ie_rejected_nssai = new Rejected_NSSAI();
        decoded_size += ie_rejected_nssai->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI (0x%x)", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "decoded RegistrationReject message len(%d)", decoded_size);
  return 1;
}
