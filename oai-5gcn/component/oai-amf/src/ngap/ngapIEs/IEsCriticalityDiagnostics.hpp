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

#ifndef _IESCRITICALITYDIAGNOSTICS_H_
#define _IESCRITICALITYDIAGNOSTICS_H_

extern "C" {
#include "Ngap_CriticalityDiagnostics-IE-Item.h"
}

namespace ngap {

class IEsCriticalityDiagnostics {
 public:
  IEsCriticalityDiagnostics();
  virtual ~IEsCriticalityDiagnostics();

  void encode2pdu(Ngap_CriticalityDiagnostics_IE_Item_t*);
  void decodefrompdu(Ngap_CriticalityDiagnostics_IE_Item_t*);
  void setIeCriticality(Ngap_Criticality_t);
  void setIeId(Ngap_ProtocolIE_ID_t);
  void setTypeOfError(Ngap_TypeOfError_t);
  Ngap_Criticality_t getIeCriticality();
  Ngap_ProtocolIE_ID_t getIeId();
  Ngap_TypeOfError_t getTypeOfError();

 private:
  Ngap_Criticality_t criticality;
  Ngap_ProtocolIE_ID_t protocolIE_ID;
  Ngap_TypeOfError_t typeOfError;
};
}  // namespace ngap

#endif
