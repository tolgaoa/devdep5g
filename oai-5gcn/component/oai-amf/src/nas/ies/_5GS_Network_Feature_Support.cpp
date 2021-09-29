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

/*! \file __5GS_Network_Feature_Support.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "_5GS_Network_Feature_Support.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
_5GS_Network_Feature_Support::_5GS_Network_Feature_Support(uint8_t iei) {
  _iei    = iei;
  _value  = 0;
  _value2 = 0;
  length  = 0;
}

//------------------------------------------------------------------------------
_5GS_Network_Feature_Support::_5GS_Network_Feature_Support() {
  _iei    = 0;
  _value  = 0;
  _value2 = 0;
  length  = 0;
}

//------------------------------------------------------------------------------
_5GS_Network_Feature_Support::~_5GS_Network_Feature_Support() {}

//------------------------------------------------------------------------------
_5GS_Network_Feature_Support::_5GS_Network_Feature_Support(
    const uint8_t iei, uint8_t value, uint8_t value2) {
  _iei    = iei;
  _value  = value;
  _value2 = value2;
  length  = 4;
}

//------------------------------------------------------------------------------
void _5GS_Network_Feature_Support::setValue(uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t _5GS_Network_Feature_Support::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int _5GS_Network_Feature_Support::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug(
      "Encoding _5GS_Network_Feature_Support IEI (0x%x)", _iei);
  if (len < length) {
    Logger::nas_mm().error("len is less than %d", length);
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = length - 2;
    encoded_size++;
    *(buf + encoded_size) = _value;
    encoded_size++;
    *(buf + encoded_size) = _value2;
    encoded_size++;
  } else {
    *(buf + encoded_size) = length - 1;
    encoded_size++;
    //		*(buf + encoded_size) = _5g_EASel; encoded_size++;
    //		*(buf + encoded_size) = _5g_IASel; encoded_size++;
  }
  Logger::nas_mm().debug(
      "Encoded _5GS_Network_Feature_Support len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GS_Network_Feature_Support::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug(
      "Decoding _5GS_Network_Feature_Support IEI (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  _value = *(buf + decoded_size);
  decoded_size++;
  Logger::nas_mm().debug(
      "Decoded _5GS_Network_Feature_Support value (0x%x)", _value);
  Logger::nas_mm().debug(
      "Decoded _5GS_Network_Feature_Support len (%d)", decoded_size);
  return decoded_size;
}
