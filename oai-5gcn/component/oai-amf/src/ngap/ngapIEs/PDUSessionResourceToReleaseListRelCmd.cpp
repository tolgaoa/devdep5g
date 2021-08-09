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

#include "PDUSessionResourceToReleaseListRelCmd.hpp"

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceToReleaseListRelCmd::PDUSessionResourceToReleaseListRelCmd() {
  pduSessionResourceToReleaseItemRelCmd = NULL;
  maxnoofPDUSessions                    = 0;
}

//------------------------------------------------------------------------------
PDUSessionResourceToReleaseListRelCmd::
    ~PDUSessionResourceToReleaseListRelCmd() {}

//------------------------------------------------------------------------------
void PDUSessionResourceToReleaseListRelCmd::
    setPDUSessionResourceToReleaseListRelCmd(
        PDUSessionResourceToReleaseItemRelCmd*
            m_pduSessionResourceToReleaseItemRelCmd,
        int num) {
  pduSessionResourceToReleaseItemRelCmd =
      m_pduSessionResourceToReleaseItemRelCmd;
  maxnoofPDUSessions = num;
}

//------------------------------------------------------------------------------
void PDUSessionResourceToReleaseListRelCmd::
    getPDUSessionResourceToReleaseListRelCmd(
        PDUSessionResourceToReleaseItemRelCmd*&
            m_pduSessionResourceToReleaseItemRelCmd,
        int& num) {
  m_pduSessionResourceToReleaseItemRelCmd =
      pduSessionResourceToReleaseItemRelCmd;
  num = maxnoofPDUSessions;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceToReleaseListRelCmd::
    encode2PDUSessionResourceToReleaseListRelCmd(
        Ngap_PDUSessionResourceToReleaseListRelCmd_t*
            pduSessionResourceToReleaseListRelCmd) {
  for (int i = 0; i < maxnoofPDUSessions; i++) {
    Ngap_PDUSessionResourceToReleaseItemRelCmd_t* rel =
        (Ngap_PDUSessionResourceToReleaseItemRelCmd_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceToReleaseItemRelCmd_t));
    if (!rel) return false;
    if (!pduSessionResourceToReleaseItemRelCmd[i]
             .encode2PDUSessionResourceToReleaseItemRelCmd(rel))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceToReleaseListRelCmd->list, rel) !=
        0)
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceToReleaseListRelCmd::
    decodefromPDUSessionResourceToReleaseListRelCmd(
        Ngap_PDUSessionResourceToReleaseListRelCmd_t*
            pduSessionResourceToReleaseListRelCmd) {
  maxnoofPDUSessions = pduSessionResourceToReleaseListRelCmd->list.count;
  pduSessionResourceToReleaseItemRelCmd =
      new PDUSessionResourceToReleaseItemRelCmd[maxnoofPDUSessions]();
  for (int i = 0; i < maxnoofPDUSessions; i++) {
    if (!pduSessionResourceToReleaseItemRelCmd[i]
             .decodefromPDUSessionResourceToReleaseItemRelCmd(
                 pduSessionResourceToReleaseListRelCmd->list.array[i]))
      return false;
  }
  return true;
}

}  // namespace ngap
