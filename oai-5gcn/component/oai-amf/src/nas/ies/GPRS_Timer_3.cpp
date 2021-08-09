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

#include "GPRS_Timer_3.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
GPRS_Timer_3::GPRS_Timer_3(uint8_t iei) {
  _iei   = iei;
  UNIT   = 0;
  _value = 0;
}

//------------------------------------------------------------------------------
GPRS_Timer_3::GPRS_Timer_3(const uint8_t iei, uint8_t unit, uint8_t value) {
  _iei   = iei;
  UNIT   = unit;
  _value = value;
}

//------------------------------------------------------------------------------
GPRS_Timer_3::GPRS_Timer_3() : _iei(), UNIT(), _value() {}

//------------------------------------------------------------------------------
GPRS_Timer_3::~GPRS_Timer_3() {}

//------------------------------------------------------------------------------
void GPRS_Timer_3::setValue(uint8_t unit, uint8_t value) {
  UNIT   = unit;
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t GPRS_Timer_3::getUnit() {
  return UNIT;
}

//------------------------------------------------------------------------------
uint8_t GPRS_Timer_3::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int GPRS_Timer_3::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding GPRS_Timer_3 IEI (0x%x)", _iei);
  if (len < 3) {
    Logger::nas_mm().error("len is less than 3");
    return 0;
  }
  uint8_t octet    = 0;
  int encoded_size = 0;
  octet            = (UNIT << 5) | (_value & 0x1f);
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = 1;
    encoded_size++;
    *(buf + encoded_size) = octet;
    encoded_size++;
    Logger::nas_mm().debug("Encoded GPRS_Timer_3 content (0x%x)", octet);
  } else {
    *(buf + encoded_size) = 1;
    encoded_size++;
    *(buf + encoded_size) = octet;
    encoded_size++;
  }
  Logger::nas_mm().debug("Encoded GPRS_Timer_3 len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int GPRS_Timer_3::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding GPRS_Timer_3 IEI (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    _iei = *buf;
    decoded_size++;
  }
  uint8_t octet = 0;
  //	length = *(buf + decoded_size);
  decoded_size++;
  octet = *(buf + decoded_size);
  decoded_size++;
  UNIT   = (octet & 0xe0) >> 5;
  _value = octet & 0x1f;
  Logger::nas_mm().debug("Decoded GPRS_Timer_3 content (0x%x)", octet);
  Logger::nas_mm().debug("Decoded GPRS_Timer_3 len (%d)", decoded_size);
  return decoded_size;
}
