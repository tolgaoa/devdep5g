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

#include "NasKeySetIdentifier.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier(
    const uint8_t m_iei, uint8_t m_tsc, uint8_t m_key_id) {
  iei    = m_iei;
  tsc    = 0x01 & m_tsc;
  key_id = 0x07 & m_key_id;
  Logger::nas_mm().debug("Encoding NasKeySetIdentifier ...");
}

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier(uint8_t tsc, uint8_t key_id) {
  this->iei    = 0;
  this->tsc    = 0x01 & tsc;
  this->key_id = 0x07 & key_id;
  Logger::nas_mm().debug("Encoding NasKeySetIdentifier???");
}

//------------------------------------------------------------------------------
NasKeySetIdentifier::NasKeySetIdentifier() {}

//------------------------------------------------------------------------------
NasKeySetIdentifier::~NasKeySetIdentifier(){};

//------------------------------------------------------------------------------
int NasKeySetIdentifier::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding NasKeySetIdentifier IE IEI 0x%x", iei);
  if (len < 1) {
    Logger::nas_mm().error("len is less than one");
    return -1;
  } else {
    uint8_t octet = 0;
    if (!(iei & 0x0f)) {
      octet = (0x0f) & ((tsc << 3) | key_id);
      *buf  = octet;
      Logger::nas_mm().debug(
          "Encoded NasKeySetIdentifier IE (TSC 0x%x,Key_id 0x%x)", tsc, key_id);
      return 1;
    } else {
      octet = (iei << 4) | (tsc << 3) | key_id;
      *buf  = octet;
      Logger::nas_mm().debug("Encoded NasKeySetIdentifier IE (len 1 octet)");
      Logger::nas_mm().debug(
          "Encoded NasKeySetIdentifier IE (TSC 0x%x, Key_id 0x%x)", tsc,
          key_id);
      return 1;
    }
  }
}

//------------------------------------------------------------------------------
int NasKeySetIdentifier::decodefrombuffer(
    uint8_t* buf, int len, bool is_option, bool is_high) {
  Logger::nas_mm().debug("Decoding NasKeySetIdentifier IE");
  if (len < 1) {
    Logger::nas_mm().error("len is less than one");
    return -1;
  } else {
    uint8_t octet = (*buf);
    if (is_option) {
      iei = (octet & 0xf0) >> 4;
    } else {
      iei = 0;
    }
    if (!is_high) {
      tsc    = octet & 0x08;
      key_id = octet & 0x07;
    } else {
      tsc    = (octet & 0x80) >> 4;
      key_id = (octet & 0x70) >> 4;
    }
    Logger::nas_mm().debug(
        "Decoded NasKeySetIdentifier IE (TSC 0x%x, Key_id 0x%x)", tsc, key_id);
    if (iei)
      return 1;
    else
      return 0;
  }
}

//------------------------------------------------------------------------------
void NasKeySetIdentifier::setTypeOfSecurityContext(uint8_t type) {
  tsc = 0x01 & type;
}

//------------------------------------------------------------------------------
void NasKeySetIdentifier::setNasKeyIdentifier(uint8_t id) {
  key_id = 0x07 & id;
}

//------------------------------------------------------------------------------
uint8_t NasKeySetIdentifier::getTypeOfSecurityContext() {
  return tsc;
}

//------------------------------------------------------------------------------
uint8_t NasKeySetIdentifier::getasKeyIdentifier() {
  return key_id;
}
