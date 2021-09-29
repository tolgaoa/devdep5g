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
#include "_5GSMCapability.h"

int encode__5gsm_capability(
    _5GSMCapability _5gsmcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  uint8_t* lenPtr              = NULL;
  uint32_t encoded             = 0;
  uint8_t _5gsmcapability_bits = 0;

  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer,
      ((iei > 0) ? _5GSM_CAPABILITY_MINIMUM_LENGTH_TLV :
                   _5GSM_CAPABILITY_MINIMUM_LENGTH_TLV - 1),
      len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = (buffer + encoded);
  encoded++;

  if (_5gsmcapability.is_MPTCP_supported) _5gsmcapability_bits |= 0X10;
  if (_5gsmcapability.is_ATSLL_supported) _5gsmcapability_bits |= 0X08;
  if (_5gsmcapability.is_EPTS1_supported) _5gsmcapability_bits |= 0X04;
  if (_5gsmcapability.is_MH6PDU_supported) _5gsmcapability_bits |= 0X02;
  if (_5gsmcapability.is_Rqos_supported) _5gsmcapability_bits |= 0X01;

  ENCODE_U8(buffer + encoded, _5gsmcapability_bits, encoded);

  *lenPtr = encoded - 1 - ((iei > 0) ? 1 : 0);

  return encoded;
}

int decode__5gsm_capability(
    _5GSMCapability* _5gsmcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  int decoded   = 0;
  uint8_t ielen = 0;

  uint8_t _5gsmcapability_bits = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  ielen = *(buffer + decoded);
  decoded++;
  CHECK_LENGTH_DECODER(len - decoded, ielen);

  DECODE_U8(buffer + decoded, _5gsmcapability_bits, decoded);

  if (_5gsmcapability_bits & 0x10)
    _5gsmcapability->is_MPTCP_supported = true;
  else
    _5gsmcapability->is_MPTCP_supported = false;

  if (_5gsmcapability_bits & 0x08)
    _5gsmcapability->is_ATSLL_supported = true;
  else
    _5gsmcapability->is_ATSLL_supported = false;

  if (_5gsmcapability_bits & 0x04)
    _5gsmcapability->is_EPTS1_supported = true;
  else
    _5gsmcapability->is_EPTS1_supported = false;

  if (_5gsmcapability_bits & 0x02)
    _5gsmcapability->is_MH6PDU_supported = true;
  else
    _5gsmcapability->is_MH6PDU_supported = false;

  if (_5gsmcapability_bits & 0x01)
    _5gsmcapability->is_Rqos_supported = true;
  else
    _5gsmcapability->is_Rqos_supported = false;

  return decoded;
}
