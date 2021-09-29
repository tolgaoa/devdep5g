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

#include "MICO_Indication.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
MICO_Indication::MICO_Indication(const uint8_t _iei, bool sprti, bool raai) {
  iei   = _iei;
  RAAI  = raai;
  SPRTI = sprti;
}

//------------------------------------------------------------------------------
MICO_Indication::MICO_Indication(bool sprti, bool raai) {
  this->iei   = 0;
  this->RAAI  = raai;
  this->SPRTI = sprti;
}

//------------------------------------------------------------------------------
MICO_Indication::MICO_Indication() {}

//------------------------------------------------------------------------------
MICO_Indication::~MICO_Indication(){};

//------------------------------------------------------------------------------
int MICO_Indication::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding MICO_Indication IE iei (0x%x)", iei);
  if (len < 1) {
    Logger::nas_mm().error("Len is less than one");
    return -1;
  } else {
    uint8_t octet = 0;
    if (!(iei & 0x0f)) {
      // octet = (0x0f) & ((tsc << 3) | key_id);
      //*buf = octet;
      // Logger::nas_mm().debug("encoded NasKeySetIdentifier IE(len(1/2
      // octet))"); return 0;
    } else {
      octet = (iei << 4) | (SPRTI << 1) | RAAI;
      *buf  = octet;
      Logger::nas_mm().debug("Encoded MICO_Indication IE (len: 1 octet)");
      return 1;
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
int MICO_Indication::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding MICO_Indication IE");
  if (len < 1) {
    Logger::nas_mm().error("Len is less than one");
    return 0;
  } else {
    uint8_t octet = (*buf);
    if (is_option) {
      iei = (octet & 0xf0) >> 4;
    } else {
      iei = 0;
    }
    SPRTI = octet & 0x02;
    RAAI  = octet & 0x01;
    Logger::nas_mm().debug(
        "decoded MICO_Indication iei (0x%x) sprti (0x%x) raai (0x%x)", iei,
        SPRTI, RAAI);
    return 1;
  }
}

//------------------------------------------------------------------------------
void MICO_Indication::setSPRTI(bool value) {
  SPRTI = value;
}

//------------------------------------------------------------------------------
void MICO_Indication::setRAAI(bool value) {
  RAAI = value;
}

//------------------------------------------------------------------------------
bool MICO_Indication::getSPRTI() {
  return SPRTI;
}

//------------------------------------------------------------------------------
bool MICO_Indication::getRAAI() {
  return RAAI;
}
