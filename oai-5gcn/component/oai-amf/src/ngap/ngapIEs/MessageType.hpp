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

#ifndef _MessageType_H_
#define _MessageType_H_

extern "C" {
#include "Ngap_Criticality.h"
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProcedureCode.h"
#include "Ngap_SuccessfulOutcome.h"
#include "Ngap_UnsuccessfulOutcome.h"
}

namespace ngap {

class MessageType {
 public:
  MessageType();
  virtual ~MessageType();

  int encode2pdu(Ngap_NGAP_PDU_t*&);
  void setProcedureCode(Ngap_ProcedureCode_t m_procedureCode);
  void setTypeOfMessage(Ngap_NGAP_PDU_PR m_typeOfMessage);
  void setCriticality(Ngap_Criticality m_criticality);
  void setValuePresent(Ngap_InitiatingMessage__value_PR m_valuePresent);
  void setValuePresent(Ngap_SuccessfulOutcome__value_PR m_valuePresent);
  void setValuePresent(Ngap_UnsuccessfulOutcome__value_PR m_valuePresent);

  // void decodefrompdu(Ngap_NGAP_PDU_t*);
  Ngap_ProcedureCode_t getProcedureCode();
  Ngap_NGAP_PDU_PR getTypeOfMessage();
  Ngap_Criticality getCriticality();

 private:
  Ngap_ProcedureCode_t procedureCode;  // Ngap_ProcedureCode.h
  Ngap_NGAP_PDU_PR typeOfMessage;      // Ngap_NGAP-PDU.h
  Ngap_Criticality criticality;        // Ngap_Criticality.h
  Ngap_InitiatingMessage__value_PR
      initiatingMsgValuePresent;  // Ngap_InitiatingMessage.h
  Ngap_SuccessfulOutcome__value_PR
      successfulOutcomeValuePresent;  // Ngap_SuccessfulOutcome.h
  Ngap_UnsuccessfulOutcome__value_PR
      unsuccessfulOutcomeValuePresent;  // Ngap_UnsuccessfulOutcome.h
};
}  // namespace ngap

#endif
