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

#include "PDUSessionResourceSetupListHOReq.hpp"

#include <iostream>
using namespace std;

namespace ngap {
PDUSessionResourceSetupListHOReq::PDUSessionResourceSetupListHOReq() {
  pduSessionResourceSetupItemHOReq      = NULL;
  numofpduSessionResourceSetupItemHOReq = 0;
}
PDUSessionResourceSetupListHOReq::~PDUSessionResourceSetupListHOReq() {}

void PDUSessionResourceSetupListHOReq::setPDUSessionResourceSetupListHOReq(
    PDUSessionResourceSetupItemHOReq* m_pduSessionResourceSetupItemHOReq,
    int num) {
  pduSessionResourceSetupItemHOReq      = m_pduSessionResourceSetupItemHOReq;
  numofpduSessionResourceSetupItemHOReq = num;
}
bool PDUSessionResourceSetupListHOReq::encode2PDUSessionResourceSetupListHOReq(
    Ngap_PDUSessionResourceSetupListHOReq_t* pduSessionResourceSetupListHOReq) {
  for (int i = 0; i < numofpduSessionResourceSetupItemHOReq; i++) {
    Ngap_PDUSessionResourceSetupItemHOReq_t* request =
        (Ngap_PDUSessionResourceSetupItemHOReq_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceSetupItemHOReq_t));
    if (!request) return false;
    if (!pduSessionResourceSetupItemHOReq[i]
             .encode2PDUSessionResourceSetupItemHOReq(request))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceSetupListHOReq->list, request) != 0)
      return false;
  }

  return true;
}
bool PDUSessionResourceSetupListHOReq::
    decodefromPDUSessionResourceSetupListHOReq(
        Ngap_PDUSessionResourceSetupListHOReq_t*
            pduSessionResourceSetupListHOReq) {
  numofpduSessionResourceSetupItemHOReq =
      pduSessionResourceSetupListHOReq->list.count;
  pduSessionResourceSetupItemHOReq = new PDUSessionResourceSetupItemHOReq
      [numofpduSessionResourceSetupItemHOReq]();
  for (int i = 0; i < numofpduSessionResourceSetupItemHOReq; i++) {
    if (!pduSessionResourceSetupItemHOReq[i]
             .decodefromPDUSessionResourceSetupItemHOReq(
                 pduSessionResourceSetupListHOReq->list.array[i]))
      return false;
  }

  return true;
}
void PDUSessionResourceSetupListHOReq::getPDUSessionResourceSetupListHOReq(
    PDUSessionResourceSetupItemHOReq*& m_pduSessionResourceSetupItemHOReq,
    int& num) {
  m_pduSessionResourceSetupItemHOReq = pduSessionResourceSetupItemHOReq;
  num                                = numofpduSessionResourceSetupItemHOReq;
}

}  // namespace ngap
