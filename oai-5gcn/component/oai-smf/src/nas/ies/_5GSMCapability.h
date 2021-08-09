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

#ifndef __5GSMCAPABILITY_H_
#define __5GSMCAPABILITY_H_

#include <stdint.h>
#include <stdbool.h>
#include "bstrlib.h"

#define _5GSM_CAPABILITY_MINIMUM_LENGTH 3
#define _5GSM_CAPABILITY_MAXIMUM_LENGTH 15

#define _5GSM_CAPABILITY_MINIMUM_LENGTH_TLV 3
#define _5GSM_CAPABILITY_MAXIMUM_LENGTH_TLV 15

#define REFLECTIVE_QOS_NOT_SUPPORTED 0
#define REFLECTIVE_QOS_SUPPORTED 1
#define MULTI_HOMED_IPV6_PDU_SESSION_NOT_SUPPORTED 0
#define MULTI_HOMED_IPV6_PDU_SESSION_SUPPORTED 1
#define ETHERNET_PDN_TYPE_IN_S1_MODE_NOT_SUPPORTED 0
#define ETHERNET_PDN_TYPE_IN_S1_MODE_SUPPORTED 1
#define EATSSS_LOW_LAYER_FUNCTIONALITY_NOT_SUPPORTED 0
#define EATSSS_LOW_LAYER_FUNCTIONALITY_SUPPORTED 1
#define MPTCP_FUNCTIONALITY_NOT_SUPPORTED 0
#define MPTCP_FUNCTIONALITY_SUPPORTED 1

typedef struct {
  bool is_Rqos_supported;
  bool is_MH6PDU_supported;
  bool is_EPTS1_supported;
  bool is_ATSLL_supported;
  bool is_MPTCP_supported;
  // bstring _5GSMCapability_spare;
} _5GSMCapability;

int encode__5gsm_capability(
    _5GSMCapability _5gsmcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode__5gsm_capability(
    _5GSMCapability* _5gsmcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len);

#endif
