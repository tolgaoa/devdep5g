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
#include "AuthenticationResponse.h"

int decode_authentication_response(
    authentication_response_msg* authentication_response, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, AUTHENTICATION_RESPONSE_MINIMUM_LENGTH, len);

  while (len - decoded > 0) {
    uint8_t ieiDecoded = *(buffer + decoded);
    printf("ieiDecoded(%x)\n", ieiDecoded);
    if (ieiDecoded !=
            AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_IEI &&
        ieiDecoded != AUTHENTICATION_RESPONSE_EAP_MESSAGE_IEI)
      break;

    switch (ieiDecoded) {
      case AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_IEI:
        printf(
            "decoding "
            "AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_IEI\n");
        if ((decoded_result = decode_authentication_response_parameter(
                 &authentication_response->authenticationresponseparameter,
                 AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          printf("decoded(%d)\n", decoded);
          authentication_response->presence |=
              AUTHENTICATION_RESPONSE_AUTNENTICATION_RESPONSE_PARAMETER_PRESENT;
        }
        break;
      case AUTHENTICATION_RESPONSE_EAP_MESSAGE_IEI:
        if ((decoded_result = decode_eap_message(
                 &authentication_response->eapmessage,
                 AUTHENTICATION_RESPONSE_EAP_MESSAGE_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          authentication_response->presence |=
              AUTHENTICATION_RESPONSE_EAP_MESSAGE_PRESENT;
        }
        break;
    }
  }

  return decoded;
}

int encode_authentication_response(
    authentication_response_msg* authentication_response, uint8_t* buffer,
    uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, AUTHENTICATION_RESPONSE_MINIMUM_LENGTH, len);

  if ((authentication_response->presence &
       AUTHENTICATION_RESPONSE_AUTNENTICATION_RESPONSE_PARAMETER_PRESENT) ==
      AUTHENTICATION_RESPONSE_AUTNENTICATION_RESPONSE_PARAMETER_PRESENT) {
    if ((encoded_result = encode_authentication_response_parameter(
             authentication_response->authenticationresponseparameter,
             AUTHENTICATION_RESPONSE_AUTHENTICATION_RESPONSE_PARAMETER_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((authentication_response->presence &
       AUTHENTICATION_RESPONSE_EAP_MESSAGE_PRESENT) ==
      AUTHENTICATION_RESPONSE_EAP_MESSAGE_PRESENT) {
    if ((encoded_result = encode_eap_message(
             authentication_response->eapmessage,
             AUTHENTICATION_RESPONSE_EAP_MESSAGE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  printf("encoded encode_authentication_response\n");
  return encoded;
}
