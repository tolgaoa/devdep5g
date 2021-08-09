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

#ifndef _5GS_NETWORK_FEATURE_SUPPORT_H_
#define _5GS_NETWORK_FEATURE_SUPPORT_H_

#include <stdint.h>
#include "bstrlib.h"

#include <stdbool.h>

#define _5GS_NETWORK_FEATURE_SUPPORT_MINIMUM_LENGTH 3
#define _5GS_NETWORK_FEATURE_SUPPORT_MAXIMUM_LENGTH 5

#define IMS_VOICE_OVER_PS_SESSION_NOT_SUPPORTED_OVER_3GPP_ACCESS 0
#define IMS_VOICE_OVER_PS_SESSION_SUPPORTED_OVER_3GPP_ACCESS 1
#define IMS_VOICE_OVER_PS_SESSION_SUPPORTED_OVER_NON_3GPP_ACCESS 1
#define IMS_VOICE_OVER_PS_SESSION_NOT_SUPPORTED_OVER_NON_3GPP_ACCESS 0
#define EMERGENCY_SERVICES_NOT_SUPPORTED 0b00
#define EMERGENCY_SERVICES_SUPPORTED_IN_NR_CONNECTED_TO_5GCN_ONLY 0b01
#define EMERGENCY_SERVICES_SUPPORTED_IN_EUTRA_CONNECTED_TO_5GCN_ONLY 0b10
#define EMERGENCY_SERVICES_SUPPORTED_IN_NR_CONNECTED_TO_5GCN_AND_EUTRA_CONNECTED_TO_5GCN \
  0b11
#define EMERGENCY_SERVICES_FALLBACK_NOT_SUPPORTED 0b00
#define EMERGENCY_SERVICES_FALLBACK_SUPPORTED_IN_NR_CONNECTED_TO_5GCN_ONLY 0b01
#define EMERGENCY_SERVICES_FALLBACK_SUPPORTED_IN_EUTRA_CONNECTED_TO_5GCN_ONLY  \
  0b10
#define EMERGENCY_SERVICES_FALLBACK_SUPPORTED_IN_NR_CONNECTED_TO_5GCN_AND_EUTRA_CONNECTED_TO_5GCN \
  0b11
#define INTERWORKING_WITHOUT_N26_INTERFACE_NOT_SUPPORTED 0
#define INTERWORKING_WITHOUT_N26_INTERFACE_SUPPORTED 1
#define ACCESS_IDENTITY_1_NOT_VALID_IN_RPLMN_OR_EQUIVALENT_PLMN 0
#define ACCESS_IDENTITY_1_VALID_IN_RPLMN_OR_EQUIVALENT_PLMN 1
#define EMERGENCY_SERVICES_NOT_SUPPORTED_OVER_NON_3GPP_ACCESS 0
#define EMERGENCY_SERVICES_SUPPORTED_OVER_NON_3GPP_ACCESS 0
#define ACCESS_IDENTITY_2_NOT_VALID_IN_RPLMN_OR_EQUIVALENT_PLMN 0
#define ACCESS_IDENTITY_2_VALID_IN_RPLMN_OR_EQUIVALENT_PLMN 1

typedef struct {
  uint8_t mpsi : 1;
  uint8_t iwk_n26 : 1;
  uint8_t emf : 2;
  uint8_t emc : 2;
  uint8_t ims_VoPS_N3GPP : 1;
  uint8_t ims_VoPS_3GPP : 1;
  uint8_t mcsi : 1;
  uint8_t emcn : 1;
} _5GSNetworkFeatureSupport;

int encode__5gs_network_feature_support(
    _5GSNetworkFeatureSupport _5gsnetworkfeaturesupport, uint8_t iei,
    uint8_t* buffer, uint32_t len);
int decode__5gs_network_feature_support(
    _5GSNetworkFeatureSupport* _5gsnetworkfeaturesupport, uint8_t iei,
    uint8_t* buffer, uint32_t len);

#endif
