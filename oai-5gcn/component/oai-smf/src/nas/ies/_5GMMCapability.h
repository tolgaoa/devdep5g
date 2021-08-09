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

#ifndef __5GMM_CAPABILITY_H_
#define __5GMM_CAPABILITY_H_

#include <stdint.h>
#include "bstrlib.h"
#include <stdbool.h>

#define _5GMM_CAPABILITY_MINIMUM_LENGTH 3
#define _5GMM_CAPABILITY_MAXIMUM_LENGTH 15

#define S1_MODE_NOT_SUPPORTED 0
#define S1_MODE_SUPPORTED 1
#define HANDEROVER_TRANSFER_PDU_SESSION_NOT_SUPPORTED 0
#define HANDEROVER_TRANSFER_PDU_SESSION_SUPPORTED 1
#define LPP_IN_N1_MODE_NOT_SUPPORTED 0
#define LPP_IN_N1_MODE_SUPPORTED 1

typedef struct {
  bool is_LPP_supported;
  bool is_HO_supported;
  bool is_S1_mode_supported;
} _5GMMCapability;

int encode__5gmm_capability(
    _5GMMCapability _5gmmcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode__5gmm_capability(
    _5GMMCapability* _5gmmcapability, uint8_t iei, uint8_t* buffer,
    uint32_t len);

#endif
