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

#include "Tac.hpp"

#include <iostream>
namespace ngap {

//------------------------------------------------------------------------------
TAC::TAC() {
  tac = 0;
}

//------------------------------------------------------------------------------
TAC::~TAC() {}

//------------------------------------------------------------------------------
void TAC::setTac(uint32_t m_tac) {
  tac = m_tac;
}

//------------------------------------------------------------------------------
bool TAC::encode2octetstring(Ngap_TAC_t& m_tac) {
  m_tac.size   = 3;  // OCTET_STRING(SIZE(3))
  m_tac.buf    = (uint8_t*) calloc(1, sizeof(uint8_t) + sizeof(uint16_t));
  m_tac.buf[2] = tac & 0x0000ff;
  m_tac.buf[1] = (tac & 0x00ff00) >> 8;
  m_tac.buf[0] = (tac & 0xff0000) >> 16;

  return true;
}

//------------------------------------------------------------------------------
bool TAC::decodefromoctetstring(Ngap_TAC_t& m_tac) {
  if (!m_tac.buf) return false;
  tac = 0;
  for (int i = 0; i < m_tac.size; i++) {
    tac |= m_tac.buf[i] << ((m_tac.size - 1 - i) * 8);
  }
  std::cout << "Received TAC: " << tac << std::endl;
  return true;
}

//------------------------------------------------------------------------------
uint32_t TAC::getTac() {
  return tac;
}

}  // namespace ngap
