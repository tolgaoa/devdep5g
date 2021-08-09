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
#include "ServiceAreaList.h"

int encode_service_area_list(
    ServiceAreaList servicearealist, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  uint8_t* lenPtr;
  uint32_t encoded = 0;
  int listIndex = 0, elementIndex = 0;
  uint8_t octet = 0x0;

  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer, _5GS_TRACKING_AREA_IDENTITY_LIST_MINIMUM_LENGTH, len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  lenPtr = (buffer + encoded);
  encoded++;

  struct PartialServiceAreaList* ptr = servicearealist.partialServiceAreaList;

  for (; listIndex < servicearealist.listSize; listIndex++) {
    if (!ptr) break;
    switch ((ptr->typeOfList) & 0x03) {
      case LIST_OF_TACS_BELONGING_TO_ONE_PLMN_WITH_NON_CONSECUTIVE_TAC_VALUES:
        if (ptr->is_allowed)
          octet = 0x80 | ((ptr->numberOfElements) & 0x1f);
        else
          octet = 0x00 | ((ptr->numberOfElements) & 0x1f);
        ENCODE_U8(buffer + encoded, octet, encoded);
        octet = (uint8_t)((ptr->mcc_mnc->mcc) & 0x00ff);
        ENCODE_U8(buffer + encoded, octet, encoded);
        octet = (uint8_t)(((ptr->mcc_mnc->mcc) & 0x0f00) >> 8) |
                (uint8_t)(((ptr->mcc_mnc->mnc) & 0x0f00) >> 4);
        ENCODE_U8(buffer + encoded, octet, encoded);
        octet = (uint8_t)((ptr->mcc_mnc->mnc) & 0x00ff);
        ENCODE_U8(buffer + encoded, octet, encoded);

        struct TrackingAreaIdentity* ptrTai = ptr->tai;
        for (; elementIndex < ptr->numberOfElements; elementIndex++) {
          if (!ptrTai) break;
          ENCODE_U8(buffer + encoded, ptrTai->tac, encoded);
          ENCODE_U16(buffer + encoded, ptrTai->tacContinued, encoded);
          ptrTai = ptrTai->next;
        }
        elementIndex = 0;
        break;
      case LIST_OF_TACS_BELONGING_TO_ONE_PLMN_WITH_CONSECUTIVE_TAC_VALUES:
        if (ptr->is_allowed)
          octet = 0x80 | ((ptr->numberOfElements) & 0x1f) | 0x20;
        else
          octet = 0x00 | ((ptr->numberOfElements) & 0x1f) | 0x20;
        ENCODE_U8(buffer + encoded, octet, encoded);
        octet = (uint8_t)((ptr->mcc_mnc->mcc) & 0x00ff);
        ENCODE_U8(buffer + encoded, octet, encoded);
        octet = (uint8_t)(((ptr->mcc_mnc->mcc) & 0x0f00) >> 8) |
                (uint8_t)(((ptr->mcc_mnc->mnc) & 0x0f00) >> 4);
        ENCODE_U8(buffer + encoded, octet, encoded);
        octet = (uint8_t)((ptr->mcc_mnc->mnc) & 0x00ff);
        ENCODE_U8(buffer + encoded, octet, encoded);
        ENCODE_U8(buffer + encoded, ptr->tai->tac, encoded);
        ENCODE_U16(buffer + encoded, ptr->tai->tacContinued, encoded);
        break;
      case LIST_OF_TAIS_BELONGING_TO_DIFFERENT_PLMNS:
        if (ptr->is_allowed)
          octet = 0x80 | ((ptr->numberOfElements) & 0x1f) | 0x40;
        else
          octet = 0x00 | ((ptr->numberOfElements) & 0x1f) | 0x40;
        ENCODE_U8(buffer + encoded, octet, encoded);

        struct TrackingAreaIdentity* ptrTai3 = ptr->tai;
        struct MccMnc* ptrMccMnc             = ptr->mcc_mnc;
        for (; elementIndex < ptr->numberOfElements; elementIndex++) {
          if (!ptrTai3 | !ptrMccMnc) break;
          octet = (uint8_t)((ptrMccMnc->mcc) & 0x00ff);
          ENCODE_U8(buffer + encoded, octet, encoded);
          octet = (uint8_t)(((ptrMccMnc->mcc) & 0x0f00) >> 8) |
                  (uint8_t)(((ptrMccMnc->mnc) & 0x0f00) >> 4);
          ENCODE_U8(buffer + encoded, octet, encoded);
          octet = (uint8_t)((ptrMccMnc->mnc) & 0x00ff);
          ENCODE_U8(buffer + encoded, octet, encoded);
          ENCODE_U8(buffer + encoded, ptrTai3->tac, encoded);
          ENCODE_U16(buffer + encoded, ptrTai3->tacContinued, encoded);
          ptrTai    = ptrTai3->next;
          ptrMccMnc = ptrMccMnc->next;
        }
        elementIndex = 0;
        break;
      case ALL_TAIS_BELONGING_TO_THE_PLMNS_IN_THE_REGISTRATION_AREA_ARE_IN_THE_ALLOWED_AREA:
        if (ptr->is_allowed)
          octet = 0x80 | ((ptr->numberOfElements) & 0x1f) | 0x60;
        else
          octet = 0x00 | ((ptr->numberOfElements) & 0x1f) | 0x60;
        ENCODE_U8(buffer + encoded, octet, encoded);

        octet = (uint8_t)((ptr->mcc_mnc->mcc) & 0x00ff);
        ENCODE_U8(buffer + encoded, octet, encoded);
        octet = (uint8_t)(((ptr->mcc_mnc->mcc) & 0x0f00) >> 8) |
                (uint8_t)(((ptr->mcc_mnc->mnc) & 0x0f00) >> 4);
        ENCODE_U8(buffer + encoded, octet, encoded);
        octet = (uint8_t)((ptr->mcc_mnc->mnc) & 0x00ff);
        ENCODE_U8(buffer + encoded, octet, encoded);
        break;
    }
    ptr = ptr->next;
  }

  *lenPtr = encoded - 1 - ((iei > 0) ? 1 : 0);
  return encoded;
}

