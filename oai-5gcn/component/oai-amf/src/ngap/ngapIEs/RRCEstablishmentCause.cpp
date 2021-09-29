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

#include "RRCEstablishmentCause.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
RRCEstablishmentCause::RRCEstablishmentCause() {
  rrcestablishmentcause = -1;
}

//------------------------------------------------------------------------------
RRCEstablishmentCause::~RRCEstablishmentCause() {}

//------------------------------------------------------------------------------
void RRCEstablishmentCause::setRRCEstablishmentCause(
    e_Ngap_RRCEstablishmentCause m_rrcestablishmentcause) {
  rrcestablishmentcause = m_rrcestablishmentcause;
}

//------------------------------------------------------------------------------
int RRCEstablishmentCause::getRRCEstablishmentCause() {
  return rrcestablishmentcause;
}

//------------------------------------------------------------------------------
bool RRCEstablishmentCause::encode2RRCEstablishmentCause(
    Ngap_RRCEstablishmentCause_t& rRCEstablishmentCause) {
  rRCEstablishmentCause = rrcestablishmentcause;
  return true;
}

//------------------------------------------------------------------------------
bool RRCEstablishmentCause::decodefromRRCEstablishmentCause(
    Ngap_RRCEstablishmentCause_t& rRCEstablishmentCause) {
  rrcestablishmentcause = rRCEstablishmentCause;
  return true;
}
}  // namespace ngap
