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

#include "NAS-PDU.hpp"

namespace ngap {

//------------------------------------------------------------------------------
NAS_PDU::NAS_PDU() {
  naspdubuffer = NULL;
  buffersize   = -1;
}

//------------------------------------------------------------------------------
NAS_PDU::~NAS_PDU() {}

//------------------------------------------------------------------------------
bool NAS_PDU::encode2octetstring(Ngap_NAS_PDU_t& m_naspdu) {
  int ret;
  ret = OCTET_STRING_fromBuf(&m_naspdu, naspdubuffer, buffersize);
  if (ret != 0) return false;
  return true;
}

//------------------------------------------------------------------------------
bool NAS_PDU::decodefromoctetstring(Ngap_NAS_PDU_t& m_naspdu) {
  naspdubuffer = (char*) m_naspdu.buf;
  buffersize   = m_naspdu.size;
  return true;
}

//------------------------------------------------------------------------------
bool NAS_PDU::getNasPdu(uint8_t*& buffer, size_t& size) {
  buffer = (uint8_t*) naspdubuffer;
  size   = buffersize;
  if (!naspdubuffer) return false;
  if (buffersize < 0) return false;

  return true;
}

//------------------------------------------------------------------------------
void NAS_PDU::setNasPdu(uint8_t* buffer, size_t size) {
  naspdubuffer = (char*) buffer;
  buffersize   = size;
}
}  // namespace ngap
