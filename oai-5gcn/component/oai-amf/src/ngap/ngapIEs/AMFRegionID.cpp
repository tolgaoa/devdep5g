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

#include "AMFRegionID.hpp"

#include <iostream>

#include "String2Value.hpp"
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
AMFRegionID::AMFRegionID() {
  regionid = 0;
}

//------------------------------------------------------------------------------
AMFRegionID::~AMFRegionID() {}

//------------------------------------------------------------------------------
void AMFRegionID::setAMFRegionID(const std::string charid) {
  regionid = fromString<int>(charid);
}

//------------------------------------------------------------------------------
void AMFRegionID::getAMFRegionID(std::string& charid) {
  charid = to_string(regionid);
}

//------------------------------------------------------------------------------
bool AMFRegionID::encode2bitstring(Ngap_AMFRegionID_t& amfregionid) {
  amfregionid.size = 1;
  uint8_t* buffer  = (uint8_t*) calloc(1, sizeof(uint8_t));
  if (!buffer) return false;
  *buffer                 = regionid;
  amfregionid.buf         = buffer;
  amfregionid.bits_unused = 0;

  return true;
}

//------------------------------------------------------------------------------
bool AMFRegionID::decodefrombitstring(Ngap_AMFRegionID_t& amfregionid) {
  if (!amfregionid.buf) return false;
  regionid = *amfregionid.buf;

  return true;
}
}  // namespace ngap
