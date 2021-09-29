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

#include "5GSRegistrationType.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GSRegistrationType::_5GSRegistrationType() {
  iei      = 0;
  is_for   = false;
  reg_type = 0;
}

//------------------------------------------------------------------------------
_5GSRegistrationType::_5GSRegistrationType(bool is_for, uint8_t type) {
  this->is_for   = is_for;
  this->reg_type = 0x07 & type;
  this->iei      = 0;
}

//------------------------------------------------------------------------------
_5GSRegistrationType::_5GSRegistrationType(
    uint8_t iei, bool is_for, uint8_t type) {
  this->is_for   = is_for;
  this->reg_type = 0x07 & type;
  this->iei      = 0x0f & iei;
}

//------------------------------------------------------------------------------
_5GSRegistrationType::~_5GSRegistrationType() {}

//------------------------------------------------------------------------------
int _5GSRegistrationType::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().error("encoding 5gsregistrationtype IE");
  if (len < 1) {
    Logger::nas_mm().error(
        "encoding 5gsregistrationtype error(len is less than one)");
    return -1;
  }
  uint8_t octet = 0;
  if (is_for) octet = 0x08;
  octet |= reg_type;
  if (!(iei & 0x0f)) {
    *buf = 0x0f & octet;
    Logger::nas_mm().debug("encoded 5GSRegistrationType IE(len(1/2 octet))");
    return 0;
  } else {
    *buf = (iei << 4) | octet;
    Logger::nas_mm().debug("encoded 5GSRegistrationType IE(len(1 octet))");
    return 1;
  }
}

//------------------------------------------------------------------------------
int _5GSRegistrationType::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  if (is_option) {
    return -1;
  }
  Logger::nas_mm().debug("Decoding 5GSRegistrationType");
  uint8_t octet = *buf;
  if (octet & 0x08)
    is_for = FOLLOW_ON_REQ_PENDING;
  else
    is_for = NO_FOLLOW_ON_REQ_PENDING;
  reg_type = 0x07 & octet;
  Logger::nas_mm().debug("Decoded 5GSRegistrationType len (1/2 octet)");
  return 0;
}

//------------------------------------------------------------------------------
void _5GSRegistrationType::setFollowOnReq(const bool is) {
  is_for = is;
}

//------------------------------------------------------------------------------
void _5GSRegistrationType::setRegType(const uint8_t type) {
  reg_type = 0x07 & type;
}

//------------------------------------------------------------------------------
bool _5GSRegistrationType::isFollowOnReq() {
  return is_for;
}

//------------------------------------------------------------------------------
uint8_t _5GSRegistrationType::getRegType() {
  return reg_type;
}
