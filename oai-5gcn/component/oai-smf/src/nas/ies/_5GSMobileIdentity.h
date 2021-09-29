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

#ifndef _5GS_MOBILE_IDENTITY_H
#define _5GS_MOBILE_IDENTITY_H

#include <stdint.h>
#include "bstrlib.h"

#define _5GS_MOBILE_IDENTITY_MINIMUM_LENGTH 4
#define _5GS_MOBILE_IDENTITY_MAXIMUM_LENGTH 9999

#define NO_IDENTITY 0b000
#define SUCI 0b001
#define _5G_GUTI 0b010
#define IMEI 0b011
#define _5G_S_TMSI 0b100
#define IMEISV 0b101
#define IMSI 0b000
#define NETWORK_SPECIFIC_IDENTIFIER 0b001
#define NULL_SCHEME 0b0000
#define ECIES_SCEME_PROFILE_A 0b0001
#define ECIES_SCEME_PROFILE_B 0b0010

typedef struct {
  uint8_t IdentityType;
  uint8_t odd_even_indication;
  /*5g-guti*/
  uint16_t mcc;
  uint16_t mnc;
  uint8_t amfRegionID;
  uint16_t amfSetID;
  uint8_t amfPointer;
  uint32_t _5g_tmsi;
  /*imei imeisv*/
  uint8_t identity;
  /*suci supi imsi*/
  uint8_t supi;
  uint8_t routingIndicator;
  uint8_t protectionSchemeID;
  uint8_t homeNetworkPublicKeyIdentifier;
  uint8_t msin;

} _5GSMobileIdentity;

int encode__5gs_mobile_identity(
    _5GSMobileIdentity _5gsmobileidentity, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode__5gs_mobile_identity(
    _5GSMobileIdentity* _5gsmobileidentity, uint8_t iei, uint8_t* buffer,
    uint32_t len);

#endif
