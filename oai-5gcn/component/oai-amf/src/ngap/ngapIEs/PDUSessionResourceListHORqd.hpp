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

#ifndef _PDUSESSIONRESOURCELISTHORqd_H_
#define _PDUSESSIONRESOURCELISTHORqd_H_

#include "PDUSessionResourceItemHORqd.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceListHORqd.h"
}

namespace ngap {

class PDUSessionResourceListHORqd {
 public:
  PDUSessionResourceListHORqd();
  virtual ~PDUSessionResourceListHORqd();

  void setPDUSessionResourceListHORqd(
      PDUSessionResourceItemHORqd* m_pduSessionResourceItemHORqd, int num);
  void getPDUSessionResourceListHORqd(
      PDUSessionResourceItemHORqd*& m_pduSessionResourceItemHORqd, int& num);

  bool encode2PDUSessionResourceListHORqd(
      Ngap_PDUSessionResourceListHORqd_t* pduSessionResourceListHORQqd);
  bool decodefromPDUSessionResourceListHORqd(
      Ngap_PDUSessionResourceListHORqd_t* pduSessionResourceListHORQqd);

 private:
  PDUSessionResourceItemHORqd* pduSessionResourceItemHORqd;
  int numofpduSessionResourceItemHORqd;
};

}  // namespace ngap

#endif
