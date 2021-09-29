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
#ifndef _PDUSESSIONRESOURCERELEASEITEMCMD_H_
#define _PDUSESSIONRESOURCERELEASEITEMCMD_H_

#include "NAS-PDU.hpp"
#include "PDUSessionID.hpp"
#include "PDUSessionResourceReleaseCommandTransfer.hpp"
#include "S-NSSAI.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceToReleaseItemRelCmd.h"
}

namespace ngap {

class PDUSessionResourceReleaseItemCmd {
 public:
  PDUSessionResourceReleaseItemCmd();
  virtual ~PDUSessionResourceReleaseItemCmd();

  void setPDUSessionResourceReleaseItemCmd(
      PDUSessionID* m_pDUSessionID,
      OCTET_STRING_t m_pDUSessionResourceReleaseCommandTransfer);
  void getPDUSessionResourceReleaseItemCmd(
      PDUSessionID*& m_pDUSessionID,
      OCTET_STRING_t& m_pDUSessionResourceReleaseCommandTransfer);

  bool encode2PDUSessionResourceReleaseItemCmd(
      Ngap_PDUSessionResourceToReleaseItemRelCmd_t*
          pduSessionResourceReleaseCommandTransfer);
  bool decodefromPDUSessionResourceReleaseItemCmd(
      Ngap_PDUSessionResourceToReleaseItemRelCmd_t*
          pduSessionResourceReleaseCommandTransfer);

 private:
  PDUSessionID* pDUSessionID;
  OCTET_STRING_t pDUSessionResourceReleaseCommandTransfer;
};

}  // namespace ngap

#endif
