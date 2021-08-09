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

#ifndef _S_NSSAI_H_
#define _S_NSSAI_H_

#include <string>

extern "C" {
#include "Ngap_S-NSSAI.h"
#include "Ngap_SD.h"
#include "Ngap_SST.h"
}

namespace ngap {

class S_NSSAI {
 public:
  S_NSSAI();
  virtual ~S_NSSAI();
  bool sSTEncode2OctetString(Ngap_SST_t&);
  bool sSTdecodefromOctetString(Ngap_SST_t);
  bool sDEncode2OctetString(Ngap_SD_t*);
  bool sDdecodefromOctetString(Ngap_SD_t*);
  void setSst(const std::string charSst);
  void getSst(std::string& charSst);
  void setSd(const std::string charSd);
  bool getSd(std::string& s_nssaiSd);
  bool encode2S_NSSAI(Ngap_S_NSSAI_t*);
  bool decodefromS_NSSAI(Ngap_S_NSSAI_t*);

 private:
  uint8_t sst;  // mandotory  OCTET_STRING(SIZE(1))
  uint32_t sd;  // OCTET_STRING(SIZE(3))
  bool sdIsSet;
};

}  // namespace ngap

#endif
