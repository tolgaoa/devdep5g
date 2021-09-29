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

#ifndef REGISTRATION_ACCEPT_H_
#define REGISTRATION_ACCEPT_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "_5GSRegistrationResult.h"
#include "_5GSMobileIdentity.h"
#include "PLMNList.h"
#include "_5GSTrackingAreaIdentityList.h"
#include "NSSAI.h"
#include "RejectedNSSAI.h"
#include "_5GSNetworkFeatureSupport.h"
#include "PDUSessionStatus.h"
#include "PDUSessionReactivationResult.h"
#include "PDUSessionReactivationResultErrorCause.h"
#include "LADNInformation.h"
#include "MICOIndication.h"
#include "NetworkSlicingIndication.h"
#include "ServiceAreaList.h"
#include "GPRSTimer3.h"
#include "GPRSTimer2.h"
#include "EmergencyNumberList.h"
#include "ExtendedEmergencyNumberList.h"
#include "SORTransparentContainer.h"
#include "EAPMessage.h"
#include "NSSAIInclusionMode.h"
#include "OperatorDefinedAccessCategoryDefinitions.h"
#include "_5GSDRXParameters.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define REGISTRATION_ACCEPT_MINIMUM_LENGTH                                     \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MINIMUM_LENGTH + MESSAGE_TYPE_MINIMUM_LENGTH +         \
   _5GS_REGISTRATION_RESULT_MINIMUM_LENGTH +                                   \
   _5GS_MOBILE_IDENTITY_MINIMUM_LENGTH + PLMN_LIST_MINIMUM_LENGTH +            \
   _5GS_TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH + NSSAI_MINIMUM_LENGTH +    \
   REJECTED_NSSAI_MINIMUM_LENGTH +                                             \
   _5GS_NETWORK_FEATURE_SUPPORT_MINIMUM_LENGTH +                               \
   PDU_SESSION_STATUS_MINIMUM_LENGTH +                                         \
   PDU_SESSION_REACTIVATION_RESULT_MINIMUM_LENGTH +                            \
   PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_MINIMUM_LENGTH +                \
   LADN_INFORMATION_MINIMUM_LENGTH + MICO_INDICATION_MINIMUM_LENGTH +          \
   NETWORK_SLICING_INDICATION_MINIMUM_LENGTH +                                 \
   SERVICE_AREA_LIST_MINIMUM_LENGTH + GPRS_TIMER3_MINIMUM_LENGTH +             \
   GPRS_TIMER2_MINIMUM_LENGTH + EMERGENCY_NUMBER_LIST_MINIMUM_LENGTH +         \
   EXTENDED_EMERGENCY_NUMBER_LIST_MINIMUM_LENGTH +                             \
   SOR_TRANSPARENT_CONTAINER_MINIMUM_LENGTH + EAP_MESSAGE_MINIMUM_LENGTH +     \
   NSSAI_INCLUSION_MODE_MINIMUM_LENGTH +                                       \
   OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_MINIMUM_LENGTH +               \
   _5GSDRX_PARAMETERS_MINIMUM_LENGTH + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define REGISTRATION_ACCEPT_MAXIMUM_LENGTH                                     \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + MESSAGE_TYPE_MAXIMUM_LENGTH +         \
   _5GS_REGISTRATION_RESULT_MAXIMUM_LENGTH +                                   \
   _5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH + PLMN_LIST_MAXIMUM_LENGTH +            \
   _5GS_TRACKING_AREA_IDENTITY_LIST_MAXIMUM_LENGTH + NSSAI_MAXIMUM_LENGTH +    \
   REJECTED_NSSAI_MAXIMUM_LENGTH +                                             \
   _5GS_NETWORK_FEATURE_SUPPORT_MAXIMUM_LENGTH +                               \
   PDU_SESSION_STATUS_MAXIMUM_LENGTH +                                         \
   PDU_SESSION_REACTIVATION_RESULT_MAXIMUM_LENGTH +                            \
   PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_MAXIMUM_LENGTH +                \
   LADN_INFORMATION_MAXIMUM_LENGTH + MICO_INDICATION_MAXIMUM_LENGTH +          \
   NETWORK_SLICING_INDICATION_MAXIMUM_LENGTH +                                 \
   SERVICE_AREA_LIST_MAXIMUM_LENGTH + GPRS_TIMER3_MAXIMUM_LENGTH +             \
   GPRS_TIMER2_MAXIMUM_LENGTH + EMERGENCY_NUMBER_LIST_MAXIMUM_LENGTH +         \
   EXTENDED_EMERGENCY_NUMBER_LIST_MAXIMUM_LENGTH +                             \
   SOR_TRANSPARENT_CONTAINER_MAXIMUM_LENGTH + EAP_MESSAGE_MAXIMUM_LENGTH +     \
   NSSAI_INCLUSION_MODE_MAXIMUM_LENGTH +                                       \
   OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_MAXIMUM_LENGTH +               \
   _5GSDRX_PARAMETERS_MAXIMUM_LENGTH + 0)

