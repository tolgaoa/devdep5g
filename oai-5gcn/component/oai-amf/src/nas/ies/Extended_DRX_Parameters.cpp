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
#include "Extended_DRX_Parameters.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Extended_DRX_Parameters::Extended_DRX_Parameters(uint8_t iei) {
  _iei         = iei;
  _paging_time = 0;
  _value       = 0;
}

//------------------------------------------------------------------------------
Extended_DRX_Parameters::Extended_DRX_Parameters(
    const uint8_t iei, uint8_t paging_time, uint8_t value) {
  _iei         = iei;
  _paging_time = paging_time & 0x0F;
  _value       = value & 0x0F;
}

//------------------------------------------------------------------------------
Extended_DRX_Parameters::Extended_DRX_Parameters()
    : _iei(), _paging_time(), _value() {}

//------------------------------------------------------------------------------
Extended_DRX_Parameters::~Extended_DRX_Parameters() {}

//------------------------------------------------------------------------------
void Extended_DRX_Parameters::setValue(uint8_t value) {
  _value = value & 0x0F;
}

//------------------------------------------------------------------------------
void Extended_DRX_Parameters::setPaging_time(uint8_t value) {
  _paging_time = value & 0x0F;
}

//------------------------------------------------------------------------------
uint8_t Extended_DRX_Parameters::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
uint8_t Extended_DRX_Parameters::getPaging_time() {
  return _paging_time;
}

//------------------------------------------------------------------------------
int Extended_DRX_Parameters::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding Extended_DRX_Parameters iei(0x%x)", _iei);
  if (len < 3) {
    Logger::nas_mm().error("len is less than 3");
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = 1;
    encoded_size++;
    *(buf + encoded_size) = (0x0F & _value) | ((_paging_time & 0x0f) << 4);
    encoded_size++;
  } else {
    //	*(buf + encoded_size) = length - 1; encoded_size++;
    //	*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug(
      "encoded Extended_DRX_Parameters len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Extended_DRX_Parameters::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding Extended_DRX_Parameters iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  _value = 0x00;
  //	length = *(buf + decoded_size);
  decoded_size++;
  _value       = *(buf + decoded_size) & 0x0f;
  _paging_time = *(buf + decoded_size) & 0xf0;
  decoded_size++;
  Logger::nas_mm().debug(
      "decoded Extended_DRX_Parameters paging_time(0x%x) value(0x%x) ",
      _paging_time, _value);
  Logger::nas_mm().debug(
      "decoded Extended_DRX_Parameters len(%d)", decoded_size);
  return decoded_size;
}
