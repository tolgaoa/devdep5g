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

/*! \file 5GMMCapability.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "5GMMCapability.hpp"

#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GMMCapability::_5GMMCapability(const uint8_t iei, uint8_t value) {
  m_iei   = iei;
  m_value = value;
  length  = 3;
}

//------------------------------------------------------------------------------
_5GMMCapability::_5GMMCapability() {}

//------------------------------------------------------------------------------
_5GMMCapability::~_5GMMCapability() {}

//------------------------------------------------------------------------------
void _5GMMCapability::setValue(uint8_t iei, uint8_t value) {
  m_iei   = iei;
  m_value = value;
}

//------------------------------------------------------------------------------
uint8_t _5GMMCapability::getValue() {
  return m_value;
}

//------------------------------------------------------------------------------
int _5GMMCapability::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding _5GMMCapability iei(0x%x)", m_iei);
  if (len < length) {
    Logger::nas_mm().error("len is less than %d", length);
    return 0;
  }
  int encoded_size = 0;
  if (m_iei) {
    *(buf + encoded_size) = m_iei;
    encoded_size++;
    *(buf + encoded_size) = length - 2;
    encoded_size++;
    *(buf + encoded_size) = m_value;
    encoded_size++;
  } else {
    *(buf + encoded_size) = length - 1;
    encoded_size++;
    *(buf + encoded_size) = m_value;
    encoded_size++;
  }
  Logger::nas_mm().debug("encoded _5GMMCapability len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GMMCapability::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding _5GMMCapability iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  m_value = *(buf + decoded_size);
  decoded_size++;
  Logger::nas_mm().debug("decoded _5GMMCapability value(0x%x)", m_value);
  Logger::nas_mm().debug("decoded _5GMMCapability len(%d)", decoded_size);
  return decoded_size;
}
