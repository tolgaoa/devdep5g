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

#ifndef _PDUSESSIONRESOURCEADMITTEDITEM_H_
#define _PDUSESSIONRESOURCEADMITTEDITEM_H_

#include "PDUSessionID.hpp"
#include "PDUSessionResourceHandoverRequestAckTransfer.hpp"
extern "C" {
#include "Ngap_PDUSessionResourceAdmittedItem.h"
}

namespace ngap {

class PDUSessionResourceAdmittedItem {
 public:
  PDUSessionResourceAdmittedItem();
  virtual ~PDUSessionResourceAdmittedItem();

  void setPDUSessionResourceAdmittedItem(
      PDUSessionID* m_pDUSessionID,
      OCTET_STRING_t m_handoverrequestAcktransfer);
  void getPDUSessionResourceAdmittedItem(
      PDUSessionID*& m_pDUSessionID,
      OCTET_STRING_t& m_handoverrequestAcktransfer);

  bool encode2PDUSessionResourceAdmittedItem(
      Ngap_PDUSessionResourceAdmittedItem_t* pdUSessionResourceAdmittedItem);
  bool decodefromPDUSessionResourceAdmittedItem(
      Ngap_PDUSessionResourceAdmittedItem_t* pdUSessionResourceAdmittedItem);

 private:
  PDUSessionID* pDUSessionID;
  OCTET_STRING_t handoverrequestAcktransfer;
};

}  // namespace ngap

#endif
