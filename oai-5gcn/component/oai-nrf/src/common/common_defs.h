/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

/*! \file common_defs.h
 \brief
 \author Sebastien ROUX, Lionel Gauthier
 \company Eurecom
 \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_COMMON_DEFS_SEEN
#define FILE_COMMON_DEFS_SEEN

#include <arpa/inet.h>
#include <stdint.h>

#define RETURNclear (int) 2
#define RETURNerror (int) 1
#define RETURNok (int) 0

//------------------------------------------------------------------------------
#define DECODE_U8(bUFFER, vALUE, sIZE)                                         \
  vALUE = *(uint8_t*) (bUFFER);                                                \
  sIZE += sizeof(uint8_t)

#define DECODE_U16(bUFFER, vALUE, sIZE)                                        \
  vALUE = ntohs(*(uint16_t*) (bUFFER));                                        \
  sIZE += sizeof(uint16_t)

#define DECODE_U24(bUFFER, vALUE, sIZE)                                        \
  vALUE = ntohl(*(uint32_t*) (bUFFER)) >> 8;                                   \
  sIZE += sizeof(uint8_t) + sizeof(uint16_t)

#define DECODE_U32(bUFFER, vALUE, sIZE)                                        \
  vALUE = ntohl(*(uint32_t*) (bUFFER));                                        \
  sIZE += sizeof(uint32_t)

#if (BYTE_ORDER == LITTLE_ENDIAN)
#define DECODE_LENGTH_U16(bUFFER, vALUE, sIZE)                                 \
  vALUE = ((*(bUFFER)) << 8) | (*((bUFFER) + 1));                              \
  sIZE += sizeof(uint16_t)
#else
#define DECODE_LENGTH_U16(bUFFER, vALUE, sIZE)                                 \
  vALUE = (*(bUFFER)) | (*((bUFFER) + 1) << 8);                                \
  sIZE += sizeof(uint16_t)
#endif

#define ENCODE_U8(buffer, value, size)                                         \
  *(uint8_t*) (buffer) = value;                                                \
  size += sizeof(uint8_t)

#define ENCODE_U16(buffer, value, size)                                        \
  *(uint16_t*) (buffer) = htons(value);                                        \
  size += sizeof(uint16_t)

#define ENCODE_U24(buffer, value, size)                                        \
  *(uint32_t*) (buffer) = htonl(value);                                        \
  size += sizeof(uint8_t) + sizeof(uint16_t)

#define ENCODE_U32(buffer, value, size)                                        \
  *(uint32_t*) (buffer) = htonl(value);                                        \
  size += sizeof(uint32_t)

#define IPV4_STR_ADDR_TO_INT_NWBO(AdDr_StR, NwBo, MeSsAgE)                     \
  do {                                                                         \
    struct in_addr inp;                                                        \
    if (inet_aton(AdDr_StR, &inp) < 0) {                                       \
      AssertFatal(0, MeSsAgE);                                                 \
    } else {                                                                   \
      NwBo = inp.s_addr;                                                       \
    }                                                                          \
  } while (0)

#define NIPADDR(addr)                                                          \
  (uint8_t)(addr & 0x000000FF), (uint8_t)((addr & 0x0000FF00) >> 8),           \
      (uint8_t)((addr & 0x00FF0000) >> 16),                                    \
      (uint8_t)((addr & 0xFF000000) >> 24)

#define HIPADDR(addr)                                                          \
  (uint8_t)((addr & 0xFF000000) >> 24), (uint8_t)((addr & 0x00FF0000) >> 16),  \
      (uint8_t)((addr & 0x0000FF00) >> 8), (uint8_t)(addr & 0x000000FF)

#define NIP6ADDR(addr)                                                         \
  ntohs((addr)->s6_addr16[0]), ntohs((addr)->s6_addr16[1]),                    \
      ntohs((addr)->s6_addr16[2]), ntohs((addr)->s6_addr16[3]),                \
      ntohs((addr)->s6_addr16[4]), ntohs((addr)->s6_addr16[5]),                \
      ntohs((addr)->s6_addr16[6]), ntohs((addr)->s6_addr16[7])

#define IN6_ARE_ADDR_MASKED_EQUAL(a, b, m)                                     \
  (((((__const uint32_t*) (a))[0] & (((__const uint32_t*) (m))[0])) ==         \
    (((__const uint32_t*) (b))[0] & (((__const uint32_t*) (m))[0]))) &&        \
   ((((__const uint32_t*) (a))[1] & (((__const uint32_t*) (m))[1])) ==         \
    (((__const uint32_t*) (b))[1] & (((__const uint32_t*) (m))[1]))) &&        \
   ((((__const uint32_t*) (a))[2] & (((__const uint32_t*) (m))[2])) ==         \
    (((__const uint32_t*) (b))[2] & (((__const uint32_t*) (m))[2]))) &&        \
   ((((__const uint32_t*) (a))[3] & (((__const uint32_t*) (m))[3])) ==         \
    (((__const uint32_t*) (b))[3] & (((__const uint32_t*) (m))[3]))))

////////////
#define IPV4_STR_ADDR_TO_INADDR(AdDr_StR, InAdDr, MeSsAgE)                     \
  do {                                                                         \
    if (inet_aton(AdDr_StR, &InAdDr) <= 0) {                                   \
      throw(MeSsAgE);                                                          \
    }                                                                          \
  } while (0)

#ifndef UNUSED
#define UNUSED(x) (void) (x)
#endif

#endif /* FILE_COMMON_DEFS_SEEN */
