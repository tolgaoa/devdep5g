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
 \author
 \date 2021
 \email: contact@openairinterface.org
 */

#ifndef _UE_ASSOCIATION_LOGICAL_NG_CONNECTION_ITEM_H_
#define _UE_ASSOCIATION_LOGICAL_NG_CONNECTION_ITEM_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "RAN-UE-NGAP-ID.hpp"

extern "C" {
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_UE-associatedLogicalNG-connectionItem.h"
}

namespace ngap {

class UEAssociationLogicalNGConnectionItem {
 public:
  UEAssociationLogicalNGConnectionItem();
  virtual ~UEAssociationLogicalNGConnectionItem(){};

  void setAmfUeNgapId(unsigned long id);
  void setRanUeNgapId(uint32_t id);

  bool encode(Ngap_UE_associatedLogicalNG_connectionItem_t& item);
  bool encode(UEAssociationLogicalNGConnectionItem& item);
  bool decode(Ngap_UE_associatedLogicalNG_connectionItem_t* item);

 private:
  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
};

}  // namespace ngap
#endif
