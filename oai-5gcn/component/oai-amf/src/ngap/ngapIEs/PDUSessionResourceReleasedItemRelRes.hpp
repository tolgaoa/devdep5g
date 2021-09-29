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
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _PDU_SESSION_RESOURCE_RELEASED_ITEM_REL_RES_H_
#define _PDU_SESSION_RESOURCE_RELEASED_ITEM_REL_RES_H_

#include "PDUSessionID.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceReleasedItemRelRes.h"
}

namespace ngap {

class PDUSessionResourceReleasedItemRelRes {
 public:
  PDUSessionResourceReleasedItemRelRes();
  virtual ~PDUSessionResourceReleasedItemRelRes();

  void setPDUSessionResourceReleasedItemRelRes(
      PDUSessionID* m_pDUSessionID,
      OCTET_STRING_t m_pDUSessionResourceReleaseResponseTransfer);

  void getPDUSessionResourceReleasedItemRelRes(
      PDUSessionID*& m_pDUSessionID,
      OCTET_STRING_t& m_pDUSessionResourceReleaseResponseTransfer);

  bool encode2PDUSessionResourceReleasedItemRelRes(
      Ngap_PDUSessionResourceReleasedItemRelRes_t*
          pduSessionResourceReleasedItemRelRes);
  bool decodefromPDUSessionResourceReleasedItemRelRes(
      Ngap_PDUSessionResourceReleasedItemRelRes_t*
          pduSessionResourceReleasedItemRelRes);

 private:
  PDUSessionID* pDUSessionID;
  OCTET_STRING_t pDUSessionResourceReleaseResponseTransfer;
  // ProtocolExtensionContainer (OPTIONAL)
};

}  // namespace ngap

#endif
