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

#include "NSSAI_Inclusion_Mode.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
NSSAI_Inclusion_Mode::NSSAI_Inclusion_Mode(const uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
NSSAI_Inclusion_Mode::NSSAI_Inclusion_Mode() {}

//------------------------------------------------------------------------------
NSSAI_Inclusion_Mode::~NSSAI_Inclusion_Mode(){};

//------------------------------------------------------------------------------
void NSSAI_Inclusion_Mode::setValue(const uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t NSSAI_Inclusion_Mode::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int NSSAI_Inclusion_Mode::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding NSSAI_Inclusion_Mode IE iei (0x%x)", _iei);
  if (len < 1) {
    Logger::nas_mm().error("Len is less than one");
    return -1;
  } else {
    uint8_t octet = 0;
    if (!(_iei & 0x0f)) {
      // octet = (0x0f) & ((tsc << 3) | key_id);
      //*buf = octet;
      // Logger::nas_mm().debug("encoded Payload_Container_Type IE(len(1/2
      // octet))"); return 0;
    } else {
      octet = (_iei << 4) | (_value & 0x0f);
      *buf  = octet;
      Logger::nas_mm().debug("Encoded NSSAI_Inclusion_Mode IE(len, 1 octet)");
      return 1;
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
int NSSAI_Inclusion_Mode::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding NSSAI_Inclusion_Mode IE");
  if (len < 1) {
    Logger::nas_mm().error("Len is less than one");
    return 0;
  } else {
    uint8_t octet = (*buf);
    if (is_option) {
      _iei = (octet & 0xf0) >> 4;
    } else {
      _iei = 0;
    }
    _value = octet & 0x0f;
    Logger::nas_mm().debug(
        "Decoded NSSAI_Inclusion_Mode iei (0x%x) value (0x%x)", _iei, _value);
    return 1;
  }
}
