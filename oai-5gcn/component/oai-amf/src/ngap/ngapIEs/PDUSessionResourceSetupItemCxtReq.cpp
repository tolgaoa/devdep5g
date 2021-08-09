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

#include "PDUSessionResourceSetupItemCxtReq.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemCxtReq::PDUSessionResourceSetupItemCxtReq() {
  pDUSessionID = NULL;
  nAS_PDU      = NULL;
  s_NSSAI      = NULL;
}

//------------------------------------------------------------------------------
PDUSessionResourceSetupItemCxtReq::~PDUSessionResourceSetupItemCxtReq() {}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemCxtReq::setPDUSessionResourceSetupItemCxtReq(
    PDUSessionID* m_pDUSessionID, NAS_PDU* m_nAS_PDU, S_NSSAI* m_s_NSSAI,
    OCTET_STRING_t m_pDUSessionResourceSetupRequestTransfer) {
  pDUSessionID = m_pDUSessionID;
  nAS_PDU      = m_nAS_PDU;
  s_NSSAI      = m_s_NSSAI;
  pDUSessionResourceSetupRequestTransfer =
      m_pDUSessionResourceSetupRequestTransfer;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemCxtReq::
    encode2PDUSessionResourceSetupItemCxtReq(
        Ngap_PDUSessionResourceSetupItemCxtReq_t*
            pduSessionResourceSetupItemCxtReq) {
  cout << "encode2pdu  pdusession_id" << endl;
  if (!pDUSessionID->encode2PDUSessionID(
          pduSessionResourceSetupItemCxtReq->pDUSessionID))
    return false;
  cout << "encode2pdu  pdusession_id over" << endl;
  if (nAS_PDU) {
    cout << "encode2pdu  nas-pdu" << endl;
    Ngap_NAS_PDU_t* naspdu =
        (Ngap_NAS_PDU_t*) calloc(1, sizeof(Ngap_NAS_PDU_t));
    if (!naspdu) return false;
    if (!nAS_PDU->encode2octetstring(*naspdu)) {
      if (naspdu != nullptr) free(naspdu);
      return false;
    }
    pduSessionResourceSetupItemCxtReq->nAS_PDU = naspdu;
    cout << "encode2pdu  nas-pdu over" << endl;
  }
  cout << "encode2pdu  nssai" << endl;
  if (!s_NSSAI->encode2S_NSSAI(&pduSessionResourceSetupItemCxtReq->s_NSSAI))
    return false;
  cout << "encode2pdu  nssai over" << endl;
  pduSessionResourceSetupItemCxtReq->pDUSessionResourceSetupRequestTransfer =
      pDUSessionResourceSetupRequestTransfer;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionResourceSetupItemCxtReq::
    decodefromPDUSessionResourceSetupItemCxtReq(
        Ngap_PDUSessionResourceSetupItemCxtReq_t*
            pduSessionResourceSetupItemCxtReq) {
  if (pDUSessionID == nullptr) pDUSessionID = new PDUSessionID();
  if (s_NSSAI == nullptr) s_NSSAI = new S_NSSAI();
  if (!pDUSessionID->decodefromPDUSessionID(
          pduSessionResourceSetupItemCxtReq->pDUSessionID))
    return false;
  if (!s_NSSAI->decodefromS_NSSAI(
          &(pduSessionResourceSetupItemCxtReq->s_NSSAI)))
    return false;

  if (pduSessionResourceSetupItemCxtReq->nAS_PDU) {
    if (nAS_PDU == nullptr) nAS_PDU = new NAS_PDU();
    if (!nAS_PDU->decodefromoctetstring(
            *pduSessionResourceSetupItemCxtReq->nAS_PDU))
      return false;
  }

  pDUSessionResourceSetupRequestTransfer =
      pduSessionResourceSetupItemCxtReq->pDUSessionResourceSetupRequestTransfer;

  return true;
}

//------------------------------------------------------------------------------
void PDUSessionResourceSetupItemCxtReq::getPDUSessionResourceSetupItemCxtReq(
    PDUSessionID*& m_pDUSessionID, NAS_PDU*& m_nAS_PDU, S_NSSAI*& m_s_NSSAI,
    OCTET_STRING_t& m_pDUSessionResourceSetupRequestTransfer) {
  m_pDUSessionID = pDUSessionID;
  m_nAS_PDU      = nAS_PDU;
  m_s_NSSAI      = s_NSSAI;
  m_pDUSessionResourceSetupRequestTransfer =
      pDUSessionResourceSetupRequestTransfer;
}

}  // namespace ngap
