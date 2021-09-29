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

#ifndef _DRBSSUBJECTTOSTATUSTRANSFERLIST_H_
#define _DRBSSUBJECTTOSTATUSTRANSFERLIST_H_
#include "dRBsSubjectToStatusTransferItem.hpp"
extern "C" {
#include "Ngap_DRBsSubjectToStatusTransferList.h"
#include "asn_SEQUENCE_OF.h"
}
namespace ngap {
class dRBSubjectList {
 public:
  dRBSubjectList();
  virtual ~dRBSubjectList();
  void setdRBSubjectItem(dRBSubjectItem* m_item, int num);
  void getdRBSubjectItem(dRBSubjectItem*& m_item, int& num);
  bool decodefromdRBSubjectlist(
      Ngap_DRBsSubjectToStatusTransferList_t& DRBsSubjectToStatusTransferList);
  bool encodefromdRBSubjectlist(
      Ngap_DRBsSubjectToStatusTransferList_t& DRBsSubjectToStatusTransferList);

 private:
  dRBSubjectItem* drbsubjectitem;
  int numofitem;
};
}  // namespace ngap
#endif
