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

#include "GlobalgNBId.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
GlobalgNBId::GlobalgNBId() {
  plmnId = NULL;
  gNB_ID = NULL;
}

//------------------------------------------------------------------------------
GlobalgNBId::~GlobalgNBId() {}

//------------------------------------------------------------------------------
void GlobalgNBId::setGlobalgNBId(PlmnId* plmn, GNB_ID* gnbid) {
  plmnId = plmn;
  gNB_ID = gnbid;
}

//------------------------------------------------------------------------------
bool GlobalgNBId::encode2GlobalgNBId(Ngap_GlobalGNB_ID_t* globalgnbid) {
  if (!plmnId->encode2octetstring(globalgnbid->pLMNIdentity)) return false;
  if (!gNB_ID->encode2bitstring(globalgnbid->gNB_ID)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool GlobalgNBId::decodefromGlobalgNBId(Ngap_GlobalGNB_ID_t* globalgnbid) {
  if (plmnId == nullptr) plmnId = new PlmnId();
  if (gNB_ID == nullptr) gNB_ID = new GNB_ID();
  if (!plmnId->decodefromoctetstring(globalgnbid->pLMNIdentity)) return false;
  if (!gNB_ID->decodefrombitstring(globalgnbid->gNB_ID)) return false;

  return true;
}

//------------------------------------------------------------------------------
void GlobalgNBId::getGlobalgNBId(PlmnId*& plmn, GNB_ID*& gnbid) {
  plmn  = plmnId;
  gnbid = gNB_ID;
}
}  // namespace ngap
