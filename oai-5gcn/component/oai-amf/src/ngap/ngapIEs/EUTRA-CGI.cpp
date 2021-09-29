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
#include "EUTRA-CGI.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
EUTRA_CGI::EUTRA_CGI() {
  plmnId            = NULL;
  eUTRACellIdentity = NULL;
}

//------------------------------------------------------------------------------
EUTRA_CGI::~EUTRA_CGI() {}

//------------------------------------------------------------------------------
void EUTRA_CGI::setEUTRA_CGI(
    PlmnId* m_plmnId, EUTRACellIdentity* m_eUTRACellIdentity) {
  plmnId            = m_plmnId;
  eUTRACellIdentity = m_eUTRACellIdentity;
}

//------------------------------------------------------------------------------
bool EUTRA_CGI::encode2EUTRA_CGI(Ngap_EUTRA_CGI_t* eutra_cgi) {
  if (!plmnId->encode2octetstring(eutra_cgi->pLMNIdentity)) return false;
  if (!eUTRACellIdentity->encode2bitstring(eutra_cgi->eUTRACellIdentity))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool EUTRA_CGI::decodefromEUTRA_CGI(Ngap_EUTRA_CGI_t* eutra_cgi) {
  if (plmnId == nullptr) plmnId = new PlmnId();
  if (eUTRACellIdentity == nullptr) eUTRACellIdentity = new EUTRACellIdentity();
  if (!plmnId->decodefromoctetstring(eutra_cgi->pLMNIdentity)) return false;
  if (!eUTRACellIdentity->decodefrombitstring(eutra_cgi->eUTRACellIdentity))
    return false;

  return true;
}

//------------------------------------------------------------------------------
void EUTRA_CGI::getEUTRA_CGI(
    PlmnId*& m_plmnId, EUTRACellIdentity*& m_eUTRACellIdentity) {
  m_plmnId            = plmnId;
  m_eUTRACellIdentity = eUTRACellIdentity;
}
}  // namespace ngap
