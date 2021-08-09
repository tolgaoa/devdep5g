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

#include "PDUSessionResourceSetupItemSURes.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemSURes::PDUSessionResourceSetupItemSURes() {
  pDUSessionID = NULL;
}

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemSURes::~PDUSessionResourceSetupItemSURes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemSURes::setPDUSessionResourceSetupItemSURes(
    PDUSessionID* m_pDUSessionID,
    OCTET_STRING_t m_pDUSessionResourceSetupResponseTransfer) {
  pDUSessionID = m_pDUSessionID;
  pDUSessionResourceSetupResponseTransfer =
      m_pDUSessionResourceSetupResponseTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemSURes::encode2PDUSessionResourceSetupItemSURes(
    Ngap_PDUSessionResourceSetupItemSURes_t* pduSessionResourceSetupItemSURes) {
  if (!pDUSessionID) return false;
  if (!pDUSessionID->encode2PDUSessionID(
          pduSessionResourceSetupItemSURes->pDUSessionID))
    return false;
  pduSessionResourceSetupItemSURes->pDUSessionResourceSetupResponseTransfer =
      pDUSessionResourceSetupResponseTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemSURes::
    decodefromPDUSessionResourceSetupItemSURes(
        Ngap_PDUSessionResourceSetupItemSURes_t*
            pduSessionResourceSetupItemSURes) {
  if (pDUSessionID == nullptr) pDUSessionID = new PDUSessionID();
  if (!pDUSessionID->decodefromPDUSessionID(
          pduSessionResourceSetupItemSURes->pDUSessionID))
    return false;
  pDUSessionResourceSetupResponseTransfer =
      pduSessionResourceSetupItemSURes->pDUSessionResourceSetupResponseTransfer;

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemSURes::getPDUSessionResourceSetupItemSURes(
    PDUSessionID*& m_pDUSessionID,
    OCTET_STRING_t& m_pDUSessionResourceSetupResponseTransfer) {
  m_pDUSessionID = pDUSessionID;
  m_pDUSessionResourceSetupResponseTransfer =
      pDUSessionResourceSetupResponseTransfer;
}

}  // namespace ngap
