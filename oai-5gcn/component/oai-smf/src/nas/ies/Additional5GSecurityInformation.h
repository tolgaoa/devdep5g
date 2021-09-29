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

#ifndef _ADDITIONAL_5G_SECURITY_INFORMATION_H_
#define _ADDITIONAL_5G_SECURITY_INFORMATION_H_

#include <stdint.h>
#include "bstrlib.h"
#include <stdbool.h>

#define ADDITIONAL_5G_SECURITY_INFORMATION_MINIMUM_LENGTH 3
#define ADDITIONAL_5G_SECURITY_INFORMATION_MAXIMUM_LENGTH 3

#define KAMF_DERIVATION_IS_NOT_REQUIRED 0
#define KAMF_DERIVATION_IS_REQUIRED 1

#define RETRANSMISSION_OF_THE_INITIAL_NAS_MESSAGE_NOT_REQUESTED 0
#define RETRANSMISSION_OF_THE_INITIAL_NAS_MESSAGE_REQUESTED 1

typedef struct {
  bool hdp;
  bool rinmr;
} Additional5GSecurityInformation;

int encode_additional_5g_security_information(
    Additional5GSecurityInformation additional5gsecurityinformation,
    uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_additional_5g_security_information(
    Additional5GSecurityInformation* additional5gsecurityinformation,
    uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
