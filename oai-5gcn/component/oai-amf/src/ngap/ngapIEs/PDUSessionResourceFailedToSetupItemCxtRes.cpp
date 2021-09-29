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

#include "PDUSessionResourceFailedToSetupItemCxtRes.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupItemCxtRes::
    PDUSessionResourceFailedToSetupItemCxtRes() {
  pDUSessionID = NULL;
}

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupItemCxtRes::
    ~PDUSessionResourceFailedToSetupItemCxtRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupItemCxtRes::
    setPDUSessionResourceFailedToSetupItemCxtRes(
        PDUSessionID* m_pDUSessionID,
        OCTET_STRING_t m_pDUSessionResourceSetupUnsuccessfulTransfer) {
  pDUSessionID = m_pDUSessionID;
  pDUSessionResourceSetupUnsuccessfulTransfer =
      m_pDUSessionResourceSetupUnsuccessfulTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupItemCxtRes::
    encode2PDUSessionResourceFailedToSetupItemCxtRes(
        Ngap_PDUSessionResourceFailedToSetupItemCxtRes_t*
            pduSessionResourceFailedToSetupItemCxtRes) {
  if (!pDUSessionID) return false;
  if (!pDUSessionID->encode2PDUSessionID(
          pduSessionResourceFailedToSetupItemCxtRes->pDUSessionID))
    return false;
  pduSessionResourceFailedToSetupItemCxtRes
      ->pDUSessionResourceSetupUnsuccessfulTransfer =
      pDUSessionResourceSetupUnsuccessfulTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupItemCxtRes::
    decodefromPDUSessionResourceFailedToSetupItemCxtRes(
        Ngap_PDUSessionResourceFailedToSetupItemCxtRes_t*
            pduSessionResourceFailedToSetupItemCxtRes) {
  if (pDUSessionID == nullptr) pDUSessionID = new PDUSessionID();
  if (!pDUSessionID->decodefromPDUSessionID(
          pduSessionResourceFailedToSetupItemCxtRes->pDUSessionID))
    return false;
  pDUSessionResourceSetupUnsuccessfulTransfer =
      pduSessionResourceFailedToSetupItemCxtRes
          ->pDUSessionResourceSetupUnsuccessfulTransfer;

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupItemCxtRes::
    getPDUSessionResourceFailedToSetupItemCxtRes(
        PDUSessionID*& m_pDUSessionID,
        OCTET_STRING_t& m_pDUSessionResourceSetupUnsuccessfulTransfer) {
  m_pDUSessionID = pDUSessionID;
  pDUSessionResourceSetupUnsuccessfulTransfer =
      pDUSessionResourceSetupUnsuccessfulTransfer;
}

}  // namespace ngap
