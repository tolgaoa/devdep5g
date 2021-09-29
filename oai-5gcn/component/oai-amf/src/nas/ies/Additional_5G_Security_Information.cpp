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

/*! \file Additional_5G_Security_Information.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "Additional_5G_Security_Information.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Additional_5G_Security_Information::Additional_5G_Security_Information(
    uint8_t iei) {
  _iei  = iei;
  RINMR = false;
  HDP   = false;
}

//------------------------------------------------------------------------------
Additional_5G_Security_Information::Additional_5G_Security_Information(
    const uint8_t iei, bool rinmr, bool hdp) {
  _iei  = iei;
  RINMR = rinmr;
  HDP   = hdp;
}

//------------------------------------------------------------------------------
Additional_5G_Security_Information::Additional_5G_Security_Information()
    : _iei() {
  RINMR = false;
  HDP   = false;
}

//------------------------------------------------------------------------------
Additional_5G_Security_Information::~Additional_5G_Security_Information() {}

//------------------------------------------------------------------------------
void Additional_5G_Security_Information::setRINMR(bool value) {
  RINMR = value;
}

//------------------------------------------------------------------------------
void Additional_5G_Security_Information::setHDP(bool value) {
  HDP = value;
}

//------------------------------------------------------------------------------
bool Additional_5G_Security_Information::getRINMR() {
  return RINMR;
}

//------------------------------------------------------------------------------
bool Additional_5G_Security_Information::getHDP() {
  return HDP;
}

//------------------------------------------------------------------------------
int Additional_5G_Security_Information::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug(
      "encoding Additional_5G_Security_Information iei(0x%x)", _iei);
  if (len < 3) {
    Logger::nas_mm().error("len is less than 3");
    return 0;
  }
  uint8_t octet    = 0;
  int encoded_size = 0;
  if (RINMR) octet |= 0x02;
  if (HDP) octet |= 0x01;
  *(buf + encoded_size) = _iei;
  encoded_size++;
  *(buf + encoded_size) = 1;
  encoded_size++;
  *(buf + encoded_size) = octet;
  encoded_size++;
  Logger::nas_mm().debug(
      "encoded Additional_5G_Security_Information IE(len(1 octet))");
  return encoded_size;
}

//------------------------------------------------------------------------------
int Additional_5G_Security_Information::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  if (len < 3) {
    Logger::nas_mm().error("len is less than 3");
    return 0;
  } else {
    int decoded_size = 0;
    if (is_option) {
      decoded_size++;
    }
    //	length = *(buf + decoded_size);
    decoded_size++;
    uint8_t octet = *(buf + decoded_size);
    decoded_size++;
    RINMR = octet & 0x02;
    HDP   = octet & 0x01;
    Logger::nas_mm().debug(
        "decoded Additional_5G_Security_Information RINMR(0x%x) HDP(0x%x)",
        RINMR, HDP);
    return decoded_size;
  }
}
