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

#ifndef _ALLOWEDSSCMODE_H_
#define _ALLOWEDSSCMODE_H_

#include <stdint.h>
#include <stdbool.h>
#include "bstrlib.h"

#define ALLOWED_SSC_MODE_MINIMUM_LENGTH 1
#define ALLOWED_SSC_MODE_MAXIMUM_LENGTH 1

#define SSC_MODE1_NOT_ALLOWED 0
#define SSC_MODE1_ALLOWED 1
#define SSC_MODE2_NOT_ALLOWED 0
#define SSC_MODE2_ALLOWED 1
#define SSC_MODE3_NOT_ALLOWED 0
#define SSC_MODE3_ALLOWED 1

// typedef bstring AllowedSSCMode;
typedef struct {
  bool is_ssc1_allowed;
  bool is_ssc2_allowed;
  bool is_ssc3_allowed;
} AllowedSSCMode;

int encode_allowed_ssc_mode(
    AllowedSSCMode allowedsscmode, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_allowed_ssc_mode(
    AllowedSSCMode* allowedsscmode, uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
