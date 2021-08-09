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

#ifndef __5GS_REGISTRATION_RESULT_H_
#define __5GS_REGISTRATION_RESULT_H_

#include <stdint.h>
#include "bstrlib.h"
#include <stdbool.h>

#define _5GS_REGISTRATION_RESULT_MINIMUM_LENGTH 3
#define _5GS_REGISTRATION_RESULT_MAXIMUM_LENGTH 3

#define _3GPP_ACCESS 0b001
#define NON_3GPP_ACCESS 0b010
#define _3GPP_AND_NON_3GPP_ACCESS 0b011
#define SMS_OVER_NAS_NOT_ALLOWED 0
#define SMS_OVER_NAS_ALLOWED 1

typedef struct {
  bool is_SMS_allowed;
  uint8_t registration_result_value : 3;
} _5GSRegistrationResult;

int encode__5gs_registration_result(
    _5GSRegistrationResult _5gsregistrationresult, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode__5gs_registration_result(
    _5GSRegistrationResult* _5gsregistrationresult, uint8_t iei,
    uint8_t* buffer, uint32_t len);

#endif
