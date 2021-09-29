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

#include "PDUSessionResourceListHORqd.hpp"

#include <iostream>
using namespace std;

namespace ngap {
PDUSessionResourceListHORqd::PDUSessionResourceListHORqd() {
  pduSessionResourceItemHORqd      = NULL;
  numofpduSessionResourceItemHORqd = 0;
}
PDUSessionResourceListHORqd::~PDUSessionResourceListHORqd() {}

void PDUSessionResourceListHORqd::setPDUSessionResourceListHORqd(
    PDUSessionResourceItemHORqd* m_pduSessionResourceItemHORqd, int num) {
  pduSessionResourceItemHORqd      = m_pduSessionResourceItemHORqd;
  numofpduSessionResourceItemHORqd = num;
}
bool PDUSessionResourceListHORqd::encode2PDUSessionResourceListHORqd(
    Ngap_PDUSessionResourceListHORqd_t* pduSessionResourceListHORQqd) {
  for (int i = 0; i < numofpduSessionResourceItemHORqd; i++) {
    Ngap_PDUSessionResourceItemHORqd_t* response =
        (Ngap_PDUSessionResourceItemHORqd_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceItemHORqd_t));
    if (!response) return false;
    if (!pduSessionResourceItemHORqd[i].encode2PDUSessionResourceItemHORqd(
            response))
      return false;
    if (ASN_SEQUENCE_ADD(&pduSessionResourceListHORQqd->list, response) != 0)
      return false;
  }

  return true;
}
bool PDUSessionResourceListHORqd::decodefromPDUSessionResourceListHORqd(
    Ngap_PDUSessionResourceListHORqd_t* pduSessionResourceListHORQqd) {
  numofpduSessionResourceItemHORqd = pduSessionResourceListHORQqd->list.count;
  pduSessionResourceItemHORqd =
      new PDUSessionResourceItemHORqd[numofpduSessionResourceItemHORqd]();
  for (int i = 0; i < numofpduSessionResourceItemHORqd; i++) {
    if (!pduSessionResourceItemHORqd[i].decodefromPDUSessionResourceItemHORqd(
            pduSessionResourceListHORQqd->list.array[i]))
      return false;
  }

  return true;
}
void PDUSessionResourceListHORqd::getPDUSessionResourceListHORqd(
    PDUSessionResourceItemHORqd*& m_pduSessionResourceItemHORqd, int& num) {
  m_pduSessionResourceItemHORqd = pduSessionResourceItemHORqd;
  num                           = numofpduSessionResourceItemHORqd;
}

}  // namespace ngap
