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

/*! \file
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "CoreNetworkAssistanceInformation.hpp"

extern "C" {
#include "Ngap_TAIListForInactiveItem.h"
}

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
CoreNetworkAssistanceInfo::CoreNetworkAssistanceInfo() {
  ueIdentityIndexValue   = nullptr;
  pagingDRX              = nullptr;
  periodicRegUpdateTimer = nullptr;
  micoModeInd            = nullptr;
  tai                    = nullptr;
  numoftai               = 0;
}

//------------------------------------------------------------------------------
CoreNetworkAssistanceInfo::~CoreNetworkAssistanceInfo() {}

//------------------------------------------------------------------------------
void CoreNetworkAssistanceInfo::setCoreNetworkAssistanceInfo(
    UEIdentityIndexValue* m_ueIdentityIndexValue, DefaultPagingDRX* m_pagingDRX,
    PeriodicRegistrationUpdateTimer* m_periodicRegUpdateTimer,
    bool m_micoModeInd, TAI* m_tai, int m_numoftai) {
  ueIdentityIndexValue   = m_ueIdentityIndexValue;
  pagingDRX              = m_pagingDRX;
  periodicRegUpdateTimer = m_periodicRegUpdateTimer;
  if (m_micoModeInd) micoModeInd = new MICOModeIndication();
  tai      = m_tai;
  numoftai = m_numoftai;
}

//------------------------------------------------------------------------------
void CoreNetworkAssistanceInfo::setCoreNetworkAssistanceInfo(
    UEIdentityIndexValue* m_ueIdentityIndexValue,
    PeriodicRegistrationUpdateTimer* m_periodicRegUpdateTimer,
    bool m_micoModeInd, TAI* m_tai, int m_numoftai) {
  ueIdentityIndexValue   = m_ueIdentityIndexValue;
  periodicRegUpdateTimer = m_periodicRegUpdateTimer;
  if (m_micoModeInd) micoModeInd = new MICOModeIndication();
  tai      = m_tai;
  numoftai = m_numoftai;
}

//------------------------------------------------------------------------------
bool CoreNetworkAssistanceInfo::encode2CoreNetworkAssistanceInfo(
    Ngap_CoreNetworkAssistanceInformation_t* coreNetworkAssistanceInformation) {
  if (!ueIdentityIndexValue->encode2UEIdentityIndexValue(
          &coreNetworkAssistanceInformation->uEIdentityIndexValue))
    return false;
  if (!periodicRegUpdateTimer->encode2PeriodicRegistrationUpdateTimer(
          &coreNetworkAssistanceInformation->periodicRegistrationUpdateTimer))
    return false;
  for (int i = 0; i < numoftai; i++) {
    Ngap_TAIListForInactiveItem_t* taiListForInactiveItem =
        (Ngap_TAIListForInactiveItem_t*) calloc(
            1, sizeof(Ngap_TAIListForInactiveItem_t));
    if (!taiListForInactiveItem) return false;
    if (!tai[i].encode2TAI(&taiListForInactiveItem->tAI)) return false;
    if (ASN_SEQUENCE_ADD(
            &coreNetworkAssistanceInformation->tAIListForInactive.list,
            taiListForInactiveItem) != 0)
      return false;
  }
  if (pagingDRX) {
    Ngap_PagingDRX_t* pagingdrx =
        (Ngap_PagingDRX_t*) calloc(1, sizeof(Ngap_PagingDRX_t));
    if (!pagingdrx) return false;
    if (!pagingDRX->encode2DefaultPagingDRX(*pagingdrx)) return false;
    coreNetworkAssistanceInformation->uESpecificDRX = pagingdrx;
  }
  if (micoModeInd) {
    Ngap_MICOModeIndication_t* micomodeindication =
        (Ngap_MICOModeIndication_t*) calloc(
            1, sizeof(Ngap_MICOModeIndication_t));
    if (!micomodeindication) return false;
    if (!micoModeInd->encode2MICOModeIndication(micomodeindication))
      return false;
    coreNetworkAssistanceInformation->mICOModeIndication = micomodeindication;
  }

  return true;
}

//------------------------------------------------------------------------------
bool CoreNetworkAssistanceInfo::decodefromCoreNetworkAssistanceInfo(
    Ngap_CoreNetworkAssistanceInformation_t* coreNetworkAssistanceInformation) {
  if (ueIdentityIndexValue == nullptr)
    ueIdentityIndexValue = new UEIdentityIndexValue();
  if (periodicRegUpdateTimer == nullptr)
    periodicRegUpdateTimer = new PeriodicRegistrationUpdateTimer();
  if (!ueIdentityIndexValue->decodefromUEIdentityIndexValue(
          &coreNetworkAssistanceInformation->uEIdentityIndexValue))
    return false;
  if (!periodicRegUpdateTimer->decodefromPeriodicRegistrationUpdateTimer(
          &coreNetworkAssistanceInformation->periodicRegistrationUpdateTimer))
    return false;
  numoftai = coreNetworkAssistanceInformation->tAIListForInactive.list.count;
  tai      = new TAI[numoftai]();
  for (int i = 0; i < numoftai; i++) {
    if (!tai[i].decodefromTAI(
            &coreNetworkAssistanceInformation->tAIListForInactive.list.array[i]
                 ->tAI))
      return false;
  }
  if (coreNetworkAssistanceInformation->uESpecificDRX) {
    pagingDRX = new DefaultPagingDRX();
    if (!pagingDRX->decodefromDefaultPagingDRX(
            *(coreNetworkAssistanceInformation->uESpecificDRX)))
      return false;
  }
  if (coreNetworkAssistanceInformation->mICOModeIndication) {
    micoModeInd = new MICOModeIndication();
    if (!micoModeInd->decodefromMICOModeIndication(
            coreNetworkAssistanceInformation->mICOModeIndication))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void CoreNetworkAssistanceInfo::getCoreNetworkAssistanceInfo(
    UEIdentityIndexValue*& m_ueIdentityIndexValue,
    DefaultPagingDRX*& m_pagingDRX,
    PeriodicRegistrationUpdateTimer*& m_periodicRegUpdateTimer,
    bool& m_micoModeInd, TAI*& m_tai, int& m_numoftai) {
  m_ueIdentityIndexValue   = ueIdentityIndexValue;
  m_pagingDRX              = pagingDRX;
  m_periodicRegUpdateTimer = periodicRegUpdateTimer;
  if (micoModeInd)
    m_micoModeInd = true;
  else
    m_micoModeInd = false;
  m_tai      = tai;
  m_numoftai = numoftai;
}
}  // namespace ngap
