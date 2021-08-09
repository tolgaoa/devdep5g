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

#ifndef NAS_SECURITY_ALGORITHMS_H_
#define NAS_SECURITY_ALGORITHMS_H_

#include <stdint.h>
#include "bstrlib.h"

#define NAS_SECURITY_ALGORITHMS_MINIMUM_LENGTH 2
#define NAS_SECURITY_ALGORITHMS_MAXIMUM_LENGTH 2

#define NAS_SECURITY_ALGORITHMS_NEA0 0b0000
#define NAS_SECURITY_ALGORITHMS_NEA1 0b0001
#define NAS_SECURITY_ALGORITHMS_NEA2 0b0010
#define NAS_SECURITY_ALGORITHMS_NEA3 0b0011
#define NAS_SECURITY_ALGORITHMS_NEA4 0b0100
#define NAS_SECURITY_ALGORITHMS_NEA5 0b0101
#define NAS_SECURITY_ALGORITHMS_NEA6 0b0110
#define NAS_SECURITY_ALGORITHMS_NEA7 0b0111

#define NAS_SECURITY_ALGORITHMS_NIA0 0b0000
#define NAS_SECURITY_ALGORITHMS_NIA1 0b0001
#define NAS_SECURITY_ALGORITHMS_NIA2 0b0010
#define NAS_SECURITY_ALGORITHMS_NIA3 0b0011
#define NAS_SECURITY_ALGORITHMS_NIA4 0b0100
#define NAS_SECURITY_ALGORITHMS_NIA5 0b0101
#define NAS_SECURITY_ALGORITHMS_NIA6 0b0110
#define NAS_SECURITY_ALGORITHMS_NIA7 0b0111

typedef struct {
  uint8_t typeOfCipheringAlgorithm : 4;
  uint8_t typeOfIntegrityProtectionAlgorithm : 4;
} NASSecurityAlgorithms;

int encode_nas_security_algorithms(
    NASSecurityAlgorithms nassecurityalgorithms, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode_nas_security_algorithms(
    NASSecurityAlgorithms* nassecurityalgorithms, uint8_t iei, uint8_t* buffer,
    uint32_t len);

#endif
