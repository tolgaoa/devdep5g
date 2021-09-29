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

#ifndef SERVICE_REJECT_H_
#define SERVICE_REJECT_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "_5GMMCause.h"
#include "PDUSessionStatus.h"
#include "GPRSTimer2.h"
#include "EAPMessage.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define SERVICE_REJECT_MINIMUM_LENGTH                                          \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MINIMUM_LENGTH + MESSAGE_TYPE_MINIMUM_LENGTH +         \
   _5GMM_CAUSE_MINIMUM_LENGTH + PDU_SESSION_STATUS_MINIMUM_LENGTH +            \
   GPRS_TIMER2_MINIMUM_LENGTH + EAP_MESSAGE_MINIMUM_LENGTH + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define SERVICE_REJECT_MAXIMUM_LENGTH                                          \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + MESSAGE_TYPE_MAXIMUM_LENGTH +         \
   _5GMM_CAUSE_MAXIMUM_LENGTH + PDU_SESSION_STATUS_MAXIMUM_LENGTH +            \
   GPRS_TIMER2_MAXIMUM_LENGTH + EAP_MESSAGE_MAXIMUM_LENGTH + 0)

typedef struct service_reject_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  SecurityHeaderType securityheadertype;
  MessageType messagetype;
  _5GMMCause _5gmmcause;
  PDUSessionStatus pdusessionstatus;
  GPRSTimer2 gprstimer2;
  EAPMessage eapmessage;
} service_reject_msg;

int decode_service_reject(
    service_reject_msg* servicereject, uint8_t* buffer, uint32_t len);
int encode_service_reject(
    service_reject_msg* servicereject, uint8_t* buffer, uint32_t len);

#endif
