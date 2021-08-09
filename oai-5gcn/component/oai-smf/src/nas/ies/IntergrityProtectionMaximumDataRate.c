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
#include <stdint.h>

#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "IntergrityProtectionMaximumDataRate.h"

int encode_intergrity_protection_maximum_data_rate(
    IntergrityProtectionMaximumDataRate intergrityprotectionmaximumdatarate,
    uint8_t iei, uint8_t* buffer, uint32_t len) {
  uint32_t encoded  = 0;
  int encode_result = 0;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer,
      ((iei > 0) ? INTERGRITY_PROTECTION_MAXIMUM_DATA_RATE_MINIMUM_LENGTH_TV :
                   INTERGRITY_PROTECTION_MAXIMUM_DATA_RATE_MINIMUM_LENGTH_V),
      len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  if ((encode_result = encode_bstring(
           intergrityprotectionmaximumdatarate, buffer + encoded,
           len - encoded)) < 0)
    return encode_result;
  else
    encoded += encode_result;

  return encoded;
}

int decode_intergrity_protection_maximum_data_rate(
    IntergrityProtectionMaximumDataRate* intergrityprotectionmaximumdatarate,
    uint8_t iei, uint8_t* buffer, uint32_t len) {
  int decoded       = 0;
  uint8_t ielen     = 2;
  int decode_result = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  if ((decode_result = decode_bstring(
           intergrityprotectionmaximumdatarate, ielen, buffer + decoded,
           len - decoded)) < 0)
    return decode_result;
  else
    decoded += decode_result;
  return decoded;
}
