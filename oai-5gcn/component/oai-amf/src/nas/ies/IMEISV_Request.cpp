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

#include "IMEISV_Request.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
IMEISV_Request::IMEISV_Request(uint8_t iei) {
  _iei   = iei;
  _value = 0;
}

//------------------------------------------------------------------------------
IMEISV_Request::IMEISV_Request(const uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
IMEISV_Request::IMEISV_Request() : _iei(), _value() {}

//------------------------------------------------------------------------------
IMEISV_Request::~IMEISV_Request() {}

//------------------------------------------------------------------------------
void IMEISV_Request::setValue(uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t IMEISV_Request::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int IMEISV_Request::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding IMEISV_Request iei (0x%x)", _iei);
  if (len < 1) {
    Logger::nas_mm().error("Len is less than 1");
    return 0;
  }
  uint8_t octet = 0;
  if (!(_iei & 0x0f)) {
    // octet = (0x0f) & ((tsc << 3) | key_id);
    //*buf = octet;
    // Logger::nas_mm().debug("encoded Payload_Container_Type IE(len(1/2
    // octet))"); return 0;
  } else {
    octet = (_iei << 4) | (_value & 0x07);
    *buf  = octet;
    Logger::nas_mm().debug("Encoded IMEISV_Request IE (len - 1 octet)");
    return 1;
  }
  return 1;
}

//------------------------------------------------------------------------------
int IMEISV_Request::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
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
    _value = octet & 0x07;
    Logger::nas_mm().debug("Decoded IMEISV_Request value (0x%x)", _value);
    return 1;
  }
}
