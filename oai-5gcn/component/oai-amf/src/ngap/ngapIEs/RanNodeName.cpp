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

#include "RanNodeName.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
RanNodeName::RanNodeName() {
  ranNodeName = NULL;
}

//------------------------------------------------------------------------------
RanNodeName::~RanNodeName() {}

//------------------------------------------------------------------------------
void RanNodeName::setValue(const std::string ranName) {
  ranNodeName = (char*) ranName.c_str();
}

//------------------------------------------------------------------------------
bool RanNodeName::getValue(std::string& ranName) {
  if (!ranNodeName) return false;
  ranName = ranNodeName;
  return true;
}

//------------------------------------------------------------------------------
bool RanNodeName::encode2RanNodeName(Ngap_RANNodeName_t* rannodename) {
  int ret = OCTET_STRING_fromBuf(rannodename, ranNodeName, strlen(ranNodeName));

  if (ret == 0)
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------
bool RanNodeName::decodefromRanNodeName(Ngap_RANNodeName_t* rannodename) {
  if (!rannodename->buf) return false;
  ranNodeName = (char*) rannodename->buf;
  return true;
}

}  // namespace ngap
