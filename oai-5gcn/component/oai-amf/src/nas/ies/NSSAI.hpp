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

#ifndef __NSSAI_H_
#define __NSSAI_H_
#include <stdint.h>

#include <vector>

#include "nas_ie_header.hpp"
namespace nas {

class NSSAI {
 public:
  NSSAI();
  NSSAI(uint8_t iei);
  NSSAI(const uint8_t iei, std::vector<struct SNSSAI_s> nssai);
  ~NSSAI();
  void setS_NAASI(uint8_t SST);
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len, bool is_option);
  void getValue(std::vector<struct SNSSAI_s>& nssai);

 private:
  uint8_t _iei;
  uint8_t length;
  std::vector<struct SNSSAI_s> S_NSSAI;
};

}  // namespace nas

#endif
