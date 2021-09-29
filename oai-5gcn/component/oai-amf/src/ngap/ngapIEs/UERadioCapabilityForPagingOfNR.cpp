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

#include "UERadioCapabilityForPagingOfNR.hpp"

namespace ngap {

//------------------------------------------------------------------------------
UERadioCapabilityForPagingOfNR::UERadioCapabilityForPagingOfNR() {
  nRbuffer       = NULL;
  sizeofnRbuffer = -1;
}

//------------------------------------------------------------------------------
UERadioCapabilityForPagingOfNR::~UERadioCapabilityForPagingOfNR() {}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPagingOfNR::encode2UERadioCapabilityForPagingOfNR(
    Ngap_UERadioCapabilityForPagingOfNR_t* ueRadioCapabilityForPagingOfNR) {
  int ret;
  ret = OCTET_STRING_fromBuf(
      ueRadioCapabilityForPagingOfNR, nRbuffer, sizeofnRbuffer);
  if (ret != 0) return false;
  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPagingOfNR::decodefromUERadioCapabilityForPagingOfNR(
    Ngap_UERadioCapabilityForPagingOfNR_t* ueRadioCapabilityForPagingOfNR) {
  nRbuffer       = (char*) ueRadioCapabilityForPagingOfNR->buf;
  sizeofnRbuffer = ueRadioCapabilityForPagingOfNR->size;
  return true;
}

//------------------------------------------------------------------------------
bool UERadioCapabilityForPagingOfNR::getUERadioCapabilityForPagingOfNR(
    uint8_t*& buffer, size_t& size) {
  buffer = (uint8_t*) nRbuffer;
  size   = sizeofnRbuffer;
  if (!nRbuffer) return false;
  if (sizeofnRbuffer < 0) return false;

  return true;
}

//------------------------------------------------------------------------------
void UERadioCapabilityForPagingOfNR::setUERadioCapabilityForPagingOfNR(
    uint8_t* buffer, size_t size) {
  nRbuffer       = (char*) buffer;
  sizeofnRbuffer = size;
}
}  // namespace ngap
