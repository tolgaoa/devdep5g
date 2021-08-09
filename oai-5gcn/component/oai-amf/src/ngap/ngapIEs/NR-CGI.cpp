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

#include "NR-CGI.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
NR_CGI::NR_CGI() {
  plmnId         = NULL;
  nRCellIdentity = NULL;
}

//------------------------------------------------------------------------------
NR_CGI::~NR_CGI() {}

//------------------------------------------------------------------------------
void NR_CGI::setNR_CGI(PlmnId* m_plmnId, NRCellIdentity* m_nRCellIdentity) {
  plmnId         = m_plmnId;
  nRCellIdentity = m_nRCellIdentity;
}

//------------------------------------------------------------------------------
bool NR_CGI::encode2NR_CGI(Ngap_NR_CGI_t* nr_cgi) {
  if (!plmnId->encode2octetstring(nr_cgi->pLMNIdentity)) return false;
  if (!nRCellIdentity->encode2bitstring(nr_cgi->nRCellIdentity)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool NR_CGI::decodefromNR_CGI(Ngap_NR_CGI_t* nr_cgi) {
  if (plmnId == nullptr) plmnId = new PlmnId();
  if (nRCellIdentity == nullptr) nRCellIdentity = new NRCellIdentity();
  if (!plmnId->decodefromoctetstring(nr_cgi->pLMNIdentity)) return false;
  if (!nRCellIdentity->decodefrombitstring(nr_cgi->nRCellIdentity))
    return false;

  return true;
}

//------------------------------------------------------------------------------
void NR_CGI::getNR_CGI(PlmnId*& m_plmnId, NRCellIdentity*& m_nRCellIdentity) {
  m_plmnId         = plmnId;
  m_nRCellIdentity = nRCellIdentity;
}
}  // namespace ngap
