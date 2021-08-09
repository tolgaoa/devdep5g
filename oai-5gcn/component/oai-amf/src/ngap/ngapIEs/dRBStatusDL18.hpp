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

#ifndef _DRBSTATUSDL18_H_
#define _DRBSTATUSDL18_H_
#include "COUNTValueForPDCP_SN18.hpp"
extern "C" {
#include "Ngap_DRBStatusDL18.h"
}
namespace ngap {
class DRBStatusDL18 {
 private:
  /* data */
  COUNTValueForPDCP_SN18* pdcp_value;

 public:
  DRBStatusDL18(/* args */);
  virtual ~DRBStatusDL18();
  void getcountvalue(COUNTValueForPDCP_SN18*& count_value);
  void setcountvalue(COUNTValueForPDCP_SN18* count_value);
  bool encodeddRBStatusDL18(Ngap_DRBStatusDL18_t* DL18);
  bool decodeddRBStatusDL18(Ngap_DRBStatusDL18_t* DL18);
};
}  // namespace ngap
#endif
