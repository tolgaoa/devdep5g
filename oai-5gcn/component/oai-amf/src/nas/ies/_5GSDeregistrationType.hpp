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
 \author
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _5GS_DEREGISTRATION_TYPE_H_
#define _5GS_DEREGISTRATION_TYPE_H_

#include <stdint.h>

namespace nas {
typedef struct _5gs_deregistration_type_s {
  uint8_t iei : 4;
  uint8_t switch_off : 1;
  uint8_t dereg_required : 1;
  uint8_t access_type : 2;
} _5gs_deregistration_type_t;

class _5GSDeregistrationType {
 public:
  _5GSDeregistrationType();
  _5GSDeregistrationType(uint8_t type);
  _5GSDeregistrationType(_5gs_deregistration_type_t type);
  ~_5GSDeregistrationType();
  int decodefrombuffer(uint8_t* buf, int len);
  int encode2buffer(uint8_t* buf, int len);

  void set(_5gs_deregistration_type_t type);
  void get(_5gs_deregistration_type_t& type);
  void set(uint8_t type);
  void get(uint8_t& type);

 public:
  union {
    struct {
      uint8_t iei : 4;
      uint8_t switch_off : 1;
      uint8_t dereg_required : 1;
      uint8_t access_type : 2;
    } bf;
    uint8_t b;
  } u1;
};

}  // namespace nas

#endif
