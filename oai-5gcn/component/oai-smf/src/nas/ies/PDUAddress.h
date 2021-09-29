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

#include <stdint.h>
#include "bstrlib.h"

#define PDU_ADDRESS_MINIMUM_LENGTH 7
#define PDU_ADDRESS_MAXIMUM_LENGTH 15

#define PDU_ADDRESS_MINIMUM_LENGTH_TLV 7
#define PDU_ADDRESS_MAXIMUM_LENGTH_TLV 15

#define PDU_ADDRESS_IPV4 0x01
#define PDU_ADDRESS_IPV6 0x02
#define PDU_ADDRESS_IPV4V6 0x03

typedef struct {
  uint8_t length;
  uint8_t spare : 5;
  uint8_t pdu_session_type_value : 3;
  bstring pdu_address_information;
} __attribute__((__packed__)) PDUAddress;

int encode_pdu_address(
    PDUAddress pduaddress, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_pdu_address(
    PDUAddress* pduaddress, uint8_t iei, uint8_t* buffer, uint32_t len);
