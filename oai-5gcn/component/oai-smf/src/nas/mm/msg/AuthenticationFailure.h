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

#ifndef AUTHENTICATION_FAILURE_H_
#define AUTHENTICATION_FAILURE_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "_5GMMCause.h"
#include "AuthenticationFailureParameter.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define AUTHENTICATION_FAILURE_MINIMUM_LENGTH                                  \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MINIMUM_LENGTH + MESSAGE_TYPE_MINIMUM_LENGTH +         \
   _5GMM_CAUSE_MINIMUM_LENGTH +                                                \
   AUTHENTICATION_FAILURE_PARAMETER_MINIMUM_LENGTH + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define AUTHENTICATION_FAILURE_MAXIMUM_LENGTH                                  \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + MESSAGE_TYPE_MAXIMUM_LENGTH +         \
   _5GMM_CAUSE_MAXIMUM_LENGTH +                                                \
   AUTHENTICATION_FAILURE_PARAMETER_MAXIMUM_LENGTH + 0)

#define AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_PRESENT (1 << 0)
#define AUTHENTICATION_FAILURE_AUTHENTICATION_FAILURE_PARAMETER_IEI 0x21

typedef struct authentication_failure_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  SecurityHeaderType securityheadertype;
  MessageType messagetype;
  _5GMMCause _5gmmcause;
  uint32_t presence;
  AuthenticationFailureParameter authenticationfailureparameter;
} authentication_failure_msg;

int decode_authentication_failure(
    authentication_failure_msg* authenticationfailure, uint8_t* buffer,
    uint32_t len);
int encode_authentication_failure(
    authentication_failure_msg* authenticationfailure, uint8_t* buffer,
    uint32_t len);

#endif
