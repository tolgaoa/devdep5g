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

#include "TAI.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
TAI::TAI() {
  plmnId = NULL;
  tac    = NULL;
}

//------------------------------------------------------------------------------
TAI::~TAI() {}

//------------------------------------------------------------------------------
void TAI::setTAI(PlmnId* m_plmnId, TAC* m_tac) {
  plmnId = m_plmnId;
  tac    = m_tac;
}

//------------------------------------------------------------------------------
bool TAI::encode2TAI(Ngap_TAI_t* tai) {
  if (!plmnId->encode2octetstring(tai->pLMNIdentity)) return false;
  if (!tac->encode2octetstring(tai->tAC)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool TAI::decodefromTAI(Ngap_TAI_t* tai) {
  if (plmnId == nullptr) plmnId = new PlmnId();
  if (tac == nullptr) tac = new TAC();
  if (!plmnId->decodefromoctetstring(tai->pLMNIdentity)) return false;
  if (!tac->decodefromoctetstring(tai->tAC)) return false;

  return true;
}

//------------------------------------------------------------------------------
void TAI::getTAI(PlmnId*& m_plmnId, TAC*& m_tac) {
  m_plmnId = plmnId;
  m_tac    = tac;
}
}  // namespace ngap
