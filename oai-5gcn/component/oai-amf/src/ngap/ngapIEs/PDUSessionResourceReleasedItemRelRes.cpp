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
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#include "PDUSessionResourceReleasedItemRelRes.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceReleasedItemRelRes::PDUSessionResourceReleasedItemRelRes() {
  pDUSessionID = NULL;
}

//------------------------------------------------------------------------------
PDUSessionResourceReleasedItemRelRes::~PDUSessionResourceReleasedItemRelRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceReleasedItemRelRes::
    setPDUSessionResourceReleasedItemRelRes(
        PDUSessionID* m_pDUSessionID,
        OCTET_STRING_t m_pDUSessionResourceReleaseResponseTransfer) {
  pDUSessionID = m_pDUSessionID;
  pDUSessionResourceReleaseResponseTransfer =
      m_pDUSessionResourceReleaseResponseTransfer;
}

//------------------------------------------------------------------------------
void PDUSessionResourceReleasedItemRelRes::
    getPDUSessionResourceReleasedItemRelRes(
        PDUSessionID*& m_pDUSessionID,
        OCTET_STRING_t& m_pDUSessionResourceReleaseResponseTransfer) {
  m_pDUSessionID = pDUSessionID;
  m_pDUSessionResourceReleaseResponseTransfer =
      pDUSessionResourceReleaseResponseTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceReleasedItemRelRes::
    encode2PDUSessionResourceReleasedItemRelRes(
        Ngap_PDUSessionResourceReleasedItemRelRes_t*
            pduSessionResourceReleasedItemRelRes) {
  if (!pDUSessionID->encode2PDUSessionID(
          pduSessionResourceReleasedItemRelRes->pDUSessionID))
    return false;

  pduSessionResourceReleasedItemRelRes
      ->pDUSessionResourceReleaseResponseTransfer =
      pDUSessionResourceReleaseResponseTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceReleasedItemRelRes::
    decodefromPDUSessionResourceReleasedItemRelRes(
        Ngap_PDUSessionResourceReleasedItemRelRes_t*
            pduSessionResourceReleasedItemRelRes) {
  if (pDUSessionID == nullptr) pDUSessionID = new PDUSessionID();
  pDUSessionID->setPDUSessionID(
      pduSessionResourceReleasedItemRelRes->pDUSessionID);
  pDUSessionResourceReleaseResponseTransfer =
      pduSessionResourceReleasedItemRelRes
          ->pDUSessionResourceReleaseResponseTransfer;

  return true;
}

}  // namespace ngap
