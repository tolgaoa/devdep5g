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

#ifndef _COUNTVALUEFORPDCP_SN18_H_
#define _COUNTVALUEFORPDCP_SN18_H_
extern "C" {
#include "Ngap_COUNTValueForPDCP-SN18.h"
}
namespace ngap {
class COUNTValueForPDCP_SN18 {
 private:
  /* data */
  long pdcp;
  long hfn_pdcp;

 public:
  COUNTValueForPDCP_SN18(/* args */);
  virtual ~COUNTValueForPDCP_SN18();
  void getvalue(long& pDCP, long& hFN_PDCP);
  void setvalue(long pDCP, long hfn_PDCP);
  bool encodedCOUNTValueForPDCP_SN18(Ngap_COUNTValueForPDCP_SN18_t* countvalue);
  bool decodedCOUNTValueForPDCP_SN18(Ngap_COUNTValueForPDCP_SN18_t& countValue);
};

}  // namespace ngap
#endif
