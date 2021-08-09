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
#include "dRBStatusDL.hpp"

#include <iostream>
#include <vector>
using namespace std;
namespace ngap {
dRBStatusDL::dRBStatusDL() {
  dl18 = (DRBStatusDL18*) calloc(1, sizeof(DRBStatusDL18));
}
dRBStatusDL::~dRBStatusDL() {}
void dRBStatusDL::setDRBStatusDL18(DRBStatusDL18* dL18) {
  dl18 = dL18;
}
void dRBStatusDL::getDRBStatusDL18(DRBStatusDL18*& dL18) {
  dL18 = dl18;
}
bool dRBStatusDL::encodedRBStatusDL(Ngap_DRBStatusDL_t* dL) {
  dL->present = Ngap_DRBStatusDL_PR_dRBStatusDL18;
  dL->choice.dRBStatusDL18 =
      (Ngap_DRBStatusDL18_t*) calloc(1, sizeof(Ngap_DRBStatusDL18_t));
  if (!dl18->encodeddRBStatusDL18(dL->choice.dRBStatusDL18)) {
    cout << "encodedRBStatusDL error" << endl;
    return false;
  }
  cout << "encodedRBStatusDL successfully" << endl;
  return true;
}
bool dRBStatusDL::decodedRBStatusDL(Ngap_DRBStatusDL_t* dL) {
  if (dl18 == nullptr) dl18 = new DRBStatusDL18();
  if (!dl18->decodeddRBStatusDL18(dL->choice.dRBStatusDL18)) {
    cout << "decodedRBStatusDL error" << endl;
    return false;
  }
  cout << "decodedRBStatusDL successfully" << endl;
  return true;
}
}  // namespace ngap
