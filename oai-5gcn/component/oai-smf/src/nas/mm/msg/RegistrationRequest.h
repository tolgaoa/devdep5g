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

#ifndef REGISTRATION_REQUEST_H_
#define REGISTRATION_REQUEST_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "_5GSRegistrationType.h"
#include "NASKeySetIdentifier.h"
#include "_5GSMobileIdentity.h"
#include "_5GMMCapability.h"
#include "UESecurityCapability.h"
#include "NSSAI.h"
#include "_5GSTrackingAreaIdentity.h"
#include "S1UENetworkCapability.h"
#include "UplinkDataStatus.h"
#include "PDUSessionStatus.h"
#include "MICOIndication.h"
#include "UEStatus.h"
#include "AllowedPDUSessionStatus.h"
#include "UESUsageSetting.h"
#include "_5GSDRXParameters.h"
#include "EPSNASMessageContainer.h"
#include "LADNIndication.h"
#include "PayloadContainerType.h"
#include "PayloadContainer.h"
#include "NetworkSlicingIndication.h"
#include "_5GSUpdateType.h"
#include "NASMessageContainer.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define REGISTRATION_REQUEST_MINIMUM_LENGTH                                    \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MINIMUM_LENGTH + MESSAGE_TYPE_MINIMUM_LENGTH +         \
   _5GS_REGISTRATION_TYPE_MINIMUM_LENGTH +                                     \
   NAS_KEY_SET_IDENTIFIER_MINIMUM_LENGTH +                                     \
   _5GS_MOBILE_IDENTITY_MINIMUM_LENGTH + _5GMM_CAPABILITY_MINIMUM_LENGTH +     \
   UE_SECURITY_CAPABILITY_MINIMUM_LENGTH + NSSAI_MINIMUM_LENGTH +              \
   _5GS_TRACKING_AREA_IDENTITY_MINIMUM_LENGTH +                                \
   S1_UE_NETWORK_CAPABILITY_MINIMUM_LENGTH +                                   \
   UPLINK_DATA_STATUS_MINIMUM_LENGTH + PDU_SESSION_STATUS_MINIMUM_LENGTH +     \
   MICO_INDICATION_MINIMUM_LENGTH + UE_STATUS_MINIMUM_LENGTH +                 \
   ALLOWED_PDU_SESSION_STATUS_MINIMUM_LENGTH +                                 \
   UES_USAGE_SETTING_MINIMUM_LENGTH + _5GSDRX_PARAMETERS_MINIMUM_LENGTH +      \
   EPSNAS_MESSAGE_CONTAINER_MINIMUM_LENGTH + LADN_INDICATION_MINIMUM_LENGTH +  \
   PAYLOAD_CONTAINER_MINIMUM_LENGTH +                                          \
   NETWORK_SLICING_INDICATION_MINIMUM_LENGTH +                                 \
   _5GS_UPDATE_TYPE_MINIMUM_LENGTH + NAS_MESSAGE_CONTAINER_MINIMUM_LENGTH + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define REGISTRATION_REQUEST_MAXIMUM_LENGTH                                    \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + MESSAGE_TYPE_MAXIMUM_LENGTH +         \
   _5GS_REGISTRATION_TYPE_MAXIMUM_LENGTH +                                     \
   NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH +                                     \
   _5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH + _5GMM_CAPABILITY_MAXIMUM_LENGTH +     \
   UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH + NSSAI_MAXIMUM_LENGTH +              \
   _5GS_TRACKING_AREA_IDENTITY_MAXIMUM_LENGTH +                                \
   S1_UE_NETWORK_CAPABILITY_MAXIMUM_LENGTH +                                   \
   UPLINK_DATA_STATUS_MAXIMUM_LENGTH + PDU_SESSION_STATUS_MAXIMUM_LENGTH +     \
   MICO_INDICATION_MAXIMUM_LENGTH + UE_STATUS_MAXIMUM_LENGTH +                 \
   ALLOWED_PDU_SESSION_STATUS_MAXIMUM_LENGTH +                                 \
   UES_USAGE_SETTING_MAXIMUM_LENGTH + _5GSDRX_PARAMETERS_MAXIMUM_LENGTH +      \
   EPSNAS_MESSAGE_CONTAINER_MAXIMUM_LENGTH + LADN_INDICATION_MAXIMUM_LENGTH +  \
   PAYLOAD_CONTAINER_MAXIMUM_LENGTH +                                          \
   NETWORK_SLICING_INDICATION_MAXIMUM_LENGTH +                                 \
   _5GS_UPDATE_TYPE_MAXIMUM_LENGTH + NAS_MESSAGE_CONTAINER_MAXIMUM_LENGTH + 0)

