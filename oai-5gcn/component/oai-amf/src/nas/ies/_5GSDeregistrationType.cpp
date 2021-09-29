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
 \author
 \date 2020
 \email: contact@openairinterface.org
 */

#include "_5GSDeregistrationType.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GSDeregistrationType::_5GSDeregistrationType() {
  u1.b = 0;
}

//------------------------------------------------------------------------------
_5GSDeregistrationType::_5GSDeregistrationType(uint8_t type) {
  u1.b = type;
}
//------------------------------------------------------------------------------
_5GSDeregistrationType::_5GSDeregistrationType(
    _5gs_deregistration_type_t type) {
  u1.b                 = 0;
  u1.bf.switch_off     = type.switch_off;
  u1.bf.dereg_required = type.dereg_required;
  u1.bf.access_type    = type.access_type;
  u1.bf.iei            = type.iei;
}

//------------------------------------------------------------------------------
_5GSDeregistrationType::~_5GSDeregistrationType() {}

//------------------------------------------------------------------------------
void _5GSDeregistrationType::set(_5gs_deregistration_type_t type) {
  u1.b                 = 0;
  u1.bf.switch_off     = type.switch_off;
  u1.bf.dereg_required = type.dereg_required;
  u1.bf.access_type    = type.access_type;
  u1.bf.iei            = type.iei;
}

//------------------------------------------------------------------------------
void _5GSDeregistrationType::get(_5gs_deregistration_type_t& type) {
  type.switch_off     = u1.bf.switch_off;
  type.dereg_required = u1.bf.dereg_required;
  type.access_type    = u1.bf.access_type;
  type.iei            = u1.bf.iei;
}

//------------------------------------------------------------------------------
void _5GSDeregistrationType::set(uint8_t type) {
  u1.b = type;
}

//------------------------------------------------------------------------------
void _5GSDeregistrationType::get(uint8_t& type) {
  type = u1.b;
}

//------------------------------------------------------------------------------
int _5GSDeregistrationType::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().error("Encoding 5GSDeregistrationType IE");
  if (len < 1) {
    Logger::nas_mm().error(
        "Encoding 5GSDeregistrationType error (len is less than one)");
    return -1;
  }
  *buf = u1.b;
  return 0;
}

//------------------------------------------------------------------------------
int _5GSDeregistrationType::decodefrombuffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding 5GSDeregistrationType");
  u1.b = *buf;

  Logger::nas_mm().debug("Decoded 5GSDeRegistrationType");
  return 0;
}
