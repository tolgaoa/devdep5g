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

#include "UERadioCapabilityForPaging.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UERadioCapabilityForPaging::UERadioCapabilityForPaging() {
  ueRadioCapabilityForPagingOfNR    = NULL;
  ueRadioCapabilityForPagingOfEUTRA = NULL;
}

//------------------------------------------------------------------------------
UERadioCapabilityForPaging::~UERadioCapabilityForPaging() {}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPaging::encode2UERadioCapabilityForPaging(
    Ngap_UERadioCapabilityForPaging_t* ueRadioCapabilityForPaging) {
  if (ueRadioCapabilityForPagingOfNR) {
    ueRadioCapabilityForPaging->uERadioCapabilityForPagingOfNR =
        (Ngap_UERadioCapabilityForPagingOfNR_t*) calloc(
            1, sizeof(Ngap_UERadioCapabilityForPagingOfNR_t));
    if (!ueRadioCapabilityForPagingOfNR->encode2UERadioCapabilityForPagingOfNR(
            ueRadioCapabilityForPaging->uERadioCapabilityForPagingOfNR))
      return false;
  }
  if (ueRadioCapabilityForPagingOfEUTRA) {
    ueRadioCapabilityForPaging->uERadioCapabilityForPagingOfEUTRA =
        (Ngap_UERadioCapabilityForPagingOfEUTRA_t*) calloc(
            1, sizeof(Ngap_UERadioCapabilityForPagingOfEUTRA_t));
    if (!ueRadioCapabilityForPagingOfEUTRA
             ->encode2UERadioCapabilityForPagingOfEUTRA(
                 ueRadioCapabilityForPaging->uERadioCapabilityForPagingOfEUTRA))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPaging::decodefromUERadioCapabilityForPaging(
    Ngap_UERadioCapabilityForPaging_t* ueRadioCapabilityForPaging) {
  if (ueRadioCapabilityForPaging->uERadioCapabilityForPagingOfNR) {
    ueRadioCapabilityForPagingOfNR = new UERadioCapabilityForPagingOfNR();
    if (!ueRadioCapabilityForPagingOfNR
             ->decodefromUERadioCapabilityForPagingOfNR(
                 ueRadioCapabilityForPaging->uERadioCapabilityForPagingOfNR))
      return false;
  }
  if (ueRadioCapabilityForPaging->uERadioCapabilityForPagingOfEUTRA) {
    ueRadioCapabilityForPagingOfEUTRA = new UERadioCapabilityForPagingOfEUTRA();
    if (!ueRadioCapabilityForPagingOfEUTRA
             ->decodefromUERadioCapabilityForPagingOfEUTRA(
                 ueRadioCapabilityForPaging->uERadioCapabilityForPagingOfEUTRA))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPaging::getUERadioCapabilityForPaging(
    UERadioCapabilityForPagingOfNR*& m_ueRadioCapabilityForPagingOfNR,
    UERadioCapabilityForPagingOfEUTRA*& m_ueRadioCapabilityForPagingOfEUTRA) {
  m_ueRadioCapabilityForPagingOfNR    = ueRadioCapabilityForPagingOfNR;
  m_ueRadioCapabilityForPagingOfEUTRA = ueRadioCapabilityForPagingOfEUTRA;

  return true;
}

//------------------------------------------------------------------------------
void UERadioCapabilityForPaging::setUERadioCapabilityForPaging(
    UERadioCapabilityForPagingOfNR* m_ueRadioCapabilityForPagingOfNR,
    UERadioCapabilityForPagingOfEUTRA* m_ueRadioCapabilityForPagingOfEUTRA) {
  ueRadioCapabilityForPagingOfNR    = m_ueRadioCapabilityForPagingOfNR;
  ueRadioCapabilityForPagingOfEUTRA = m_ueRadioCapabilityForPagingOfEUTRA;
}
}  // namespace ngap
