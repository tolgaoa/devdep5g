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

#ifndef SECURITY_MODE_COMMAND_H_
#define SECURITY_MODE_COMMAND_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "NASSecurityAlgorithms.h"
#include "NASKeySetIdentifier.h"
#include "UESecurityCapability.h"
#include "IMEISVRequest.h"
#include "EPSNASSecurityAlgorithms.h"
#include "Additional5GSecurityInformation.h"
#include "EAPMessage.h"
#include "ABBA.h"
#include "S1UESecurityCapability.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define SECURITY_MODE_COMMAND_MINIMUM_LENGTH                                   \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MINIMUM_LENGTH + MESSAGE_TYPE_MINIMUM_LENGTH +         \
   NAS_SECURITY_ALGORITHMS_MINIMUM_LENGTH +                                    \
   NAS_KEY_SET_IDENTIFIER_MINIMUM_LENGTH +                                     \
   UE_SECURITY_CAPABILITY_MINIMUM_LENGTH + IMEISV_REQUEST_MINIMUM_LENGTH +     \
   EPSNAS_SECURITY_ALGORITHMS_MINIMUM_LENGTH +                                 \
   ADDITIONAL_5G_SECURITY_INFORMATION_MINIMUM_LENGTH +                         \
   EAP_MESSAGE_MINIMUM_LENGTH + ABBA_MINIMUM_LENGTH +                          \
   S1_UE_SECURITY_CAPABILITY_MINIMUM_LENGTH + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define SECURITY_MODE_COMMAND_MAXIMUM_LENGTH                                   \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + MESSAGE_TYPE_MAXIMUM_LENGTH +         \
   NAS_SECURITY_ALGORITHMS_MAXIMUM_LENGTH +                                    \
   NAS_KEY_SET_IDENTIFIER_MAXIMUM_LENGTH +                                     \
   UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH + IMEISV_REQUEST_MAXIMUM_LENGTH +     \
   EPSNAS_SECURITY_ALGORITHMS_MAXIMUM_LENGTH +                                 \
   ADDITIONAL_5G_SECURITY_INFORMATION_MAXIMUM_LENGTH +                         \
   EAP_MESSAGE_MAXIMUM_LENGTH + ABBA_MAXIMUM_LENGTH +                          \
   S1_UE_SECURITY_CAPABILITY_MAXIMUM_LENGTH + 0)

#define SECURITY_MODE_COMMAND_IMEISV_REQUEST_IEI 0xE0
#define SECURITY_MODE_COMMAND_IMEISV_REQUEST_PRESENT (1 << 0)
#define SECURITY_MODE_COMMAND_EPS_NAS_SECURITY_ALGORITHMS_IEI 0x57
#define SECURITY_MODE_COMMAND_EPS_NAS_SECURITY_ALGORITHMS_PRESENT (1 << 1)
#define SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_IEI 0x36
#define SECURITY_MODE_COMMAND_ADDITIONAL_5G_SECURITY_INFORMATION_PRESENT       \
  (1 << 2)
#define SECURITY_MODE_COMMAND_EAP_MESSAGE_IEI 0x78
#define SECURITY_MODE_COMMAND_EAP_MESSAGE_PRESENT (1 << 3)
#define SECURITY_MODE_COMMAND_ABBA_IEI 0x38
#define SECURITY_MODE_COMMAND_ABBA_PRESENT (1 << 4)
#define SECURITY_MODE_COMMAND_S1_UE_SECURITY_CAPABILITY_IEI 0x19
#define SECURITY_MODE_COMMAND_S1_UE_SECURITY_CAPABILITY_PRESENT (1 << 5)

typedef struct security_mode_command_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  SecurityHeaderType securityheadertype;
  MessageType messagetype;
  NASSecurityAlgorithms nassecurityalgorithms;
  NASKeySetIdentifier naskeysetidentifier;
  UESecurityCapability uesecuritycapability;
  uint8_t presence;
  IMEISVRequest imeisvrequest;
  EPSNASSecurityAlgorithms epsnassecurityalgorithms;
  Additional5GSecurityInformation additional5gsecurityinformation;
  EAPMessage eapmessage;
  ABBA abba;
  // S1UESecurityCapability s1uesecuritycapability;
} security_mode_command_msg;

int decode_security_mode_command(
    security_mode_command_msg* securitymodecommand, uint8_t* buffer,
    uint32_t len);
int encode_security_mode_command(
    security_mode_command_msg* securitymodecommand, uint8_t* buffer,
    uint32_t len);

#endif
