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

#ifndef _PDUSESSIONRESOURCESETUPITEMHOREQ_H_
#define _PDUSESSIONRESOURCESETUPITEMHOREQ_H_

#include "NAS-PDU.hpp"
#include "PDUSessionID.hpp"
#include "PDUSessionResourceSetupRequestTransfer.hpp"
#include "S-NSSAI.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceSetupItemHOReq.h"
}

namespace ngap {

class PDUSessionResourceSetupItemHOReq {
 public:
  PDUSessionResourceSetupItemHOReq();
  virtual ~PDUSessionResourceSetupItemHOReq();

  void setPDUSessionResourceSetupItemHOReq(
      PDUSessionID* m_pDUSessionID, S_NSSAI* m_s_NSSAI,
      OCTET_STRING_t m_pDUSessionResourceSetupRequestTransfer);
  void getPDUSessionResourceSetupItemHOReq(
      PDUSessionID*& m_pDUSessionID, S_NSSAI*& m_s_NSSAI,
      OCTET_STRING_t& m_pDUSessionResourceSetupRequestTransfer);

  bool encode2PDUSessionResourceSetupItemHOReq(
      Ngap_PDUSessionResourceSetupItemHOReq_t*
          pduSessionResourceSetupItemHOReq);
  bool decodefromPDUSessionResourceSetupItemHOReq(
      Ngap_PDUSessionResourceSetupItemHOReq_t*
          pduSessionResourceSetupItemHOReq);

 private:
  PDUSessionID* pDUSessionID;
  S_NSSAI* s_NSSAI;
  OCTET_STRING_t pDUSessionResourceSetupRequestTransfer;
};

}  // namespace ngap

#endif
