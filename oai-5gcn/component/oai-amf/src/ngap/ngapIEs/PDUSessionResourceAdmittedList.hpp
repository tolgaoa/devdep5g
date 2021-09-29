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

#ifndef _PDUSESSIONRESOURCEADMITTEDLIST_H_
#define _PDUSESSIONRESOURCEADMITTEDLIST_H_

#include "PDUSessionResourceAdmittedItem.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceAdmittedList.h"
}

namespace ngap {

class PDUSessionResourceAdmittedList {
 public:
  PDUSessionResourceAdmittedList();
  virtual ~PDUSessionResourceAdmittedList();

  void setPDUSessionResourceAdmittedList(
      PDUSessionResourceAdmittedItem* m_pduSessionResourceAdmittedItem,
      int num);
  void getPDUSessionResourceAdmittedList(
      PDUSessionResourceAdmittedItem*& m_pduSessionResourceAdmittedItem,
      int& num);

  bool encode2PDUSessionResourceAdmittedList(
      Ngap_PDUSessionResourceAdmittedList_t* pduSessionResourceAdmittedList);
  bool decodefromPDUSessionResourceAdmittedList(
      Ngap_PDUSessionResourceAdmittedList_t* pduSessionResourceAdmittedList);

 private:
  PDUSessionResourceAdmittedItem* pduSessionResourceAdmittedItem;
  int numofSessionResourceAdmittedItem;
};

}  // namespace ngap

#endif
