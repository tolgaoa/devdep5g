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

#include "PDUSessionResourceReleaseListCmd.hpp"

#include <iostream>
using namespace std;

namespace ngap {
PDUSessionResourceReleaseListCmd::PDUSessionResourceReleaseListCmd() {
  pduSessionResourceReleaseItemCmd      = NULL;
  numofpduSessionResourceReleaseItemCmd = 0;
}
PDUSessionResourceReleaseListCmd::~PDUSessionResourceReleaseListCmd() {
  if (pduSessionResourceReleaseItemCmd)
    delete[] pduSessionResourceReleaseItemCmd;
}

void PDUSessionResourceReleaseListCmd::setPDUSessionResourceReleaseListCmd(
    PDUSessionResourceReleaseItemCmd* m_pduSessionResourceReleaseItemCmd,
    int num) {
  pduSessionResourceReleaseItemCmd      = m_pduSessionResourceReleaseItemCmd;
  numofpduSessionResourceReleaseItemCmd = num;
}
bool PDUSessionResourceReleaseListCmd::encode2PDUSessionResourceReleaseListCmd(
    Ngap_PDUSessionResourceToReleaseListRelCmd_t*
        pduSessionResourceReleaseListCmd) {
  for (int i = 0; i < numofpduSessionResourceReleaseItemCmd; i++) {
    Ngap_PDUSessionResourceToReleaseItemRelCmd_t* response =
        (Ngap_PDUSessionResourceToReleaseItemRelCmd_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceToReleaseItemRelCmd_t));
    if (!response) return false;
    if (!pduSessionResourceReleaseItemCmd[i]
             .encode2PDUSessionResourceReleaseItemCmd(response))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceReleaseListCmd->list, response) !=
        0)
      return false;
  }

  return true;
}
bool PDUSessionResourceReleaseListCmd::
    decodefromPDUSessionResourceReleaseListCmd(
        Ngap_PDUSessionResourceToReleaseListRelCmd_t*
            pduSessionResourceReleaseListCmd) {
  numofpduSessionResourceReleaseItemCmd =
      pduSessionResourceReleaseListCmd->list.count;
  pduSessionResourceReleaseItemCmd = new PDUSessionResourceReleaseItemCmd
      [numofpduSessionResourceReleaseItemCmd]();
  for (int i = 0; i < numofpduSessionResourceReleaseItemCmd; i++) {
    if (!pduSessionResourceReleaseItemCmd[i]
             .decodefromPDUSessionResourceReleaseItemCmd(
                 pduSessionResourceReleaseListCmd->list.array[i]))
      return false;
  }

  return true;
}
void PDUSessionResourceReleaseListCmd::getPDUSessionResourceReleaseListCmd(
    PDUSessionResourceReleaseItemCmd*& m_pduSessionResourceReleaseItemCmd,
    int& num) {
  m_pduSessionResourceReleaseItemCmd = pduSessionResourceReleaseItemCmd;
  num                                = numofpduSessionResourceReleaseItemCmd;
}

}  // namespace ngap
