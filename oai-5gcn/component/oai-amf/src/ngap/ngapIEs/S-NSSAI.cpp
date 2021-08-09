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

#include "S-NSSAI.hpp"

#include "String2Value.hpp"

using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
S_NSSAI::S_NSSAI() {
  sdIsSet = false;
  sst     = 0;
  sd      = 0;
}

//------------------------------------------------------------------------------
S_NSSAI::~S_NSSAI() {}

//------------------------------------------------------------------------------
bool S_NSSAI::sSTEncode2OctetString(Ngap_SST_t& m_sst) {
  m_sst.size      = 1;
  uint8_t* buffer = (uint8_t*) calloc(1, sizeof(uint8_t));
  if (!buffer) return false;
  *buffer   = sst;
  m_sst.buf = buffer;
  return true;
}

//------------------------------------------------------------------------------
bool S_NSSAI::sSTdecodefromOctetString(Ngap_SST_t m_sst) {
  if (!m_sst.buf) return false;
  sst = *m_sst.buf;
  return true;
}

//------------------------------------------------------------------------------
bool S_NSSAI::sDEncode2OctetString(Ngap_SD_t* m_sd) {
  m_sd->size      = 3;
  uint8_t* buffer = (uint8_t*) calloc(1, sizeof(uint8_t) + sizeof(uint16_t));
  if (!buffer) return false;
  buffer[0] = (sd & 0x00ff0000) >> 16;
  buffer[1] = (sd & 0x0000ff00) >> 8;
  buffer[2] = (sd & 0x000000ff) >> 0;
  m_sd->buf = buffer;
  return true;
}

//------------------------------------------------------------------------------
bool S_NSSAI::sDdecodefromOctetString(Ngap_SD_t* m_sd) {
  if (!m_sd->buf) return false;
  sd = *(uint32_t*) m_sd->buf & 0x00ffffff;
  return true;
}

//------------------------------------------------------------------------------
void S_NSSAI::setSst(const std::string charSst) {
  sst = fromString<int>(charSst);
}

//------------------------------------------------------------------------------
void S_NSSAI::getSst(std::string& charSst) {
  charSst = to_string((int) sst);
}

//------------------------------------------------------------------------------
void S_NSSAI::setSd(const std::string charSd) {
  sdIsSet = true;
  sd      = fromString<int>(charSd);
}

//------------------------------------------------------------------------------
bool S_NSSAI::getSd(std::string& s_nssaiSd) {
  if (sdIsSet) {
    s_nssaiSd = to_string(sd);
  } else
    s_nssaiSd = "None";

  return sdIsSet;
}

//------------------------------------------------------------------------------
bool S_NSSAI::encode2S_NSSAI(Ngap_S_NSSAI_t* s_NSSAI) {
  if (!sSTEncode2OctetString(s_NSSAI->sST)) return false;
  if (sdIsSet) {
    Ngap_SD_t* sd = (Ngap_SD_t*) calloc(1, sizeof(Ngap_SD_t));
    if (!sd) return false;
    if (!sDEncode2OctetString(sd)) {
      free(sd);
      return false;
    }
    s_NSSAI->sD = sd;
  }
  return true;
}

//------------------------------------------------------------------------------
bool S_NSSAI::decodefromS_NSSAI(Ngap_S_NSSAI_t* s_NSSAI) {
  if (!sSTdecodefromOctetString(s_NSSAI->sST)) return false;
  if (s_NSSAI->sD) {
    sdIsSet = true;
    if (!sDdecodefromOctetString(s_NSSAI->sD)) return false;
  }
  return true;
}

}  // namespace ngap
