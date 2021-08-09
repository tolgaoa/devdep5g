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

#ifndef _DRBSTATUSUL18_H_
#define _DRBSTATUSUL18_H_
#include "COUNTValueForPDCP_SN18.hpp"
extern "C" {
#include "Ngap_DRBStatusUL18.h"
}
namespace ngap {
class dRBStatusUL18 {
 private:
  /* data */
  COUNTValueForPDCP_SN18* pdcp_value;

 public:
  dRBStatusUL18(/* args */);
  virtual ~dRBStatusUL18();
  void getcountvalue(COUNTValueForPDCP_SN18*& count_value);
  void setcountvalue(COUNTValueForPDCP_SN18* count_value);
  bool encodeddRBStatusUL18(Ngap_DRBStatusUL18_t* UL18);
  bool decodeddRBStatusUL18(Ngap_DRBStatusUL18_t* UL18);
};

}  // namespace ngap
#endif
