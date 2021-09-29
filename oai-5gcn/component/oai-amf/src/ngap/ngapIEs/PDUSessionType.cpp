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

#include "PDUSessionType.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PDUSessionType::PDUSessionType() {
  pdusessiontype = 0;
}

//------------------------------------------------------------------------------
PDUSessionType::~PDUSessionType() {}

//------------------------------------------------------------------------------
void PDUSessionType::setPDUSessionType(e_Ngap_PDUSessionType m_pdusessiontype) {
  pdusessiontype = m_pdusessiontype;
}

//------------------------------------------------------------------------------
bool PDUSessionType::getPDUSessionType(long& m_pdusessiontype) {
  m_pdusessiontype = pdusessiontype;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionType::encode2PDUSessionType(Ngap_PDUSessionType_t& Type) {
  Type = pdusessiontype;

  return true;
}

//------------------------------------------------------------------------------
bool PDUSessionType::decodefromPDUSessionType(Ngap_PDUSessionType_t type) {
  pdusessiontype = type;

  return true;
}

}  // namespace ngap
