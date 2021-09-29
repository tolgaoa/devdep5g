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
#include "EPS_NAS_Security_Algorithms.hpp"

#include <iostream>

#include "logger.hpp"
using namespace nas;
using namespace std;

//------------------------------------------------------------------------------
EPS_NAS_Security_Algorithms::EPS_NAS_Security_Algorithms() {}

//------------------------------------------------------------------------------
EPS_NAS_Security_Algorithms::~EPS_NAS_Security_Algorithms() {}

//------------------------------------------------------------------------------
EPS_NAS_Security_Algorithms::EPS_NAS_Security_Algorithms(
    uint8_t iei, uint8_t ciphering, uint8_t integrity_protection) {
  _iei                 = iei;
  CIPHERING            = ciphering;
  INTEGRITY_PROTECTION = integrity_protection;
}

//------------------------------------------------------------------------------
void EPS_NAS_Security_Algorithms::setCIPHERING(uint8_t value) {
  CIPHERING = value;
}

//------------------------------------------------------------------------------
void EPS_NAS_Security_Algorithms::setINTEGRITY_PROTECTION(uint8_t value) {
  INTEGRITY_PROTECTION = value;
}

//------------------------------------------------------------------------------
uint8_t EPS_NAS_Security_Algorithms::getCIPHERING() {
  return CIPHERING;
}

//------------------------------------------------------------------------------
uint8_t EPS_NAS_Security_Algorithms::getINTEGRITY_PROTECTION() {
  return INTEGRITY_PROTECTION;
}

//------------------------------------------------------------------------------
int EPS_NAS_Security_Algorithms::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug(
      "encoding EPS_NAS_Security_Algorithms iei(0x%x)", _iei);
  int encoded_size = 0;
  if (len < 2) {
    Logger::nas_mm().error("len is less than 2");
    return -1;
  }

  else {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) =
        ((CIPHERING & 0x07) << 4) | (INTEGRITY_PROTECTION & 0x07);
    encoded_size++;
    Logger::nas_mm().debug("encoded EPS_NAS_Security_Algorithms IE 0x%x", *buf);
  }
  return encoded_size;
}

//------------------------------------------------------------------------------
int EPS_NAS_Security_Algorithms::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding EPS_NAS_Security_Algorithms IE");
  if (len < 2) {
    Logger::nas_mm().error("len is less than 2");
    return 0;
  } else {
    int decoded_size = 0;
    decoded_size++;
    CIPHERING            = (*(buf + decoded_size) & 0x70) >> 4;
    INTEGRITY_PROTECTION = *(buf + decoded_size) & 0x07;
    decoded_size++;
    Logger::nas_mm().debug(
        "decoded NAS_Security_Algorithms len 1 "
        "octet,CIPHERING=0x%x,INTEGRITY_PROTECTION=0x%x",
        CIPHERING, INTEGRITY_PROTECTION);
    return decoded_size;
  }
}
