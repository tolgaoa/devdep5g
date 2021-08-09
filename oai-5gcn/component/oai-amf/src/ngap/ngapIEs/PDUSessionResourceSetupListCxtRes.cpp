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

#include "PDUSessionResourceSetupListCxtRes.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupListCxtRes::PDUSessionResourceSetupListCxtRes() {
  pduSessionResourceSetupItemCxtRes      = NULL;
  numofpduSessionResourceSetupItemCxtRes = 0;
}

//------------------------------------------------------------------------------
PDUSessionResourceSetupListCxtRes::~PDUSessionResourceSetupListCxtRes() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListCxtRes::setPDUSessionResourceSetupListCxtRes(
    PDUSessionResourceSetupItemCxtRes* m_pduSessionResourceSetupItemCxtRes,
    int num) {
  pduSessionResourceSetupItemCxtRes      = m_pduSessionResourceSetupItemCxtRes;
  numofpduSessionResourceSetupItemCxtRes = num;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListCxtRes::
    encode2PDUSessionResourceSetupListCxtRes(
        Ngap_PDUSessionResourceSetupListCxtRes_t*
            pduSessionResourceSetupListCxtRes) {
  for (int i = 0; i < numofpduSessionResourceSetupItemCxtRes; i++) {
    Ngap_PDUSessionResourceSetupItemCxtRes_t* response =
        (Ngap_PDUSessionResourceSetupItemCxtRes_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemCxtRes_t));
    if (!response) return false;
    if (!pduSessionResourceSetupItemCxtRes[i]
             .encode2PDUSessionResourceSetupItemCxtRes(response))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListCxtRes->list, response) !=
        0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListCxtRes::
    decodefromPDUSessionResourceSetupListCxtRes(
        Ngap_PDUSessionResourceSetupListCxtRes_t*
            pduSessionResourceSetupListCxtRes) {
  numofpduSessionResourceSetupItemCxtRes =
      pduSessionResourceSetupListCxtRes->list.count;
  pduSessionResourceSetupItemCxtRes = new PDUSessionResourceSetupItemCxtRes
      [numofpduSessionResourceSetupItemCxtRes]();
  for (int i = 0; i < numofpduSessionResourceSetupItemCxtRes; i++) {
    if (!pduSessionResourceSetupItemCxtRes[i]
             .decodefromPDUSessionResourceSetupItemCxtRes(
                 pduSessionResourceSetupListCxtRes->list.array[i]))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListCxtRes::getPDUSessionResourceSetupListCxtRes(
    PDUSessionResourceSetupItemCxtRes*& m_pduSessionResourceSetupItemCxtRes,
    int& num) {
  m_pduSessionResourceSetupItemCxtRes = pduSessionResourceSetupItemCxtRes;
  num                                 = numofpduSessionResourceSetupItemCxtRes;
}

}  // namespace ngap
