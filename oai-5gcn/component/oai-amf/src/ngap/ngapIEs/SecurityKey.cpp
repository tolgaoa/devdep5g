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

#include "SecurityKey.hpp"

namespace ngap {

//------------------------------------------------------------------------------
SecurityKey::SecurityKey() {
  securitykeybuffer = nullptr;
  buffersize        = 0;
}

//------------------------------------------------------------------------------
SecurityKey::~SecurityKey() {}

//------------------------------------------------------------------------------
bool SecurityKey::encode2bitstring(Ngap_SecurityKey_t& m_securitykey) {
  m_securitykey.bits_unused = 0;
  m_securitykey.size        = 32;
  uint8_t* buffer           = (uint8_t*) calloc(1, 32);
  if (!buffer) return false;
  memcpy(buffer, securitykeybuffer, 32);
  m_securitykey.buf = buffer;

  return true;
}

//------------------------------------------------------------------------------
bool SecurityKey::decodefrombitstring(Ngap_SecurityKey_t& m_securitykey) {
  securitykeybuffer = m_securitykey.buf;
  return true;
}

//------------------------------------------------------------------------------
bool SecurityKey::getSecurityKey(uint8_t*& buffer) {
  buffer = (uint8_t*) securitykeybuffer;
  if (!securitykeybuffer) return false;

  return true;
}

//------------------------------------------------------------------------------
void SecurityKey::setSecurityKey(uint8_t* buffer) {
  securitykeybuffer = buffer;
}
}  // namespace ngap
