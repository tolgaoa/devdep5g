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

#include "PLMN_List.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
PLMN_List::PLMN_List(uint8_t iei) {
  _iei      = iei;
  _MNC_MCC1 = 0;
  _MNC_MCC2 = 0;
  _MNC_MCC3 = 0;
}

//------------------------------------------------------------------------------
PLMN_List::PLMN_List(
    const uint8_t iei, uint8_t MNC_MCC1, uint8_t MNC_MCC2, uint8_t MNC_MCC3) {
  _iei      = iei;
  _MNC_MCC1 = MNC_MCC1;
  _MNC_MCC2 = MNC_MCC2;
  _MNC_MCC3 = MNC_MCC3;
}

//------------------------------------------------------------------------------
PLMN_List::PLMN_List() {
  _iei      = 0;
  _MNC_MCC1 = 0;
  _MNC_MCC2 = 0;
  _MNC_MCC3 = 0;
}

//------------------------------------------------------------------------------
PLMN_List::~PLMN_List() {}

//------------------------------------------------------------------------------
void PLMN_List::setMNC_MCC1(uint8_t iei, uint8_t value) {
  _iei      = iei;
  _MNC_MCC1 = value;
}

//------------------------------------------------------------------------------
void PLMN_List::setMNC_MCC2(uint8_t iei, uint8_t value) {
  _iei      = iei;
  _MNC_MCC2 = value;
}

//------------------------------------------------------------------------------
void PLMN_List::setMNC_MCC3(uint8_t iei, uint8_t value) {
  _iei      = iei;
  _MNC_MCC3 = value;
}

//------------------------------------------------------------------------------
uint8_t PLMN_List::getMNC_MCC1() {
  return _MNC_MCC1;
}

//------------------------------------------------------------------------------
uint8_t PLMN_List::getMNC_MCC2() {
  return _MNC_MCC2;
}

//------------------------------------------------------------------------------
uint8_t PLMN_List::getMNC_MCC3() {
  return _MNC_MCC3;
}

//------------------------------------------------------------------------------
int PLMN_List::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding PLMN_List iei(0x%x)", _iei);
  if (len < 5) {
    Logger::nas_mm().error("len is less than 5");
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = 3;
    encoded_size++;
    *(buf + encoded_size) = (_MNC_MCC1 & 0x0F) | ((_MNC_MCC2 & 0x0F) << 4);
    encoded_size++;
    *(buf + encoded_size) = _MNC_MCC3;
    encoded_size++;
    *(buf + encoded_size) = ((_MNC_MCC1 & 0xF0) >> 4) | (_MNC_MCC2 & 0xF0);
    encoded_size++;
  } else {
    //	*(buf + encoded_size) = length - 1; encoded_size++;
    //	*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug("encoded PLMN_List len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int PLMN_List::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding PLMN_List iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  decoded_size++;
  _MNC_MCC1 = 0x00;
  _MNC_MCC2 = 0x00;
  _MNC_MCC3 = 0x00;
  _MNC_MCC1 |= *(buf + decoded_size) & 0x0F;
  _MNC_MCC2 |= (*(buf + decoded_size) & 0xF0) >> 4;
  decoded_size++;
  _MNC_MCC3 = *(buf + decoded_size);
  decoded_size++;
  _MNC_MCC1 |= (*(buf + decoded_size) & 0x0F) << 4;
  _MNC_MCC2 |= *(buf + decoded_size) & 0xF0;
  decoded_size++;
  Logger::nas_mm().debug(
      "decoded PLMN_List MNC_MCC1(0x%x),MNC_MCC2(0x%x),MNC_MCC3(0x%x)",
      _MNC_MCC1, _MNC_MCC2, _MNC_MCC3);
  Logger::nas_mm().debug("decoded PLMN_List len(%d)", decoded_size);
  return decoded_size;
}
