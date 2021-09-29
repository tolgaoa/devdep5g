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

#ifndef __PLMN_List_H_
#define __PLMN_List_H_

#include <stdint.h>

namespace nas {

class PLMN_List {
 public:
  PLMN_List();
  PLMN_List(uint8_t iei);
  PLMN_List(
      const uint8_t iei, uint8_t MNC_MCC1, uint8_t MNC_MCC2, uint8_t MNC_MCC3);
  ~PLMN_List();
  void setMNC_MCC1(uint8_t iei, uint8_t value);
  void setMNC_MCC2(uint8_t iei, uint8_t value);
  void setMNC_MCC3(uint8_t iei, uint8_t value);
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len, bool is_option);
  uint8_t getMNC_MCC1();
  uint8_t getMNC_MCC2();
  uint8_t getMNC_MCC3();

 private:
  uint8_t _iei;
  uint8_t _MNC_MCC1;
  uint8_t _MNC_MCC2;
  uint8_t _MNC_MCC3;
};
}  // namespace nas

#endif
