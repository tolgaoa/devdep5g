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

#ifndef __5GSDRX_PARAMETERS_H_
#define __5GSDRX_PARAMETERS_H_

#include <stdint.h>
#include "bstrlib.h"

#define _5GSDRX_PARAMETERS_MINIMUM_LENGTH 3
#define _5GSDRX_PARAMETERS_MAXIMUM_LENGTH 3

#define DRX_VALUE_NOT_SPECIFIED 0b0000
#define DRX_CYCLE_PARAMETER_T_32 0b0001
#define DRX_CYCLE_PARAMETER_T_64 0b0010
#define DRX_CYCLE_PARAMETER_T_128 0b0011
#define DRX_CYCLE_PARAMETER_T_256 0b0100

typedef uint8_t _5GSDRXParameters;

int encode__5gsdrx_parameters(
    _5GSDRXParameters _5gsdrxparameters, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode__5gsdrx_parameters(
    _5GSDRXParameters* _5gsdrxparameters, uint8_t iei, uint8_t* buffer,
    uint32_t len);

#endif
