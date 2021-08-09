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

/*! \file __5GSTrackingAreaIdList.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef __5GSTrackingAreaIdList_H_
#define __5GSTrackingAreaIdList_H_

#include <vector>

#include "struct.hpp"

namespace nas {

class _5GSTrackingAreaIdList {
 public:
  _5GSTrackingAreaIdList(uint8_t iei, std::vector<p_tai_t> tai_list);
  int encode2buffer(uint8_t* buf, int len);

 private:
  uint8_t m_iei;
  std::vector<p_tai_t> m_tai_list;

 private:
  int encode_00_type(p_tai_t item, uint8_t* buf, int len);
  int encode_01_type(p_tai_t item, uint8_t* buf, int len);
  int encode_10_type(p_tai_t item, uint8_t* buf, int len);
  int encode_mcc_mnc(nas_plmn_t plmn, uint8_t* buf, int len);
};

}  // namespace nas

#endif
