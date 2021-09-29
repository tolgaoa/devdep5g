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

#include "AMFName.hpp"

extern "C" {
#include "OCTET_STRING.h"
}

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
AmfName::AmfName() {
  amfname = NULL;
}

//------------------------------------------------------------------------------
AmfName::~AmfName() {
  free(amfname);
  amfname = NULL;
}

//------------------------------------------------------------------------------
void AmfName::setValue(const std::string m_amfName) {
  if (amfname) {
    free(amfname);
    amfname = NULL;
  }
  amfname = (char*) calloc(1, m_amfName.size() + 1);
  memcpy(amfname, m_amfName.c_str(), m_amfName.size());
  amfname[m_amfName.size()] = '\0';
}

//------------------------------------------------------------------------------
void AmfName::getValue(std::string& m_amfName) {
  m_amfName = amfname;
}

//------------------------------------------------------------------------------
bool AmfName::encode2AmfName(Ngap_AMFName_t* amfNameIe) {
  if (OCTET_STRING_fromBuf(amfNameIe, amfname, strlen(amfname)) < 0)
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool AmfName::decodefromAmfName(Ngap_AMFName_t* pdu) {
  if (!pdu->buf) return false;
  amfname = (char*) pdu->buf;
  return true;
}

}  // namespace ngap
