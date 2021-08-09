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

#include "PDUSessionResourceSetupListSUReq.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupListSUReq::PDUSessionResourceSetupListSUReq() {
  pduSessionResourceSetupItemSUReq      = NULL;
  numofpduSessionResourceSetupItemSUReq = 0;
}

//------------------------------------------------------------------------------
PDUSessionResourceSetupListSUReq::~PDUSessionResourceSetupListSUReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListSUReq::setPDUSessionResourceSetupListSUReq(
    PDUSessionResourceSetupItemSUReq* m_pduSessionResourceSetupItemSUReq,
    int num) {
  pduSessionResourceSetupItemSUReq      = m_pduSessionResourceSetupItemSUReq;
  numofpduSessionResourceSetupItemSUReq = num;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListSUReq::encode2PDUSessionResourceSetupListSUReq(
    Ngap_PDUSessionResourceSetupListSUReq_t* pduSessionResourceSetupListSUReq) {
  for (int i = 0; i < numofpduSessionResourceSetupItemSUReq; i++) {
    Ngap_PDUSessionResourceSetupItemSUReq_t* request =
        (Ngap_PDUSessionResourceSetupItemSUReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemSUReq_t));
    if (!request) return false;
    if (!pduSessionResourceSetupItemSUReq[i]
             .encode2PDUSessionResourceSetupItemSUReq(request))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListSUReq->list, request) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListSUReq::
    decodefromPDUSessionResourceSetupListSUReq(
        Ngap_PDUSessionResourceSetupListSUReq_t*
            pduSessionResourceSetupListSUReq) {
  numofpduSessionResourceSetupItemSUReq =
      pduSessionResourceSetupListSUReq->list.count;
  pduSessionResourceSetupItemSUReq = new PDUSessionResourceSetupItemSUReq
      [numofpduSessionResourceSetupItemSUReq]();
  for (int i = 0; i < numofpduSessionResourceSetupItemSUReq; i++) {
    if (!pduSessionResourceSetupItemSUReq[i]
             .decodefromPDUSessionResourceSetupItemSUReq(
                 pduSessionResourceSetupListSUReq->list.array[i]))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListSUReq::getPDUSessionResourceSetupListSUReq(
    PDUSessionResourceSetupItemSUReq*& m_pduSessionResourceSetupItemSUReq,
    int& num) {
  m_pduSessionResourceSetupItemSUReq = pduSessionResourceSetupItemSUReq;
  num                                = numofpduSessionResourceSetupItemSUReq;
}

}  // namespace ngap
