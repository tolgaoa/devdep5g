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

#ifndef _5GS_REGISTRATION_TYPE_H_
#define _5GS_REGISTRATION_TYPE_H_

#include <stdint.h>

namespace nas {

class _5GSRegistrationType {
 public:
  _5GSRegistrationType();
  _5GSRegistrationType(bool is_for, uint8_t type);
  _5GSRegistrationType(uint8_t iei, bool is_for, uint8_t type);
  ~_5GSRegistrationType();
  int decodefrombuffer(uint8_t* buf, int len, bool is_option);
  int encode2buffer(uint8_t* buf, int len);

  void setFollowOnReq(const bool is);
  void setRegType(const uint8_t type);
  bool isFollowOnReq();
  uint8_t getRegType();

 private:
  uint8_t iei : 4;
  bool is_for;
  uint8_t reg_type : 3;
};

}  // namespace nas

#endif
