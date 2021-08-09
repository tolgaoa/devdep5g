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
#include "RegistrationAccept.h"

int decode_registration_accept(
    registration_accept_msg* registration_accept, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded   = 0;
  int decoded_result = 0;

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_DECODER(
      buffer, REGISTRATION_ACCEPT_MINIMUM_LENGTH, len);
  if ((decoded_result = decode__5gs_registration_result(
           &registration_accept->_5gsregistrationresult, 0, buffer + decoded,
           len - decoded)) < 0)
    return decoded_result;
  else
    decoded += decoded_result;

  /*
   if((decoded_result = decode__5gs_mobile_identity
   (&registration_accept->_5gsmobileidentity, 0, buffer+decoded,len-decoded))<0)
   return decoded_result;
   else
   decoded+=decoded_result;
   */

  while (len - decoded > 0) {
    uint8_t ieiDecoded = *(buffer + decoded);
    // printf("ieiDecoded(%x)\n",ieiDecoded&0xf0);
    if (ieiDecoded == 0x0) break;
    switch ((ieiDecoded & 0xf0)) {
      case REGISTRATION_ACCEPT_MICO_INDICATION_IEI:
        if ((decoded_result = decode_mico_indication(
                 &registration_accept->micoindication,
                 REGISTRATION_ACCEPT_MICO_INDICATION_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_MICO_INDICATION_PRESENT;
        }
        break;
      case REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_IEI:
        if ((decoded_result = decode_network_slicing_indication(
                 &registration_accept->networkslicingindication,
                 REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_PRESENT;
        }
        break;
      case REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_IEI:
        if ((decoded_result = decode_nssai_inclusion_mode(
                 &registration_accept->nssaiinclusionmode,
                 REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_PRESENT;
        }
        break;
    }

#if 0
      switch((ieiDecoded&0xf0)>>4){
        case REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_IEI:
          if((decoded_result = decode_nssai_inclusion_mode (&registration_accept->nssaiinclusionmode, REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_IEI, buffer+decoded,len-decoded))<0)
            return decoded_result;
          else{
            decoded+=decoded_result;
            registration_accept->presence |= REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_PRESENT;
          }
		  return decoded;
          break;
      }
#endif

    switch (ieiDecoded) {
      case REGISTRATION_ACCEPT_PLMN_LIST_IEI:
        if ((decoded_result = decode_plmn_list(
                 registration_accept->plmnlist,
                 REGISTRATION_ACCEPT_PLMN_LIST_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_PLMN_LIST_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_5GS_TRACKING_AREA_IDENTITY_LIST_IEI:
        if ((decoded_result = decode__5gs_tracking_area_identity_list(
                 &registration_accept->_5gstrackingareaidentitylist,
                 REGISTRATION_ACCEPT_5GS_TRACKING_AREA_IDENTITY_LIST_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_5GS_TRACKING_AREA_IDENTITY_LIST_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_IEI:
        if ((decoded_result = decode__5gs_network_feature_support(
                 &registration_accept->_5gsnetworkfeaturesupport,
                 REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_PDU_SESSION_STATUS_IEI:
        if ((decoded_result = decode_pdu_session_status(
                 &registration_accept->pdusessionstatus,
                 REGISTRATION_ACCEPT_PDU_SESSION_STATUS_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_IEI:
        if ((decoded_result = decode_pdu_session_reactivation_result(
                 &registration_accept->pdusessionreactivationresult,
                 REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT;
        }

        break;

      case REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_IEI:
        if ((decoded_result = decode_pdu_session_reactivation_result_error_cause(
                 &registration_accept->pdusessionreactivationresulterrorcause,
                 REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT;
        }

        break;

#if 0
        case REGISTRATION_ACCEPT_MICO_INDICATION_IEI:
        if((decoded_result = decode_mico_indication (&registration_accept->micoindication, REGISTRATION_ACCEPT_MICO_INDICATION_IEI, buffer+decoded,len-decoded))<0)
          return decoded_result;
        else{
          decoded+=decoded_result;
          registration_accept->presence |= REGISTRATION_ACCEPT_MICO_INDICATION_PRESENT;
		  return decoded;
          }
        break;
#endif

      case REGISTRATION_ACCEPT_SERVICE_AREA_LIST_IEI:
        if ((decoded_result = decode_service_area_list(
                 &registration_accept->servicearealist,
                 REGISTRATION_ACCEPT_SERVICE_AREA_LIST_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_SERVICE_AREA_LIST_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_GPRS_TIMER3_T3512_VALUE_IEI:
        if ((decoded_result = decode_gprs_timer3(
                 &registration_accept->t3512,
                 REGISTRATION_ACCEPT_GPRS_TIMER3_T3512_VALUE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_GPRS_TIMER3_T3512_VALUE_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_GPRS_TIMER2_NON_3GPP_DEREGISTRATION_TIMER_VALUE_IEI:
        if ((decoded_result = decode_gprs_timer2(
                 &registration_accept->non_3gpp_deregistration_timer,
                 REGISTRATION_ACCEPT_GPRS_TIMER2_NON_3GPP_DEREGISTRATION_TIMER_VALUE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_GPRS_TIMER2_NON_3GPP_DEREGISTRATION_TIMER_VALUE_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_GPRS_TIMER2_T3502_VALUE_IEI:
        if ((decoded_result = decode_gprs_timer2(
                 &registration_accept->t3502,
                 REGISTRATION_ACCEPT_GPRS_TIMER2_T3502_VALUE_IEI,
                 buffer + decoded, len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_GPRS_TIMER2_T3502_VALUE_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_EAP_MESSAGE_IEI:
        if ((decoded_result = decode_eap_message(
                 &registration_accept->eapmessage,
                 REGISTRATION_ACCEPT_EAP_MESSAGE_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_EAP_MESSAGE_PRESENT;
        }
        break;

      case REGISTRATION_ACCEPT_5GS_DRX_PARAMETERS_IEI:
        if ((decoded_result = decode__5gsdrx_parameters(
                 &registration_accept->_5gsdrxparameters,
                 REGISTRATION_ACCEPT_5GS_DRX_PARAMETERS_IEI, buffer + decoded,
                 len - decoded)) < 0)
          return decoded_result;
        else {
          decoded += decoded_result;
          registration_accept->presence |=
              REGISTRATION_ACCEPT_5GS_DRX_PARAMETERS_PRESENT;
        }
        break;
    }
  }
  /*

   if((decoded_result = decode_nssai (&registration_accept->nssai, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_rejected_nssai
   (&registration_accept->rejectednssai, 0, buffer+decoded,len-decoded))<0)
   return decoded_result;
   else
   decoded+=decoded_result;


   if((decoded_result = decode_ladn_information
   (&registration_accept->ladninformation, 0, buffer+decoded,len-decoded))<0)
   return decoded_result;
   else
   decoded+=decoded_result;

   if((decoded_result = decode_gprs_timer2 (&registration_accept->gprstimer2, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_emergency_number_list
   (&registration_accept->emergencynumberlist, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_extended_emergency_number_list
   (&registration_accept->extendedemergencynumberlist, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_sor_transparent_container
   (&registration_accept->sortransparentcontainer, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_eap_message (&registration_accept->eapmessage, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;

   if((decoded_result = decode_nssai_inclusion_mode
   (&registration_accept->nssaiinclusionmode, 0, buffer+decoded,len-decoded))<0)
   return decoded_result;
   else
   decoded+=decoded_result;

   if((decoded_result = decode_operator_defined_access_category_definitions
   (&registration_accept->operatordefinedaccesscategorydefinitions, 0,
   buffer+decoded,len-decoded))<0) return decoded_result; else
   decoded+=decoded_result;


   */
  return decoded;
}

int encode_registration_accept(
    registration_accept_msg* registration_accept, uint8_t* buffer,
    uint32_t len) {
  uint32_t encoded   = 0;
  int encoded_result = 0;

  printf("registration_accept->presence(%x)\n", registration_accept->presence);

  // Check if we got a NULL pointer and if buffer length is >= minimum length
  // expected for the message.
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, REGISTRATION_ACCEPT_MINIMUM_LENGTH, len);

  if ((encoded_result = encode__5gs_registration_result(
           registration_accept->_5gsregistrationresult, 0, buffer + encoded,
           len - encoded)) < 0)
    return encoded_result;
  else
    encoded += encoded_result;
  /*
   if((encoded_result = encode__5gs_mobile_identity
   (registration_accept->_5gsmobileidentity, 0, buffer+encoded,len-encoded))<0)
   return encoded_result;
   else
   encoded+=encoded_result;
   */
  if ((registration_accept->presence & REGISTRATION_ACCEPT_PLMN_LIST_PRESENT) ==
      REGISTRATION_ACCEPT_PLMN_LIST_PRESENT) {
    if ((encoded_result = encode_plmn_list(
             registration_accept->plmnlist, REGISTRATION_ACCEPT_PLMN_LIST_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_5GS_TRACKING_AREA_IDENTITY_LIST_PRESENT) ==
      REGISTRATION_ACCEPT_5GS_TRACKING_AREA_IDENTITY_LIST_PRESENT) {
    if ((encoded_result = encode__5gs_tracking_area_identity_list(
             registration_accept->_5gstrackingareaidentitylist,
             REGISTRATION_ACCEPT_5GS_TRACKING_AREA_IDENTITY_LIST_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  /*

   if((encoded_result = encode_nssai (registration_accept->nssai, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;

   if((encoded_result = encode_rejected_nssai
   (registration_accept->rejectednssai, 0, buffer+encoded,len-encoded))<0)
   return encoded_result;
   else
   encoded+=encoded_result;
   */
  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_PRESENT) ==
      REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_PRESENT) {
    printf("?????\n");
    if ((encoded_result = encode__5gs_network_feature_support(
             registration_accept->_5gsnetworkfeaturesupport,
             REGISTRATION_ACCEPT_5GS_NETWORK_FEATURE_SUPPORT_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT) ==
      REGISTRATION_ACCEPT_PDU_SESSION_STATUS_PRESENT) {
    if ((encoded_result = encode_pdu_session_status(
             registration_accept->pdusessionstatus,
             REGISTRATION_ACCEPT_PDU_SESSION_STATUS_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT) ==
      REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_PRESENT) {
    if ((encoded_result = encode_pdu_session_reactivation_result(
             registration_accept->pdusessionreactivationresult,
             REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT) ==
      REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_PRESENT) {
    if ((encoded_result = encode_pdu_session_reactivation_result_error_cause(
             registration_accept->pdusessionreactivationresulterrorcause,
             REGISTRATION_ACCEPT_PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  /*
   if((encoded_result = encode_ladn_information
   (registration_accept->ladninformation, 0, buffer+encoded,len-encoded))<0)
   return encoded_result;
   else
   encoded+=encoded_result;
   */
  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_MICO_INDICATION_PRESENT) ==
      REGISTRATION_ACCEPT_MICO_INDICATION_PRESENT) {
    if ((encoded_result = encode_mico_indication(
             registration_accept->micoindication,
             REGISTRATION_ACCEPT_MICO_INDICATION_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_PRESENT) ==
      REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_PRESENT) {
    if ((encoded_result = encode_network_slicing_indication(
             registration_accept->networkslicingindication,
             REGISTRATION_ACCEPT_NETWORK_SLICING_INDICATION_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_SERVICE_AREA_LIST_PRESENT) ==
      REGISTRATION_ACCEPT_SERVICE_AREA_LIST_PRESENT) {
    if ((encoded_result = encode_service_area_list(
             registration_accept->servicearealist,
             REGISTRATION_ACCEPT_SERVICE_AREA_LIST_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_GPRS_TIMER3_T3512_VALUE_PRESENT) ==
      REGISTRATION_ACCEPT_GPRS_TIMER3_T3512_VALUE_PRESENT) {
    if ((encoded_result = encode_gprs_timer3(
             registration_accept->t3512,
             REGISTRATION_ACCEPT_GPRS_TIMER3_T3512_VALUE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_GPRS_TIMER2_NON_3GPP_DEREGISTRATION_TIMER_VALUE_PRESENT) ==
      REGISTRATION_ACCEPT_GPRS_TIMER2_NON_3GPP_DEREGISTRATION_TIMER_VALUE_PRESENT) {
    if ((encoded_result = encode_gprs_timer2(
             registration_accept->non_3gpp_deregistration_timer,
             REGISTRATION_ACCEPT_GPRS_TIMER2_NON_3GPP_DEREGISTRATION_TIMER_VALUE_IEI,
             buffer + encoded, len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_GPRS_TIMER2_T3502_VALUE_PRESENT) ==
      REGISTRATION_ACCEPT_GPRS_TIMER2_T3502_VALUE_PRESENT) {
    if ((encoded_result = encode_gprs_timer2(
             registration_accept->t3502,
             REGISTRATION_ACCEPT_GPRS_TIMER2_T3502_VALUE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  /*
   if((encoded_result = encode_emergency_number_list
   (registration_accept->emergencynumberlist, 0, buffer+encoded,len-encoded))<0)
   return encoded_result;
   else
   encoded+=encoded_result;

   if((encoded_result = encode_extended_emergency_number_list
   (registration_accept->extendedemergencynumberlist, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;

   if((encoded_result = encode_sor_transparent_container
   (registration_accept->sortransparentcontainer, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;
   */
  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_EAP_MESSAGE_PRESENT) ==
      REGISTRATION_ACCEPT_EAP_MESSAGE_PRESENT) {
    if ((encoded_result = encode_eap_message(
             registration_accept->eapmessage,
             REGISTRATION_ACCEPT_EAP_MESSAGE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }

  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_PRESENT) ==
      REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_PRESENT) {
    if ((encoded_result = encode_nssai_inclusion_mode(
             registration_accept->nssaiinclusionmode,
             REGISTRATION_ACCEPT_NSSAI_INCLUSION_MODE_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  /*
   if((encoded_result = encode_operator_defined_access_category_definitions
   (registration_accept->operatordefinedaccesscategorydefinitions, 0,
   buffer+encoded,len-encoded))<0) return encoded_result; else
   encoded+=encoded_result;
   */
  if ((registration_accept->presence &
       REGISTRATION_ACCEPT_5GS_DRX_PARAMETERS_PRESENT) ==
      REGISTRATION_ACCEPT_5GS_DRX_PARAMETERS_PRESENT) {
    if ((encoded_result = encode__5gsdrx_parameters(
             registration_accept->_5gsdrxparameters,
             REGISTRATION_ACCEPT_5GS_DRX_PARAMETERS_IEI, buffer + encoded,
             len - encoded)) < 0)
      return encoded_result;
    else
      encoded += encoded_result;
  }
  return encoded;
}