#define REGISTRATION_ACCEPT_5GS_MOBILE_IDENTITY_IEI 0x77
#define REGISTRATION_ACCEPT_5GS_MOBILE_IDENTITY_PRESENT (uint32_t)(1 << 0)

#define REGISTRATION_ACCEPT_PLMN_LIST_IEI 0x4A
#define REGISTRATION_ACCEPT_PLMN_LIST_PRESENT (uint32_t)(1 << 1)

#define REGISTRATION_ACCEPT_5GS_TRACKING_AREA_IDENTITY_LIST_IEI 0x54
#define REGISTRATION_ACCEPT_5GS_TRACKING_AREA_IDENTITY_LIST_PRESENT            \
  (uint32_t)(1 << 2)

#define REGISTRATION_ACCEPT_ALLOWED_NSSAI_IEI 0x15
#define REGISTRATION_ACCEPT_ALLOWED_NSSAI_PRESENT (uint32_t)(1 << 3)

#define REGISTRATION_ACCEPT_REJECTED_NSSAI_IEI 0x11
#define REGISTRATION_ACCEPT_REJECTED_NSSAI_PRESENT (uint32_t)(1 << 4)

#define REGISTRATION_ACCEPT_CONFIGURED_NSSAI_IEI 0x31
#define REGISTRATION_ACCEPT_CONFIGURED_NSSAI_PRESENT (uint32_t)(1 << 5)

#define REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_IEI 0x21
#define REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_PRESENT                \
  (uint32_t)(1 << 6)

#define REGISTRATION_ACCEPT_PDU_SESSION_STATUS_IEI 0x50
#define REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT (uint32_t)(1 << 7)

#define REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_IEI 0x26
#define REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT            \
  (uint32_t)(1 << 8)

#define REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_IEI 0x72
#define REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT \
  (uint32_t)(1 << 9)

#define REGISTRATION_ACCEPT_LADN_INFORMATION_IEI 0x79
#define REGISTRATION_ACCEPT_LADN_INFORMATION_PRESENT (uint32_t)(1 << 10)

#define REGISTRATION_ACCEPT_MICO_INDICATION_IEI 0xB0
#define REGISTRATION_ACCEPT_MICO_INDICATION_PRESENT (uint32_t)(1 << 11)

#define REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_IEI 0x90
#define REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_PRESENT                 \
  (uint32_t)(1 << 12)

#define REGISTRATION_ACCEPT_SERVICE_AREA_LIST_IEI 0x27
#define REGISTRATION_ACCEPT_SERVICE_AREA_LIST_PRESENT (uint32_t)(1 << 13)

#define REGISTRATION_ACCEPT_GPRS_TIMER3_T3512_VALUE_IEI 0x5E
#define REGISTRATION_ACCEPT_GPRS_TIMER3_T3512_VALUE_PRESENT (uint32_t)(1 << 14)

