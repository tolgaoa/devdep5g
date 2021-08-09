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

#include "MA_PDU_Session_Information.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
MA_PDU_Session_Information::MA_PDU_Session_Information(uint8_t iei) {
  _iei   = iei;
  _value = 0;
}

//------------------------------------------------------------------------------
MA_PDU_Session_Information::MA_PDU_Session_Information(
    const uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
MA_PDU_Session_Information::MA_PDU_Session_Information() {
  _iei   = 0;
  _value = 0;
}

//------------------------------------------------------------------------------
MA_PDU_Session_Information::~MA_PDU_Session_Information() {}

//------------------------------------------------------------------------------
void MA_PDU_Session_Information::setValue(uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t MA_PDU_Session_Information::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int MA_PDU_Session_Information::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding MA_PDU_Session_Information iei(0x%x)", _iei);
  if (len < 1) {
    Logger::nas_mm().error("len is less than 1");
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = ((_iei & 0x0f) << 4) | (_value & 0x0f);
    encoded_size++;
  } else {
    *(buf + encoded_size) = _value & 0x0f;
    encoded_size++;
  }
  Logger::nas_mm().debug(
      "encoded MA_PDU_Session_Information len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int MA_PDU_Session_Information::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding MA_PDU_Session_Information iei(0x%x)", *buf);
  int decoded_size = 0;
  uint8_t octet    = 0;
  if (is_option) {
    octet = *buf;
    decoded_size++;
  }
  _value = 0x00;
  _value = octet & 0x0f;
  Logger::nas_mm().debug(
      "decoded MA_PDU_Session_Information _value(0x%x)", _value);
  Logger::nas_mm().debug(
      "decoded MA_PDU_Session_Information len(%d)", decoded_size);
  return decoded_size;
}
