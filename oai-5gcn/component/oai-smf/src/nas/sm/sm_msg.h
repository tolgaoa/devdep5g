/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

#ifndef SM_MSG_H_
#define SM_MSG_H_

#include "smMsgDef.h"
#include "PDUSessionEstablishmentRequest.h"
#include "PDUSessionEstablishmentAccept.h"
#include "PDUSessionEstablishmentReject.h"
#include "PDUSessionAuthenticationCommand.h"
#include "PDUSessionAuthenticationComplete.h"
#include "PDUSessionAuthenticationResult.h"
#include "PDUSessionModificationRequest.h"
#include "PDUSessionModificationReject.h"
#include "PDUSessionModificationComplete.h"
#include "PDUSessionModificationCommand.h"
#include "PDUSessionModificationCommandReject.h"
#include "PDUSessionReleaseRequest.h"
#include "PDUSessionReleaseReject.h"
#include "PDUSessionReleaseCommand.h"
#include "PDUSessionReleaseComplete.h"
#include "_5GSMStatus.h"

typedef union {
  sm_msg_header_t header;
  pdu_session_establishment_request_msg pdu_session_establishment_request;
  pdu_session_establishment_accept_msg pdu_session_establishment_accept;
  pdu_session_establishment_reject_msg pdu_session_establishment_reject;

  pdu_session_authentication_command_msg pdu_session_authentication_command;
  pdu_session_authentication_complete_msg pdu_session_authentication_complete;
  pdu_session_authentication_result_msg pdu_session_authentication_result;

  pdu_session_modification_request_msg pdu_session_modification_request;
  pdu_session_modification_reject_msg pdu_session_modification_reject;
  pdu_session_modification_complete_msg pdu_session_modification_complete;
  pdu_session_modification_command_msg pdu_session_modification_command;
  pdu_session_modification_command_reject_msg
      pdu_session_modification_command_reject;

  pdu_session_release_request_msg pdu_session_release_request;
  pdu_session_release_reject_msg pdu_session_release_reject;
  pdu_session_release_command_msg pdu_session_release_command;
  pdu_session_release_complete_msg pdu_session_release_complete;

  _5gsm_status_msg _5gsm_status;
} SM_msg;

/*
 * Decode EPS Session Management messages
 * @param [SM_msg* ] msg: The SM message structure to be filled
 * @param [uint8_t* ] buffer: Pointer to the buffer containing the SM message
 * @param [uint32_t ] len: Number of bytes that should be decoded
 * @return The number of bytes in the buffer if data have been successfully
 * decoded; A negative error code otherwise.
 */
int sm_msg_decode(SM_msg* msg, uint8_t* buffer, uint32_t len);

/*
 * Encode 5GS Session Management messages
 * @param [SM_msg* ] msg: The SM message structure to encode
 * @param [uint8_t* ] buffer: Pointer to the encoded data buffer
 * @param [uint32_t ] len:  Maximal capacity of the output buffer
 * @return The number of bytes in the buffer if data have been successfully
 * encoded; A negative error code otherwise.
 */
int fivegsm_msg_encode(SM_msg* msg, uint8_t* buffer, uint32_t len);
#endif
