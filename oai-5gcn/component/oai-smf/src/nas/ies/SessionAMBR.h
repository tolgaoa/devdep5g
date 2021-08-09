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

#ifndef _SESSIONAMBR_H_
#define _SESSIONAMBR_H_

#include <stdint.h>
#include "bstrlib.h"

#define SESSION_AMBR_MINIMUM_LENGTH 8
#define SESSION_AMBR_MAXIMUM_LENGTH 8

#define SESSION_AMBR_MINIMUM_LENGTH_TLV 8
#define SESSION_AMBR_MAXIMUM_LENGTH_TLV 8
#define SESSION_AMBR_MINIMUM_LENGTH_LV 7
#define SESSION_AMBR_MAXIMUM_LENGTH_LV 7

// Unit for Session-AMBR for downlink (octet 3)
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1KBPS 0b00000001
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4KBPS 0b00000010
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16KBPS 0b00000011
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64KBPS 0b00000100
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256KBPS 0b00000101
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS 0b00000110
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4MBPS 0b00000111
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16MBPS 0b00001000
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64MBPS 0b00001001
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256MBPS 0b00001010
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1GBPS 0b00001011
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4GBPS 0b00001100
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16GBPS 0b00001101
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64GBPS 0b00001110
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256GBPS 0b00001111
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1TBPS 0b00010000
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4TBPS 0b00010001
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16TBPS 0b00010010
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64TBPS 0b00010011
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256TBPS 0b00010100
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1PBPS 0b00010101
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4PBPS 0b00010110
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16PBPS 0b00010111
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64PBPS 0b00011000
#define AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256PBPS 0b00011001

// typedef bstring SessionAMBR;
typedef struct {
  uint8_t length;
  uint8_t uint_for_session_ambr_for_downlink;
  uint16_t session_ambr_for_downlink;
  uint8_t uint_for_session_ambr_for_uplink;
  uint16_t session_ambr_for_uplink;
} SessionAMBR;

int encode_session_ambr(
    SessionAMBR sessionambr, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_session_ambr(
    SessionAMBR* sessionambr, uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
