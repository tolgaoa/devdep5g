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
#include "EPS_NAS_Message_Container.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
EPS_NAS_Message_Container::EPS_NAS_Message_Container(uint8_t iei) : _value() {
  _iei   = iei;
  length = 0;
}

//------------------------------------------------------------------------------
EPS_NAS_Message_Container::EPS_NAS_Message_Container(
    const uint8_t iei, bstring value) {
  _iei   = iei;
  _value = bstrcpy(value);
  length = blength(value) + 3;
}

//------------------------------------------------------------------------------
EPS_NAS_Message_Container::EPS_NAS_Message_Container()
    : _iei(), length(), _value() {}

//------------------------------------------------------------------------------
EPS_NAS_Message_Container::~EPS_NAS_Message_Container() {}

//------------------------------------------------------------------------------
void EPS_NAS_Message_Container::setValue(uint8_t iei, uint8_t value) {
  _iei = iei;
  //_value = value;
}

//------------------------------------------------------------------------------
void EPS_NAS_Message_Container::getValue(bstring& value) {
  value = bstrcpy(_value);
}

//------------------------------------------------------------------------------
int EPS_NAS_Message_Container::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding EPS_NAS_Message_Container iei (0x%x)", _iei);
  if (len < length) {
    Logger::nas_mm().error("Len is less than %d", length);
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = (length - 3) & 0x00ff;
    encoded_size++;
    *(buf + encoded_size) = ((length - 3) & 0xff000) >> 8;
    encoded_size++;
    int size = encode_bstring(_value, (buf + encoded_size), len - encoded_size);
    encoded_size += size;

  } else {
    //		*(buf + encoded_size) = length - 1; encoded_size++;
    //		*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug(
      "Encoded EPS_NAS_Message_Container len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int EPS_NAS_Message_Container::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding EPS_NAS_Message_Container iei (0x%x)", *buf);
  int decoded_size = 0;
  int result       = 0;
  if (is_option) {
    decoded_size++;
  }
  length = 0;
  DECODE_U16(buf + decoded_size, length, decoded_size);
  result =
      decode_bstring(&_value, length, (buf + decoded_size), len - decoded_size);
  decoded_size += length;
  if (result == length) {
    for (int i = 0; i < length; i++) {
      Logger::nas_mm().debug(
          "Decoded EPS_NAS_Message_Container value (0x%x)",
          (uint8_t) _value->data[i]);
    }
  }

  Logger::nas_mm().debug(
      "Decoded EPS_NAS_Message_Container Length (%d)", decoded_size);
  return decoded_size;
}
