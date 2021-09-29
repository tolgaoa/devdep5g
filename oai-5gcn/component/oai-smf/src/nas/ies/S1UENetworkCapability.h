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

#define S1_UE_NETWORK_CAPABILITY_MINIMUM_LENGTH 4
#define S1_UE_NETWORK_CAPABILITY_MAXIMUM_LENGTH 15

typedef struct {
  uint8_t eea;
  uint8_t eia;
} S1UENetworkCapability;

int encode_s1_ue_network_capability(
    S1UENetworkCapability s1uenetworkcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode_s1_ue_network_capability(
    S1UENetworkCapability* s1uenetworkcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len);
