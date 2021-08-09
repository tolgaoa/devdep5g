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

#include "common_types.h"
#include "sm_msg.h"
#include "TLVDecoder.h"
#include "TLVEncoder.h"

/*
 * Decode header of 5GS Mobility Management message. The protocol discriminator
 * and the security header type have already been decoded.
 * @param [sm_msg_header_t* ] header: The ESM message header to be filled
 * @param [const uint8_t* ] buffer: Pointer to the buffer containing the 5GS SM
 * message
 * @param [uint32_t ] len:  Number of bytes that should be decoded
 * @return The size of the header if data have been successfully decoded; A
 * negative error code otherwise.
 */
static int _fivegsm_msg_decode_header(
    sm_msg_header_t* header, const uint8_t* buffer, uint32_t len);

/*
 * Decode header of 5GS Mobility Management message. The protocol discriminator
 * and the security header type have already been decoded.
 * @param [const sm_msg_header_t*] header: The 5G SM message header to encode
 * @param [uint8_t* ] buffer: Pointer to the encoded data buffer
 * @param [uint32_t ] len:  Maximal capacity of the output buffer
 * @return The number of bytes in the buffer if data have been successfully
 * encoded; A negative error code otherwise.
 */
static int _fivegsm_msg_encode_header(
    const sm_msg_header_t* header, uint8_t* buffer, uint32_t len);