int decode_service_area_list(
    ServiceAreaList* servicearealist, uint8_t iei, uint8_t* buffer,
    uint32_t len) {
  printf("decode_service_area_list\n");
  int decoded           = 0;
  uint8_t ielen         = 0;
  uint8_t octet         = 0x0;
  uint16_t tacContinued = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  ielen = *(buffer + decoded);
  printf("decode_service_area_list ielen(%d)\n", ielen);
  decoded++;
  CHECK_LENGTH_DECODER(len - decoded, ielen);

  servicearealist->listSize                                 = 0;
  struct PartialServiceAreaList* lastPartialServiceAreaList = NULL;

  while (len - decoded > 0) {
    if ((ielen + 2) == decoded) break;
    DECODE_U8(buffer + decoded, octet, decoded);
    servicearealist->listSize += 1;
    struct PartialServiceAreaList* partialServiceAreaList =
        (struct PartialServiceAreaList*) calloc(
            1, sizeof(struct PartialServiceAreaList));

    if (((octet & 0x80) >> 8) == 0x01)
      partialServiceAreaList->is_allowed = true;
    else
      partialServiceAreaList->is_allowed = true;
    switch ((octet & 0x60) >> 5) {
      case LIST_OF_TACS_BELONGING_TO_ONE_PLMN_WITH_NON_CONSECUTIVE_TAC_VALUES:
        partialServiceAreaList->typeOfList       = 0x00;
        partialServiceAreaList->numberOfElements = octet & 0x1f;

        DECODE_U8(buffer + decoded, octet, decoded);
        struct MccMnc* mcc_mnc =
            (struct MccMnc*) calloc(1, sizeof(struct MccMnc));
        mcc_mnc->mcc = 0x0000 | octet;
        DECODE_U8(buffer + decoded, octet, decoded);
        mcc_mnc->mcc |= ((octet & 0x0f) << 8);
        mcc_mnc->mnc = 0x0000 | ((octet & 0xf0) << 4);
        DECODE_U8(buffer + decoded, octet, decoded);
        mcc_mnc->mnc |= octet;
        mcc_mnc->next                   = NULL;
        partialServiceAreaList->mcc_mnc = mcc_mnc;

        int elementIndex                     = 0;
        struct TrackingAreaIdentity* lastTai = NULL;
        for (; elementIndex < partialServiceAreaList->numberOfElements;
             elementIndex++) {
          struct TrackingAreaIdentity* tai =
              (struct TrackingAreaIdentity*) calloc(
                  1, sizeof(struct TrackingAreaIdentity));
          DECODE_U8(buffer + decoded, octet, decoded);
          tai->tac = octet;
          DECODE_U16(buffer + decoded, tacContinued, decoded);
          tai->tacContinued = tacContinued;
          if (lastTai)
            lastTai->next = tai;
          else
            partialServiceAreaList->tai = tai;
          lastTai = tai;
        }
        if (lastTai) lastTai->next = NULL;
        break;
      case LIST_OF_TACS_BELONGING_TO_ONE_PLMN_WITH_CONSECUTIVE_TAC_VALUES:
        printf(
            "LIST_OF_TACS_BELONGING_TO_ONE_PLMN_WITH_CONSECUTIVE_TAC_VALUES\n");
        partialServiceAreaList->typeOfList       = 0x01;
        partialServiceAreaList->numberOfElements = octet & 0x1f;

        DECODE_U8(buffer + decoded, octet, decoded);
        struct MccMnc* mcc_mnc2 =
            (struct MccMnc*) calloc(1, sizeof(struct MccMnc));
        mcc_mnc2->mcc = 0x0000 | octet;
        DECODE_U8(buffer + decoded, octet, decoded);
        mcc_mnc2->mcc |= ((octet & 0x0f) << 8);
        mcc_mnc2->mnc = 0x0000 | ((octet & 0xf0) << 4);
        DECODE_U8(buffer + decoded, octet, decoded);
        mcc_mnc2->mnc |= octet;
        mcc_mnc2->next                  = NULL;
        partialServiceAreaList->mcc_mnc = mcc_mnc2;

        struct TrackingAreaIdentity* tai =
            (struct TrackingAreaIdentity*) calloc(
                1, sizeof(struct TrackingAreaIdentity));
        DECODE_U8(buffer + decoded, octet, decoded);
        tai->tac = octet;
        DECODE_U16(buffer + decoded, tacContinued, decoded);
        tai->tacContinued           = tacContinued;
        tai->next                   = NULL;
        partialServiceAreaList->tai = tai;
        break;
      case LIST_OF_TAIS_BELONGING_TO_DIFFERENT_PLMNS:
        partialServiceAreaList->typeOfList       = 0x10;
        partialServiceAreaList->numberOfElements = octet & 0x1f;

        elementIndex                          = 0;
        struct TrackingAreaIdentity* lastTai3 = NULL;
        struct MccMnc* last_mcc_mnc           = NULL;
        for (; elementIndex < partialServiceAreaList->numberOfElements;
             elementIndex++) {
          DECODE_U8(buffer + decoded, octet, decoded);
          struct MccMnc* mcc_mnc =
              (struct MccMnc*) calloc(1, sizeof(struct MccMnc));
          mcc_mnc->mcc = 0x0000 | octet;
          DECODE_U8(buffer + decoded, octet, decoded);
          mcc_mnc->mcc |= ((octet & 0x0f) << 8);
          mcc_mnc->mnc = 0x0000 | ((octet & 0xf0) << 4);
          DECODE_U8(buffer + decoded, octet, decoded);
          mcc_mnc->mnc |= octet;
          if (last_mcc_mnc)
            last_mcc_mnc->next = mcc_mnc;
          else
            partialServiceAreaList->mcc_mnc = mcc_mnc;
          last_mcc_mnc = mcc_mnc;

          struct TrackingAreaIdentity* tai =
              (struct TrackingAreaIdentity*) calloc(
                  1, sizeof(struct TrackingAreaIdentity));
          DECODE_U8(buffer + decoded, octet, decoded);
          tai->tac = octet;
          DECODE_U16(buffer + decoded, tacContinued, decoded);
          tai->tacContinued = tacContinued;
          if (lastTai3)
            lastTai3->next = tai;
          else
            partialServiceAreaList->tai = tai;
          lastTai3 = tai;
        }
        if (last_mcc_mnc) last_mcc_mnc->next = NULL;
        if (lastTai3) lastTai3->next = NULL;
        break;
      case ALL_TAIS_BELONGING_TO_THE_PLMNS_IN_THE_REGISTRATION_AREA_ARE_IN_THE_ALLOWED_AREA:
        partialServiceAreaList->typeOfList       = 0x11;
        partialServiceAreaList->numberOfElements = octet & 0x1f;
        DECODE_U8(buffer + decoded, octet, decoded);
        struct MccMnc* mcc_mnc4 =
            (struct MccMnc*) calloc(1, sizeof(struct MccMnc));
        mcc_mnc4->mcc = 0x0000 | octet;
        DECODE_U8(buffer + decoded, octet, decoded);
        mcc_mnc4->mcc |= ((octet & 0x0f) << 8);
        mcc_mnc4->mnc = 0x0000 | ((octet & 0xf0) << 4);
        DECODE_U8(buffer + decoded, octet, decoded);
        mcc_mnc4->mnc |= octet;
        partialServiceAreaList->mcc_mnc = mcc_mnc4;
        break;
    }
    if (lastPartialServiceAreaList)
      lastPartialServiceAreaList->next = partialServiceAreaList;
    else
      servicearealist->partialServiceAreaList = partialServiceAreaList;
    lastPartialServiceAreaList = partialServiceAreaList;
  }
  return decoded;
}
