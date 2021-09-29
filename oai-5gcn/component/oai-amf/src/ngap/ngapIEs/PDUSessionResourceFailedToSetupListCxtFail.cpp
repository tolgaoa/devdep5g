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

#include "PDUSessionResourceFailedToSetupListCxtFail.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupListCxtFail::
    PDUSessionResourceFailedToSetupListCxtFail() {
  pduSessionResourceFailedToSetupItemCxtFail      = NULL;
  numofpduSessionResourceFailedToSetupItemCxtFail = 0;
}

//------------------------------------------------------------------------------
PDUSessionResourceFailedToSetupListCxtFail::
    ~PDUSessionResourceFailedToSetupListCxtFail() {}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupListCxtFail::
    setPDUSessionResourceFailedToSetupListCxtFail(
        PDUSessionResourceFailedToSetupItemCxtFail*
            m_pduSessionResourceFailedToSetupItemCxtFail,
        int num) {
  pduSessionResourceFailedToSetupItemCxtFail =
      m_pduSessionResourceFailedToSetupItemCxtFail;
  numofpduSessionResourceFailedToSetupItemCxtFail = num;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupListCxtFail::
    encode2PDUSessionResourceFailedToSetupListCxtFail(
        Ngap_PDUSessionResourceFailedToSetupListCxtFail_t*
            pduSessionResourceFailedToSetupListCxtFail) {
  for (int i = 0; i < numofpduSessionResourceFailedToSetupItemCxtFail; i++) {
    Ngap_PDUSessionResourceFailedToSetupItemCxtFail_t* failedToFailure =
        (Ngap_PDUSessionResourceFailedToSetupItemCxtFail_t*) calloc(
            1, sizeof(Ngap_PDUSessionResourceFailedToSetupItemCxtFail_t));
    if (!failedToFailure) return false;
    if (!pduSessionResourceFailedToSetupItemCxtFail[i]
             .encode2PDUSessionResourceFailedToSetupItemCxtFail(
                 failedToFailure))
      return false;
    if (ASN_SEQUENCE_ADD(
            &pduSessionResourceFailedToSetupListCxtFail->list,
            failedToFailure) != 0)
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceFailedToSetupListCxtFail::
    decodefromPDUSessionResourceFailedToSetupListCxtFail(
        Ngap_PDUSessionResourceFailedToSetupListCxtFail_t*
            pduSessionResourceFailedToSetupListCxtFail) {
  numofpduSessionResourceFailedToSetupItemCxtFail =
      pduSessionResourceFailedToSetupListCxtFail->list.count;
  pduSessionResourceFailedToSetupItemCxtFail =
      new PDUSessionResourceFailedToSetupItemCxtFail
          [numofpduSessionResourceFailedToSetupItemCxtFail]();
  for (int i = 0; i < numofpduSessionResourceFailedToSetupItemCxtFail; i++) {
    if (!pduSessionResourceFailedToSetupItemCxtFail[i]
             .decodefromPDUSessionResourceFailedToSetupItemCxtFail(
                 pduSessionResourceFailedToSetupListCxtFail->list.array[i]))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceFailedToSetupListCxtFail::
    getPDUSessionResourceFailedToSetupListCxtFail(
        PDUSessionResourceFailedToSetupItemCxtFail*&
            m_pduSessionResourceFailedToSetupItemCxtFail,
        int& num) {
  m_pduSessionResourceFailedToSetupItemCxtFail =
      pduSessionResourceFailedToSetupItemCxtFail;
  num = numofpduSessionResourceFailedToSetupItemCxtFail;
}

}  // namespace ngap
