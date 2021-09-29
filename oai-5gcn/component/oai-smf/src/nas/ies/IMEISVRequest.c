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
#include "IMEISVRequest.h"

int encode_imeisv_request(
    IMEISVRequest imeisvrequest, uint8_t iei, uint8_t* buffer, uint32_t len) {
  uint32_t encoded  = 0;
  uint8_t bitStream = 0x0;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, IMEISV_REQUEST_MINIMUM_LENGTH, len);

  bitStream = (iei & 0xf0) | imeisvrequest;
  ENCODE_U8(buffer + encoded, bitStream, encoded);

  return encoded;
}

int decode_imeisv_request(
    IMEISVRequest* imeisvrequest, uint8_t iei, uint8_t* buffer, uint32_t len) {
  printf("decode_imeisv_request\n");
  int decoded       = 0;
  uint8_t bitStream = 0x0;

  DECODE_U8(buffer + decoded, bitStream, decoded);
  if (iei != (bitStream & 0xf0)) return -1;
  *imeisvrequest = bitStream & 0x07;

  return decoded;
}
