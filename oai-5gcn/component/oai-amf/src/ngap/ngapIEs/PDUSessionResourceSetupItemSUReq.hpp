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

#ifndef _PDUSESSIONRESOURCESETUPITEMSUREQ_H_
#define _PDUSESSIONRESOURCESETUPITEMSUREQ_H_

#include "NAS-PDU.hpp"
#include "PDUSessionID.hpp"
#include "S-NSSAI.hpp"
//#include "PDUSessionResourceSetupRequestTransfer.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupItemSUReq.h"
}

namespace ngap {

class PDUSessionResourceSetupItemSUReq {
 public:
  PDUSessionResourceSetupItemSUReq();
  virtual ~PDUSessionResourceSetupItemSUReq();

  void setPDUSessionResourceSetupItemSUReq(
      PDUSessionID* m_pDUSessionID, NAS_PDU* m_nAS_PDU, S_NSSAI* m_s_NSSAI,
      OCTET_STRING_t m_pDUSessionResourceSetupRequestTransfer);
  void getPDUSessionResourceSetupItemSUReq(
      PDUSessionID*& m_pDUSessionID, NAS_PDU*& m_nAS_PDU, S_NSSAI*& m_s_NSSAI,
      OCTET_STRING_t& m_pDUSessionResourceSetupRequestTransfer);

  bool encode2PDUSessionResourceSetupItemSUReq(
      Ngap_PDUSessionResourceSetupItemSUReq_t*
          pduSessionResourceSetupItemSUReq);
  bool decodefromPDUSessionResourceSetupItemSUReq(
      Ngap_PDUSessionResourceSetupItemSUReq_t*
          pduSessionResourceSetupItemSUReq);

 private:
  PDUSessionID* pDUSessionID;
  NAS_PDU* nAS_PDU; /* OPTIONAL */
  S_NSSAI* s_NSSAI;
  // PduSessionResourceSetupRequestTransferIE
  // *pduSessionResourceSetupRequestTransfer;
  OCTET_STRING_t pDUSessionResourceSetupRequestTransfer;
};

}  // namespace ngap

#endif
