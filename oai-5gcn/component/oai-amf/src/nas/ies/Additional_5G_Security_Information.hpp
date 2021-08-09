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

#ifndef __Additional_5G_Security_Information_H_
#define __Additional_5G_Security_Information_H_

#include <stdint.h>

namespace nas {

class Additional_5G_Security_Information {
 public:
  Additional_5G_Security_Information();
  Additional_5G_Security_Information(uint8_t iei);
  Additional_5G_Security_Information(const uint8_t iei, bool RINMR, bool HDP);
  ~Additional_5G_Security_Information();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len, bool is_option);
  void setRINMR(bool value);
  void setHDP(bool value);
  bool getRINMR();
  bool getHDP();

 private:
  uint8_t _iei;
  bool RINMR;
  bool HDP;
};
}  // namespace nas

#endif
