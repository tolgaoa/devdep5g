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

/*! \file nas_message.h
 \brief
 \author
 \company EURECOM
 \date 2019
 \email:
 */

#ifndef NAS_MESSAGE_H_
#define NAS_MESSAGE_H_

#include "mm_msg.h"
#include "sm_msg.h"
#include "common_types.h"

#define DIRECTION__ 1  // SECU_DIRECTION_DOWNLINK
#define TEST_MAC_ENCRYPT_DECRYPT__ 0
#define NAS_MESSAGE_SECURITY_HEADER_SIZE 7

// Section 9.1 3GPP TS 24.501
/* Structure of security protected header */
typedef struct nas_message_security_header_s {
#ifdef __LITTLE_ENDIAN_BITFIELD
  extended_protocol_discriminator_t extended_protocol_discriminator;
  uint8_t security_header_type : 4;
  uint8_t spare_half_octet : 4;
#endif
#ifdef __BIG_ENDIAN_BITFIELD
  uint8_t spare_half_octet : 4;
  uint8_t security_header_type : 4;
#endif
  uint32_t message_authentication_code;
  uint8_t sequence_number;
} __attribute__((__packed__)) nas_message_security_header_t;

/* Structure of plain NAS message */
typedef union {
  MM_msg mm; /* NAS MM */
  SM_msg sm; /* NAS SM  */
} nas_message_plain_t;

/* Structure of security protected NAS message */
typedef struct nas_message_security_protected_s {
  nas_message_security_header_t header;
  nas_message_plain_t plain;
} nas_message_security_protected_t;

/*
 * Structure of a layer 3 NAS message
 */
typedef struct {
  nas_message_security_header_t header;
  nas_message_security_protected_t security_protected;
  nas_message_plain_t plain;
} nas_message_t;

typedef struct nas_message_decode_status_s {
  uint8_t integrity_protected_message : 1;
  uint8_t ciphered_message : 1;
  uint8_t mac_matched : 1;
  uint8_t security_context_available : 1;
  int fivegmm_cause;
} nas_message_decode_status_t;

/*
 * Encode layer 3 NAS message
 * @param [unsigned char* ] buffer: Pointer to the encoded data buffer
 * @param [const nas_message_t* const] msg: L3 NAS message structure to encode
 * @param [size_t] length: Maximal capacity of the output buffer
 * @param [void*] security: L3 NAS message structure to encode
 * @return he number of bytes in the buffer if the data have been successfully
 * encoded. A negative error code otherwise.
 */
int nas_message_encode(
    unsigned char* buffer, const nas_message_t* const msg, size_t length,
    void* security);

int nas_message_decode(
    const unsigned char* const buffer, nas_message_t* msg, size_t length,
    void* security, nas_message_decode_status_t* status);

#endif
