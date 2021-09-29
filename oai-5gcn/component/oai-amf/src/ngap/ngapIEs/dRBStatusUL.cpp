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
 \author  niuxiansheng-niu, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */
#include "dRBStatusUL.hpp"

#include <iostream>
#include <vector>
using namespace std;

namespace ngap {
dRBStatusUL::dRBStatusUL() {
  ul18 = (dRBStatusUL18*) calloc(1, sizeof(dRBStatusUL18));
}
dRBStatusUL::~dRBStatusUL() {}
void dRBStatusUL::setdRBStatusUL(dRBStatusUL18* uL18) {
  ul18 = uL18;
}
void dRBStatusUL::getdRBStatusUL(dRBStatusUL18*& uL18) {
  uL18 = ul18;
}
bool dRBStatusUL::encodedRBStatusUL(Ngap_DRBStatusUL_t* uL) {
  uL->present = Ngap_DRBStatusUL_PR_dRBStatusUL18;
  uL->choice.dRBStatusUL18 =
      (Ngap_DRBStatusUL18_t*) calloc(1, sizeof(Ngap_DRBStatusUL18_t));
  if (!ul18->encodeddRBStatusUL18(uL->choice.dRBStatusUL18)) {
    cout << "encodeddRBStatusUL18 error" << endl;
    return false;
  }
  cout << "encodedRBStatusUL successfully" << endl;
  return true;
}
bool dRBStatusUL::decodedRBStatusUL(Ngap_DRBStatusUL_t* uL) {
  if (ul18 == nullptr) ul18 = new dRBStatusUL18();
  if (!ul18->decodeddRBStatusUL18(uL->choice.dRBStatusUL18)) {
    cout << "decodeddRBStatusUL18 error" << endl;
    return false;
  }
  cout << "decodedRBStatusUL successfully" << endl;
  return true;
}
}  // namespace ngap
