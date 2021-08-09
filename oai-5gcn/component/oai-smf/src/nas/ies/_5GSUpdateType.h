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

#ifndef __5GS_UPDATE_TYPE_H_
#define __5GS_UPDATE_TYPE_H_

#include <stdint.h>
#include "bstrlib.h"

#define _5GS_UPDATE_TYPE_MINIMUM_LENGTH 3
#define _5GS_UPDATE_TYPE_MAXIMUM_LENGTH 3

#define SMS_OVER_NAS_NOT_SUPPORTED 0
#define SMS_OVER_NAS_SUPPORTED 1

#define NG_RAN_RADIO_CAPABILITY_UPDATE_NOT_NEEDED 0
#define NG_RAN_RADIO_CAPABILITY_UPDATE_NEEDED 1

typedef struct {
  uint8_t ng_ran_rcu;
  uint8_t sms_requested;
} _5GSUpdateType;

int encode__5gs_update_type(
    _5GSUpdateType _5gsupdatetype, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode__5gs_update_type(
    _5GSUpdateType* _5gsupdatetype, uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
