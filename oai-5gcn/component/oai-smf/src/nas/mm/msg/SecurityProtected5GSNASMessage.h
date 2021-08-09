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

#ifndef SECURITY_PROTECTED_5GS_NAS_MESSAGE_H_
#define SECURITY_PROTECTED_5GS_NAS_MESSAGE_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageAuthenticationCode.h"
#include "SequenceNumber.h"
#include "Plain5GSNASMessage.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define SECURITY_PROTECTED5_GSNAS_MESSAGE_MINIMUM_LENGTH                       \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MINIMUM_LENGTH +                                       \
   MESSAGE_AUTHENTICATION_CODE_MINIMUM_LENGTH +                                \
   SEQUENCE_NUMBER_MINIMUM_LENGTH + PLAIN_5GSNAS_MESSAGE_MINIMUM_LENGTH + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define SECURITY_PROTECTED5_GSNAS_MESSAGE_MAXIMUM_LENGTH                       \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MAXIMUM_LENGTH +                                       \
   MESSAGE_AUTHENTICATION_CODE_MAXIMUM_LENGTH +                                \
   SEQUENCE_NUMBER_MAXIMUM_LENGTH + PLAIN_5GSNAS_MESSAGE_MAXIMUM_LENGTH + 0)

typedef struct security_protected5_gsnas_message_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  SecurityHeaderType securityheadertype;
  MessageAuthenticationCode messageauthenticationcode;
  SequenceNumber sequencenumber;
  Plain5GSNASMessage plain5gsnasmessage;
} security_protected5_gsnas_message_msg;

int decode_security_protected5_gsnas_message(
    security_protected5_gsnas_message_msg* securityprotected5gsnasmessage,
    uint8_t* buffer, uint32_t len);
int encode_security_protected5_gsnas_message(
    security_protected5_gsnas_message_msg* securityprotected5gsnasmessage,
    uint8_t* buffer, uint32_t len);

#endif
