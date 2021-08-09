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

#include "PDUSessionResourceSetupListSURes.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupListSURes::PDUSessionResourceSetupListSURes() {
  pduSessionResourceSetupItemSURes      = NULL;
  numofpduSessionResourceSetupItemSURes = 0;
}

//------------------------------------------------------------------------------
PDUSessionResourceSetupListSURes::~PDUSessionResourceSetupListSURes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListSURes::setPDUSessionResourceSetupListSURes(
    PDUSessionResourceSetupItemSURes* m_pduSessionResourceSetupItemSURes,
    int num) {
  pduSessionResourceSetupItemSURes      = m_pduSessionResourceSetupItemSURes;
  numofpduSessionResourceSetupItemSURes = num;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListSURes::encode2PDUSessionResourceSetupListSURes(
    Ngap_PDUSessionResourceSetupListSURes_t* pduSessionResourceSetupListSURes) {
  for (int i = 0; i < numofpduSessionResourceSetupItemSURes; i++) {
    Ngap_PDUSessionResourceSetupItemSURes_t* response =
        (Ngap_PDUSessionResourceSetupItemSURes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemSURes_t));
    if (!response) return false;
    if (!pduSessionResourceSetupItemSURes[i]
             .encode2PDUSessionResourceSetupItemSURes(response))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListSURes->list, response) !=
        0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListSURes::
    decodefromPDUSessionResourceSetupListSURes(
        Ngap_PDUSessionResourceSetupListSURes_t*
            pduSessionResourceSetupListSURes) {
  numofpduSessionResourceSetupItemSURes =
      pduSessionResourceSetupListSURes->list.count;
  pduSessionResourceSetupItemSURes = new PDUSessionResourceSetupItemSURes
      [numofpduSessionResourceSetupItemSURes]();
  for (int i = 0; i < numofpduSessionResourceSetupItemSURes; i++) {
    if (!pduSessionResourceSetupItemSURes[i]
             .decodefromPDUSessionResourceSetupItemSURes(
                 pduSessionResourceSetupListSURes->list.array[i]))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListSURes::getPDUSessionResourceSetupListSURes(
    PDUSessionResourceSetupItemSURes*& m_pduSessionResourceSetupItemSURes,
    int& num) {
  m_pduSessionResourceSetupItemSURes = pduSessionResourceSetupItemSURes;
  num                                = numofpduSessionResourceSetupItemSURes;
}

}  // namespace ngap
