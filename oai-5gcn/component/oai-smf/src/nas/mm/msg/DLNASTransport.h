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

#ifndef DLNAS_TRANSPORT_H_
#define DLNAS_TRANSPORT_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "SecurityHeaderType.h"
#include "MessageType.h"
#include "PayloadContainerType.h"
#include "PayloadContainer.h"
#include "PDUSessionIdentity2.h"
#include "AdditionalInformation.h"
#include "_5GMMCause.h"
#include "GPRSTimer3.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define DLNAS_TRANSPORT_MINIMUM_LENGTH                                         \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MINIMUM_LENGTH + MESSAGE_TYPE_MINIMUM_LENGTH +         \
   PAYLOAD_CONTAINER_TYPE_MINIMUM_LENGTH + PAYLOAD_CONTAINER_MINIMUM_LENGTH +  \
   PDU_SESSION_IDENTITY2_MINIMUM_LENGTH +                                      \
   ADDITIONAL_INFORMATION_MINIMUM_LENGTH + _5GMM_CAUSE_MINIMUM_LENGTH +        \
   GPRS_TIMER3_MINIMUM_LENGTH + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define DLNAS_TRANSPORT_MAXIMUM_LENGTH                                         \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   SECURITY_HEADER_TYPE_MAXIMUM_LENGTH + MESSAGE_TYPE_MAXIMUM_LENGTH +         \
   PAYLOAD_CONTAINER_TYPE_MAXIMUM_LENGTH + PAYLOAD_CONTAINER_MAXIMUM_LENGTH +  \
   PDU_SESSION_IDENTITY2_MAXIMUM_LENGTH +                                      \
   ADDITIONAL_INFORMATION_MAXIMUM_LENGTH + _5GMM_CAUSE_MAXIMUM_LENGTH +        \
   GPRS_TIMER3_MAXIMUM_LENGTH + 0)

typedef struct dlnas_transport_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  SecurityHeaderType securityheadertype;
  MessageType messagetype;
  PayloadContainerType payloadcontainertype;
  PayloadContainer payloadcontainer;
  PDUSessionIdentity2 pdusessionidentity2;
  AdditionalInformation additionalinformation;
  _5GMMCause _5gmmcause;
  GPRSTimer3 gprstimer3;
} dlnas_transport_msg;

int decode_dlnas_transport(
    dlnas_transport_msg* dlnastransport, uint8_t* buffer, uint32_t len);
int encode_dlnas_transport(
    dlnas_transport_msg* dlnastransport, uint8_t* buffer, uint32_t len);

#endif
