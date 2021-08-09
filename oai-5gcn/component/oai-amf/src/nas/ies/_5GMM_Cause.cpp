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

/*! \file _5GMM_Cause.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "_5GMM_Cause.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
_5GMM_Cause::_5GMM_Cause(uint8_t iei, uint8_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
void _5GMM_Cause::setValue(uint8_t value) {
  _value = value;
}

//------------------------------------------------------------------------------
uint8_t _5GMM_Cause::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
_5GMM_Cause::_5GMM_Cause() {}

//------------------------------------------------------------------------------
_5GMM_Cause::~_5GMM_Cause(){};

//------------------------------------------------------------------------------
int _5GMM_Cause::encode2buffer(uint8_t* buf, int len) {
  int encoded_size = 0;
  Logger::nas_mm().debug("encoding _5GMM_Cause IE ");
  if (_iei) {
    if (len < 2) {
      Logger::nas_mm().error("len is less than 2");
      return -1;
    }
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = _value;
    encoded_size++;
    return 2;
  } else {
    if (len < 1) {
      Logger::nas_mm().error("len is less than one");
      return -1;
    }
    *buf = _value;
    Logger::nas_mm().debug("encoded _5GMM_Cause IE 0x%x", *buf);
    return 1;
  }
}

//------------------------------------------------------------------------------
int _5GMM_Cause::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding _5GMM_Cause IE");
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
    if (len < 2) {
      Logger::nas_mm().error("len is less than one");
      return 0;
    }
  }
  if (len < 1) {
    Logger::nas_mm().error("len is less than one");
    return 0;
  }
  _value = *(buf + decoded_size);
  decoded_size++;
  Logger::nas_mm().debug(
      "decoded _5GMM_Cause len 1 octet,5G_Couse=0x%x", _value);
  return decoded_size;
}
