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

#ifndef FILE_COMMON_TYPES_SEEN
#define FILE_COMMON_TYPES_SEEN

#include "3gpp_38.401.h"
#include "3gpp_23.003.h"
#include "3gpp_24.007.h"
#include "3gpp_24.501.h"

typedef uint16_t sctp_stream_id_t;
typedef uint32_t sctp_assoc_id_t;

// UE NGAP IDs

#define INVALID_RAN_UE_NGAP_ID_KEY 0xFFFFFFFFFFFFFFFF
#define RAN_UE_NGAP_ID_MASK 0x00FFFFFF
#define RAN_UE_NGAP_ID_FMT "0x%06" PRIX32
#define GNB_UE_NGAP_ID_FMT "0x%06" PRIX32
#define AMF_UE_NGAP_ID_FMT "0x%08" PRIX32
#define INVALID_AMF_UE_NGAP_ID 0x0

#define AMF_UE_NGAP_ID_MASK_ 0x0000FFFFFFFFFF

typedef uint8_t ksi_t;
#define KSI_NO_KEY_AVAILABLE 0x07

typedef uint32_t ipv4_nbo_t;

#endif
