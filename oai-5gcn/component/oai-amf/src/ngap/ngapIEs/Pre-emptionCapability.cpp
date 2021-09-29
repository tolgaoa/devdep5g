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

#include "Pre-emptionCapability.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
Pre_emptionCapability::Pre_emptionCapability() {
  capability = 0;
}

//------------------------------------------------------------------------------
Pre_emptionCapability::~Pre_emptionCapability() {}

//------------------------------------------------------------------------------
void Pre_emptionCapability::setPre_emptionCapability(
    e_Ngap_Pre_emptionCapability value) {
  capability = value;
}

//------------------------------------------------------------------------------
bool Pre_emptionCapability::getPre_emptionCapability(
    e_Ngap_Pre_emptionCapability& value) {
  if ((capability == 0) || (capability == 1))
    value = (e_Ngap_Pre_emptionCapability) capability;
  else
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool Pre_emptionCapability::encode2Pre_emptionCapability(
    Ngap_Pre_emptionCapability_t* pre_emptionCapability) {
  *pre_emptionCapability = capability;

  return true;
}

//------------------------------------------------------------------------------
bool Pre_emptionCapability::decodefromPre_emptionCapability(
    Ngap_Pre_emptionCapability_t* pre_emptionCapability) {
  capability = *pre_emptionCapability;

  return true;
}
}  // namespace ngap
