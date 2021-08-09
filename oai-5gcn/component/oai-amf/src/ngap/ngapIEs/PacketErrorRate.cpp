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

#include "PacketErrorRate.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PacketErrorRate::PacketErrorRate() {
  pERScalar   = 0;
  pERExponent = 0;
}

//------------------------------------------------------------------------------
PacketErrorRate::~PacketErrorRate() {}

//------------------------------------------------------------------------------
void PacketErrorRate::setPacketErrorRate(long m_pERScalar, long m_pERExponent) {
  pERScalar   = m_pERScalar;
  pERExponent = m_pERExponent;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::getPacketErrorRate(
    long& m_pERScalar, long& m_pERExponent) {
  m_pERScalar   = pERScalar;
  m_pERExponent = pERExponent;

  return true;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::encode2PacketErrorRate(
    Ngap_PacketErrorRate_t* packetErrorRate) {
  packetErrorRate->pERScalar   = pERScalar;
  packetErrorRate->pERExponent = pERExponent;

  return true;
}

//------------------------------------------------------------------------------
bool PacketErrorRate::decodefromPacketErrorRate(
    Ngap_PacketErrorRate_t* packetErrorRate) {
  pERScalar   = packetErrorRate->pERScalar;
  pERExponent = packetErrorRate->pERExponent;

  return true;
}
}  // namespace ngap
