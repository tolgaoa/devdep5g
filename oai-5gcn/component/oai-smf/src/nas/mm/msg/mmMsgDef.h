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

#ifndef MM_MSG_DEF_H_
#define MM_MSG_DEF_H_

#include <stdint.h>
#include <asm/byteorder.h>

typedef struct {
#ifdef __LITTLE_ENDIAN_BITFIELD
  uint8_t extended_protocol_discriminator;
  uint8_t security_header_type : 4;
  uint8_t spare_half_octet : 4;
#endif
#ifdef __BIG_ENDIAN_BITFIELD
  uint8_t spare_half_octet : 4;
  uint8_t security_header_type : 4;
#endif
  uint8_t message_type;
} __attribute__((__packed__)) mm_msg_header_t;

#endif
