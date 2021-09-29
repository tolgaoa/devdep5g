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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "ConfigurationUpdateCommand.h"

int decode_configuration_update_command(
    configuration_update_command_msg* configuration_update_command,
    uint8_t* buffer, uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, CONFIGURATION_UPDATE_COMMAND_MINIMUM_LENGTH, len);

  if ((decoded_result = decode_extended_protocol_discriminator(
           &configuration_update_command->extendedprotocoldiscriminator, 0,
           buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_security_header_type(
           &configuration_update_command->securityheadertype, 0,
           buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_message_type(
           &configuration_update_command->messagetype, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_configuration_update_indication(
           &configuration_update_command->configurationupdateindication, 0,
           buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode__5gs_mobile_identity(
           &configuration_update_command->_5gsmobileidentity, 0,
           buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode__5gs_tracking_area_identity_list(
           &configuration_update_command->_5gstrackingareaidentitylist, 0,
           buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_nssai(
           &configuration_update_command->nssai, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_service_area_list(
           &configuration_update_command->servicearealist, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_network_name(
           &configuration_update_command->networkname, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_time_zone(
           &configuration_update_command->timezone, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_time_zone_and_time(
           &configuration_update_command->timezoneandtime, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_daylight_saving_time(
           &configuration_update_command->daylightsavingtime, 0,
           buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_ladn_information(
           &configuration_update_command->ladninformation, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_mico_indication(
           &configuration_update_command->micoindication, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_network_slicing_indication(
           &configuration_update_command->networkslicingindication, 0,
           buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_rejected_nssai(
           &configuration_update_command->rejectednssai, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_operator_defined_access_category_definitions(
           &configuration_update_command
                ->operatordefinedaccesscategorydefinitions,
           0, buffer + decoded, len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_sms_indication(
           &configuration_update_command->smsindication, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  return decoded;
}

int encode_configuration_update_command(
    configuration_update_command_msg* configuration_update_command,
    uint8_t* buffer, uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, CONFIGURATION_UPDATE_COMMAND_MINIMUM_LENGTH, len);

  if ((encoded_result = encode_extended_protocol_discriminator(
           configuration_update_command->extendedprotocoldiscriminator, 0,
           buffer + encoded, len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_security_header_type(
           configuration_update_command->securityheadertype, 0,
           buffer + encoded, len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_message_type(
           configuration_update_command->messagetype, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_configuration_update_indication(
           configuration_update_command->configurationupdateindication, 0,
           buffer + encoded, len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode__5gs_mobile_identity(
           configuration_update_command->_5gsmobileidentity, 0,
           buffer + encoded, len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode__5gs_tracking_area_identity_list(
           configuration_update_command->_5gstrackingareaidentitylist, 0,
           buffer + encoded, len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_nssai(
           configuration_update_command->nssai, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_service_area_list(
           configuration_update_command->servicearealist, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_network_name(
           configuration_update_command->networkname, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_time_zone(
           configuration_update_command->timezone, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_time_zone_and_time(
           configuration_update_command->timezoneandtime, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_daylight_saving_time(
           configuration_update_command->daylightsavingtime, 0,
           buffer + encoded, len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_ladn_information(
           configuration_update_command->ladninformation, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_mico_indication(
           configuration_update_command->micoindication, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_network_slicing_indication(
           configuration_update_command->networkslicingindication, 0,
           buffer + encoded, len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_rejected_nssai(
           configuration_update_command->rejectednssai, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_operator_defined_access_category_definitions(
           configuration_update_command
               ->operatordefinedaccesscategorydefinitions,
           0, buffer + encoded, len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  if ((encoded_result = encode_sms_indication(
           configuration_update_command->smsindication, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  return encoded;
}
