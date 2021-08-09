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

#ifndef FILE_3GPP_24_007_SEEN
#define FILE_3GPP_24_007_SEEN

#include <stdint.h>

typedef enum extended_protocol_discriminator_e {
  // 5G Session Management
  EPD_5GS_SESSION_MANAGEMENT_MESSAGES = 0x2e,
  // 5G Mobility Management
  EPD_5GS_MOBILITY_MANAGEMENT_MESSAGES = 0x7e,

} extended_protocol_discriminator_t;

// Procedure transaction identity
// 8 bits
#define PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED (uint8_t) 0
#define PROCEDURE_TRANSACTION_IDENTITY_FIRST (uint8_t) 1
#define PROCEDURE_TRANSACTION_IDENTITY_LAST (uint8_t) 254
#define PROCEDURE_TRANSACTION_IDENTITY_RESERVED (uint8_t) 255

// PDU Session Identity
typedef uint8_t pdu_session_id_t;

// 8 bits
#define PDU_SESSION_IDENTITY_UNASSIGNED (uint8_t) 0
#define PDU_SESSION_IDENTITY_FIRST (uint8_t) 1
#define PDU_SESSION_IDENTITY_LAST (uint8_t) 15

// QFI
// type: integer,  minimum: 0, maximum: 63
#define NO_QOS_FLOW_IDENTIFIER_ASSIGNED (uint8_t) 0
#define QOS_FLOW_IDENTIFIER_FIRST (uint8_t) 1
#define QOS_FLOW_IDENTIFIER_LAST (uint8_t) 63

// QoS Rule
#define NO_QOS_RULE_IDENTIFIER_ASSIGNED (uint8_t) 0
#define QOS_RULE_IDENTIFIER_FIRST (uint8_t) 1
#define QOS_RULE_IDENTIFIER_LAST (uint8_t) 255

// Integrity protection maximum data rate
typedef struct ipmdr_s {
  uint8_t ul;
  uint8_t dl;
} ipmdr_t;

#endif
