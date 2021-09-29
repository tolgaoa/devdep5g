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
#include "_5GSTrackingAreaIdentity.h"

int encode__5gs_tracking_area_identity(
    _5GSTrackingAreaIdentity _5gstrackingareaidentity, uint8_t iei,
    uint8_t* buffer, uint32_t len) {
  uint32_t encoded = 0;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, _5GS_TRACKING_AREA_IDENTITY_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  ENCODE_U8(
      buffer + encoded, (uint8_t)(_5gstrackingareaidentity.mcc & 0x00ff),
      encoded);
  ENCODE_U8(
      buffer + encoded,
      (uint8_t)((_5gstrackingareaidentity.mcc & 0x0f00) >> 8) |
          (uint8_t)((_5gstrackingareaidentity.mnc & 0x0f00) >> 4),
      encoded);
  ENCODE_U8(
      buffer + encoded, (uint8_t)(_5gstrackingareaidentity.mnc & 0x00ff),
      encoded);
  ENCODE_U32(
      buffer + encoded, _5gstrackingareaidentity.tac & 0x00ffffff, encoded);

  return encoded;
}

int decode__5gs_tracking_area_identity(
    _5GSTrackingAreaIdentity* _5gstrackingareaidentity, uint8_t iei,
    uint8_t* buffer, uint32_t len) {
  int decoded            = 0;
  uint8_t mcc_mnc_decode = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  DECODE_U8(buffer + decoded, mcc_mnc_decode, decoded);
  _5gstrackingareaidentity->mcc = 0x0000 | mcc_mnc_decode;
  DECODE_U8(buffer + decoded, mcc_mnc_decode, decoded);
  _5gstrackingareaidentity->mcc =
      _5gstrackingareaidentity->mcc | ((mcc_mnc_decode & 0x0f) << 8);
  _5gstrackingareaidentity->mnc = 0x0000 | ((mcc_mnc_decode & 0xf0) << 4);
  DECODE_U8(buffer + decoded, mcc_mnc_decode, decoded);
  _5gstrackingareaidentity->mnc =
      _5gstrackingareaidentity->mnc | mcc_mnc_decode;

  DECODE_U32(buffer + decoded, _5gstrackingareaidentity->tac, decoded);

  return decoded;
}
