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
#include "RegistrationRequest.h"

int decode_registration_request(
    registration_request_msg* registration_request, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;
  printf("decode_registration_request len:%d\n", len);

  if ((decoded_result = decode__5gs_registration_type(
           &registration_request->_5gsregistrationtype, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  if ((decoded_result = decode_u8_nas_key_set_identifier(
           &registration_request->naskeysetidentifier, 0,
           *(buffer + decoded) >> 4, len - decoded)) < 0)
    return decoded_result;
  decoded++;
  return decoded;
  /*
   if ((decoded_result = decode__5gs_mobile_identity
   (&registration_request->_5gsmobileidentity, 0, buffer + decoded, len -
   decoded)) < 0) return decoded_result; decoded++;

   */

  while (len - decoded > 0) {
    uint8_t ieiDecoded = *(buffer + decoded);
    if (ieiDecoded == 0) break;

    switch (ieiDecoded & 0xf0) {
      case REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_IEI:
        if ((decoded_result = decode_nas_key_set_identifier(
                 &registration_request
                      ->non_current_native_nas_key_set_identifier,
                 REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_MICO_IDICATION_IEI:
        if ((decoded_result = decode_mico_indication(
                 &registration_request->micoindication,
                 REGISTRATION_REQUEST_MICO_IDICATION_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_MICO_IDICATION_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_IEI:
        if ((decoded_result = decode_payload_container_type(
                 &registration_request->payloadcontainertype,
                 REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_IEI:
        if ((decoded_result = decode_network_slicing_indication(
                 &registration_request->networkslicingindication,
                 REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_PRESENT;
        }
        break;
    }

    switch (ieiDecoded) {
      /*
       case REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_IEI:
       if((decoded_result =
       decode_nas_key_set_identifier(&registration_request->non_current_native_nas_key_set_identifier,REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_IEI,buffer+decoded,len-decoded))<0)
       return decoded_result;
       else{
       decoded+=decoded_result;
       registration_request->presence |=
       REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT;
       }
       break;
       */
      case REGISTRATION_REQUEST_5GMM_CAPABILITY_IEI:
        if ((decoded_result = decode__5gmm_capability(
                 &registration_request->_5gmmcapability,
                 REGISTRATION_REQUEST_5GMM_CAPABILITY_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_IEI:
        if ((decoded_result = decode_ue_security_capability(
                 &registration_request->uesecuritycapability,
                 REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_IEI:
        if ((decoded_result = decode__5gs_tracking_area_identity(
                 &registration_request->_5gstrackingareaidentity,
                 REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_IEI:
        if ((decoded_result = decode_s1_ue_network_capability(
                 &registration_request->s1uenetworkcapability,
                 REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_UPLINK_DATA_STATUS_IEI:
        if ((decoded_result = decode_uplink_data_status(
                 &registration_request->uplinkdatastatus,
                 REGISTRATION_REQUEST_UPLINK_DATA_STATUS_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_PDU_SESSION_STATUS_IEI:
        if ((decoded_result = decode_pdu_session_status(
                 &registration_request->pdusessionstatus,
                 REGISTRATION_REQUEST_PDU_SESSION_STATUS_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT;
        }
        break;
        /*
         case REGISTRATION_REQUEST_MICO_IDICATION_IEI:
         if((decoded_result = decode_mico_indication
         (&registration_request->micoindication,
         REGISTRATION_REQUEST_MICO_IDICATION_IEI,
         buffer+decoded,len-decoded))<0) return decoded_result; else{
         decoded+=decoded_result;
         registration_request->presence |=
         REGISTRATION_REQUEST_MICO_IDICATION_PRESENT;
         }
         break;
         */
      case REGISTRATION_REQUEST_UE_STATUS_IEI:
        if ((decoded_result = decode_ue_status(
                 &registration_request->uestatus,
                 REGISTRATION_REQUEST_UE_STATUS_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_UE_STATUS_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_IEI:
        if ((decoded_result = decode_allowed_pdu_session_status(
                 &registration_request->allowedpdusessionstatus,
                 REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT;
        }

        break;
      case REGISTRATION_REQUEST_UE_USAGE_SETTING_IEI:
        if ((decoded_result = decode_ues_usage_setting(
                 &registration_request->uesusagesetting,
                 REGISTRATION_REQUEST_UE_USAGE_SETTING_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_UE_USAGE_SETTING_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_5GS_DRX_PARAMETERS_IEI:
        if ((decoded_result = decode__5gsdrx_parameters(
                 &registration_request->_5gsdrxparameters,
                 REGISTRATION_REQUEST_5GS_DRX_PARAMETERS_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_5GS_DRX_PARAMETERS_PRESENT;
        }

        break;
      case REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_IEI:
        if ((decoded_result = decode_epsnas_message_container(
                 &registration_request->epsnasmessagecontainer,
                 REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_PRESENT;
        }
        break;
        /*
         case REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_IEI:
         if((decoded_result = decode_payload_container_type
         (&registration_request->payloadcontainer,
         REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_IEI,
         buffer+decoded,len-decoded))<0) return decoded_result; else{
         decoded+=decoded_result;
         registration_request->presence |=
         REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_PRESENT;
         }
         break;
         */
      case REGISTRATION_REQUEST_PAYLOAD_CONTAINER_IEI:
        if ((decoded_result = decode_payload_container(
                 &registration_request->payloadcontainer,
                 REGISTRATION_REQUEST_PAYLOAD_CONTAINER_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_PAYLOAD_CONTAINER_PRESENT;
        }

        break;
        /*
         case REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_IEI:
         if((decoded_result = decode_network_slicing_indication
         (&registration_request->networkslicingindication,
         REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_IEI,
         buffer+decoded,len-decoded))<0) return decoded_result; else{
         decoded+=decoded_result;
         registration_request->presence |=
         REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_PRESENT;
         }
         break;
         */
      case REGISTRATION_REQUEST_5GS_UPDATE_TYPE_IEI:
        if ((decoded_result = decode__5gs_update_type(
                 &registration_request->_5gsupdatetype,
                 REGISTRATION_REQUEST_5GS_UPDATE_TYPE_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_5GS_UPDATE_TYPE_PRESENT;
        }
        break;
      case REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_IEI:
        if ((decoded_result = decode_nas_message_container(
                 &registration_request->nasmessagecontainer,
                 REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_request->presence |=
              REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT;
        }

        break;
        /*
         case REGISTRATION_REQUEST_NSSAI_IEI:
         if((decoded_result = decode_nssai(&registration_request->nssai,
         REGISTRATION_REQUEST_NSSAI_IEI, buffer+decoded,len-decoded))<0) return
         decoded_result; else{ decoded+=decoded_result;
         registration_request->presence |= REGISTRATION_REQUEST_NSSAI_PRESENT;
         }
         break;
         case REGISTRATION_REQUEST_5GS_MOBILE_IDENTITY_IEI:
         if((decoded_result =
         decode__5gs_mobile_identity(&registration_request->AdditionalGUTI,
         REGISTRATION_REQUEST_5GS_MOBILE_IDENTITY_IEI,
         buffer+decoded,len-decoded))<0) return decoded_result; else{
         decoded+=decoded_result;
         registration_request->presence |=
         REGISTRATION_REQUEST_5GS_MOBILE_IDENTITY_PRESENT;
         }
         break;
         case REGISTRATION_REQUEST_LADN_INDICATION_IEI:
         if((decoded_result =
         decode_ladn_indication(&registration_request->ladnindication,
         REGISTRATION_REQUEST_LADN_INDICATION_IEI,
         buffer+decoded,len-decoded))<0) return decoded_result; else{
         decoded+=decoded_result;
         registration_request->presence |=
         REGISTRATION_REQUEST_LADN_INDICATION_PRESENT;
         }
         break;
         */
    }
  }

  return decoded;
}

int encode_registration_request(
    registration_request_msg* registration_request, uint8_t* buffer,
    uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, REGISTRATION_REQUEST_MINIMUM_LENGTH, len);

  if ((encoded_result = encode__5gs_registration_type(
           &registration_request->_5gsregistrationtype, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;

  *(buffer + encoded) = ((encode_u8_nas_key_set_identifier(
                              &registration_request->naskeysetidentifier) &
                          0x0f)
                         << 4) |
                        0x00;
  encoded++;

  // encode mobile identity
  /*
   if((encoded_result =
   encode__5gs_mobile_identity(&registration_request->_5gsmobileidentity,0,buffer+encoded,len-encoded))<0)
   return encoded_result;
   else
   encoded+=encoded_result;
   */

  if ((registration_request->presence &
       REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT) ==
      REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_PRESENT) {
    if ((encoded_result = encode_nas_key_set_identifier(
             &registration_request->non_current_native_nas_key_set_identifier,
             REGISTRATION_REQUEST_NAS_KEY_SET_IDENTIFIER_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_request->presence &
       REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT) ==
      REGISTRATION_REQUEST_5GMM_CAPABILITY_PRESENT) {
    if ((encoded_result = encode__5gmm_capability(
             registration_request->_5gmmcapability,
             REGISTRATION_REQUEST_5GMM_CAPABILITY_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_request->presence &
       REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT) ==
      REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_PRESENT) {
    if ((encoded_result = encode_ue_security_capability(
             registration_request->uesecuritycapability,
             REGISTRATION_REQUEST_UE_SECURITY_CAPABILITY_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  // encode nssai
  /*
   if(registration_request->presence & REGISTRATION_REQUEST_NSSAI_PRESENT
   == REGISTRATION_REQUEST_NSSAI_PRESENT){
   if((encoded_result = encode_nssai (registration_request->nssai,
   REGISTRATION_REQUEST_NSSAI_IEI, buffer+encoded,len-encoded))<0) return
   encoded_result; else encoded+=encoded_result;
   }
   */
  if ((registration_request->presence &
       REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_PRESENT) ==
      REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_PRESENT) {
    if ((encoded_result = encode__5gs_tracking_area_identity(
             registration_request->_5gstrackingareaidentity,
             REGISTRATION_REQUEST_5GS_TRACKING_AREA_IDENTITY_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  if ((registration_request->presence &
       REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT) ==
      REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_PRESENT) {
    if ((encoded_result = encode_s1_ue_network_capability(
             registration_request->s1uenetworkcapability,
             REGISTRATION_REQUEST_S1_UE_NETWORK_CAPABILITY_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  if ((registration_request->presence &
       REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT) ==
      REGISTRATION_REQUEST_UPLINK_DATA_STATUS_PRESENT) {
    if ((encoded_result = encode_uplink_data_status(
             registration_request->uplinkdatastatus,
             REGISTRATION_REQUEST_UPLINK_DATA_STATUS_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_request->presence &
       REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT) ==
      REGISTRATION_REQUEST_PDU_SESSION_STATUS_PRESENT) {
    if ((encoded_result = encode_pdu_session_status(
             registration_request->pdusessionstatus,
             REGISTRATION_REQUEST_PDU_SESSION_STATUS_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_request->presence &
       REGISTRATION_REQUEST_MICO_IDICATION_PRESENT) ==
      REGISTRATION_REQUEST_MICO_IDICATION_PRESENT) {
    if ((encoded_result = encode_mico_indication(
             registration_request->micoindication,
             REGISTRATION_REQUEST_MICO_IDICATION_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  if ((registration_request->presence &
       REGISTRATION_REQUEST_UE_STATUS_PRESENT) ==
      REGISTRATION_REQUEST_UE_STATUS_PRESENT) {
    if ((encoded_result = encode_ue_status(
             registration_request->uestatus, REGISTRATION_REQUEST_UE_STATUS_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  // encode
  /*
   if(registration_request->presence &
   REGISTRATION_REQUEST_5GS_MOBILE_IDENTITY_PRESENT
   == REGISTRATION_REQUEST_5GS_MOBILE_IDENTITY_PRESENT){
   if((encoded_result = encode__5gs_mobile_identity
   (registration_request->AdditionalGUTI,
   REGISTRATION_REQUEST_5GS_MOBILE_IDENTITY_IEI, buffer+encoded,len-encoded))<0)
   return encoded_result;
   else
   encoded+=encoded_result;
   }
   */
  // test
  if ((registration_request->presence &
       REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT) ==
      REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_PRESENT) {
    if ((encoded_result = encode_allowed_pdu_session_status(
             registration_request->allowedpdusessionstatus,
             REGISTRATION_REQUEST_ALLOWED_PDU_SESSION_STATUS_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  // test
  if ((registration_request->presence &
       REGISTRATION_REQUEST_UE_USAGE_SETTING_PRESENT) ==
      REGISTRATION_REQUEST_UE_USAGE_SETTING_PRESENT) {
    if ((encoded_result = encode_ues_usage_setting(
             registration_request->uesusagesetting,
             REGISTRATION_REQUEST_UE_USAGE_SETTING_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  // test
  if ((registration_request->presence &
       REGISTRATION_REQUEST_5GS_DRX_PARAMETERS_PRESENT) ==
      REGISTRATION_REQUEST_5GS_DRX_PARAMETERS_PRESENT) {
    if ((encoded_result = encode__5gsdrx_parameters(
             registration_request->_5gsdrxparameters,
             REGISTRATION_REQUEST_5GS_DRX_PARAMETERS_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  if ((registration_request->presence &
       REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_PRESENT) ==
      REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_PRESENT) {
    if ((encoded_result = encode_epsnas_message_container(
             registration_request->epsnasmessagecontainer,
             REGISTRATION_REQUEST_EPS_NAS_MESSAGE_CONTAINER_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  /*
   if(registration_request->presence &
   REGISTRATION_REQUEST_LADN_INDICATION_PRESENT
   == REGISTRATION_REQUEST_LADN_INDICATION_PRESENT){
   if((encoded_result =
   encode_ladn_indication(registration_request->ladnindication,
   REGISTRATION_REQUEST_LADN_INDICATION_PRESENT, buffer+encoded,len-encoded))<0)
   return encoded_result;
   else
   encoded+=encoded_result;
   }
   */
  if ((registration_request->presence &
       REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_PRESENT) ==
      REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_PRESENT) {
    if ((encoded_result = encode_payload_container_type(
             registration_request->payloadcontainertype,
             REGISTRATION_REQUEST_PAYLOAD_CONTAINER_TYPE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  if ((registration_request->presence &
       REGISTRATION_REQUEST_PAYLOAD_CONTAINER_PRESENT) ==
      REGISTRATION_REQUEST_PAYLOAD_CONTAINER_PRESENT) {
    if ((encoded_result = encode_payload_container(
             registration_request->payloadcontainer,
             REGISTRATION_REQUEST_PAYLOAD_CONTAINER_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  if ((registration_request->presence &
       REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_PRESENT) ==
      REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_PRESENT) {
    if ((encoded_result = encode_network_slicing_indication(
             registration_request->networkslicingindication,
             REGISTRATION_REQUEST_NETWORK_SLICING_INDICATION_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_request->presence &
       REGISTRATION_REQUEST_5GS_UPDATE_TYPE_PRESENT) ==
      REGISTRATION_REQUEST_5GS_UPDATE_TYPE_PRESENT) {
    if ((encoded_result = encode__5gs_update_type(
             registration_request->_5gsupdatetype,
             REGISTRATION_REQUEST_5GS_UPDATE_TYPE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_request->presence &
       REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT) ==
      REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_PRESENT) {
    if ((encoded_result = encode_nas_message_container(
             registration_request->nasmessagecontainer,
             REGISTRATION_REQUEST_NAS_MESSAGE_CONTAINER_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  return encoded;
}
