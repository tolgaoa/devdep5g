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

#include "SOR_Transparent_Container.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
SOR_Transparent_Container::SOR_Transparent_Container(uint8_t iei) : _value() {
  _iei   = iei;
  HEADER = 0;
}

//------------------------------------------------------------------------------
SOR_Transparent_Container::SOR_Transparent_Container(
    const uint8_t iei, uint8_t header, uint8_t* value) {
  _iei   = iei;
  HEADER = header;
  for (int i = 0; i < 16; i++) {
    this->_value[i] = value[i];
  }
}

//------------------------------------------------------------------------------
SOR_Transparent_Container::SOR_Transparent_Container()
    : _iei(), HEADER(), _value() {}

//------------------------------------------------------------------------------
SOR_Transparent_Container::~SOR_Transparent_Container() {}

//------------------------------------------------------------------------------
uint8_t SOR_Transparent_Container::getValue() {
  for (int j = 0; j < 16; j++) {
    Logger::nas_mm().debug(
        "decoded SOR_Transparent_Container value(0x%2x)", _value[j]);
  }
  return 1;
}

//------------------------------------------------------------------------------
int SOR_Transparent_Container::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding SOR_Transparent_Container iei(0x%x)", _iei);
  if (len < 18) {
    Logger::nas_mm().error("len is less than 18");
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = 17;
    encoded_size++;
    encoded_size++;
    *(buf + encoded_size) = HEADER;
    encoded_size++;
    for (int i = 0; i < 16; i++) {
      *(buf + encoded_size) = _value[i];
      encoded_size++;
    }
    return encoded_size;
  } else {
    //		*(buf + encoded_size) = length - 1; encoded_size++;
    //		*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug(
      "encoded SOR_Transparent_Container len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int SOR_Transparent_Container::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding SOR_Transparent_Container iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  decoded_size++;
  decoded_size++;
  HEADER = *(buf + decoded_size);
  decoded_size++;
  for (int i = 0; i < 16; i++) {
    _value[i] = *(buf + decoded_size);
    decoded_size++;
  }
  for (int j = 0; j < 16; j++) {
    Logger::nas_mm().debug(
        "decoded SOR_Transparent_Container value(0x%2x)", _value[j]);
  }
  Logger::nas_mm().debug(
      "decoded SOR_Transparent_Container len(%d)", decoded_size);
  return decoded_size;
}
