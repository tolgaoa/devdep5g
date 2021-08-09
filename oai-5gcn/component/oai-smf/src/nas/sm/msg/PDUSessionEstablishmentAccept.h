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

#ifndef _PDUSESSIONESTABLISHMENTACCEPT_H_
#define _PDUSESSIONESTABLISHMENTACCEPT_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "PDUSessionIdentity.h"
#include "ProcedureTransactionIdentity.h"
#include "MessageType.h"
#include "_PDUSessionType.h"
#include "SSCMode.h"
#include "QOSRules.h"
#include "SessionAMBR.h"
#include "_5GSMCause.h"
#include "PDUAddress.h"
#include "GPRSTimer.h"
#include "SNSSAI.h"
#include "AlwaysonPDUSessionIndication.h"
#include "MappedEPSBearerContexts.h"
#include "EAPMessage.h"
#include "QOSFlowDescriptions.h"
#include "ExtendedProtocolConfigurationOptions.h"
#include "DNN.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_MINIMUM_LENGTH                        \
  (_PDU_SESSION_TYPE_MINIMUM_LENGTH + SSC_MODE_MINIMUM_LENGTH +                \
   QOS_RULES_MINIMUM_LENGTH_LVE + SESSION_AMBR_MINIMUM_LENGTH_LV + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_MAXIMUM_LENGTH                        \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   PDU_SESSION_IDENTITY_MAXIMUM_LENGTH +                                       \
   PROCEDURE_TRANSACTION_IDENTITY_MAXIMUM_LENGTH +                             \
   MESSAGE_TYPE_MAXIMUM_LENGTH + _PDU_SESSION_TYPE_MAXIMUM_LENGTH +            \
   SSC_MODE_MAXIMUM_LENGTH + QOS_RULES_MAXIMUM_LENGTH_LVE +                    \
   SESSION_AMBR_MAXIMUM_LENGTH_LV + _5GSM_CAUSE_MAXIMUM_LENGTH_TV +            \
   PDU_ADDRESS_MAXIMUM_LENGTH_TLV + GPRS_TIMER_MAXIMUM_LENGTH_TV +             \
   SNSSAI_MAXIMUM_LENGTH_TLV +                                                 \
   ALWAYSON_PDU_SESSION_INDICATION_MAXIMUM_LENGTH +                            \
   MAPPED_EPS_BEARER_CONTEXTS_MAXIMUM_LENGTH +                                 \
   EAP_MESSAGE_MAXIMUM_LENGTH_TLVE +                                           \
   QOS_FLOW_DESCRIPTIONS_MAXIMUM_LENGTH_TLVE +                                 \
   EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MAXIMUM_LENGTH_TLVE +               \
   DNN_MAXIMUM_LENGTH_TLV + 0)

#define PDU_SESSION_ESTABLISHMENT_ACCEPT__5GSM_CAUSE_IEI 0x59
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_IEI 0x29
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_GPRS_TIMER_IEI 0x56
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_SNSSAI_IEI 0x22
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_ALWAYSON_PDU_SESSION_INDICATION_IEI   \
  0x80
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_IEI 0x75
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_IEI 0x78
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_QOS_FLOW_DESCRIPTIONS_IEI 0x79
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_EPCO_IEI 0x7B
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_IEI 0x25

#define PDU_SESSION_ESTABLISHMENT_ACCEPT__5GSM_CAUSE_PRESENCE (1 << 0)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_PRESENCE (1 << 1)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_GPRS_TIMER_PRESENCE (1 << 2)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_SNSSAI_PRESENCE (1 << 3)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_ALWAYSON_PDU_SESSION_INDICATION_PRESENCE \
  (1 << 4)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_PRESENCE   \
  (1 << 5)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_PRESENCE (1 << 6)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_QOS_FLOW_DESCRIPTIONS_PRESENCE (1 << 7)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_EPCO_PRESENCE (1 << 8)
#define PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_PRESENCE (1 << 9)

typedef struct pdu_session_establishment_accept_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  PDUSessionIdentity pdusessionidentity;
  ProcedureTransactionIdentity proceduretransactionidentity;
  MessageType messagetype;
  _PDUSessionType _pdusessiontype;
  SSCMode sscmode;
  QOSRules qosrules;
  SessionAMBR sessionambr;
  uint16_t presence;
  _5GSMCause _5gsmcause;
  PDUAddress pduaddress;
  GPRSTimer gprstimer;
  SNSSAI snssai;
  AlwaysonPDUSessionIndication alwaysonpdusessionindication;
  MappedEPSBearerContexts mappedepsbearercontexts;
  EAPMessage eapmessage;
  QOSFlowDescriptions qosflowdescriptions;
  ExtendedProtocolConfigurationOptions extendedprotocolconfigurationoptions;
  DNN dnn;
} pdu_session_establishment_accept_msg;

int decode_pdu_session_establishment_accept(
    pdu_session_establishment_accept_msg* pdusessionestablishmentaccept,
    uint8_t* buffer, uint32_t len);
int encode_pdu_session_establishment_accept(
    pdu_session_establishment_accept_msg* pdusessionestablishmentaccept,
    uint8_t* buffer, uint32_t len);

#endif
