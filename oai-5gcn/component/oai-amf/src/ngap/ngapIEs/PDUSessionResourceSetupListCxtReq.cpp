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

#include "PDUSessionResourceSetupListCxtReq.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupListCxtReq::PDUSessionResourceSetupListCxtReq() {
  pduSessionResourceSetupItemCxtReq      = NULL;
  numofpduSessionResourceSetupItemCxtReq = 0;
}

//------------------------------------------------------------------------------
PDUSessionResourceSetupListCxtReq::~PDUSessionResourceSetupListCxtReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListCxtReq::setPDUSessionResourceSetupListCxtReq(
    PDUSessionResourceSetupItemCxtReq* m_pduSessionResourceSetupItemCxtReq,
    int num) {
  pduSessionResourceSetupItemCxtReq      = m_pduSessionResourceSetupItemCxtReq;
  numofpduSessionResourceSetupItemCxtReq = num;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListCxtReq::
    encode2PDUSessionResourceSetupListCxtReq(
        Ngap_PDUSessionResourceSetupListCxtReq_t*
            pduSessionResourceSetupListCxtReq) {
  for (int i = 0; i < numofpduSessionResourceSetupItemCxtReq; i++) {
    cout << "encoding items" << endl;
    Ngap_PDUSessionResourceSetupItemCxtReq_t* request =
        (Ngap_PDUSessionResourceSetupItemCxtReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemCxtReq_t));
    if (!request) return false;
    if (!pduSessionResourceSetupItemCxtReq[i]
             .encode2PDUSessionResourceSetupItemCxtReq(request))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListCxtReq->list, request) !=
        0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupListCxtReq::
    decodefromPDUSessionResourceSetupListCxtReq(
        Ngap_PDUSessionResourceSetupListCxtReq_t*
            pduSessionResourceSetupListCxtReq) {
  numofpduSessionResourceSetupItemCxtReq =
      pduSessionResourceSetupListCxtReq->list.count;
  pduSessionResourceSetupItemCxtReq = new PDUSessionResourceSetupItemCxtReq
      [numofpduSessionResourceSetupItemCxtReq]();
  for (int i = 0; i < numofpduSessionResourceSetupItemCxtReq; i++) {
    if (!pduSessionResourceSetupItemCxtReq[i]
             .decodefromPDUSessionResourceSetupItemCxtReq(
                 pduSessionResourceSetupListCxtReq->list.array[i]))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupListCxtReq::getPDUSessionResourceSetupListCxtReq(
    PDUSessionResourceSetupItemCxtReq*& m_pduSessionResourceSetupItemCxtReq,
    int& num) {
  m_pduSessionResourceSetupItemCxtReq = pduSessionResourceSetupItemCxtReq;
  num                                 = numofpduSessionResourceSetupItemCxtReq;
}

}  // namespace ngap
