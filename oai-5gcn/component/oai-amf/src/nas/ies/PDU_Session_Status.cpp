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

#include "PDU_Session_Status.hpp"

#include "common_defs.h"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
PDU_Session_Status::PDU_Session_Status(uint8_t iei) {
  _iei   = iei;
  _value = 0;
  length = 0;
}

//------------------------------------------------------------------------------
PDU_Session_Status::PDU_Session_Status(const uint8_t iei, uint16_t value) {
  _iei   = iei;
  _value = value;
  length = 4;
}

//------------------------------------------------------------------------------
PDU_Session_Status::PDU_Session_Status() {
  _iei   = 0;
  _value = 0;
  length = 0;
}

//------------------------------------------------------------------------------
PDU_Session_Status::~PDU_Session_Status() {}

//------------------------------------------------------------------------------
void PDU_Session_Status::setValue(uint8_t iei, uint16_t value) {
  _iei   = iei;
  _value = value;
}

//------------------------------------------------------------------------------
uint16_t PDU_Session_Status::getValue() {
  return _value;
}

//------------------------------------------------------------------------------
int PDU_Session_Status::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding PDU_Session_Status iei(0x%x)", _iei);
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
    *(buf + encoded_size) = (_value & 0x00ff);
    encoded_size++;
    *(buf + encoded_size) = (_value & 0xff00) >> 8;
    encoded_size++;
  } else {
    //*(buf + encoded_size) = length - 1; encoded_size++;
    //*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug("encoded PDU_Session_Status len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int PDU_Session_Status::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding PDU_Session_Status iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }

  _value = 0x0000;
  length = *(buf + decoded_size);
  decoded_size++;
  DECODE_U16(buf + decoded_size, _value, decoded_size);
  Logger::nas_mm().debug("decoded PDU_Session_Status value (0x%x)", _value);
  Logger::nas_mm().debug("decoded PDU_Session_Status len(%d)", decoded_size);
  return decoded_size;
}
