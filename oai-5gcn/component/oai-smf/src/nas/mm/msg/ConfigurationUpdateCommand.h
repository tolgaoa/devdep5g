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

#ifndef CONFIGURATION_UPDATE_COMMAND_H_
#define CONFIGURATION_UPDATE_COMMAND_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "ConfigurationUpdateIndication.h"
#include "_5GSMobileIdentity.h"
#include "_5GSTrackingAreaIdentityList.h"
#include "NSSAI.h"
#include "ServiceAreaList.h"
#include "NetworkName.h"
#include "TimeZone.h"
#include "TimeZoneAndTime.h"
#include "DaylightSavingTime.h"
#include "LADNInformation.h"
#include "MICOIndication.h"
#include "NetworkSlicingIndication.h"
#include "RejectedNSSAI.h"
#include "OperatorDefinedAccessCategoryDefinitions.h"
#include "SMSIndication.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define CONFIGURATION_UPDATE_COMMAND_MINIMUM_LENGTH                            \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MINIMUM_LENGTH + MESSAGE_TYPE_MINIMUM_LENGTH +         \
   CONFIGURATION_UPDATE_INDICATION_MINIMUM_LENGTH +                            \
   _5GS_MOBILE_IDENTITY_MINIMUM_LENGTH +                                       \
   _5GS_TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH + NSSAI_MINIMUM_LENGTH +    \
   SERVICE_AREA_LIST_MINIMUM_LENGTH + NETWORK_NAME_MINIMUM_LENGTH +            \
   TIME_ZONE_MINIMUM_LENGTH + TIME_ZONE_AND_TIME_MINIMUM_LENGTH +              \
   DAYLIGHT_SAVING_TIME_MINIMUM_LENGTH + LADN_INFORMATION_MINIMUM_LENGTH +     \
   MICO_INDICATION_MINIMUM_LENGTH +                                            \
   NETWORK_SLICING_INDICATION_MINIMUM_LENGTH + REJECTED_NSSAI_MINIMUM_LENGTH + \
   OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_MINIMUM_LENGTH +               \
   SMS_INDICATION_MINIMUM_LENGTH + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define CONFIGURATION_UPDATE_COMMAND_MAXIMUM_LENGTH                            \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + MESSAGE_TYPE_MAXIMUM_LENGTH +         \
   CONFIGURATION_UPDATE_INDICATION_MAXIMUM_LENGTH +                            \
   _5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH +                                       \
   _5GS_TRACKING_AREA_IDENTITY_LIST_MAXIMUM_LENGTH + NSSAI_MAXIMUM_LENGTH +    \
   SERVICE_AREA_LIST_MAXIMUM_LENGTH + NETWORK_NAME_MAXIMUM_LENGTH +            \
   TIME_ZONE_MAXIMUM_LENGTH + TIME_ZONE_AND_TIME_MAXIMUM_LENGTH +              \
   DAYLIGHT_SAVING_TIME_MAXIMUM_LENGTH + LADN_INFORMATION_MAXIMUM_LENGTH +     \
   MICO_INDICATION_MAXIMUM_LENGTH +                                            \
   NETWORK_SLICING_INDICATION_MAXIMUM_LENGTH + REJECTED_NSSAI_MAXIMUM_LENGTH + \
   OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS_MAXIMUM_LENGTH +               \
   SMS_INDICATION_MAXIMUM_LENGTH + 0)

typedef struct configuration_update_command_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  SecurityHeaderType securityheadertype;
  MessageType messagetype;
  ConfigurationUpdateIndication configurationupdateindication;
  _5GSMobileIdentity _5gsmobileidentity;
  _5GSTrackingAreaIdentityList _5gstrackingareaidentitylist;
  NSSAI nssai;
  ServiceAreaList servicearealist;
  NetworkName networkname;
  TimeZone timezone;
  TimeZoneAndTime timezoneandtime;
  DaylightSavingTime daylightsavingtime;
  LADNInformation ladninformation;
  MICOIndication micoindication;
  NetworkSlicingIndication networkslicingindication;
  RejectedNSSAI rejectednssai;
  OperatorDefinedAccessCategoryDefinitions
      operatordefinedaccesscategorydefinitions;
  SMSIndication smsindication;
} configuration_update_command_msg;

int decode_configuration_update_command(
    configuration_update_command_msg* configurationupdatecommand,
    uint8_t* buffer, uint32_t len);
int encode_configuration_update_command(
    configuration_update_command_msg* configurationupdatecommand,
    uint8_t* buffer, uint32_t len);

#endif
