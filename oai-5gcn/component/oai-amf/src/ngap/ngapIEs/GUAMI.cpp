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

#include "GUAMI.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
GUAMI::GUAMI() {
  plmnId      = NULL;
  aMFRegionID = NULL;
  aMFSetID    = NULL;
  aMFPointer  = NULL;
}

//------------------------------------------------------------------------------
GUAMI::~GUAMI() {}

//------------------------------------------------------------------------------
void GUAMI::setGUAMI(
    PlmnId* m_plmnId, AMFRegionID* m_aMFRegionID, AMFSetID* m_aMFSetID,
    AMFPointer* m_aMFPointer) {
  plmnId      = m_plmnId;
  aMFRegionID = m_aMFRegionID;
  aMFSetID    = m_aMFSetID;
  aMFPointer  = m_aMFPointer;
}

//------------------------------------------------------------------------------
bool GUAMI::encode2GUAMI(Ngap_GUAMI_t* guami) {
  if (!plmnId->encode2octetstring(guami->pLMNIdentity)) return false;
  if (!aMFRegionID->encode2bitstring(guami->aMFRegionID)) return false;
  if (!aMFSetID->encode2bitstring(guami->aMFSetID)) return false;
  if (!aMFPointer->encode2bitstring(guami->aMFPointer)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool GUAMI::decodefromGUAMI(Ngap_GUAMI_t* pdu) {
  if (plmnId == nullptr) plmnId = new PlmnId();
  if (aMFRegionID == nullptr) aMFRegionID = new AMFRegionID();
  if (aMFSetID == nullptr) aMFSetID = new AMFSetID();
  if (aMFPointer == nullptr) aMFPointer = new AMFPointer();
  if (!plmnId->decodefromoctetstring(pdu->pLMNIdentity)) return false;
  if (!aMFRegionID->decodefrombitstring(pdu->aMFRegionID)) return false;
  if (!aMFSetID->decodefrombitstring(pdu->aMFSetID)) return false;
  if (!aMFPointer->decodefrombitstring(pdu->aMFPointer)) return false;

  return true;
}

//------------------------------------------------------------------------------
void GUAMI::getGUAMI(
    PlmnId*& m_plmnId, AMFRegionID*& m_aMFRegionID, AMFSetID*& m_aMFSetID,
    AMFPointer*& m_aMFPointer) {
  m_plmnId      = plmnId;
  m_aMFRegionID = aMFRegionID;
  m_aMFSetID    = aMFSetID;
  m_aMFPointer  = aMFPointer;
}
}  // namespace ngap
