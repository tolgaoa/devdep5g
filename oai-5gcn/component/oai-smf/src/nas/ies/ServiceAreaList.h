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

#ifndef _SERVICE_AREA_LIST_H_
#define _SERVICE_AREA_LIST_H_

#include <stdint.h>
#include "bstrlib.h"
#include "_5GSTrackingAreaIdentityList.h"
#include <stdbool.h>

#define SERVICE_AREA_LIST_MINIMUM_LENGTH 6
#define SERVICE_AREA_LIST_MAXIMUM_LENGTH 114

#define ALL_TAIS_BELONGING_TO_THE_PLMNS_IN_THE_REGISTRATION_AREA_ARE_IN_THE_ALLOWED_AREA \
  0b11

struct PartialServiceAreaList {
  bool is_allowed;
  uint8_t typeOfList;
  uint8_t numberOfElements;
  struct MccMnc* mcc_mnc;
  struct TrackingAreaIdentity* tai;
  struct PartialServiceAreaList* next;
};

typedef struct {
  int listSize;
  struct PartialServiceAreaList* partialServiceAreaList;
} ServiceAreaList;

int encode_service_area_list(
    ServiceAreaList servicearealist, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode_service_area_list(
    ServiceAreaList* servicearealist, uint8_t iei, uint8_t* buffer,
    uint32_t len);

#endif
