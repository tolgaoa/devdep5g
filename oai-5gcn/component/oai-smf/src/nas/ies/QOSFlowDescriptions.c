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
#include "QOSFlowDescriptions.h"

int encode_qos_flow_descriptions(
    QOSFlowDescriptions qosflowdescriptions, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  uint8_t* lenPtr                    = NULL;
  uint32_t encoded                   = 0;
  uint32_t currentlen                = 0;
  uint8_t bitstream                  = 0;
  uint16_t bit16stream               = 0;
  uint8_t* lengthofparametercontents = NULL;

  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer,
      ((iei > 0) ? QOS_FLOW_DESCRIPTIONS_MINIMUM_LENGTH_TLVE :
                   QOS_FLOW_DESCRIPTIONS_MINIMUM_LENGTH_TLVE - 1),
      len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = buffer + encoded;
  encoded++;
  encoded++;

  for (int i = 0; i < qosflowdescriptions.qosflowdescriptionsnumber; i++) {
    bitstream =
        (uint8_t) qosflowdescriptions.qosflowdescriptionscontents[i].qfi;
    ENCODE_U8(buffer + encoded, bitstream, encoded);

    bitstream = (uint8_t) qosflowdescriptions.qosflowdescriptionscontents[i]
                    .operationcode
                << 5;
    ENCODE_U8(buffer + encoded, bitstream, encoded);

    bitstream = (uint8_t) qosflowdescriptions.qosflowdescriptionscontents[i].e
                << 6;
    if ((qosflowdescriptions.qosflowdescriptionscontents[i].operationcode ==
         DELETE_EXISTING_QOS_FLOW_DESCRIPTION) &&
        (qosflowdescriptions.qosflowdescriptionscontents[i].e ==
         PARAMETERS_LIST_IS_NOT_INCLUDED)) {
      bitstream &= 0X40;
      ENCODE_U8(buffer + encoded, bitstream, encoded);
    } else {
      bitstream |= (uint8_t) qosflowdescriptions.qosflowdescriptionscontents[i]
                       .numberofparameters;
      ENCODE_U8(buffer + encoded, bitstream, encoded);

      for (int j = 0; j < qosflowdescriptions.qosflowdescriptionscontents[i]
                              .numberofparameters;
           j++) {
        ENCODE_U8(
            buffer + encoded,
            qosflowdescriptions.qosflowdescriptionscontents[i]
                .parameterslist[j]
                .parameteridentifier,
            encoded);

        lengthofparametercontents = buffer + encoded;
        encoded++;
        currentlen = encoded;

        if (qosflowdescriptions.qosflowdescriptionscontents[i]
                .parameterslist[j]
                .parameteridentifier == PARAMETER_IDENTIFIER_5QI) {
          ENCODE_U8(
              buffer + encoded,
              qosflowdescriptions.qosflowdescriptionscontents[i]
                  .parameterslist[j]
                  .parametercontents._5qi,
              encoded);
        } else if (
            (qosflowdescriptions.qosflowdescriptionscontents[i]
                 .parameterslist[j]
                 .parameteridentifier == PARAMETER_IDENTIFIER_GFBR_UPLINK) ||
            (qosflowdescriptions.qosflowdescriptionscontents[i]
                 .parameterslist[j]
                 .parameteridentifier == PARAMETER_IDENTIFIER_GFBR_DOWNLINK) ||
            (qosflowdescriptions.qosflowdescriptionscontents[i]
                 .parameterslist[j]
                 .parameteridentifier == PARAMETER_IDENTIFIER_MFBR_UPLINK) ||
            (qosflowdescriptions.qosflowdescriptionscontents[i]
                 .parameterslist[j]
                 .parameteridentifier == PARAMETER_IDENTIFIER_MFBR_DOWNLINK)) {
          ENCODE_U8(
              buffer + encoded,
              qosflowdescriptions.qosflowdescriptionscontents[i]
                  .parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.uint,
              encoded);

          bit16stream =
              qosflowdescriptions.qosflowdescriptionscontents[i]
                  .parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.value;
          *(buffer + encoded) = bit16stream / (1 << 8);
          encoded++;
          *(buffer + encoded) = bit16stream % (1 << 8);
          encoded++;
        } else if (
            qosflowdescriptions.qosflowdescriptionscontents[i]
                .parameterslist[j]
                .parameteridentifier == PARAMETER_IDENTIFIER_AVERAGING_WINDOW) {
          ENCODE_U8(
              buffer + encoded,
              qosflowdescriptions.qosflowdescriptionscontents[i]
                  .parameterslist[j]
                  .parametercontents.averagingwindow.uplinkinmilliseconds,
              encoded);
          ENCODE_U8(
              buffer + encoded,
              qosflowdescriptions.qosflowdescriptionscontents[i]
                  .parameterslist[j]
                  .parametercontents.averagingwindow.downlinkinmilliseconds,
              encoded);
        } else if (
            qosflowdescriptions.qosflowdescriptionscontents[i]
                .parameterslist[j]
                .parameteridentifier ==
            PARAMETER_IDENTIFIER_EPS_BEARER_IDENTITY) {
          bitstream = qosflowdescriptions.qosflowdescriptionscontents[i]
                          .parameterslist[j]
                          .parametercontents.epsbeareridentity
                      << 4;
          ENCODE_U8(buffer + encoded, bitstream, encoded);
        }

        *lengthofparametercontents = encoded - currentlen;
      }
    }
  }

  uint32_t res = encoded - 2 - ((iei > 0) ? 1 : 0);
  *lenPtr      = res / (1 << 8);
  lenPtr++;
  *lenPtr = res % (1 << 8);

  return encoded;
}

