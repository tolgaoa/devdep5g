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

#ifndef NSSAI_INCLUSION_MODE_H_
#define NSSAI_INCLUSION_MODE_H_

#include <stdint.h>
#include "bstrlib.h"

#define NSSAI_INCLUSION_MODE_MINIMUM_LENGTH 1
#define NSSAI_INCLUSION_MODE_MAXIMUM_LENGTH 1

#define NSSAI_INCLUSION_MODE_A 0b00
#define NSSAI_INCLUSION_MODE_B 0b01
#define NSSAI_INCLUSION_MODE_C 0b10
#define NSSAI_INCLUSION_MODE_D 0b11

typedef uint8_t NSSAIInclusionMode;

int encode_nssai_inclusion_mode(
    NSSAIInclusionMode nssaiinclusionmode, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode_nssai_inclusion_mode(
    NSSAIInclusionMode* nssaiinclusionmode, uint8_t iei, uint8_t* buffer,
    uint32_t len);

#endif
