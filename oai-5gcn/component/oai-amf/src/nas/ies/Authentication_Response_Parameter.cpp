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

#include "Authentication_Response_Parameter.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Authentication_Response_Parameter::Authentication_Response_Parameter(
    uint8_t iei) {
  _iei = iei;
}

//------------------------------------------------------------------------------
Authentication_Response_Parameter::Authentication_Response_Parameter(
    const uint8_t iei, bstring para) {
  _iei = iei;
  PARA = bstrcpy(para);
}

//------------------------------------------------------------------------------
Authentication_Response_Parameter::Authentication_Response_Parameter() {}

//------------------------------------------------------------------------------
Authentication_Response_Parameter::~Authentication_Response_Parameter() {}

//------------------------------------------------------------------------------
void Authentication_Response_Parameter::getValue(bstring& para) {
  para = bstrcpy(PARA);
}

//------------------------------------------------------------------------------
int Authentication_Response_Parameter::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug(
      "Encoding Authentication_Response_Parameter IEI 0x%x", _iei);
  if (len < 18) {
    Logger::nas_mm().error("len is less than 18");
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = 16;
    encoded_size++;
    int size = encode_bstring(PARA, (buf + encoded_size), len - encoded_size);
    encoded_size += size;
    return encoded_size;
  } else {
    //		*(buf + encoded_size) = length - 1; encoded_size++;
    //		*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug(
      "Encoded Authentication_Response_Parameter (len %d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Authentication_Response_Parameter::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug(
      "Decoding Authentication_Response_Parameter IEI 0x%x", *buf);
  int decoded_size = 0;
  uint8_t length   = 0;
  if (is_option) {
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  decode_bstring(&PARA, length, (buf + decoded_size), len - decoded_size);
  decoded_size += length;
  for (int i = 0; i < length; i++) {
    Logger::nas_mm().debug(
        "Decoded NAS_Message_Container value 0x%x", (uint8_t) PARA->data[i]);
  }
  Logger::nas_mm().debug(
      "Decoded Authentication_Response_Parameter (len %d)", decoded_size);
  return decoded_size;
}
