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
#include "DeregistrationRequest.h"

// TODO: to be implemeted
int decode_deregistration_request(
    deregistration_request_msg* deregistration_request, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded = 0;
  //  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, DEREGISTRATION_REQUEST_MINIMUM_LENGTH, len);
  /*
   if((decoded_result = decode_extended_protocol_discriminator
   (&deregistration_request->extendedprotocoldiscriminator, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_security_header_type
   (&deregistration_request->securityheadertype, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_message_type
   (&deregistration_request->messagetype, 0, buffer+decoded,len-decoded))<0)
   return decoded_result;
   else
   decoded+=decoded_result;

   if((decoded_result = decode_deregistration_type
   (&deregistration_request->deregistrationtype, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_nas_key_set_identifier
   (&deregistration_request->naskeysetidentifier, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode__5gs_mobile_identity
   (&deregistration_request->_5gsmobileidentity, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   */
  return decoded;
}

int encode_deregistration_request(
    deregistration_request_msg* deregistration_request, uint8_t* buffer,
    uint32_t len) {
  uint32_t encoded = 0;
  //  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, DEREGISTRATION_REQUEST_MINIMUM_LENGTH, len);
  /*
   if((encoded_result = encode_extended_protocol_discriminator
   (deregistration_request->extendedprotocoldiscriminator, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;

   if((encoded_result = encode_security_header_type
   (deregistration_request->securityheadertype, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;

   if((encoded_result = encode_message_type
   (deregistration_request->messagetype, 0, buffer+encoded,len-encoded))<0)
   return encoded_result;
   else
   encoded+=encoded_result;

   if((encoded_result = encode_deregistration_type
   (deregistration_request->deregistrationtype, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;

   if((encoded_result = encode_nas_key_set_identifier
   (deregistration_request->naskeysetidentifier, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;

   if((encoded_result = encode__5gs_mobile_identity
   (deregistration_request->_5gsmobileidentity, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;

   */
  return encoded;
}
