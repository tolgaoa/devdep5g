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
#include "PDUSessionModificationCommand.h"

int decode_pdu_session_modification_command(
    pdu_session_modification_command_msg* pdu_session_modification_command,
    uint8_t* buffer, uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, PDU_SESSION_MODIFICATION_COMMAND_MINIMUM_LENGTH, len);

  while (len - decoded > 0) {
    uint8_t ieiDecoded = *(buffer + decoded);
    if (ieiDecoded == 0) break;

    switch (ieiDecoded & 0xf0) {
      case PDU_SESSION_MODIFICATION_COMMAND_ALWAYSON_PDU_SESSION_INDICATION_IEI:
        if ((decoded_result = decode_alwayson_pdu_session_indication(
                 &pdu_session_modification_command
                      ->alwaysonpdusessionindication,
                 PDU_SESSION_MODIFICATION_COMMAND_ALWAYSON_PDU_SESSION_INDICATION_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_modification_command->presence |=
              PDU_SESSION_MODIFICATION_COMMAND_ALWAYSON_PDU_SESSION_INDICATION_PRESENCE;
        }
        break;
    }
    switch (ieiDecoded) {
      case PDU_SESSION_MODIFICATION_COMMAND__5GSM_CAUSE_IEI:
        if ((decoded_result = decode__5gsm_cause(
                 &pdu_session_modification_command->_5gsmcause,
                 PDU_SESSION_MODIFICATION_COMMAND__5GSM_CAUSE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_modification_command->presence |=
              PDU_SESSION_MODIFICATION_COMMAND__5GSM_CAUSE_PRESENCE;
        }
        break;
      case PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_IEI:
        if ((decoded_result = decode_session_ambr(
                 &pdu_session_modification_command->sessionambr,
                 PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_modification_command->presence |=
              PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENCE;
        }
        break;
      case PDU_SESSION_MODIFICATION_COMMAND_GPRS_TIMER_IEI:
        if ((decoded_result = decode_gprs_timer(
                 &pdu_session_modification_command->gprstimer,
                 PDU_SESSION_MODIFICATION_COMMAND_GPRS_TIMER_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_modification_command->presence |=
              PDU_SESSION_MODIFICATION_COMMAND_GPRS_TIMER_PRESENCE;
        }
        break;
      case PDU_SESSION_MODIFICATION_COMMAND_QOS_RULES_IEI:
        if ((decoded_result = decode_qos_rules(
                 &pdu_session_modification_command->qosrules,
                 PDU_SESSION_MODIFICATION_COMMAND_QOS_RULES_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_modification_command->presence |=
              PDU_SESSION_MODIFICATION_COMMAND_QOS_RULES_PRESENCE;
        }
        break;
        /*
         case PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_IEI:
         if((decoded_result = decode_mapped_eps_bearer_contexts
         (&pdu_session_modification_command->mappedepsbearercontexts,
         PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_IEI,
         buffer+decoded,len-decoded))<0) return decoded_result; else
         {
         decoded+=decoded_result;
         pdu_session_modification_command->presence |=
         PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_PRESENCE;
         }
         break;
         */
      case PDU_SESSION_MODIFICATION_COMMAND_QOS_FLOW_DESCRIPTIONS_IEI:
        if ((decoded_result = decode_qos_flow_descriptions(
                 &pdu_session_modification_command->qosflowdescriptions,
                 PDU_SESSION_MODIFICATION_COMMAND_QOS_FLOW_DESCRIPTIONS_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_modification_command->presence |=
              PDU_SESSION_MODIFICATION_COMMAND_QOS_FLOW_DESCRIPTIONS_PRESENCE;
        }
        break;
      case PDU_SESSION_MODIFICATION_COMMAND_E_P_C_O_IEI:
        if ((decoded_result = decode_extended_protocol_configuration_options(
                 &pdu_session_modification_command
                      ->extendedprotocolconfigurationoptions,
                 PDU_SESSION_MODIFICATION_COMMAND_E_P_C_O_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          pdu_session_modification_command->presence |=
              PDU_SESSION_MODIFICATION_COMMAND_E_P_C_O_PRESENCE;
        }
        break;
    }
  }

  return decoded;
}

int encode_pdu_session_modification_command(
    pdu_session_modification_command_msg* pdu_session_modification_command,
    uint8_t* buffer, uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, PDU_SESSION_MODIFICATION_COMMAND_MINIMUM_LENGTH, len);

  if ((pdu_session_modification_command->presence &
       PDU_SESSION_MODIFICATION_COMMAND__5GSM_CAUSE_PRESENCE) ==
      PDU_SESSION_MODIFICATION_COMMAND__5GSM_CAUSE_PRESENCE) {
    if ((encoded_result = encode__5gsm_cause(
             pdu_session_modification_command->_5gsmcause,
             PDU_SESSION_MODIFICATION_COMMAND__5GSM_CAUSE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_modification_command->presence &
       PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENCE) ==
      PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_PRESENCE) {
    if ((encoded_result = encode_session_ambr(
             pdu_session_modification_command->sessionambr,
             PDU_SESSION_MODIFICATION_COMMAND_SESSION_AMBR_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_modification_command->presence &
       PDU_SESSION_MODIFICATION_COMMAND_GPRS_TIMER_PRESENCE) ==
      PDU_SESSION_MODIFICATION_COMMAND_GPRS_TIMER_PRESENCE) {
    if ((encoded_result = encode_gprs_timer(
             pdu_session_modification_command->gprstimer,
             PDU_SESSION_MODIFICATION_COMMAND_GPRS_TIMER_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_modification_command->presence &
       PDU_SESSION_MODIFICATION_COMMAND_ALWAYSON_PDU_SESSION_INDICATION_PRESENCE) ==
      PDU_SESSION_MODIFICATION_COMMAND_ALWAYSON_PDU_SESSION_INDICATION_PRESENCE) {
    if ((encoded_result = encode_alwayson_pdu_session_indication(
             pdu_session_modification_command->alwaysonpdusessionindication,
             PDU_SESSION_MODIFICATION_COMMAND_ALWAYSON_PDU_SESSION_INDICATION_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_modification_command->presence &
       PDU_SESSION_MODIFICATION_COMMAND_QOS_RULES_PRESENCE) ==
      PDU_SESSION_MODIFICATION_COMMAND_QOS_RULES_PRESENCE) {
    if ((encoded_result = encode_qos_rules(
             pdu_session_modification_command->qosrules,
             PDU_SESSION_MODIFICATION_COMMAND_QOS_RULES_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  /*
   if((pdu_session_modification_command->presence &
   PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_PRESENCE) ==
   PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_PRESENCE)
   {
   if((encoded_result = encode_mapped_eps_bearer_contexts
   (pdu_session_modification_command->mappedepsbearercontexts,
   PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_IEI,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;
   }
   */
  if ((pdu_session_modification_command->presence &
       PDU_SESSION_MODIFICATION_COMMAND_QOS_FLOW_DESCRIPTIONS_PRESENCE) ==
      PDU_SESSION_MODIFICATION_COMMAND_QOS_FLOW_DESCRIPTIONS_PRESENCE) {
    if ((encoded_result = encode_qos_flow_descriptions(
             pdu_session_modification_command->qosflowdescriptions,
             PDU_SESSION_MODIFICATION_COMMAND_QOS_FLOW_DESCRIPTIONS_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((pdu_session_modification_command->presence &
       PDU_SESSION_MODIFICATION_COMMAND_E_P_C_O_PRESENCE) ==
      PDU_SESSION_MODIFICATION_COMMAND_E_P_C_O_PRESENCE) {
    if ((encoded_result = encode_extended_protocol_configuration_options(
             pdu_session_modification_command
                 ->extendedprotocolconfigurationoptions,
             PDU_SESSION_MODIFICATION_COMMAND_E_P_C_O_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  return encoded;
}
