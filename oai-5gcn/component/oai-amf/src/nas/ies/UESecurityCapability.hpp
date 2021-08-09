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

#ifndef _UESecurityCapability_H
#define _UESecurityCapability_H

#include <stdint.h>

namespace nas {

class UESecurityCapability {
 public:
  UESecurityCapability(uint8_t iei);
  UESecurityCapability();
  ~UESecurityCapability();
  UESecurityCapability(
      const uint8_t iei, uint8_t _5gg_EASel, uint8_t _5gg_IASel);
  UESecurityCapability(
      const uint8_t iei, uint8_t _5gg_EASel, uint8_t _5gg_IASel, uint8_t EEASel,
      uint8_t EIASel);
  void setEASel(uint8_t sel);
  void setIASel(uint8_t sel);
  uint8_t getEASel();
  uint8_t getIASel();

  void setEEASel(uint8_t sel);
  void setEIASel(uint8_t sel);
  uint8_t getEEASel();
  uint8_t getEIASel();

  void setLength(uint8_t len);
  uint8_t getLength();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(uint8_t* buf, int len, bool is_option);

 private:
  uint8_t _iei;
  uint8_t length;
  uint8_t _5g_EASel;
  uint8_t _5g_IASel;
  uint8_t EEASel;
  uint8_t EIASel;
};

}  // namespace nas

#endif
