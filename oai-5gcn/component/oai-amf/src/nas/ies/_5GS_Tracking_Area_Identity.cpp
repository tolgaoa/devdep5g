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

/*! \file _5GS_Tracking_Area_Identity.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "_5GS_Tracking_Area_Identity.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
_5GS_Tracking_Area_Identity::_5GS_Tracking_Area_Identity(uint8_t iei) {
  _iei      = iei;
  _MNC_MCC1 = 0;
  _MNC_MCC2 = 0;
  _MNC_MCC3 = 0;
  _TAC1     = 0;
  _TAC2     = 0;
  _TAC3     = 0;
}

//------------------------------------------------------------------------------
_5GS_Tracking_Area_Identity::_5GS_Tracking_Area_Identity(
    const uint8_t iei, uint8_t MNC_MCC1, uint8_t MNC_MCC2, uint8_t MNC_MCC3,
    uint32_t TAC) {
  _iei      = iei;
  _MNC_MCC1 = MNC_MCC1;
  _MNC_MCC2 = MNC_MCC2;
  _MNC_MCC3 = MNC_MCC3;
  _TAC1     = TAC & 0x000000FF;
  // Logger::nas_mm().debug("decoded
  // _5GS_Tracking_Area_Identity,buf(0x%x)0000000",_TAC1);
  _TAC2 = (TAC & 0x0000FF00) >> 8;
  // Logger::nas_mm().debug("decoded
  // _5GS_Tracking_Area_Identity,buf(0x%x)0000000",_TAC2);
  _TAC3 = (TAC & 0x00FF0000) >> 16;
  // Logger::nas_mm().debug("decoded
  // _5GS_Tracking_Area_Identity,buf(0x%x)0000000",_TAC3);
}

//------------------------------------------------------------------------------
_5GS_Tracking_Area_Identity::_5GS_Tracking_Area_Identity() {
  _iei      = 0;
  _MNC_MCC1 = 0;
  _MNC_MCC2 = 0;
  _MNC_MCC3 = 0;
  _TAC1     = 0;
  _TAC2     = 0;
  _TAC3     = 0;
}
_5GS_Tracking_Area_Identity::~_5GS_Tracking_Area_Identity() {}

//------------------------------------------------------------------------------
void _5GS_Tracking_Area_Identity::setMNC_MCC1(uint8_t iei, uint8_t value) {
  _iei      = iei;
  _MNC_MCC1 = value;
}

//------------------------------------------------------------------------------
void _5GS_Tracking_Area_Identity::setMNC_MCC2(uint8_t iei, uint8_t value) {
  _iei      = iei;
  _MNC_MCC2 = value;
}

//------------------------------------------------------------------------------
void _5GS_Tracking_Area_Identity::setMNC_MCC3(uint8_t iei, uint8_t value) {
  _iei      = iei;
  _MNC_MCC3 = value;
}

//------------------------------------------------------------------------------
void _5GS_Tracking_Area_Identity::setTAC(uint8_t iei, uint32_t value) {
  _iei  = iei;
  _TAC1 = value & 0x000000FF;
  _TAC2 |= value & 0x0000FF00 >> 8;
  _TAC3 |= value & 0x00FF0000 >> 16;
}

//------------------------------------------------------------------------------
uint8_t _5GS_Tracking_Area_Identity::getMNC_MCC1() {
  return _MNC_MCC1;
}

//------------------------------------------------------------------------------
uint8_t _5GS_Tracking_Area_Identity::getMNC_MCC2() {
  return _MNC_MCC2;
}

//------------------------------------------------------------------------------
uint8_t _5GS_Tracking_Area_Identity::getMNC_MCC3() {
  return _MNC_MCC3;
}

//------------------------------------------------------------------------------
uint32_t _5GS_Tracking_Area_Identity::getTAC() {
  uint32_t tac;
  tac = _MNC_MCC3;
  tac << 8;
  tac = _MNC_MCC2;
  tac << 8;
  tac = _MNC_MCC1;
  return tac;
}

//------------------------------------------------------------------------------
int _5GS_Tracking_Area_Identity::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug(
      "encoding _5GS_Tracking_Area_Identity iei(0x%x)", _iei);
  if (len < 7) {
    //		Logger::nas_mm().error("len is less than %d", length);
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    //*(buf + encoded_size) = length - 2; encoded_size++;
    *(buf + encoded_size) = (_MNC_MCC1 & 0x0F) | ((_MNC_MCC2 & 0x0F) << 4);
    encoded_size++;
    //	Logger::nas_mm().debug("decoded
    //_5GS_Tracking_Area_Identity,buf(0x%x)",*(buf+encoded_size-1));
    *(buf + encoded_size) = _MNC_MCC3;
    encoded_size++;
    //	Logger::nas_mm().debug("decoded
    //_5GS_Tracking_Area_Identity,buf(0x%x)",*(buf+encoded_size-1));
    *(buf + encoded_size) = ((_MNC_MCC1 & 0xF0) >> 4) | (_MNC_MCC2 & 0xF0);
    encoded_size++;
    //	Logger::nas_mm().debug("decoded
    //_5GS_Tracking_Area_Identity,buf(0x%x)",*(buf+encoded_size-1));
    *(buf + encoded_size) = _TAC1;
    encoded_size++;
    //	Logger::nas_mm().debug("decoded
    //_5GS_Tracking_Area_Identity,buf(0x%x)",*(buf+encoded_size-1));
    *(buf + encoded_size) = _TAC2;
    encoded_size++;
    //	Logger::nas_mm().debug("decoded
    //_5GS_Tracking_Area_Identity,buf(0x%x)",*(buf+encoded_size-1));
    *(buf + encoded_size) = _TAC3;
    encoded_size++;
    //	Logger::nas_mm().debug("decoded
    //_5GS_Tracking_Area_Identity,buf(0x%x)",*(buf+encoded_size-1));
  } else {
    //	*(buf + encoded_size) = length - 1; encoded_size++;
    //	*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug(
      "encoded _5GS_Tracking_Area_Identity len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GS_Tracking_Area_Identity::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug(
      "decoding _5GS_Tracking_Area_Identity iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  _MNC_MCC1 = 0x00;
  _MNC_MCC2 = 0x00;
  _MNC_MCC3 = 0x00;
  _MNC_MCC1 |= *(buf + decoded_size) & 0x0F;
  //	Logger::nas_mm().debug("decoded
  //_5GS_Tracking_Area_Identity,buf(0x%x)",*(buf+decoded_size));
  //	Logger::nas_mm().debug("decoded
  //_5GS_Tracking_Area_Identity,buf(0x%x)",(_MNC_MCC1));
  _MNC_MCC2 |= (*(buf + decoded_size) & 0xF0) >> 4;
  decoded_size++;
  //	Logger::nas_mm().debug("decoded
  //_5GS_Tracking_Area_Identity,buf(0x%x)",(_MNC_MCC2));
  _MNC_MCC3 = *(buf + decoded_size);
  decoded_size++;
  _MNC_MCC1 |= (*(buf + decoded_size) & 0x0F) << 4;
  //	Logger::nas_mm().debug("decoded
  //_5GS_Tracking_Area_Identity,buf(0x%x)",(_MNC_MCC1));
  _MNC_MCC2 |= *(buf + decoded_size) & 0xF0;
  decoded_size++;
  //	Logger::nas_mm().debug("decoded
  //_5GS_Tracking_Area_Identity,buf(0x%x)",(_MNC_MCC2));
  _TAC1 = *(buf + decoded_size);
  decoded_size++;
  _TAC2 = *(buf + decoded_size);
  decoded_size++;
  _TAC3 = *(buf + decoded_size);
  decoded_size++;
  Logger::nas_mm().debug(
      "decoded _5GS_Tracking_Area_Identity "
      "MNC_MCC1(0x%x),MNC_MCC2(0x%x),MNC_MCC3(0x%x),TAC(0x%8x)",
      _MNC_MCC1, _MNC_MCC2, _MNC_MCC3, (_TAC1 << 16) | (_TAC2 << 8) | (_TAC3));
  Logger::nas_mm().debug(
      "decoded _5GS_Tracking_Area_Identity len(%d)", decoded_size);
  return decoded_size;
}
