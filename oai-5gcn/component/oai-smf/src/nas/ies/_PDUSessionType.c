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
#include "_PDUSessionType.h"

int encode__pdu_session_type(
    _PDUSessionType _pdusessiontype, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  uint32_t encoded  = 0;
  uint8_t bitStream = 0x00;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, _PDU_SESSION_TYPE_MINIMUM_LENGTH, len);

  if (iei > 0) {
    bitStream |= (iei & 0xf0) ;
  }

  bitStream |= ((_pdusessiontype.pdu_session_type_value & 0x07) | 0x10);
  ENCODE_U8(buffer + encoded, bitStream, encoded);

  return encoded;
}

int decode__pdu_session_type(
    _PDUSessionType* _pdusessiontype, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  int decoded       = 0;
  uint8_t bitStream = 0x00;

  DECODE_U8(buffer + decoded, bitStream, decoded);

  if (iei != (bitStream & 0xf0)) {
    return -1;
  }

  if (iei > 0) {
    bitStream = (bitStream & 0x07);
  }

  _pdusessiontype->pdu_session_type_value = bitStream;

  return decoded;
}
