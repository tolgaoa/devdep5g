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

#include "ServiceType.hpp"
using namespace nas;

//------------------------------------------------------------------------------
ServiceType::ServiceType() {}

//------------------------------------------------------------------------------
ServiceType::~ServiceType() {}

//------------------------------------------------------------------------------
ServiceType::ServiceType(uint8_t iei, uint8_t stp) {
  _iei  = iei;
  value = stp;
}

//------------------------------------------------------------------------------
int ServiceType::encode2buffer(uint8_t* buf, int len) {
  if (len < 1) return -1;
  int encoded_size = 0;
  if (_iei) {
    *buf = _iei;
    encoded_size++;
    *(buf + encoded_size) = value;
    encoded_size++;
    return encoded_size;
  } else {
    *buf = 0x00 | ((value & 0x0f) << 4);
    return 0;
  }
}

//------------------------------------------------------------------------------
int ServiceType::decodefrombuffer(
    uint8_t* buf, int len, bool is_optional, bool is_high) {
  if (len < 1) return -1;
  if (is_optional) {
    _iei = *buf;
  } else {
    if (is_high)
      value = ((*buf) & 0xf0) >> 4;
    else
      value = (*buf) & 0x0f;
    return 0;
  }
  return 0;
}

//------------------------------------------------------------------------------
uint8_t ServiceType::getValue() {
  return value;
}
