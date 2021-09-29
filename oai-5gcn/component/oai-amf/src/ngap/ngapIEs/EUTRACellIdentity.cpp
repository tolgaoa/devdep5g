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

#include "EUTRACellIdentity.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
EUTRACellIdentity::EUTRACellIdentity() {
  eutracellidentity = 0;
}

//------------------------------------------------------------------------------
EUTRACellIdentity::~EUTRACellIdentity() {}

//------------------------------------------------------------------------------
void EUTRACellIdentity::setEUTRACellIdentity(uint32_t m_eutracellidentity) {
  eutracellidentity = m_eutracellidentity;
}

//------------------------------------------------------------------------------
bool EUTRACellIdentity::encode2bitstring(
    Ngap_EUTRACellIdentity_t& eUTRACellIdentity) {
  eUTRACellIdentity.bits_unused = 4;
  eUTRACellIdentity.size        = 4;
  eUTRACellIdentity.buf         = (uint8_t*) calloc(1, sizeof(uint32_t));
  if (!eUTRACellIdentity.buf) return false;
  eUTRACellIdentity.buf[3] = eutracellidentity & 0x000000ff;
  eUTRACellIdentity.buf[2] = (eutracellidentity & 0x0000ff00) >> 8;
  eUTRACellIdentity.buf[1] = (eutracellidentity & 0x00ff0000) >> 16;
  eUTRACellIdentity.buf[0] = (eutracellidentity & 0xff000000) >> 24;

  return true;
}

//------------------------------------------------------------------------------
bool EUTRACellIdentity::decodefrombitstring(
    Ngap_EUTRACellIdentity_t& eUTRACellIdentity) {
  if (!eUTRACellIdentity.buf) return false;

  eutracellidentity = eUTRACellIdentity.buf[0] << 24;
  eutracellidentity |= eUTRACellIdentity.buf[1] << 16;
  eutracellidentity |= eUTRACellIdentity.buf[2] << 8;
  eutracellidentity |= eUTRACellIdentity.buf[3];

  return true;
}

//------------------------------------------------------------------------------
uint32_t EUTRACellIdentity::getEUTRACellIdentity() {
  return eutracellidentity;
}
}  // namespace ngap
