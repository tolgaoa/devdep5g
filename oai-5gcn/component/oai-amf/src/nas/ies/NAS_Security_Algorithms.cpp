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

#include "NAS_Security_Algorithms.hpp"

#include <iostream>

#include "logger.hpp"
using namespace nas;
using namespace std;

//------------------------------------------------------------------------------
NAS_Security_Algorithms::NAS_Security_Algorithms() {}

//------------------------------------------------------------------------------
NAS_Security_Algorithms::~NAS_Security_Algorithms() {}

//------------------------------------------------------------------------------
NAS_Security_Algorithms::NAS_Security_Algorithms(
    uint8_t ciphering, uint8_t integrity_protection) {
  CIPHERING            = ciphering;
  INTEGRITY_PROTECTION = integrity_protection;
}

//------------------------------------------------------------------------------
void NAS_Security_Algorithms::setCIPHERING(uint8_t value) {
  CIPHERING = value;
}

//------------------------------------------------------------------------------
void NAS_Security_Algorithms::setINTEGRITY_PROTECTION(uint8_t value) {
  INTEGRITY_PROTECTION = value;
}

//------------------------------------------------------------------------------
uint8_t NAS_Security_Algorithms::getCIPHERING() {
  return CIPHERING;
}

//------------------------------------------------------------------------------
uint8_t NAS_Security_Algorithms::getINTEGRITY_PROTECTION() {
  return INTEGRITY_PROTECTION;
}

//------------------------------------------------------------------------------
int NAS_Security_Algorithms::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding NAS_Security_Algorithms ");
  if (len < 1) {
    Logger::nas_mm().error("len is less than one");
    return -1;
  } else {
    *buf = ((CIPHERING & 0x0f) << 4) | (INTEGRITY_PROTECTION & 0x0f);
    Logger::nas_mm().debug("encoded NAS_Security_Algorithms IE 0x%x", *buf);
  }
  return 1;
}

//------------------------------------------------------------------------------
int NAS_Security_Algorithms::decodefrombuffer(
    uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding NAS_Security_Algorithms IE");
  if (len < 1) {
    Logger::nas_mm().error("len is less than one");
    return 0;
  } else {
    CIPHERING            = (*buf & 0xf0) >> 4;
    INTEGRITY_PROTECTION = *buf & 0x0f;
    Logger::nas_mm().debug(
        "decoded NAS_Security_Algorithms len 1 "
        "octet,CIPHERING=0x%x,INTEGRITY_PROTECTION=0x%x",
        CIPHERING, INTEGRITY_PROTECTION);
    return 1;
  }
}