#define REGISTRATION_ACCEPT_GPRS_TIMER2_NON_3GPP_DEREGISTRATION_TIMER_VALUE_IEI \
  0x5D
#define REGISTRATION_ACCEPT_GPRS_TIMER2_NON_3GPP_DEREGISTRATION_TIMER_VALUE_PRESENT \
  (uint32_t)(1 << 15)

#define REGISTRATION_ACCEPT_GPRS_TIMER2_T3502_VALUE_IEI 0x16
#define REGISTRATION_ACCEPT_GPRS_TIMER2_T3502_VALUE_PRESENT (uint32_t)(1 << 16)

#define REGISTRATION_ACCEPT_EMERGENCY_NUMBER_LIST_IEI 0x34
#define REGISTRATION_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT (uint32_t)(1 << 17)

#define REGISTRATION_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_IEI 0x7A
#define REGISTRATION_ACCEPT_EXTENDED_EMERGENCY_NUMBER_LIST_PRESENT             \
  (uint32_t)(1 << 18)

#define REGISTRATION_ACCEPT_SOR_TRANSPARENT_CONTAINER_IEI 0x73
#define REGISTRATION_ACCEPT_SOR_TRANSPARENT_CONTAINER_PRESENT                  \
  (uint32_t)(1 << 19)

#define REGISTRATION_ACCEPT_EAP_MESSAGE_IEI 0x78
#define REGISTRATION_ACCEPT_EAP_MESSAGE_PRESENT (uint32_t)(1 << 20)

#define REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_IEI 0xA0
#define REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_PRESENT (uint32_t)(1 << 21)

#define REGISTRATION_ACCEPT_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_IEI   \
  0x76
#define REGISTRATION_ACCEPT_OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_PRESENT \
  (uint32_t)(1 << 22)

#define REGISTRATION_ACCEPT_5GS_DRX_PARAMETERS_IEI 0x51
#define REGISTRATION_ACCEPT_5GS_DRX_PARAMETERS_PRESENT (uint32_t)(1 << 23)

typedef struct registration_accept_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  SecurityHeaderType securityheadertype;
  MessageType messagetype;
  _5GSRegistrationResult _5gsregistrationresult;
  uint32_t presence;
  //_5GSMobileIdentity _5gsmobileidentity;
  PLMNList plmnlist;
  _5GSTrackingAreaIdentityList _5gstrackingareaidentitylist;
  // NSSAI allowednssai;
  // RejectedNSSAI rejectednssai;
  // NSSAI configurednssai;
  _5GSNetworkFeatureSupport _5gsnetworkfeaturesupport;
  PDUSessionStatus pdusessionstatus;
  PDUSessionReactivationResult pdusessionreactivationresult;
  PDUSessionReactivationResultErrorCause pdusessionreactivationresulterrorcause;
  // LADNInformation ladninformation;
  MICOIndication micoindication;
  NetworkSlicingIndication networkslicingindication;
  ServiceAreaList servicearealist;
  GPRSTimer3 t3512;
  GPRSTimer2 non_3gpp_deregistration_timer;
  GPRSTimer2 t3502;
  // EmergencyNumberList emergencynumberlist;
  // ExtendedEmergencyNumberList extendedemergencynumberlist;
  // SORTransparentContainer sortransparentcontainer;
  EAPMessage eapmessage;
  NSSAIInclusionMode nssaiinclusionmode;
  // OperatorDefinedAccessCategoryDefinitions
  // operatordefinedaccesscategorydefinitions;
  _5GSDRXParameters _5gsdrxparameters;
  // Non_3GPP_NW_Provided_Policies non_3gpp_nw_policies;
} registration_accept_msg;

int decode_registration_accept(
    registration_accept_msg* registrationaccept, uint8_t* buffer, uint32_t len);
int encode_registration_accept(
    registration_accept_msg* registrationaccept, uint8_t* buffer, uint32_t len);

#endif