int decode_qos_flow_descriptions(
    QOSFlowDescriptions* qosflowdescriptions, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  uint32_t decoded                   = 0;
  uint16_t ielen                     = 0;
  uint8_t bitstream                  = 0;
  uint16_t bit16stream               = 0;
  uint16_t qosflowdescriptionsnumber = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  ielen = *(buffer + decoded);
  decoded++;
  ielen = (ielen << 8) + *(buffer + decoded);
  decoded++;
  CHECK_LENGTH_DECODER(len - decoded, ielen);

  uint32_t decoded_tmp = decoded;
  while ((decoded_tmp - 2 - ((iei > 0) ? 1 : 0)) < ielen) {
    uint8_t numberparameters = 0;
    decoded_tmp += 2;
    if ((((*(buffer + decoded_tmp) >> 6) & 0x01) ==
         PARAMETERS_LIST_IS_NOT_INCLUDED) &&
        ((*(buffer + decoded_tmp - 1) >> 5) ==
         DELETE_EXISTING_QOS_FLOW_DESCRIPTION)) {
      numberparameters = 0;
      decoded_tmp++;
    } else {
      numberparameters = *(buffer + decoded_tmp) & 0x3f;
      decoded_tmp++;
      for (int i = 0; i < numberparameters; i++) {
        decoded_tmp++;
        decoded_tmp = decoded_tmp + *(buffer + decoded_tmp) + 1;
      }
    }
    qosflowdescriptionsnumber++;
  }

  qosflowdescriptions->qosflowdescriptionsnumber = qosflowdescriptionsnumber;

  qosflowdescriptions->qosflowdescriptionscontents =
      (QOSFlowDescriptionsContents*) calloc(
          qosflowdescriptions->qosflowdescriptionsnumber,
          sizeof(QOSFlowDescriptionsContents));
  for (int i = 0; i < qosflowdescriptions->qosflowdescriptionsnumber; i++) {
    DECODE_U8(buffer + decoded, bitstream, decoded);
    qosflowdescriptions->qosflowdescriptionscontents[i].qfi = bitstream & 0x3f;

    DECODE_U8(buffer + decoded, bitstream, decoded);
    qosflowdescriptions->qosflowdescriptionscontents[i].operationcode =
        bitstream >> 5;

    DECODE_U8(buffer + decoded, bitstream, decoded);
    qosflowdescriptions->qosflowdescriptionscontents[i].e =
        (bitstream >> 6) & 0x01;
    qosflowdescriptions->qosflowdescriptionscontents[i].numberofparameters =
        bitstream & 0x3f;

    qosflowdescriptions->qosflowdescriptionscontents[i]
        .parameterslist = (ParametersList*) calloc(
        qosflowdescriptions->qosflowdescriptionscontents[i].numberofparameters,
        sizeof(ParametersList));
    for (int j = 0;
         j <
         qosflowdescriptions->qosflowdescriptionscontents[i].numberofparameters;
         j++) {
      DECODE_U8(buffer + decoded, bitstream, decoded);
      qosflowdescriptions->qosflowdescriptionscontents[i]
          .parameterslist[j]
          .parameteridentifier = bitstream;
      decoded++;
      if (qosflowdescriptions->qosflowdescriptionscontents[i]
              .parameterslist[j]
              .parameteridentifier == PARAMETER_IDENTIFIER_5QI) {
        DECODE_U8(buffer + decoded, bitstream, decoded);
        qosflowdescriptions->qosflowdescriptionscontents[i]
            .parameterslist[j]
            .parametercontents._5qi = bitstream;
      } else if (
          (qosflowdescriptions->qosflowdescriptionscontents[i]
               .parameterslist[j]
               .parameteridentifier == PARAMETER_IDENTIFIER_GFBR_UPLINK) ||
          (qosflowdescriptions->qosflowdescriptionscontents[i]
               .parameterslist[j]
               .parameteridentifier == PARAMETER_IDENTIFIER_GFBR_DOWNLINK) ||
          (qosflowdescriptions->qosflowdescriptionscontents[i]
               .parameterslist[j]
               .parameteridentifier == PARAMETER_IDENTIFIER_MFBR_UPLINK) ||
          (qosflowdescriptions->qosflowdescriptionscontents[i]
               .parameterslist[j]
               .parameteridentifier == PARAMETER_IDENTIFIER_MFBR_DOWNLINK)) {
        DECODE_U8(buffer + decoded, bitstream, decoded);
        qosflowdescriptions->qosflowdescriptionscontents[i]
            .parameterslist[j]
            .parametercontents.gfbrormfbr_uplinkordownlink.uint = bitstream;
        bit16stream = *(buffer + decoded);
        decoded++;
        bit16stream = (bit16stream << 8) + *(buffer + decoded);
        decoded++;
        qosflowdescriptions->qosflowdescriptionscontents[i]
            .parameterslist[j]
            .parametercontents.gfbrormfbr_uplinkordownlink.value = bit16stream;
      } else if (
          qosflowdescriptions->qosflowdescriptionscontents[i]
              .parameterslist[j]
              .parameteridentifier == PARAMETER_IDENTIFIER_AVERAGING_WINDOW) {
        DECODE_U8(buffer + decoded, bitstream, decoded);
        qosflowdescriptions->qosflowdescriptionscontents[i]
            .parameterslist[j]
            .parametercontents.averagingwindow.uplinkinmilliseconds = bitstream;
        DECODE_U8(buffer + decoded, bitstream, decoded);
        qosflowdescriptions->qosflowdescriptionscontents[i]
            .parameterslist[j]
            .parametercontents.averagingwindow.downlinkinmilliseconds =
            bitstream;
      } else if (
          qosflowdescriptions->qosflowdescriptionscontents[i]
              .parameterslist[j]
              .parameteridentifier ==
          PARAMETER_IDENTIFIER_EPS_BEARER_IDENTITY) {
        DECODE_U8(buffer + decoded, bitstream, decoded);
        qosflowdescriptions->qosflowdescriptionscontents[i]
            .parameterslist[j]
            .parametercontents.epsbeareridentity = bitstream >> 4;
      }
    }
  }

  return decoded;
}

void free_decode_qos_flow_descriptions(
    QOSFlowDescriptions* qosflowdescriptions) {
  for (int i = 0; i < qosflowdescriptions->qosflowdescriptionsnumber; i++) {
    free(qosflowdescriptions->qosflowdescriptionscontents[i].parameterslist);
    qosflowdescriptions->qosflowdescriptionscontents[i].parameterslist = NULL;
  }
  free(qosflowdescriptions->qosflowdescriptionscontents);
  qosflowdescriptions->qosflowdescriptionscontents = NULL;
}
