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

#include "IntegrityProtectionResult.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
IntegrityProtectionResult::IntegrityProtectionResult() {
  integrityProtectionResult = -1;
}

//------------------------------------------------------------------------------
IntegrityProtectionResult::~IntegrityProtectionResult() {}

//------------------------------------------------------------------------------
void IntegrityProtectionResult::setIntegrityProtectionResult(
    e_Ngap_IntegrityProtectionResult m_integrityProtectionResult) {
  integrityProtectionResult = m_integrityProtectionResult;
}

//------------------------------------------------------------------------------
bool IntegrityProtectionResult::getIntegrityProtectionResult(
    long& m_integrityProtectionResult) {
  m_integrityProtectionResult = integrityProtectionResult;

  return true;
}

//------------------------------------------------------------------------------
bool IntegrityProtectionResult::encode2IntegrityProtectionResult(
    Ngap_IntegrityProtectionResult_t& integrityProtectionResultValue) {
  integrityProtectionResultValue = integrityProtectionResult;

  return true;
}

//------------------------------------------------------------------------------
bool IntegrityProtectionResult::decodefromIntegrityProtectionResult(
    Ngap_IntegrityProtectionResult_t integrityProtectionResultValue) {
  integrityProtectionResult = integrityProtectionResultValue;

  return true;
}
}  // namespace ngap
