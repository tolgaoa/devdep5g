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
#include "RegistrationReject.h"

int decode_registration_reject(
    registration_reject_msg* registration_reject, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, REGISTRATION_REJECT_MINIMUM_LENGTH, len);

  if ((decoded_result = decode__5gmm_cause(
           &registration_reject->_5gmmcause, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  while (len - decoded > 0) {
    uint8_t ieiDecoded = *(buffer + decoded);
    if ((ieiDecoded != REGISTRATION_REJECT_GPRSTIMER2_T3346_VALUE_IEI) &&
        (ieiDecoded != REGISTRATION_REJECT_GPRSTIMER2_T3502_VALUE_IEI) &&
        (ieiDecoded != REGISTRATION_REJECT_EAP_MESSAGE_IEI))
      break;
    switch (ieiDecoded) {
      case REGISTRATION_REJECT_GPRSTIMER2_T3346_VALUE_IEI:
        if ((decoded_result = decode_gprs_timer2(
                 &registration_reject->t3346,
                 REGISTRATION_REJECT_GPRSTIMER2_T3346_VALUE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_reject->presence |=
              REGISTRATION_REJECT_GPRSTIMER2_T3346_VALUE_PRESENT;
        }
        break;
      case REGISTRATION_REJECT_GPRSTIMER2_T3502_VALUE_IEI:
        if ((decoded_result = decode_gprs_timer2(
                 &registration_reject->t3502,
                 REGISTRATION_REJECT_GPRSTIMER2_T3502_VALUE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_reject->presence |=
              REGISTRATION_REJECT_GPRSTIMER2_T3502_VALUE_PRESENT;
        }
        break;
      case REGISTRATION_REJECT_EAP_MESSAGE_IEI:
        if ((decoded_result = decode_eap_message(
                 &registration_reject->eapmessage,
                 REGISTRATION_REJECT_EAP_MESSAGE_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_reject->presence |=
              REGISTRATION_REJECT_EAP_MESSAGE_PRESENT;
        }
        break;
    }
  }

  return decoded;
}

int encode_registration_reject(
    registration_reject_msg* registration_reject, uint8_t* buffer,
    uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, REGISTRATION_REJECT_MINIMUM_LENGTH, len);

  if ((encoded_result = encode__5gmm_cause(
           registration_reject->_5gmmcause, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((registration_reject->presence &
       REGISTRATION_REJECT_GPRSTIMER2_T3346_VALUE_PRESENT) ==
      REGISTRATION_REJECT_GPRSTIMER2_T3346_VALUE_PRESENT) {
    if ((encoded_result = encode_gprs_timer2(
             registration_reject->t3346,
             REGISTRATION_REJECT_GPRSTIMER2_T3346_VALUE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_reject->presence &
       REGISTRATION_REJECT_GPRSTIMER2_T3502_VALUE_PRESENT) ==
      REGISTRATION_REJECT_GPRSTIMER2_T3502_VALUE_PRESENT) {
    if ((encoded_result = encode_gprs_timer2(
             registration_reject->t3502,
             REGISTRATION_REJECT_GPRSTIMER2_T3502_VALUE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_reject->presence &
       REGISTRATION_REJECT_EAP_MESSAGE_PRESENT) ==
      REGISTRATION_REJECT_EAP_MESSAGE_PRESENT) {
    if ((encoded_result = encode_eap_message(
             registration_reject->eapmessage,
             REGISTRATION_REJECT_EAP_MESSAGE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  return encoded;
}
