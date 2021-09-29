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

#ifndef _CRITICALITYDIAGNOSTICS_H_
#define _CRITICALITYDIAGNOSTICS_H_

#include "IEsCriticalityDiagnostics.hpp"

extern "C" {
#include "Ngap_CriticalityDiagnostics-IE-List.h"
#include "Ngap_CriticalityDiagnostics.h"
#include "Ngap_NGSetupFailure.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {

class CriticalityDiagnostics {
 public:
  CriticalityDiagnostics();
  virtual ~CriticalityDiagnostics();

  int encode2pdu(Ngap_NGSetupFailure_t*);
  bool decodefrompdu(Ngap_CriticalityDiagnostics_t*);
  void setProcedureCodeValue(Ngap_ProcedureCode_t);
  void setTriggeringMessageValue(Ngap_TriggeringMessage_t);
  void setCriticalityValue(Ngap_Criticality_t);
  void setIEsCriticalityDiagnosticsList(
      IEsCriticalityDiagnostics* m_iEsCriticalityDiagnostics, int num);
  bool getProcedureCodeValue(Ngap_ProcedureCode_t&);
  bool getTriggeringMessageValue(Ngap_TriggeringMessage_t&);
  bool getCriticalityValue(Ngap_Criticality_t&);
  bool getIEsCriticalityDiagnosticsList(
      IEsCriticalityDiagnostics*& m_iEsCriticalityDiagnostics, int& num);

 private:
  Ngap_ProcedureCode_t procedureCode;         /* OPTIONAL */
  Ngap_TriggeringMessage_t triggeringMessage; /* OPTIONAL */
  Ngap_Criticality_t procedureCriticality;    /* OPTIONAL */
  // Ngap_CriticalityDiagnostics_IE_List_t iEsCriticalityDiagnostics;	/*
  // OPTIONAL */
  IEsCriticalityDiagnostics* iEsCriticalityDiagnostics;

  bool procedureCodeIsSet;
  bool triggeringMessageIsSet;
  bool procedureCriticalityIsSet;
  int numberOfIEsCriticalityDiagnostics;
};
}  // namespace ngap

#endif
