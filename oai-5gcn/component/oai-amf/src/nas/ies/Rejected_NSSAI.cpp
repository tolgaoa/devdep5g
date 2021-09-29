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

#include "Rejected_NSSAI.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Rejected_NSSAI::Rejected_NSSAI(uint8_t iei) {
  _iei            = iei;
  length          = 0;
  _s_nssai_SST    = 0;
  _s_nssai_length = 0;
  _cause          = 0;
}

//------------------------------------------------------------------------------
Rejected_NSSAI::Rejected_NSSAI(const uint8_t iei, uint8_t cause, uint8_t SST) {
  _iei            = iei;
  length          = 4;
  _s_nssai_SST    = SST;
  _s_nssai_length = 2;
  _cause          = cause;
}

//------------------------------------------------------------------------------
Rejected_NSSAI::Rejected_NSSAI() {
  _iei            = 0;
  length          = 0;
  _s_nssai_SST    = 0;
  _s_nssai_length = 0;
  _cause          = 0;
}

//------------------------------------------------------------------------------
Rejected_NSSAI::~Rejected_NSSAI() {}

//------------------------------------------------------------------------------
void Rejected_NSSAI::setSST(uint8_t SST) {
  _s_nssai_SST = SST;
}

//------------------------------------------------------------------------------
void Rejected_NSSAI::setCause(uint8_t SST) {
  _s_nssai_SST = SST;
}

//------------------------------------------------------------------------------
uint8_t Rejected_NSSAI::getCause() {
  return _cause;
}

//------------------------------------------------------------------------------
uint8_t Rejected_NSSAI::getSST() {
  return _s_nssai_SST;
}

//------------------------------------------------------------------------------
int Rejected_NSSAI::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding Rejected_NSSAI iei(0x%x)", _iei);
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
    *(buf + encoded_size) = ((_s_nssai_length & 0x0f) << 4) | (_cause & 0x0f);
    encoded_size++;
    *(buf + encoded_size) = _s_nssai_SST;
    encoded_size++;
  } else {
    //     *(buf + encoded_size) = length - 1; encoded_size++;
    //   *(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug("encoded Rejected_NSSAI len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int Rejected_NSSAI::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding Rejected_NSSAI iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  _s_nssai_length = (*(buf + decoded_size) & 0xf0) >> 4;
  _cause          = *(buf + decoded_size) & 0x0f;
  decoded_size++;
  _s_nssai_SST = *(buf + decoded_size);
  decoded_size++;
  Logger::nas_mm().debug(
      "decoded Rejected_NSSAI value_length(0x%x),cause(0x%x),value_SST(0x%x)",
      _s_nssai_length, _cause, _s_nssai_SST);
  Logger::nas_mm().debug("decoded Rejected_NSSAI len(%d)", decoded_size);
  return decoded_size;
}
