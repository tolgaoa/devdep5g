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
#include <stdint.h>

#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "ExtendedProtocolConfigurationOptions.h"

int encode_extended_protocol_configuration_options(
    protocol_configuration_options_nas_t extendedprotocolconfigurationoptions,
    uint8_t iei, uint8_t* buffer, uint32_t len) {
  uint8_t* lenPtr  = NULL;
  uint32_t encoded = 0;
  //  int encode_result = 0;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer,
      ((iei > 0) ?
           EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MINIMUM_LENGTH_TLVE :
           EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_MINIMUM_LENGTH_TLVE - 1),
      len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr        = (buffer + encoded);
  uint16_t temp = 0;
  encoded += 2;  // ENCODE_U16

  encoded += encode_protocol_configuration_options(
      extendedprotocolconfigurationoptions, buffer + encoded, len - encoded);

  uint32_t len_ie = encoded - 2 - ((iei > 0) ? 1 : 0);
  ENCODE_U16(lenPtr, len_ie, temp);

  return encoded;
}

//------------------------------------------------------------------------------
int encode_protocol_configuration_options(
    const protocol_configuration_options_nas_t protocolconfigurationoptions,
    uint8_t* buffer, const uint32_t len) {
  uint8_t num_protocol_or_container_id = 0;
  uint32_t encoded                     = 0;
  int encode_result                    = 0;

  *(buffer + encoded) =
      0x00 | (1 << 7) |
      (protocolconfigurationoptions.configuration_protocol & 0x7);
  encoded++;

  while (num_protocol_or_container_id <
         protocolconfigurationoptions.num_protocol_or_container_id) {
    ENCODE_U16(
        buffer + encoded,
        protocolconfigurationoptions
            .protocol_or_container_ids[num_protocol_or_container_id]
            .id,
        encoded);
    *(buffer + encoded) =
        protocolconfigurationoptions
            .protocol_or_container_ids[num_protocol_or_container_id]
            .length;
    encoded++;
    if (protocolconfigurationoptions
            .protocol_or_container_ids[num_protocol_or_container_id]
            .length > 0) {
      if ((encode_result = encode_bstring(
               protocolconfigurationoptions
                   .protocol_or_container_ids[num_protocol_or_container_id]
                   .contents,
               buffer + encoded,
               protocolconfigurationoptions
                   .protocol_or_container_ids[num_protocol_or_container_id]
                   .length)) < 0)
        return encode_result;
      else
        encoded += encode_result;
    } else {
      encoded += protocolconfigurationoptions
                     .protocol_or_container_ids[num_protocol_or_container_id]
                     .length;
    }
    num_protocol_or_container_id += 1;
  }
  return encoded;
}

int decode_extended_protocol_configuration_options(
    protocol_configuration_options_nas_t* extendedprotocolconfigurationoptions,
    uint8_t iei, uint8_t* buffer, uint32_t len) {
  int decoded       = 0;
  uint32_t ielen    = 0;
  int decode_result = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  DECODE_U16(buffer + decoded, ielen, decoded);

  if (((*(buffer + decoded) >> 7) & 0x1) != 1) {
    return TLV_VALUE_DOESNT_MATCH;
  }

  // Bits 7 to 4 of octet 3 are spare, read as 0
  if (((*(buffer + decoded) & 0x78) >> 3) != 0) {
    return TLV_VALUE_DOESNT_MATCH;
  }

  extendedprotocolconfigurationoptions->configuration_protocol =
      (*(buffer + decoded) >> 1) & 0x7;
  decoded++;

  decode_result = decode_protocol_configuration_options(
      extendedprotocolconfigurationoptions, buffer + decoded, ielen);

  if (decode_result < 0) {
    return decode_result;
  }

  decoded += (uint32_t) decode_result;

  return decoded;
}

//------------------------------------------------------------------------------
int decode_protocol_configuration_options(
    protocol_configuration_options_nas_t* protocolconfigurationoptions,
    const uint8_t* const buffer, const uint32_t len) {
  int decoded       = 0;
  int decode_result = 0;

  protocolconfigurationoptions->num_protocol_or_container_id = 0;

  while (3 <= ((int32_t) len - (int32_t) decoded)) {
    DECODE_U16(
        buffer + decoded,
        protocolconfigurationoptions
            ->protocol_or_container_ids[protocolconfigurationoptions
                                            ->num_protocol_or_container_id]
            .id,
        decoded);

    DECODE_U8(
        buffer + decoded,
        protocolconfigurationoptions
            ->protocol_or_container_ids[protocolconfigurationoptions
                                            ->num_protocol_or_container_id]
            .length,
        decoded);

    if (0 < protocolconfigurationoptions
                ->protocol_or_container_ids[protocolconfigurationoptions
                                                ->num_protocol_or_container_id]
                .length) {
      if ((decode_result = decode_bstring(
               &protocolconfigurationoptions
                    ->protocol_or_container_ids
                        [protocolconfigurationoptions
                             ->num_protocol_or_container_id]
                    .contents,
               protocolconfigurationoptions
                   ->protocol_or_container_ids
                       [protocolconfigurationoptions
                            ->num_protocol_or_container_id]
                   .length,
               buffer + decoded, len - decoded)) < 0) {
        return decode_result;
      } else {
        decoded += decode_result;
      }
    } else {
      protocolconfigurationoptions
          ->protocol_or_container_ids[protocolconfigurationoptions
                                          ->num_protocol_or_container_id]
          .contents = NULL;
    }
    protocolconfigurationoptions->num_protocol_or_container_id += 1;
  }

  return decoded;
}
