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

/*! \file _5GS_Registration_Result.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef ___5GS_Registration_Result_H_
#define ___5GS_Registration_Result_H_

#include <stdint.h>

namespace nas {

class _5GS_Registration_Result {
 public:
  _5GS_Registration_Result();
  _5GS_Registration_Result(uint8_t iei);
  _5GS_Registration_Result(
      const uint8_t iei, bool emergency, bool nssaa, bool sms, uint8_t value);
  ~_5GS_Registration_Result();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len, bool is_option);
  void setValue(uint8_t value);
  uint8_t getValue();

 private:
  uint8_t _iei;
  bool Emergency;
  bool NSSAA;
  bool SMS;
  uint8_t _value;
};
}  // namespace nas

#endif
