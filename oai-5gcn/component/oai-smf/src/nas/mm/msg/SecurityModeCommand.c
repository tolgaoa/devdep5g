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
#include "SecurityModeCommand.h"

int decode_security_mode_command(
    security_mode_command_msg* security_mode_command, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, SECURITY_MODE_COMMAND_MINIMUM_LENGTH, len);

  if ((decoded_result = decode_nas_security_algorithms(
           &security_mode_command->nassecurityalgorithms, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_nas_key_set_identifier(
           &security_mode_command->naskeysetidentifier, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_ue_security_capability(
           &security_mode_command->uesecuritycapability, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  while (len - decoded > 0) {
    uint8_t ieiDecoded = *(buffer + decoded);
    if (!ieiDecoded) break;

    switch (ieiDecoded & 0xf0) {
      case SECURITY_MODE_COMMAND_IMEISV_REQUEST_IEI:
        if ((decoded_result = decode_imeisv_request(
                 &security_mode_command->imeisvrequest,
                 SECURITY_MODE_COMMAND_IMEISV_REQUEST_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          security_mode_command->presence |=
              SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT;
        }
        break;
    }

    switch (ieiDecoded) {
      case SECURITY_MODE_COMMAND_EPS_NAS_SECURITY_ALGORITHMS_IEI:
        if ((decoded_result = decode_epsnas_security_algorithms(
                 &security_mode_command->epsnassecurityalgorithms,
                 SECURITY_MODE_COMMAND_EPS_NAS_SECURITY_ALGORITHMS_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          security_mode_command->presence |=
              SECURITY_MODE_COMMAND_EPS_NAS_SECURITY_ALGORITHMS_PRESENT;
        }
        break;
      case SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_IEI:
        if ((decoded_result = decode_additional_5g_security_information(
                 &security_mode_command->additional5gsecurityinformation,
                 SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          security_mode_command->presence |=
              SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_PRESENT;
        }
        break;
      case SECURITY_MODE_COMMAND_EAP_MESSAGE_IEI:
        if ((decoded_result = decode_eap_message(
                 &security_mode_command->eapmessage,
                 SECURITY_MODE_COMMAND_EAP_MESSAGE_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          security_mode_command->presence |=
              SECURITY_MODE_COMMAND_EAP_MESSAGE_PRESENT;
        }
        break;
      case SECURITY_MODE_COMMAND_ABBA_IEI:
        if ((decoded_result = decode_abba(
                 &security_mode_command->abba, SECURITY_MODE_COMMAND_ABBA_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          security_mode_command->presence |= SECURITY_MODE_COMMAND_ABBA_PRESENT;
        }
        break;
    }
  }
  /*
   if((decoded_result = decode_s1_ue_security_capability
   (&security_mode_command->s1uesecuritycapability, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   */
  return decoded;
}

int encode_security_mode_command(
    security_mode_command_msg* security_mode_command, uint8_t* buffer,
    uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, SECURITY_MODE_COMMAND_MINIMUM_LENGTH, len);

  if ((encoded_result = encode_nas_security_algorithms(
           security_mode_command->nassecurityalgorithms, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_nas_key_set_identifier(
           &security_mode_command->naskeysetidentifier, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_ue_security_capability(
           security_mode_command->uesecuritycapability, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((security_mode_command->presence &
       SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT) ==
      SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT) {
    if ((encoded_result = encode_imeisv_request(
             security_mode_command->imeisvrequest,
             SECURITY_MODE_COMMAND_IMEISV_REQUEST_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((security_mode_command->presence &
       SECURITY_MODE_COMMAND_EPS_NAS_SECURITY_ALGORITHMS_PRESENT) ==
      SECURITY_MODE_COMMAND_EPS_NAS_SECURITY_ALGORITHMS_PRESENT) {
    if ((encoded_result = encode_epsnas_security_algorithms(
             security_mode_command->epsnassecurityalgorithms,
             SECURITY_MODE_COMMAND_EPS_NAS_SECURITY_ALGORITHMS_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((security_mode_command->presence &
       SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_PRESENT) ==
      SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_PRESENT) {
    if ((encoded_result = encode_additional_5g_security_information(
             security_mode_command->additional5gsecurityinformation,
             SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((security_mode_command->presence &
       SECURITY_MODE_COMMAND_EAP_MESSAGE_PRESENT) ==
      SECURITY_MODE_COMMAND_EAP_MESSAGE_PRESENT) {
    if ((encoded_result = encode_eap_message(
             security_mode_command->eapmessage,
             SECURITY_MODE_COMMAND_EAP_MESSAGE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((security_mode_command->presence & SECURITY_MODE_COMMAND_ABBA_PRESENT) ==
      SECURITY_MODE_COMMAND_ABBA_PRESENT) {
    if ((encoded_result = encode_abba(
             security_mode_command->abba, SECURITY_MODE_COMMAND_ABBA_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  /*
   if((encoded_result = encode_s1_ue_security_capability
   (security_mode_command->s1uesecuritycapability, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;

   */
  return encoded;
}
