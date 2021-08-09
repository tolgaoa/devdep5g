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

/*! \file comUt.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _PRINT_BUFFER_H
#define _PRINT_BUFFER_H

#include <string>

#include "logger.hpp"

//------------------------------------------------------------------------------
void print_buffer(
    const std::string app, const std::string commit, uint8_t* buf, int len) {
  if (!app.compare("amf_app")) Logger::amf_app().debug(commit.c_str());
  if (!app.compare("amf_n1")) Logger::amf_n1().debug(commit.c_str());
  if (!app.compare("amf_server")) Logger::amf_server().debug(commit.c_str());
  if (!app.compare("amf_n11")) Logger::amf_n11().debug(commit.c_str());
#if DEBUG_IS_ON
  for (int i = 0; i < len; i++) printf("%x ", buf[i]);
  printf("\n");
#endif
}

//------------------------------------------------------------------------------
void hexStr2Byte(const char* src, unsigned char* dest, int len) {
  short i;
  unsigned char hBy, lBy;
  for (i = 0; i < len; i += 2) {
    hBy = toupper(src[i]);
    lBy = toupper(src[i + 1]);
    if (hBy > 0x39)
      hBy -= 0x37;
    else
      hBy -= 0x30;
    if (lBy > 0x39)
      lBy -= 0x37;
    else
      lBy -= 0x30;
    dest[i / 2] = (hBy << 4) | lBy;
  }
}

#endif
