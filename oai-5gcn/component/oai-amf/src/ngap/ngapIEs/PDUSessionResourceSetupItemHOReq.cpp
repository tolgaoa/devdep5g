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

#include "PDUSessionResourceSetupItemHOReq.hpp"

#include <iostream>
using namespace std;

namespace ngap {
PDUSessionResourceSetupItemHOReq::PDUSessionResourceSetupItemHOReq() {
  pDUSessionID = NULL;
  s_NSSAI      = NULL;
}
PDUSessionResourceSetupItemHOReq::~PDUSessionResourceSetupItemHOReq() {}

void PDUSessionResourceSetupItemHOReq::setPDUSessionResourceSetupItemHOReq(
    PDUSessionID* m_pDUSessionID, S_NSSAI* m_s_NSSAI,
    OCTET_STRING_t m_pDUSessionResourceSetupRequestTransfer) {
  pDUSessionID = m_pDUSessionID;
  s_NSSAI      = m_s_NSSAI;
  pDUSessionResourceSetupRequestTransfer =
      m_pDUSessionResourceSetupRequestTransfer;
}
bool PDUSessionResourceSetupItemHOReq::encode2PDUSessionResourceSetupItemHOReq(
    Ngap_PDUSessionResourceSetupItemHOReq_t* pduSessionResourceSetupItemHOReq) {
  if (!pDUSessionID->encode2PDUSessionID(
          pduSessionResourceSetupItemHOReq->pDUSessionID))
    return false;
  if (!s_NSSAI->encode2S_NSSAI(&pduSessionResourceSetupItemHOReq->s_NSSAI))
    return false;
  pduSessionResourceSetupItemHOReq->handoverRequestTransfer =
      pDUSessionResourceSetupRequestTransfer;

  return true;
}
bool PDUSessionResourceSetupItemHOReq::
    decodefromPDUSessionResourceSetupItemHOReq(
        Ngap_PDUSessionResourceSetupItemHOReq_t*
            pduSessionResourceSetupItemHOReq) {
  if (pDUSessionID == nullptr) pDUSessionID = new PDUSessionID();
  s_NSSAI = new S_NSSAI();
  if (!pDUSessionID->decodefromPDUSessionID(
          pduSessionResourceSetupItemHOReq->pDUSessionID))
    return false;
  if (!s_NSSAI->decodefromS_NSSAI(&pduSessionResourceSetupItemHOReq->s_NSSAI))
    return false;
  pDUSessionResourceSetupRequestTransfer =
      pduSessionResourceSetupItemHOReq->handoverRequestTransfer;

  return true;
}
void PDUSessionResourceSetupItemHOReq::getPDUSessionResourceSetupItemHOReq(
    PDUSessionID*& m_pDUSessionID, S_NSSAI*& m_s_NSSAI,
    OCTET_STRING_t& m_pDUSessionResourceSetupRequestTransfer) {
  m_pDUSessionID = pDUSessionID;
  m_s_NSSAI      = s_NSSAI;
  m_pDUSessionResourceSetupRequestTransfer =
      pDUSessionResourceSetupRequestTransfer;
}

}  // namespace ngap
