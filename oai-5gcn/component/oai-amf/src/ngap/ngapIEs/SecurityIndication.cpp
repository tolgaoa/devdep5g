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

#include "SecurityIndication.hpp"

#include <iostream>

extern "C" {
#include "dynamic_memory_check.h"
}

using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
SecurityIndication::SecurityIndication() {
  integrityProtectionIndication       = NULL;
  confidentialityProtectionIndication = NULL;
  maximumIntegrityProtectedDataRate   = NULL;
}

//------------------------------------------------------------------------------
SecurityIndication::~SecurityIndication() {}

//------------------------------------------------------------------------------
void SecurityIndication::setSecurityIndication(
    IntegrityProtectionIndication* m_integrityProtectionIndication,
    ConfidentialityProtectionIndication* m_confidentialityProtectionIndication,
    MaximumIntegrityProtectedDataRate* m_maximumIntegrityProtectedDataRate) {
  integrityProtectionIndication       = m_integrityProtectionIndication;
  confidentialityProtectionIndication = m_confidentialityProtectionIndication;
  maximumIntegrityProtectedDataRate   = m_maximumIntegrityProtectedDataRate;
}

//------------------------------------------------------------------------------
bool SecurityIndication::encode2SecurityIndication(
    Ngap_SecurityIndication_t* securityIndication) {
  if (!integrityProtectionIndication->encode2IntegrityProtectionIndication(
          securityIndication->integrityProtectionIndication))
    return false;
  if (!confidentialityProtectionIndication
           ->encode2ConfidentialityProtectionIndication(
               securityIndication->confidentialityProtectionIndication))
    return false;
  if (maximumIntegrityProtectedDataRate) {
    Ngap_MaximumIntegrityProtectedDataRate_t* maxIPDataRate =
        (Ngap_MaximumIntegrityProtectedDataRate_t*) calloc(
            1, sizeof(Ngap_MaximumIntegrityProtectedDataRate_t));
    if (!maxIPDataRate) return false;
    if (!maximumIntegrityProtectedDataRate
             ->encode2MaximumIntegrityProtectedDataRate(*maxIPDataRate)) {
      free_wrapper((void**) &maxIPDataRate);
      return false;
    }

    securityIndication->maximumIntegrityProtectedDataRate = maxIPDataRate;
    // free_wrapper((void**) &maxIPDataRate);
  }

  return true;
}

//------------------------------------------------------------------------------
bool SecurityIndication::decodefromSecurityIndication(
    Ngap_SecurityIndication_t* securityIndication) {
  integrityProtectionIndication = new IntegrityProtectionIndication();
  confidentialityProtectionIndication =
      new ConfidentialityProtectionIndication();

  if (!integrityProtectionIndication->decodefromIntegrityProtectionIndication(
          securityIndication->integrityProtectionIndication))
    return false;
  if (!confidentialityProtectionIndication
           ->decodefromConfidentialityProtectionIndication(
               securityIndication->confidentialityProtectionIndication))
    return false;
  if (securityIndication->maximumIntegrityProtectedDataRate) {
    maximumIntegrityProtectedDataRate = new MaximumIntegrityProtectedDataRate();
    if (!maximumIntegrityProtectedDataRate
             ->decodefromMaximumIntegrityProtectedDataRate(
                 *securityIndication->maximumIntegrityProtectedDataRate))
      return false;
  }

  return true;
}

//------------------------------------------------------------------------------
void SecurityIndication::getSecurityIndication(
    IntegrityProtectionIndication*& m_integrityProtectionIndication,
    ConfidentialityProtectionIndication*& m_confidentialityProtectionIndication,
    MaximumIntegrityProtectedDataRate*& m_maximumIntegrityProtectedDataRate) {
  m_integrityProtectionIndication       = integrityProtectionIndication;
  m_confidentialityProtectionIndication = confidentialityProtectionIndication;
  m_maximumIntegrityProtectedDataRate   = maximumIntegrityProtectedDataRate;
}

}  // namespace ngap
