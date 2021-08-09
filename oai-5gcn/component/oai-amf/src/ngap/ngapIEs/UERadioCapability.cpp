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

#include "UERadioCapability.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UERadioCapability::UERadioCapability() {
  ueRadioCapabilitybuffer       = NULL;
  sizeofueRadioCapabilitybuffer = -1;
}

//------------------------------------------------------------------------------
UERadioCapability::~UERadioCapability() {}

//------------------------------------------------------------------------------
bool UERadioCapability::encode2UERadioCapability(
    Ngap_UERadioCapability_t& ueRadioCapability) {
  int ret;
  ret = OCTET_STRING_fromBuf(
      &ueRadioCapability, ueRadioCapabilitybuffer,
      sizeofueRadioCapabilitybuffer);
  if (ret != 0) return false;
  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapability::decodefromUERadioCapability(
    Ngap_UERadioCapability_t& ueRadioCapability) {
  ueRadioCapabilitybuffer       = (char*) ueRadioCapability.buf;
  sizeofueRadioCapabilitybuffer = ueRadioCapability.size;
  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapability::getUERadioCapability(uint8_t*& buffer, size_t& size) {
  buffer = (uint8_t*) ueRadioCapabilitybuffer;
  size   = sizeofueRadioCapabilitybuffer;
  if (!ueRadioCapabilitybuffer) return false;
  if (sizeofueRadioCapabilitybuffer < 0) return false;

  return true;
}

//------------------------------------------------------------------------------
void UERadioCapability::setUERadioCapability(uint8_t* buffer, size_t size) {
  ueRadioCapabilitybuffer       = (char*) buffer;
  sizeofueRadioCapabilitybuffer = size;
}
}  // namespace ngap
