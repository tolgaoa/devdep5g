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

#include "IEsCriticalityDiagnostics.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
IEsCriticalityDiagnostics::IEsCriticalityDiagnostics() {}

//------------------------------------------------------------------------------
IEsCriticalityDiagnostics::~IEsCriticalityDiagnostics() {}

//------------------------------------------------------------------------------
void IEsCriticalityDiagnostics::setIeCriticality(
    Ngap_Criticality_t m_criticality) {
  criticality = m_criticality;
}

//------------------------------------------------------------------------------
void IEsCriticalityDiagnostics::setIeId(Ngap_ProtocolIE_ID_t m_protocolIE_ID) {
  protocolIE_ID = m_protocolIE_ID;
}

//------------------------------------------------------------------------------
void IEsCriticalityDiagnostics::setTypeOfError(
    Ngap_TypeOfError_t m_typeOfError) {
  typeOfError = m_typeOfError;
}

//------------------------------------------------------------------------------
void IEsCriticalityDiagnostics::encode2pdu(
    Ngap_CriticalityDiagnostics_IE_Item_t* IE_Item) {
  IE_Item->iECriticality = criticality;
  IE_Item->iE_ID         = protocolIE_ID;
  IE_Item->typeOfError   = typeOfError;
}

//------------------------------------------------------------------------------
void IEsCriticalityDiagnostics::decodefrompdu(
    Ngap_CriticalityDiagnostics_IE_Item_t* pdu) {
  criticality   = pdu->iECriticality;
  protocolIE_ID = pdu->iE_ID;
  typeOfError   = pdu->typeOfError;
}

//------------------------------------------------------------------------------
Ngap_Criticality_t IEsCriticalityDiagnostics::getIeCriticality() {
  return criticality;
}

//------------------------------------------------------------------------------
Ngap_ProtocolIE_ID_t IEsCriticalityDiagnostics::getIeId() {
  return protocolIE_ID;
}

//------------------------------------------------------------------------------
Ngap_TypeOfError_t IEsCriticalityDiagnostics::getTypeOfError() {
  return typeOfError;
}
}  // namespace ngap
