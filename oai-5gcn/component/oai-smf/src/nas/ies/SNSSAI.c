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
#include "SNSSAI.h"

int encode_snssai(SNSSAI snssai, uint8_t iei, uint8_t* buffer, uint32_t len) {
  uint32_t encoded     = 0;
  uint8_t ielen        = 0;
  uint8_t bitStream    = 0;
  uint32_t bit32Stream = 0;

  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer,
      ((iei > 0) ? SNSSAI_MINIMUM_LENGTH_TLV : SNSSAI_MINIMUM_LENGTH_TLV - 1),
      len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  ielen = snssai.len;

  if (snssai.sd == 0) ielen = 1;  // Don't include SD if it = 0

  *(buffer + encoded) = ielen;
  encoded++;

  bitStream = snssai.sst;
  ENCODE_U8(buffer + encoded, bitStream, encoded);

  if ((ielen == SST_AND_SD_LENGTH) ||
      (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_LENGTH) ||
      (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_AND_MAPPEDHPLMNSD_LENGTH)) {
    bit32Stream = snssai.sd;
    ENCODE_U8(buffer + encoded, (uint8_t)(bit32Stream >> 16), encoded);
    ENCODE_U8(buffer + encoded, (uint8_t)(bit32Stream >> 8), encoded);
    ENCODE_U8(buffer + encoded, (uint8_t) bit32Stream, encoded);
  }

  if ((ielen == SST_AND_MAPPEDHPLMNSST_LENGTH) ||
      (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_LENGTH) ||
      (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_AND_MAPPEDHPLMNSD_LENGTH)) {
    bitStream = snssai.mappedhplmnsst;
    ENCODE_U8(buffer + encoded, bitStream, encoded);
  }
  if (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_AND_MAPPEDHPLMNSD_LENGTH) {
    bit32Stream = snssai.mappedhplmnsd;
    ENCODE_U8(buffer + encoded, (uint8_t) bit32Stream, encoded);
    ENCODE_U8(buffer + encoded, (uint8_t)(bit32Stream >> 8), encoded);
    ENCODE_U8(buffer + encoded, (uint8_t)(bit32Stream >> 16), encoded);
  }

  return encoded;
}

int decode_snssai(SNSSAI* snssai, uint8_t iei, uint8_t* buffer, uint32_t len) {
  int decoded          = 0;
  uint8_t ielen        = 0;
  uint8_t bitStream    = 0;
  uint32_t bit32Stream = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  ielen = *(buffer + decoded);
  decoded++;
  CHECK_LENGTH_DECODER(len - decoded, ielen);

  snssai->len = ielen;

  DECODE_U8(buffer + decoded, bitStream, decoded);
  snssai->sst = bitStream;

  if ((ielen == SST_AND_SD_LENGTH) ||
      (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_LENGTH) ||
      (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_AND_MAPPEDHPLMNSD_LENGTH)) {
    DECODE_U8(buffer + decoded, bitStream, decoded);
    bit32Stream = (uint32_t)(bitStream & 0Xff);
    DECODE_U8(buffer + decoded, bitStream, decoded);
    bit32Stream |= (uint32_t)((bitStream << 8) & 0xff00);
    DECODE_U8(buffer + decoded, bitStream, decoded);
    bit32Stream |= (uint32_t)((bitStream << 16) & 0xff0000);

    snssai->sd = bit32Stream;
  }

  if ((ielen == SST_AND_MAPPEDHPLMNSST_LENGTH) ||
      (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_LENGTH) ||
      (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_AND_MAPPEDHPLMNSD_LENGTH)) {
    DECODE_U8(buffer + decoded, bitStream, decoded);
    snssai->mappedhplmnsst = bitStream;
  }
  if (ielen == SST_AND_SD_AND_MAPPEDHPLMNSST_AND_MAPPEDHPLMNSD_LENGTH) {
    DECODE_U8(buffer + decoded, bitStream, decoded);
    bit32Stream = (uint32_t)(bitStream & 0Xff);
    DECODE_U8(buffer + decoded, bitStream, decoded);
    bit32Stream |= (uint32_t)((bitStream << 8) & 0xff00);
    DECODE_U8(buffer + decoded, bitStream, decoded);
    bit32Stream |= (uint32_t)((bitStream << 16) & 0xff0000);

    snssai->mappedhplmnsd = bit32Stream;
  }

  return decoded;
}
