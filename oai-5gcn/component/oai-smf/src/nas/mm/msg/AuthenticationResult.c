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
#include "AuthenticationResult.h"

int decode_authentication_result(
    authentication_result_msg* authentication_result, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, AUTHENTICATION_RESULT_MINIMUM_LENGTH, len);

  if ((decoded_result = decode_u8_nas_key_set_identifier(
           &authentication_result->naskeysetidentifier, 0,
           *(buffer + decoded) >> 4, len - decoded)) < 0)
    return decoded_result;
  decoded++;

  if ((decoded_result = decode_eap_message(
           &authentication_result->eapmessage, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  while (len - decoded > 0) {
    uint8_t ieiDecoded = *(buffer + decoded);
    printf("ieiDecoded:0x%x\n", ieiDecoded);
    if (ieiDecoded == 0) break;
    switch (ieiDecoded) {
      case AUTHENTICATION_RESULT_ABBA_IEI:
        if ((decoded_result = decode_abba(
                 &authentication_result->abba, AUTHENTICATION_RESULT_ABBA_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          authentication_result->presence |= AUTHENTICATION_RESULT_ABBA_PRESENT;
        }
    }
  }
  /*
   if ((decoded_result = decode_abba (&authentication_result->abba,0 , buffer +
   decoded, len - decoded)) < 0) return decoded_result; else decoded +=
   decoded_result;
   */
  return decoded;
}

int encode_authentication_result(
    authentication_result_msg* authentication_result, uint8_t* buffer,
    uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, AUTHENTICATION_RESULT_MINIMUM_LENGTH, len);

  *(buffer + encoded) = ((encode_u8_nas_key_set_identifier(
                              &authentication_result->naskeysetidentifier) &
                          0x0f)
                         << 4) |
                        0x00;
  encoded++;

  if ((encoded_result = encode_eap_message(
           authentication_result->eapmessage, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((authentication_result->presence & AUTHENTICATION_RESULT_ABBA_PRESENT) ==
      AUTHENTICATION_RESULT_ABBA_PRESENT) {
    if ((encoded_result = encode_abba(
             authentication_result->abba, AUTHENTICATION_RESULT_ABBA_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  /*
   if((encoded_result = encode_abba (authentication_result->abba,0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;
   */
  return encoded;
}
