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

#include "CriticalityDiagnostics.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
CriticalityDiagnostics::CriticalityDiagnostics() {
  procedureCodeIsSet                = false;
  triggeringMessageIsSet            = false;
  procedureCriticalityIsSet         = false;
  numberOfIEsCriticalityDiagnostics = 0;
  iEsCriticalityDiagnostics         = nullptr;
}

//------------------------------------------------------------------------------
CriticalityDiagnostics::~CriticalityDiagnostics() {}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setProcedureCodeValue(
    Ngap_ProcedureCode_t m_procedureCode) {
  procedureCodeIsSet = true;
  procedureCode      = m_procedureCode;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setTriggeringMessageValue(
    Ngap_TriggeringMessage_t m_triggeringMessage) {
  triggeringMessageIsSet = true;
  triggeringMessage      = m_triggeringMessage;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setCriticalityValue(
    Ngap_Criticality_t m_procedureCriticality) {
  procedureCriticalityIsSet = true;
  procedureCriticality      = m_procedureCriticality;
}

//------------------------------------------------------------------------------
void CriticalityDiagnostics::setIEsCriticalityDiagnosticsList(
    IEsCriticalityDiagnostics* m_iEsCriticalityDiagnostics, int num) {
  iEsCriticalityDiagnostics         = m_iEsCriticalityDiagnostics;
  numberOfIEsCriticalityDiagnostics = num;
}

//------------------------------------------------------------------------------
int CriticalityDiagnostics::encode2pdu(Ngap_NGSetupFailure_t* ngSetupFailure) {
  Ngap_NGSetupFailureIEs_t* ie =
      (Ngap_NGSetupFailureIEs_t*) calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_CriticalityDiagnostics;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupFailureIEs__value_PR_CriticalityDiagnostics;

  if (procedureCodeIsSet) {
    Ngap_ProcedureCode_t* procedureCodeIE =
        (Ngap_ProcedureCode_t*) calloc(1, sizeof(Ngap_ProcedureCode_t));
    *procedureCodeIE                                      = procedureCode;
    ie->value.choice.CriticalityDiagnostics.procedureCode = procedureCodeIE;
  }
  if (triggeringMessageIsSet) {
    Ngap_TriggeringMessage_t* triggeringMessageIE =
        (Ngap_TriggeringMessage_t*) calloc(1, sizeof(Ngap_TriggeringMessage_t));
    *triggeringMessageIE = triggeringMessage;
    ie->value.choice.CriticalityDiagnostics.triggeringMessage =
        triggeringMessageIE;
  }
  if (procedureCriticalityIsSet) {
    Ngap_Criticality_t* procedureCriticalityIE =
        (Ngap_Criticality_t*) calloc(1, sizeof(Ngap_Criticality_t));
    *procedureCriticalityIE = procedureCriticality;
    ie->value.choice.CriticalityDiagnostics.procedureCriticality =
        procedureCriticalityIE;
  }
  if (numberOfIEsCriticalityDiagnostics) {
    Ngap_CriticalityDiagnostics_IE_List_t* ieList =
        (Ngap_CriticalityDiagnostics_IE_List_t*) calloc(
            1, sizeof(Ngap_CriticalityDiagnostics_IE_List_t));
    for (int i = 0; i < numberOfIEsCriticalityDiagnostics; i++) {
      Ngap_CriticalityDiagnostics_IE_Item_t* ieItem =
          (Ngap_CriticalityDiagnostics_IE_Item_t*) calloc(
              1, sizeof(Ngap_CriticalityDiagnostics_IE_Item_t));
      iEsCriticalityDiagnostics[i].encode2pdu(ieItem);
      ASN_SEQUENCE_ADD(&ieList->list, ieItem);
    }
    ie->value.choice.CriticalityDiagnostics.iEsCriticalityDiagnostics = ieList;
  }
  if (!procedureCodeIsSet && !triggeringMessageIsSet &&
      !procedureCriticalityIsSet && !numberOfIEsCriticalityDiagnostics) {
    free(ie);
    return 1;
  }
  int ret = ASN_SEQUENCE_ADD(&ngSetupFailure->protocolIEs.list, ie);
  if (ret != 0) cout << "encode CriticalityDiagnostics IE error" << endl;
  return ret;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::decodefrompdu(Ngap_CriticalityDiagnostics_t* pdu) {
  // timeValue = *pdu;
  if (pdu->procedureCode) {
    procedureCodeIsSet = true;
    procedureCode      = *pdu->procedureCode;
  }
  if (pdu->triggeringMessage) {
    triggeringMessageIsSet = true;
    triggeringMessage      = *pdu->triggeringMessage;
  }
  if (pdu->procedureCriticality) {
    procedureCriticalityIsSet = true;
    procedureCriticality      = *pdu->procedureCriticality;
  }
  if (pdu->iEsCriticalityDiagnostics) {
    numberOfIEsCriticalityDiagnostics =
        pdu->iEsCriticalityDiagnostics->list.count;
    iEsCriticalityDiagnostics =
        new IEsCriticalityDiagnostics[numberOfIEsCriticalityDiagnostics]();
    for (int i = 0; i < numberOfIEsCriticalityDiagnostics; i++) {
      iEsCriticalityDiagnostics[i].decodefrompdu(
          pdu->iEsCriticalityDiagnostics->list.array[i]);
    }
  }
  if (!procedureCodeIsSet && !triggeringMessageIsSet &&
      !procedureCriticalityIsSet && !numberOfIEsCriticalityDiagnostics) {
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getProcedureCodeValue(
    Ngap_ProcedureCode_t& m_procedureCode) {
  m_procedureCode = procedureCode;
  return procedureCodeIsSet;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getTriggeringMessageValue(
    Ngap_TriggeringMessage_t& m_triggeringMessage) {
  m_triggeringMessage = triggeringMessage;
  return triggeringMessageIsSet;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getCriticalityValue(
    Ngap_Criticality_t& m_procedureCriticality) {
  m_procedureCriticality = procedureCriticality;
  return procedureCriticalityIsSet;
}

//------------------------------------------------------------------------------
bool CriticalityDiagnostics::getIEsCriticalityDiagnosticsList(
    IEsCriticalityDiagnostics*& m_iEsCriticalityDiagnostics, int& num) {
  m_iEsCriticalityDiagnostics = iEsCriticalityDiagnostics;
  num                         = numberOfIEsCriticalityDiagnostics;

  if (numberOfIEsCriticalityDiagnostics)
    return true;
  else
    return false;
}
}  // namespace ngap