#define REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_IEI 0xC0
#define REGISTRATION_REQUEST_5GMM_CAPABILITY_IEI 0x10
#define REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_IEI 0x2E
#define REGISTRATION_REQUEST_NSSAI_IEI 0x2F
#define REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_IEI 0x52
#define REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_IEI 0x17
#define REGISTRATION_REQUEST_UPLINK_DATA_STATUS_IEI 0x40
#define REGISTRATION_REQUEST_PDU_SESSION_STATUS_IEI 0x50
#define REGISTRATION_REQUEST_MICO_IDICATION_IEI 0xB0
#define REGISTRATION_REQUEST_UE_STATUS_IEI 0x2B
#define REGISTRATION_REQUEST_5GS_MOBILE_IDENTITY_IEI 0x77
#define REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_IEI 0x25
#define REGISTRATION_REQUEST_UE_USAGE_SETTING_IEI 0x18
#define REGISTRATION_REQUEST_5GS_DRX_PARAMETERS_IEI 0x51
#define REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_IEI 0x70
#define REGISTRATION_REQUEST_LADN_INDICATION_IEI 0x74
#define REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_IEI 0x80
#define REGISTRATION_REQUEST_PAYLOAD_CONTAINER_IEI 0x7B
#define REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_IEI 0x90
#define REGISTRATION_REQUEST_5GS_UPDATE_TYPE_IEI 0x53
#define REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_IEI 0x71

#define REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT (1 << 0)
#define REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT (1 << 1)
#define REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT (1 << 2)
#define REGISTRATION_REQUEST_NSSAI_PRESENT (1 << 3)
#define REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_PRESENT (1 << 4)
#define REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT (1 << 5)
#define REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT (1 << 6)
#define REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT (1 << 8)
#define REGISTRATION_REQUEST_MICO_IDICATION_PRESENT (1 << 9)
#define REGISTRATION_REQUEST_UE_STATUS_PRESENT (1 << 10)
#define REGISTRATION_REQUEST_5GS_MOBILE_IDENTITY_PRESENT (1 << 11)
#define REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT (1 << 12)
#define REGISTRATION_REQUEST_UE_USAGE_SETTING_PRESENT (1 << 13)
#define REGISTRATION_REQUEST_5GS_DRX_PARAMETERS_PRESENT (1 << 14)
#define REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_PRESENT (1 << 15)
#define REGISTRATION_REQUEST_LADN_INDICATION_PRESENT (1 << 16)
#define REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_PRESENT (1 << 17)
#define REGISTRATION_REQUEST_PAYLOAD_CONTAINER_PRESENT (1 << 18)
#define REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_PRESENT (1 << 19)
#define REGISTRATION_REQUEST_5GS_UPDATE_TYPE_PRESENT (1 << 20)
#define REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT (1 << 21)

typedef struct registration_request_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  SecurityHeaderType securityheadertype;
  MessageType messagetype;
  _5GSRegistrationType _5gsregistrationtype;
  NASKeySetIdentifier naskeysetidentifier;
  //_5GSMobileIdentity _5gsmobileidentity;
  uint32_t presence;
  NASKeySetIdentifier non_current_native_nas_key_set_identifier;
  _5GMMCapability _5gmmcapability;
  UESecurityCapability uesecuritycapability;
  // NSSAI nssai;
  _5GSTrackingAreaIdentity _5gstrackingareaidentity;
  S1UENetworkCapability s1uenetworkcapability;
  UplinkDataStatus uplinkdatastatus;
  PDUSessionStatus pdusessionstatus;
  MICOIndication micoindication;
  UEStatus uestatus;
  //_5GSMobileIdentity AdditionalGUTI;
  AllowedPDUSessionStatus allowedpdusessionstatus;
  UESUsageSetting uesusagesetting;
  _5GSDRXParameters _5gsdrxparameters;
  EPSNASMessageContainer epsnasmessagecontainer;
  // LADNIndication ladnindication;
  PayloadContainerType payloadcontainertype;
  PayloadContainer payloadcontainer;
  NetworkSlicingIndication networkslicingindication;
  _5GSUpdateType _5gsupdatetype;
  NASMessageContainer nasmessagecontainer;
} registration_request_msg;

int decode_registration_request(
    registration_request_msg* registrationrequest, uint8_t* buffer,
    uint32_t len);
int encode_registration_request(
    registration_request_msg* registrationrequest, uint8_t* buffer,
    uint32_t len);

#endif
