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

#include "PDUSessionResourceToReleaseItemRelCmd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceToReleaseItemRelCmd::PDUSessionResourceToReleaseItemRelCmd() {
  pDUSessionID = NULL;
}

//------------------------------------------------------------------------------
PDUSessionResourceToReleaseItemRelCmd::
    ~PDUSessionResourceToReleaseItemRelCmd() {}

//------------------------------------------------------------------------------
void PDUSessionResourceToReleaseItemRelCmd::
    setPDUSessionResourceToReleaseItemRelCmd(
        PDUSessionID* m_pDUSessionID,
        OCTET_STRING_t m_pDUSessionResourceReleaseCommandTransfer) {
  pDUSessionID = m_pDUSessionID;
  pDUSessionResourceReleaseCommandTransfer =
      m_pDUSessionResourceReleaseCommandTransfer;
}

//------------------------------------------------------------------------------
void PDUSessionResourceToReleaseItemRelCmd::
    getPDUSessionResourceToReleaseItemRelCmd(
        PDUSessionID*& m_pDUSessionID,
        OCTET_STRING_t& m_pDUSessionResourceReleaseCommandTransfer) {
  m_pDUSessionID = pDUSessionID;
  m_pDUSessionResourceReleaseCommandTransfer =
      pDUSessionResourceReleaseCommandTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceToReleaseItemRelCmd::
    encode2PDUSessionResourceToReleaseItemRelCmd(
        Ngap_PDUSessionResourceToReleaseItemRelCmd_t*
            pduSessionResourceToReleaseItemRelCmd) {
  if (!pDUSessionID->encode2PDUSessionID(
          pduSessionResourceToReleaseItemRelCmd->pDUSessionID))
    return false;

  pduSessionResourceToReleaseItemRelCmd
      ->pDUSessionResourceReleaseCommandTransfer =
      pDUSessionResourceReleaseCommandTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceToReleaseItemRelCmd::
    decodefromPDUSessionResourceToReleaseItemRelCmd(
        Ngap_PDUSessionResourceToReleaseItemRelCmd_t*
            pduSessionResourceToReleaseItemRelCmd) {
  if (pDUSessionID == nullptr) pDUSessionID = new PDUSessionID();

  pDUSessionResourceReleaseCommandTransfer =
      pduSessionResourceToReleaseItemRelCmd
          ->pDUSessionResourceReleaseCommandTransfer;

  return true;
}

}  // namespace ngap
