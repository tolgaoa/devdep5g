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

#include "UES_Usage_Setting.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
UES_Usage_Setting::UES_Usage_Setting(uint8_t iei) {
  _iei               = iei;
  _ues_usage_setting = false;
}

//------------------------------------------------------------------------------
UES_Usage_Setting::UES_Usage_Setting(
    const uint8_t iei, bool ues_usage_setting) {
  _iei               = iei;
  _ues_usage_setting = ues_usage_setting;
}

//------------------------------------------------------------------------------
UES_Usage_Setting::UES_Usage_Setting() {
  _iei               = 0;
  _ues_usage_setting = false;
}

//------------------------------------------------------------------------------
UES_Usage_Setting::~UES_Usage_Setting() {}

//------------------------------------------------------------------------------
void UES_Usage_Setting::setValue(bool value) {
  _ues_usage_setting = value;
}

//------------------------------------------------------------------------------
bool UES_Usage_Setting::getValue() {
  return _ues_usage_setting;
}

//------------------------------------------------------------------------------
int UES_Usage_Setting::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding UES_Usage_Setting iei(0x%x)", _iei);
  if (len < 3) {
    //	Logger::nas_mm().error("len is less than %d", length);
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    Logger::nas_mm().debug(
        "encoded UES_Usage_Setting len(%d)", *(buf + encoded_size - 1));
    *(buf + encoded_size) = 1;
    encoded_size++;
    *(buf + encoded_size) = 0x01 & _ues_usage_setting;
    encoded_size++;
  } else {
    //	*(buf + encoded_size) = length - 1; encoded_size++;
    //	*(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug("encoded UES_Usage_Setting len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int UES_Usage_Setting::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding UES_Usage_Setting iei(0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  _ues_usage_setting = 0x00;
  //	length = *(buf + decoded_size);
  decoded_size++;
  _ues_usage_setting = *(buf + decoded_size) & 0x01;
  decoded_size++;
  Logger::nas_mm().debug(
      "decoded UES_Usage_Setting _ues_usage_setting(0x%x),iei(0x%x)",
      _ues_usage_setting, _iei);
  Logger::nas_mm().debug("decoded UES_Usage_Setting len(%d)", decoded_size);
  return decoded_size;
}
