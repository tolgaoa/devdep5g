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
 \author  niuxiansheng-niu, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */
#include "PDUSessionResourceItemHORqd.hpp"

#include <iostream>
using namespace std;

namespace ngap {
PDUSessionResourceItemHORqd::PDUSessionResourceItemHORqd() {
  pDUSessionID = NULL;
}
PDUSessionResourceItemHORqd::~PDUSessionResourceItemHORqd() {}

void PDUSessionResourceItemHORqd::setPDUSessionResourceItemHORqd(
    PDUSessionID* m_pDUSessionID, OCTET_STRING_t m_handoverrequiredtransfer) {
  pDUSessionID             = m_pDUSessionID;
  handoverrequiredtransfer = m_handoverrequiredtransfer;
}
bool PDUSessionResourceItemHORqd::encode2PDUSessionResourceItemHORqd(
    Ngap_PDUSessionResourceItemHORqd_t* pdUSessionResourceItemHORqd) {
  if (!pDUSessionID) return false;
  if (!pDUSessionID->encode2PDUSessionID(
          pdUSessionResourceItemHORqd->pDUSessionID))
    return false;
  pdUSessionResourceItemHORqd->handoverRequiredTransfer =
      handoverrequiredtransfer;

  return true;
}
bool PDUSessionResourceItemHORqd::decodefromPDUSessionResourceItemHORqd(
    Ngap_PDUSessionResourceItemHORqd_t* pdUSessionResourceItemHORqd) {
  if (pDUSessionID == nullptr) pDUSessionID = new PDUSessionID();
  if (!pDUSessionID->decodefromPDUSessionID(
          pdUSessionResourceItemHORqd->pDUSessionID))
    return false;
  handoverrequiredtransfer =
      pdUSessionResourceItemHORqd->handoverRequiredTransfer;

  return true;
}
void PDUSessionResourceItemHORqd::getPDUSessionResourceItemHORqd(
    PDUSessionID*& m_pDUSessionID, OCTET_STRING_t& m_handoverrequiredtransfer) {
  m_pDUSessionID             = pDUSessionID;
  m_handoverrequiredtransfer = handoverrequiredtransfer;
}

}  // namespace ngap
