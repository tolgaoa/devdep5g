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
#include "PDUAddress.h"

int encode_pdu_address(
    PDUAddress pduaddress, uint8_t iei, uint8_t* buffer, uint32_t len) {
  uint8_t* lenPtr   = NULL;
  uint32_t encoded  = 0;
  int encode_result = 0;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer,
      ((iei > 0) ? PDU_ADDRESS_MINIMUM_LENGTH_TLV :
                   PDU_ADDRESS_MINIMUM_LENGTH_TLV - 1),
      len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = buffer + encoded;
  encoded++;  // ENCODE_U8(buffer+encoded,pduaddress.length, encoded);
  uint8_t len_pos = encoded;

  ENCODE_U8(
      buffer + encoded, (uint8_t)(pduaddress.pdu_session_type_value & 0x07),
      encoded);

  if ((encode_result = encode_bstring(
           pduaddress.pdu_address_information, buffer + encoded,
           len - encoded)) < 0)
    return encode_result;
  else
    encoded += encode_result;

  // set length
  uint16_t temp = 0;
  //*(uint8_t*) (lenPtr) = encoded - len_pos;
  ENCODE_U8(lenPtr, encoded - len_pos, temp);

  return encoded;
}

int decode_pdu_address(
    PDUAddress* pduaddress, uint8_t iei, uint8_t* buffer, uint32_t len) {
  int decoded       = 0;
  uint8_t ielen     = 0;
  int decode_result = 0;
  uint8_t bitStream = 0x00;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  ielen = *(buffer + decoded);
  decoded++;
  CHECK_LENGTH_DECODER(len - decoded, ielen);

  DECODE_U8(buffer + decoded, bitStream, decoded);
  pduaddress->pdu_session_type_value = bitStream & 0x07;

  if ((decode_result = decode_bstring(
           &pduaddress->pdu_address_information, ielen - 1, buffer + decoded,
           len - decoded)) < 0)
    return decode_result;
  else
    decoded += decode_result;

  return decoded;
}
