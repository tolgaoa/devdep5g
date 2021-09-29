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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "PDUSessionEstablishmentReject.h"

int decode_pdu_session_establishment_reject(
    pdu_session_establishment_reject_msg* pdu_session_establishment_reject,
    uint8_t* buffer, uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, PDU_SESSION_ESTABLISHMENT_REJECT_MINIMUM_LENGTH, len);

  if ((decoded_result = decode__5gsm_cause(
           &pdu_session_establishment_reject->_5gsmcause, 0, buffer + decoded,
           len - decoded)) < 0) {
    return decoded_result;
  } else {
    decoded += decoded_result;
  }

  while (len - decoded > 0) {
    uint8_t ieiDecoded = *(buffer + decoded);

    if (ieiDecoded == 0) break;

    switch (ieiDecoded & 0xf0) {
      case PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_IEI:
        if ((decoded_result = decode_allowed_ssc_mode(
                 &pdu_session_establishment_reject->allowedsscmode,
                 PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_establishment_reject->presence |=
              PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENCE;
        }
        break;
    }
    switch (ieiDecoded) {
      case PDU_SESSION_ESTABLISHMENT_REJECT_GPRS_TIMER3_IEI:
        if ((decoded_result = decode_gprs_timer3(
                 &pdu_session_establishment_reject->gprstimer3,
                 PDU_SESSION_ESTABLISHMENT_REJECT_GPRS_TIMER3_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_establishment_reject->presence |=
              PDU_SESSION_ESTABLISHMENT_REJECT_GPRS_TIMER3_PRESENCE;
        }
        break;
      case PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_IEI:
        if ((decoded_result = decode_eap_message(
                 &pdu_session_establishment_reject->eapmessage,
                 PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_establishment_reject->presence |=
              PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_PRESENCE;
        }
        break;
      case PDU_SESSION_ESTABLISHMENT_REJECT_E_P_C_O_IEI:
        if ((decoded_result = decode_extended_protocol_configuration_options(
                 &pdu_session_establishment_reject
                      ->extendedprotocolconfigurationoptions,
                 PDU_SESSION_ESTABLISHMENT_REJECT_E_P_C_O_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_establishment_reject->presence |=
              PDU_SESSION_ESTABLISHMENT_REJECT_E_P_C_O_PRESENCE;
        }
        break;
      case PDU_SESSION_ESTABLISHMENT_REJECT__5GSM_CONGESTION_REATTEMPT_INDICATOR_IEI:
        if ((decoded_result = decode__5gsm_congestion_reattempt_indicator(
                 &pdu_session_establishment_reject
                      ->_5gsmcongestionreattemptindicator,
                 PDU_SESSION_ESTABLISHMENT_REJECT__5GSM_CONGESTION_REATTEMPT_INDICATOR_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_establishment_reject->presence |=
              PDU_SESSION_ESTABLISHMENT_REJECT__5GSM_CONGESTION_REATTEMPT_INDICATOR_PRESENCE;
        }
        break;
    }
  }

  return decoded;
}

int encode_pdu_session_establishment_reject(
    pdu_session_establishment_reject_msg* pdu_session_establishment_reject,
    uint8_t* buffer, uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, PDU_SESSION_ESTABLISHMENT_REJECT_MINIMUM_LENGTH, len);

  if ((encoded_result = encode__5gsm_cause(
           pdu_session_establishment_reject->_5gsmcause, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((pdu_session_establishment_reject->presence &
       PDU_SESSION_ESTABLISHMENT_REJECT_GPRS_TIMER3_PRESENCE) ==
      PDU_SESSION_ESTABLISHMENT_REJECT_GPRS_TIMER3_PRESENCE) {
    if ((encoded_result = encode_gprs_timer3(
             pdu_session_establishment_reject->gprstimer3,
             PDU_SESSION_ESTABLISHMENT_REJECT_GPRS_TIMER3_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_establishment_reject->presence &
       PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENCE) ==
      PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENCE) {
    if ((encoded_result = encode_allowed_ssc_mode(
             pdu_session_establishment_reject->allowedsscmode,
             PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_establishment_reject->presence &
       PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_PRESENCE) ==
      PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_PRESENCE) {
    if ((encoded_result = encode_eap_message(
             pdu_session_establishment_reject->eapmessage,
             PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_establishment_reject->presence &
       PDU_SESSION_ESTABLISHMENT_REJECT_E_P_C_O_PRESENCE) ==
      PDU_SESSION_ESTABLISHMENT_REJECT_E_P_C_O_PRESENCE) {
    if ((encoded_result = encode_extended_protocol_configuration_options(
             pdu_session_establishment_reject
                 ->extendedprotocolconfigurationoptions,
             PDU_SESSION_ESTABLISHMENT_REJECT_E_P_C_O_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_establishment_reject->presence &
       PDU_SESSION_ESTABLISHMENT_REJECT__5GSM_CONGESTION_REATTEMPT_INDICATOR_PRESENCE) ==
      PDU_SESSION_ESTABLISHMENT_REJECT__5GSM_CONGESTION_REATTEMPT_INDICATOR_PRESENCE) {
    if ((encoded_result = encode__5gsm_congestion_reattempt_indicator(
             pdu_session_establishment_reject
                 ->_5gsmcongestionreattemptindicator,
             PDU_SESSION_ESTABLISHMENT_REJECT__5GSM_CONGESTION_REATTEMPT_INDICATOR_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  return encoded;
}
