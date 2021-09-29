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
#include "_5GMMCapability.h"

int encode__5gmm_capability(
    _5GMMCapability _5gmmcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  uint8_t* lenPtr;
  uint32_t encoded = 0;

  uint8_t _5gmmcapability_bits = 0;

  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, _5GMM_CAPABILITY_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = (buffer + encoded);
  encoded++;

  if (_5gmmcapability.is_LPP_supported) _5gmmcapability_bits |= 0x04;
  if (_5gmmcapability.is_HO_supported) _5gmmcapability_bits |= 0x02;
  if (_5gmmcapability.is_S1_mode_supported) _5gmmcapability_bits |= 0x01;

  ENCODE_U8(buffer + encoded, _5gmmcapability_bits, encoded);
  *lenPtr = encoded - 1 - ((iei > 0) ? 1 : 0);
  return encoded;
}

int decode__5gmm_capability(
    _5GMMCapability* _5gmmcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  int decoded   = 0;
  uint8_t ielen = 0;

  uint8_t _5gmmcapability_bits = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }
  ielen = *(buffer + decoded);
  decoded++;
  CHECK_LENGTH_DECODER(len - decoded, ielen);

  DECODE_U8(buffer + decoded, _5gmmcapability_bits, decoded);

  if (_5gmmcapability_bits & 0x04)
    _5gmmcapability->is_LPP_supported = true;
  else
    _5gmmcapability->is_LPP_supported = false;

  if (_5gmmcapability_bits & 0x02)
    _5gmmcapability->is_HO_supported = true;
  else
    _5gmmcapability->is_HO_supported = false;

  if (_5gmmcapability_bits & 0x01)
    _5gmmcapability->is_S1_mode_supported = true;
  else
    _5gmmcapability->is_S1_mode_supported = false;

  return decoded;
}
