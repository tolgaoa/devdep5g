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

#include "Authentication_Parameter_RAND.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Authentication_Parameter_RAND::Authentication_Parameter_RAND(uint8_t iei)
    : _value() {
  _iei = iei;
}

//------------------------------------------------------------------------------
Authentication_Parameter_RAND::Authentication_Parameter_RAND(
    const uint8_t iei, uint8_t* value) {
  _iei = iei;
  for (int i = 0; i < 16; i++) {
    this->_value[i] = value[i];
  }
}

//------------------------------------------------------------------------------
Authentication_Parameter_RAND::Authentication_Parameter_RAND()
    : _iei(), _value() {}

//------------------------------------------------------------------------------
Authentication_Parameter_RAND::~Authentication_Parameter_RAND() {}

//------------------------------------------------------------------------------
uint8_t* Authentication_Parameter_RAND::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int Authentication_Parameter_RAND::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug(
      "Encoding Authentication_Parameter_RAND IEI (0x%x)", _iei);
  if (len < 17) {
    Logger::nas_mm().error("len is less than 17");
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    memcpy((void*) (buf + encoded_size), (void*) _value, 16);
    encoded_size += 16;
    // int i = 0;
    // for (int i = 0; i < 16; i++) {
    //	*(buf + encoded_size) = _value[i]; encoded_size++;
    //}
    return encoded_size;
  } else {
    //		*(buf + encoded_size) = length - 1; encoded_size++;
    //		*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug(
      "Encoded Authentication_Parameter_RAND len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Authentication_Parameter_RAND::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug(
      "Decoding Authentication_Parameter_RAND IEI (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  for (int i = 0; i < 16; i++) {
    _value[i] = *(buf + decoded_size);
    decoded_size++;
  }
  for (int j = 0; j < 16; j++) {
    Logger::nas_mm().debug(
        "Decoded Authentication_Parameter_RAND value (0x%2x)", _value[j]);
  }
  Logger::nas_mm().debug(
      "Decoded Authentication_Parameter_RAND len (%d)", decoded_size);
  return decoded_size;
}
