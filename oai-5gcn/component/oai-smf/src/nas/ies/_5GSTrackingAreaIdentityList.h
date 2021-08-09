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

#ifndef _5GS_TRACKING_AREA_IDENTITY_LIST_H_
#define _5GS_TRACKING_AREA_IDENTITY_LIST_H_

#include <stdint.h>
#include "bstrlib.h"

#define _5GS_TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH 9
#define _5GS_TRACKING_AREA_IDENTITY_LIST_MAXIMUM_LENGTH 114

#define LIST_OF_TACS_BELONGING_TO_ONE_PLMN_WITH_NON_CONSECUTIVE_TAC_VALUES 0x00
#define LIST_OF_TACS_BELONGING_TO_ONE_PLMN_WITH_CONSECUTIVE_TAC_VALUES 0x01
#define LIST_OF_TAIS_BELONGING_TO_DIFFERENT_PLMNS 0x02

struct MccMnc {
  uint16_t mcc;
  uint16_t mnc;
  struct MccMnc* next;
};

struct TrackingAreaIdentity {
  uint8_t tac;
  uint16_t tacContinued;
  struct TrackingAreaIdentity* next;
};

struct PartialTrackingAreaIdentityList {
  uint8_t typeOfList;
  uint8_t numberOfElements;
  struct MccMnc* mcc_mnc;
  struct TrackingAreaIdentity* tai;
  struct PartialTrackingAreaIdentityList* next;
};

typedef struct {
  int listSize;
  struct PartialTrackingAreaIdentityList* partialTrackingAreaIdentityList;
} _5GSTrackingAreaIdentityList;

int encode__5gs_tracking_area_identity_list(
    _5GSTrackingAreaIdentityList _5gstrackingareaidentitylist, uint8_t iei,
    uint8_t* buffer, uint32_t len);
int decode__5gs_tracking_area_identity_list(
    _5GSTrackingAreaIdentityList* _5gstrackingareaidentitylist, uint8_t iei,
    uint8_t* buffer, uint32_t len);

#endif
