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

#ifndef _RESET_TYPE_H_
#define _RESET_TYPE_H_

#include "UEAssociationLogicalNGConnectionItem.hpp"

extern "C" {
#include "Ngap_ResetType.h"
}
#include <vector>

namespace ngap {
class ResetType {
 public:
  ResetType();
  virtual ~ResetType();

  void setResetType(long);
  void setResetType(std::vector<UEAssociationLogicalNGConnectionItem> list);
  bool encode(Ngap_ResetType_t* type);
  bool decode(Ngap_ResetType_t* type);

  void getResetType(long&);
  uint8_t getResetType();
  void getResetType(struct Ngap_UE_associatedLogicalNG_connectionList*&);

  void setUE_associatedLogicalNG_connectionList(
      const std::vector<UEAssociationLogicalNGConnectionItem> list);

 private:
  Ngap_ResetType_PR present;
  long nG_Interface;
  struct Ngap_UE_associatedLogicalNG_connectionList* partOfNG_Interface;
  UEAssociationLogicalNGConnectionItem* ueAssociationLogicalNGConnectionItem;
  //	struct Ngap_ProtocolIE_SingleContainer	*choice_Extensions;
};

}  // namespace ngap

#endif
