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

#include "dRBStatusUL18.hpp"

#include <iostream>
using namespace std;
namespace ngap {
dRBStatusUL18::dRBStatusUL18() {
  pdcp_value = NULL;
}
dRBStatusUL18::~dRBStatusUL18() {}
void dRBStatusUL18::getcountvalue(COUNTValueForPDCP_SN18*& count_value) {
  count_value = pdcp_value;
}
void dRBStatusUL18::setcountvalue(COUNTValueForPDCP_SN18* count_value) {
  pdcp_value = count_value;
}
bool dRBStatusUL18::encodeddRBStatusUL18(Ngap_DRBStatusUL18_t* UL18) {
  if (!pdcp_value->encodedCOUNTValueForPDCP_SN18(&UL18->uL_COUNTValue)) {
    cout << "encodeddRBStatusUL18 error" << endl;
    return false;
  }
  return true;
}
bool dRBStatusUL18::decodeddRBStatusUL18(Ngap_DRBStatusUL18_t* UL18) {
  if (pdcp_value == nullptr) pdcp_value = new COUNTValueForPDCP_SN18();
  if (!pdcp_value->decodedCOUNTValueForPDCP_SN18(UL18->uL_COUNTValue)) {
    cout << "decodeddRBStatusUL18 error" << endl;
    return false;
  }
  return true;
}
}  // namespace ngap
