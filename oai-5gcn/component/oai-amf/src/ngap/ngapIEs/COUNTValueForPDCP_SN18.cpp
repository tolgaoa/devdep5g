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

#include "COUNTValueForPDCP_SN18.hpp"

#include <iostream>
#include <vector>

using namespace std;
namespace ngap {
COUNTValueForPDCP_SN18::COUNTValueForPDCP_SN18() {
  pdcp     = 0;
  hfn_pdcp = 0;
}
COUNTValueForPDCP_SN18::~COUNTValueForPDCP_SN18() {}
void COUNTValueForPDCP_SN18::setvalue(long pDCP, long hfn_PDCP) {
  pdcp     = pDCP;
  hfn_pdcp = hfn_PDCP;
}
void COUNTValueForPDCP_SN18::getvalue(long& pDCP, long& hFN_PDCP) {
  pDCP     = pdcp;
  hFN_PDCP = hfn_pdcp;
}
bool COUNTValueForPDCP_SN18::encodedCOUNTValueForPDCP_SN18(
    Ngap_COUNTValueForPDCP_SN18_t* countvalue) {
  countvalue->pDCP_SN18     = pdcp;
  countvalue->hFN_PDCP_SN18 = hfn_pdcp;
  cout << "the encode pdcp_sn18 is" << countvalue->pDCP_SN18 << endl;
  cout << "the encode hfn_pdcp_sn18 is" << countvalue->hFN_PDCP_SN18 << endl;
  return true;
}
bool COUNTValueForPDCP_SN18::decodedCOUNTValueForPDCP_SN18(
    Ngap_COUNTValueForPDCP_SN18_t& countValue) {
  pdcp     = countValue.pDCP_SN18;
  hfn_pdcp = countValue.hFN_PDCP_SN18;
  cout << "the decode pdcp_sn18 is" << countValue.pDCP_SN18 << endl;
  cout << "the decode hfn_pdcp_sn18 is" << countValue.hFN_PDCP_SN18 << endl;
  return true;
}
}  // namespace ngap
