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

#ifndef _PDUSESSIONMODIFICATIONREJECT_H_
#define _PDUSESSIONMODIFICATIONREJECT_H_

#include <stdint.h>

#include "ExtendedProtocolDiscriminator.h"
#include "PDUSessionIdentity.h"
#include "ProcedureTransactionIdentity.h"
#include "MessageType.h"
#include "_5GSMCause.h"
#include "GPRSTimer3.h"
#include "ExtendedProtocolConfigurationOptions.h"
#include "_5GSMCongestionReattemptIndicator.h"

/* Minimum length macro. Formed by minimum length of each mandatory field */
#define PDU_SESSION_MODIFICATION_REJECT_MINIMUM_LENGTH                         \
  (_5GSM_CAUSE_MINIMUM_LENGTH_V + 0)

/* Maximum length macro. Formed by maximum length of each field */
#define PDU_SESSION_MODIFICATION_REJECT_MAXIMUM_LENGTH                         \
  (EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH +                            \
   PDU_SESSION_IDENTITY_MAXIMUM_LENGTH +                                       \
   PROCEDURE_TRANSACTION_IDENTITY_MAXIMUM_LENGTH +                             \
   MESSAGE_TYPE_MAXIMUM_LENGTH + _5GSM_CAUSE_MAXIMUM_LENGTH_V +                \
   GPRS_TIMER3_MAXIMUM_LENGTH_TLV +                                            \
   EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MAXIMUM_LENGTH_TLVE + 0)

#define PDU_SESSION_MODIFICATION_REJECT_GPRS_TIMER3_IEI 0x37
#define PDU_SESSION_MODIFICATION_REJECT_E_P_C_O_IEI 0x7B
#define PDU_SESSION_MODIFICATION_REJECT__5GSM_CONGESTION_REATTEMPT_INDICATOR_IEI \
  0x61

#define PDU_SESSION_MODIFICATION_REJECT_GPRS_TIMER3_PRESENCE (1 << 0)
#define PDU_SESSION_MODIFICATION_REJECT_E_P_C_O_PRESENCE (1 << 1)
#define PDU_SESSION_MODIFICATION_REJECT__5GSM_CONGESTION_REATTEMPT_INDICATOR_PRESENCE \
  (1 << 2)

typedef struct pdu_session_modification_reject_msg_tag {
  ExtendedProtocolDiscriminator extendedprotocoldiscriminator;
  PDUSessionIdentity pdusessionidentity;
  ProcedureTransactionIdentity proceduretransactionidentity;
  MessageType messagetype;
  _5GSMCause _5gsmcause;
  uint8_t presence;
  GPRSTimer3 gprstimer3;
  ExtendedProtocolConfigurationOptions extendedprotocolconfigurationoptions;
  _5GSMCongestionReattemptIndicator _5gsmcongestionreattemptindicator;
} pdu_session_modification_reject_msg;

int decode_pdu_session_modification_reject(
    pdu_session_modification_reject_msg* pdusessionmodificationreject,
    uint8_t* buffer, uint32_t len);
int encode_pdu_session_modification_reject(
    pdu_session_modification_reject_msg* pdusessionmodificationreject,
    uint8_t* buffer, uint32_t len);

#endif
