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

#ifndef _CORENETWORKASSISTANCEINFORMATION_H_
#define _CORENETWORKASSISTANCEINFORMATION_H_

#include "DefaultPagingDRX.hpp"
#include "MICOModeIndication.hpp"
#include "PeriodicRegistrationUpdateTimer.hpp"
#include "TAI.hpp"
#include "UEIdentityIndexValue.hpp"

extern "C" {
#include "Ngap_CoreNetworkAssistanceInformation.h"
}

namespace ngap {

class CoreNetworkAssistanceInfo {
 public:
  CoreNetworkAssistanceInfo();
  virtual ~CoreNetworkAssistanceInfo();

  void setCoreNetworkAssistanceInfo(
      UEIdentityIndexValue* m_ueIdentityIndexValue,
      DefaultPagingDRX* m_pagingDRX,
      PeriodicRegistrationUpdateTimer* m_periodicRegUpdateTimer,
      bool m_micoModeInd, TAI* m_tai, int m_numoftai);
  void setCoreNetworkAssistanceInfo(
      UEIdentityIndexValue* m_ueIdentityIndexValue,
      PeriodicRegistrationUpdateTimer* m_periodicRegUpdateTimer,
      bool m_micoModeInd, TAI* m_tai, int m_numoftai);
  void getCoreNetworkAssistanceInfo(
      UEIdentityIndexValue*& m_ueIdentityIndexValue,
      DefaultPagingDRX*& m_pagingDRX,
      PeriodicRegistrationUpdateTimer*& m_periodicRegUpdateTimer,
      bool& m_micoModeInd, TAI*& m_tai, int& m_numoftai);

  bool encode2CoreNetworkAssistanceInfo(Ngap_CoreNetworkAssistanceInformation_t*
                                            coreNetworkAssistanceInformation);
  bool decodefromCoreNetworkAssistanceInfo(
      Ngap_CoreNetworkAssistanceInformation_t*
          coreNetworkAssistanceInformation);

 private:
  UEIdentityIndexValue* ueIdentityIndexValue;
  DefaultPagingDRX* pagingDRX;
  PeriodicRegistrationUpdateTimer* periodicRegUpdateTimer;
  MICOModeIndication* micoModeInd;
  TAI* tai;
  int numoftai;
};

}  // namespace ngap

#endif
