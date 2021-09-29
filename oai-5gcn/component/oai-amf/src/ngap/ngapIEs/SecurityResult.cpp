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

#include "SecurityResult.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
SecurityResult::SecurityResult() {
  integrityProtectionResult       = NULL;
  confidentialityProtectionResult = NULL;
}

//------------------------------------------------------------------------------
SecurityResult::~SecurityResult() {}

//------------------------------------------------------------------------------
void SecurityResult::setSecurityResult(
    IntegrityProtectionResult* m_integrityProtectionResult,
    ConfidentialityProtectionResult* m_confidentialityProtectionResult) {
  integrityProtectionResult       = m_integrityProtectionResult;
  confidentialityProtectionResult = m_confidentialityProtectionResult;
}

//------------------------------------------------------------------------------
bool SecurityResult::getSecurityResult(
    IntegrityProtectionResult*& m_integrityProtectionResult,
    ConfidentialityProtectionResult*& m_confidentialityProtectionResult) {
  m_integrityProtectionResult       = integrityProtectionResult;
  m_confidentialityProtectionResult = confidentialityProtectionResult;

  return true;
}

//------------------------------------------------------------------------------
bool SecurityResult::encode2SecurityResult(
    Ngap_SecurityResult_t* securityResult) {
  if (!integrityProtectionResult) return false;
  if (!confidentialityProtectionResult) return false;

  if (!integrityProtectionResult->encode2IntegrityProtectionResult(
          securityResult->integrityProtectionResult))
    return false;
  if (!confidentialityProtectionResult->encode2ConfidentialityProtectionResult(
          securityResult->confidentialityProtectionResult))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool SecurityResult::decodefromSecurityResult(
    Ngap_SecurityResult_t* securityResult) {
  if (integrityProtectionResult == nullptr)
    integrityProtectionResult = new IntegrityProtectionResult();
  if (integrityProtectionResult == nullptr)
    confidentialityProtectionResult = new ConfidentialityProtectionResult();

  if (!integrityProtectionResult->decodefromIntegrityProtectionResult(
          securityResult->integrityProtectionResult))
    return false;
  if (!confidentialityProtectionResult
           ->decodefromConfidentialityProtectionResult(
               securityResult->confidentialityProtectionResult))
    return false;

  return true;
}
}  // namespace ngap