//------------------------------------------------------------------------------
int sm_msg_decode(SM_msg* msg, uint8_t* buffer, uint32_t len) {
  int header_result = 0;
  int decode_result = 0;

  /*
   * First decode the SM message header
   */
  header_result = _fivegsm_msg_decode_header(&msg->header, buffer, len);

  if (header_result < 0) {
    printf("ESM-MSG   - Failed to decode ESM message header %d", header_result);
    return header_result;
  }

  buffer += header_result;
  len -= header_result;

  switch (msg->header.message_type) {
    case PDU_SESSION_ESTABLISHMENT_REQUEST:
      decode_result = decode_pdu_session_establishment_request(
          &msg->pdu_session_establishment_request, buffer, len);
      break;
    case PDU_SESSION_ESTABLISHMENT_ACCEPT:
      decode_result = decode_pdu_session_establishment_accept(
          &msg->pdu_session_establishment_accept, buffer, len);
      break;
    case PDU_SESSION_ESTABLISHMENT_REJECT:
      decode_result = decode_pdu_session_establishment_reject(
          &msg->pdu_session_establishment_reject, buffer, len);
      break;
    case PDU_SESSION_AUTHENTICATION_COMMAND:
      decode_result = decode_pdu_session_authentication_command(
          &msg->pdu_session_authentication_command, buffer, len);
      break;
    case PDU_SESSION_AUTHENTICATION_COMPLETE:
      decode_result = decode_pdu_session_authentication_complete(
          &msg->pdu_session_authentication_complete, buffer, len);
      break;
    case PDU_SESSION_AUTHENTICATION_RESULT:
      decode_result = decode_pdu_session_authentication_result(
          &msg->pdu_session_authentication_result, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_REQUEST:
      decode_result = decode_pdu_session_modification_request(
          &msg->pdu_session_modification_request, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_REJECT:
      decode_result = decode_pdu_session_modification_reject(
          &msg->pdu_session_modification_reject, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_COMMAND:
      decode_result = decode_pdu_session_modification_command(
          &msg->pdu_session_modification_command, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_COMPLETE:
      decode_result = decode_pdu_session_modification_complete(
          &msg->pdu_session_modification_complete, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_COMMAND_REJECT:
      decode_result = decode_pdu_session_modification_command_reject(
          &msg->pdu_session_modification_command_reject, buffer, len);
      break;
    case PDU_SESSION_RELEASE_REQUEST:
      decode_result = decode_pdu_session_release_request(
          &msg->pdu_session_release_request, buffer, len);
      break;
    case PDU_SESSION_RELEASE_REJECT:
      decode_result = decode_pdu_session_release_reject(
          &msg->pdu_session_release_reject, buffer, len);
      break;
    case PDU_SESSION_RELEASE_COMMAND:
      decode_result = decode_pdu_session_release_command(
          &msg->pdu_session_release_command, buffer, len);
      break;
    case PDU_SESSION_RELEASE_COMPLETE:
      decode_result = decode_pdu_session_release_complete(
          &msg->pdu_session_release_complete, buffer, len);
      break;
    case _5GSM_STATUS:
      decode_result = decode__5gsm_status(&msg->_5gsm_status, buffer, len);
      break;
    default:
      decode_result = TLV_WRONG_MESSAGE_TYPE;
      break;
  }

  if (decode_result < 0) {
    return decode_result;
  }
  return header_result + decode_result;
}

//------------------------------------------------------------------------------
int fivegsm_msg_encode(SM_msg* msg, uint8_t* buffer, uint32_t len) {
  int header_result = 0;
  int encode_result = 0;
  /*
   * First encode the ESM message header
   */
  header_result = _fivegsm_msg_encode_header(&msg->header, buffer, len);

  if (header_result < 0) {
    // OAILOG_ERROR (LOG_NAS, "ESM-MSG   - Failed to encode ESM message header
    // (%d)\n", header_result); OAILOG_FUNC_RETURN (LOG_NAS, header_result);
    return header_result;
  }

  buffer += header_result;
  len -= header_result;
#if DEBUG_IS_ON
  printf("message type %d", msg->header.message_type);
#endif
  switch (msg->header.message_type) {
    case PDU_SESSION_ESTABLISHMENT_REQUEST:
      encode_result = encode_pdu_session_establishment_request(
          &msg->pdu_session_establishment_request, buffer, len);
      break;
    case PDU_SESSION_ESTABLISHMENT_ACCEPT:
      encode_result = encode_pdu_session_establishment_accept(
          &msg->pdu_session_establishment_accept, buffer, len);
      break;
    case PDU_SESSION_ESTABLISHMENT_REJECT:
      encode_result = encode_pdu_session_establishment_reject(
          &msg->pdu_session_establishment_reject, buffer, len);
      break;
    case PDU_SESSION_AUTHENTICATION_COMMAND:
      encode_result = encode_pdu_session_authentication_command(
          &msg->pdu_session_authentication_command, buffer, len);
      break;
    case PDU_SESSION_AUTHENTICATION_COMPLETE:
      encode_result = encode_pdu_session_authentication_complete(
          &msg->pdu_session_authentication_complete, buffer, len);
      break;
    case PDU_SESSION_AUTHENTICATION_RESULT:
      encode_result = encode_pdu_session_authentication_result(
          &msg->pdu_session_authentication_result, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_REQUEST:
      encode_result = encode_pdu_session_modification_request(
          &msg->pdu_session_modification_request, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_REJECT:
      encode_result = encode_pdu_session_modification_reject(
          &msg->pdu_session_modification_reject, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_COMMAND:
      encode_result = encode_pdu_session_modification_command(
          &msg->pdu_session_modification_command, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_COMPLETE:
      encode_result = encode_pdu_session_modification_complete(
          &msg->pdu_session_modification_complete, buffer, len);
      break;
    case PDU_SESSION_MODIFICATION_COMMAND_REJECT:
      encode_result = encode_pdu_session_modification_command_reject(
          &msg->pdu_session_modification_command_reject, buffer, len);
      break;
    case PDU_SESSION_RELEASE_REQUEST:
      encode_result = encode_pdu_session_release_request(
          &msg->pdu_session_release_request, buffer, len);
      break;
    case PDU_SESSION_RELEASE_REJECT:
      encode_result = encode_pdu_session_release_reject(
          &msg->pdu_session_release_reject, buffer, len);
      break;
    case PDU_SESSION_RELEASE_COMMAND:
      encode_result = encode_pdu_session_release_command(
          &msg->pdu_session_release_command, buffer, len);
      break;
    case PDU_SESSION_RELEASE_COMPLETE:
      encode_result = encode_pdu_session_release_complete(
          &msg->pdu_session_release_complete, buffer, len);
      break;
    case _5GSM_STATUS:
      encode_result = encode__5gsm_status(&msg->_5gsm_status, buffer, len);
      break;
    default:
      encode_result = TLV_WRONG_MESSAGE_TYPE;
      break;
  }

  if (encode_result < 0) {
    // OAILOG_ERROR (LOG_NAS, "SM-MSG   - Failed to encode L3 ESM message 0x%x
    // ""(%d)\n", msg->header.message_type, encode_result);
    return encode_result;
  }
  return header_result + encode_result;
}

//------------------------------------------------------------------------------
static int _fivegsm_msg_decode_header(
    sm_msg_header_t* header, const uint8_t* buffer, uint32_t len) {
  int size = 0;

  /*
   * Check the buffer length
   */
  if (len < sizeof(sm_msg_header_t)) {
    return (TLV_BUFFER_TOO_SHORT);
  }

  /*
   * Decode the EPS bearer identity and the protocol discriminator
   */
  DECODE_U8(buffer + size, header->extended_protocol_discriminator, size);

  /*
   * Decode the procedure transaction identity
   */
  DECODE_U8(buffer + size, header->pdu_session_identity, size);

  /*
   * Decode the procedure transaction identity
   */
  DECODE_U8(buffer + size, header->procedure_transaction_identity, size);
  /*
   * Decode the message type
   */
  DECODE_U8(buffer + size, header->message_type, size);

  /*
   * Check the protocol discriminator
   */
  if (header->extended_protocol_discriminator !=
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES) {
    return (TLV_PROTOCOL_NOT_SUPPORTED);
  }

  return (size);
}

//------------------------------------------------------------------------------
static int _fivegsm_msg_encode_header(
    const sm_msg_header_t* header, uint8_t* buffer, uint32_t len) {
  int size = 0;

  /*
   * Check the buffer length
   */
  if (len < sizeof(sm_msg_header_t)) {
    return (TLV_BUFFER_TOO_SHORT);
  }
  /*
   * Check the protocol discriminator
   */
  else if (
      header->extended_protocol_discriminator !=
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES) {
    return (TLV_PROTOCOL_NOT_SUPPORTED);
  }
  /*
   * Encode the EPS bearer identity and the protocol discriminator
   */
  // ENCODE_U8 (buffer + size, *(uint8_t *) (header), size);
  ENCODE_U8(buffer + size, header->extended_protocol_discriminator, size);
  /*
   * Encode the procedure session identity
   */
  ENCODE_U8(buffer + size, header->pdu_session_identity, size);

  /*
   * Encode the procedure transaction identity
   */
  ENCODE_U8(buffer + size, header->procedure_transaction_identity, size);
  /*
   * Encode the message type
   */
  ENCODE_U8(buffer + size, header->message_type, size);

  return (size);
}
