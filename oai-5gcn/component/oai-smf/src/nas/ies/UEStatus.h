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
#include <stdbool.h>

#define UE_STATUS_MINIMUM_LENGTH 3
#define UE_STATUS_MAXIMUM_LENGTH 3

#define UE_NOT_IN_EMM_REGISTERED 0
#define UE_IN_EMM_REGISTERED 1
#define UE_NOT_IN_5GMM_REGISTERED 0
#define UE_IN_5GMM_REGISTERED 1

typedef struct {
  bool n1_mode_reg;
  bool s1_mode_reg;
} UEStatus;

int encode_ue_status(
    UEStatus uestatus, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_ue_status(
    UEStatus* uestatus, uint8_t iei, uint8_t* buffer, uint32_t len);
