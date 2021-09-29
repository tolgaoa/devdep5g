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

#include "Authentication_Failure_Parameter.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Authentication_Failure_Parameter::Authentication_Failure_Parameter(
    uint8_t iei) {
  _iei   = iei;
  length = 0;
  value  = 0;
}

//------------------------------------------------------------------------------
Authentication_Failure_Parameter::Authentication_Failure_Parameter(
    const uint8_t iei, bstring auts) {
  _iei   = iei;
  value  = bstrcpy(auts);
  length = blength(auts) + 2;
}

//------------------------------------------------------------------------------
Authentication_Failure_Parameter::Authentication_Failure_Parameter()
    : _iei(), length(), value() {}

//------------------------------------------------------------------------------
Authentication_Failure_Parameter::~Authentication_Failure_Parameter() {}

//------------------------------------------------------------------------------
void Authentication_Failure_Parameter::getValue(bstring& auts) {
  auts = bstrcpy(value);
}

//------------------------------------------------------------------------------
int Authentication_Failure_Parameter::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug(
      "encoding Authentication_Failure_Parameter iei(0x%x)", _iei);
  if (len < length) {
    Logger::nas_mm().error("len is less than %x", length);
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = length - 2;
    encoded_size++;
    int size = encode_bstring(value, (buf + encoded_size), len - encoded_size);
    encoded_size += size;
    return encoded_size;
  } else {
    //		*(buf + encoded_size) = length - 1; encoded_size++;
    //		*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug(
      "Encoded Authentication_Failure_Parameter len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Authentication_Failure_Parameter::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug(
      "Decoding Authentication_Failure_Parameter iei (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  Logger::nas_mm().debug(
      "Decoded IE Authentication_Failure_Parameter length (%d)", length);
  decode_bstring(&value, length, (buf + decoded_size), len - decoded_size);
  decoded_size += length;
  for (int i = 0; i < length; i++) {
    Logger::nas_mm().debug(
        "Decoded Authentication_Failure_Parameter value (0x%x)",
        (uint8_t) value->data[i]);
  }
  Logger::nas_mm().debug(
      "Decoded Authentication_Failure_Parameter len (%d)", decoded_size);
  return decoded_size;
}
