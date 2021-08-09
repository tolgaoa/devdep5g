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

/*! \file ___5GS_DRX_arameters.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "_5GS_DRX_arameters.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
_5GS_DRX_arameters::_5GS_DRX_arameters(uint8_t iei) {
  _iei   = iei;
  _value = 0;
}

//------------------------------------------------------------------------------
_5GS_DRX_arameters::_5GS_DRX_arameters(const uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value & 0x0F;
}

//------------------------------------------------------------------------------
_5GS_DRX_arameters::_5GS_DRX_arameters() {
  _iei   = 0;
  _value = 0;
}
_5GS_DRX_arameters::~_5GS_DRX_arameters() {}

//------------------------------------------------------------------------------
void _5GS_DRX_arameters::setValue(uint8_t value) {
  _value = value & 0x0F;
}

//------------------------------------------------------------------------------
uint8_t _5GS_DRX_arameters::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int _5GS_DRX_arameters::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding _5GS_DRX_arameters iei(0x%x)", _iei);
  if (len < 3) {
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = 1;
    encoded_size++;
    *(buf + encoded_size) = 0x0F & _value;
    encoded_size++;
  } else {
    //	*(buf + encoded_size) = length - 1; encoded_size++;
    //	*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug("encoded _5GS_DRX_arameters len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GS_DRX_arameters::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding _5GS_DRX_arameters iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  _value = 0x00;
  //	length = *(buf + decoded_size);
  decoded_size++;
  _value = *(buf + decoded_size) & 0x0f;
  decoded_size++;
  Logger::nas_mm().debug("decoded _5GS_DRX_arameters DRX value(0x%x)", _value);
  Logger::nas_mm().debug("decoded UE_Status len(%d)", decoded_size);
  return decoded_size;
}
