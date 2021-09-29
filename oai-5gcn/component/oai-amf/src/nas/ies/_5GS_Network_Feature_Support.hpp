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

/*! \file __5GS_Network_Feature_Support.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef __5GS_Network_Feature_Support_H
#define __5GS_Network_Feature_Support_H

#include <stdint.h>

namespace nas {

class _5GS_Network_Feature_Support {
 public:
  _5GS_Network_Feature_Support(uint8_t iei);
  _5GS_Network_Feature_Support();
  ~_5GS_Network_Feature_Support();
  _5GS_Network_Feature_Support(
      const uint8_t iei, uint8_t value, uint8_t value2);
  void setValue(uint8_t value);
  uint8_t getValue();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len, bool is_option);

 private:
  uint8_t _iei;
  uint8_t length;
  uint8_t _value;
  uint8_t _value2;
};

}  // namespace nas

#endif
