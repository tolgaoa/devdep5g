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

#ifndef _PDUSESSIONMODIFICATIONREQUEST_H_
#define _PDUSESSIONMODIFICATIONREQUEST_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "PDUSessionIdentity.h"
#include "ProcedureTransactionIdentity.h"
#include "MessageType.h"
#include "_5GSMCapability.h"
#include "_5GSMCause.h"
#include "MaximumNumberOfSupportedPacketFilters.h"
#include "AlwaysonPDUSessionRequested.h"
#include "IntergrityProtectionMaximumDataRate.h"
#include "QOSRules.h"
#include "QOSFlowDescriptions.h"
#include "MappedEPSBearerContexts.h"
#include "ExtendedProtocolConfigurationOptions.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define PDU_SESSION_MODIFICATION_REQUEST_MINIMUM_LENGTH 0

/* Maximum length macro. Formed by maximum length of each field */
#define PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_LENGTH                        \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   PDU_SESSION_IDENTITY_MAXIMUM_LENGTH +                                       \
   PROCEDURE_TRANSACTION_IDENTITY_MAXIMUM_LENGTH +                             \
   MESSAGE_TYPE_MAXIMUM_LENGTH + _5GSM_CAPABILITY_MAXIMUM_LENGTH_TLV +         \
   _5GSM_CAUSE_MAXIMUM_LENGTH_TV +                                             \
   MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_MAXIMUM_LENGTH_TV +              \
   ALWAYSON_PDU_SESSION_REQUESTED_MAXIMUM_LENGTH +                             \
   INTERGRITY_PROTECTION_MAXIMUM_DATA_RATE_MAXIMUM_LENGTH_TV +                 \
   QOS_RULES_MAXIMUM_LENGTH_TLVE + QOS_FLOW_DESCRIPTIONS_MAXIMUM_LENGTH_TLVE + \
   MAPPED_EPS_BEARER_CONTEXTS_MAXIMUM_LENGTH +                                 \
   EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MAXIMUM_LENGTH_TLVE + 0)

#define PDU_SESSION_MODIFICATION_REQUEST__5GSM_CAPABILITY_IEI 0x28
#define PDU_SESSION_MODIFICATION_REQUEST__5GSM_CAUSE_IEI 0x59
#define PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_IEI 0x55
#define PDU_SESSION_MODIFICATION_REQUEST_ALWAYSON_PDU_SESSION_REQUESTED_IEI 0xB0
#define PDU_SESSION_MODIFICATION_REQUEST_INTGRITY_PROTECTION_MAXIMUM_DATA_RATE_IEI \
  0x13
#define PDU_SESSION_MODIFICATION_REQUEST_QOS_RULES_IEI 0x7A
#define PDU_SESSION_MODIFICATION_REQUEST_QOS_FLOW_DESCRIPTIONS_IEI 0x79
#define PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_IEI 0x75
#define PDU_SESSION_MODIFICATION_REQUEST_E_P_C_O_IEI 0x7B

#define PDU_SESSION_MODIFICATION_REQUEST__5GSM_CAPABILITY_PRESENCE (1 << 0)
#define PDU_SESSION_MODIFICATION_REQUEST__5GSM_CAUSE_PRESENCE (1 << 1)
#define PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PRESENCE  \
  (1 << 2)
#define PDU_SESSION_MODIFICATION_REQUEST_ALWAYSON_PDU_SESSION_REQUESTED_PRESENCE \
  (1 << 3)
#define PDU_SESSION_MODIFICATION_REQUEST_INTGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENCE \
  (1 << 4)
#define PDU_SESSION_MODIFICATION_REQUEST_QOS_RULES_PRESENCE (1 << 5)
#define PDU_SESSION_MODIFICATION_REQUEST_QOS_FLOW_DESCRIPTIONS_PRESENCE (1 << 6)
#define PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_PRESENCE   \
  (1 << 7)
#define PDU_SESSION_MODIFICATION_REQUEST_E_P_C_O_PRESENCE (1 << 8)

typedef struct pdu_session_modification_request_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  PDUSessionIdentity pdusessionidentity;
  ProcedureTransactionIdentity proceduretransactionidentity;
  MessageType messagetype;
  uint16_t presence;
  _5GSMCapability _5gsmcapability;
  _5GSMCause _5gsmcause;
  MaximumNumberOfSupportedPacketFilters maximumnumberofsupportedpacketfilters;
  AlwaysonPDUSessionRequested alwaysonpdusessionrequested;
  IntergrityProtectionMaximumDataRate intergrityprotectionmaximumdatarate;
  QOSRules qosrules;
  QOSFlowDescriptions qosflowdescriptions;
  MappedEPSBearerContexts mappedepsbearercontexts;
  ExtendedProtocolConfigurationOptions extendedprotocolconfigurationoptions;
} pdu_session_modification_request_msg;

int decode_pdu_session_modification_request(
    pdu_session_modification_request_msg* pdusessionmodificationrequest,
    uint8_t* buffer, uint32_t len);
int encode_pdu_session_modification_request(
    pdu_session_modification_request_msg* pdusessionmodificationrequest,
    uint8_t* buffer, uint32_t len);

#endif
