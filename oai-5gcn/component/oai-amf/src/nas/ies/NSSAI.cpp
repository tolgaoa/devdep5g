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

#include "NSSAI.hpp"

#include <vector>

#include "logger.hpp"
using namespace nas;
using namespace std;

//------------------------------------------------------------------------------
NSSAI::NSSAI(uint8_t iei) {
  _iei    = iei;
  length  = 0;
  S_NSSAI = {};
}

//------------------------------------------------------------------------------
NSSAI::NSSAI(const uint8_t iei, std::vector<struct SNSSAI_s> nssai) {
  _iei   = iei;
  length = 0;
  S_NSSAI.assign(nssai.begin(), nssai.end());
  for (int i = 0; i < nssai.size(); i++) {
    length += 2;  // for sst
    if (nssai[i].sd != -1) length += 3;
    if (nssai[i].mHplmnSst != -1) length += 1;
    if (nssai[i].mHplmnSd != -1) length += 3;
  }
}

//------------------------------------------------------------------------------
NSSAI::NSSAI() : _iei(), length(), S_NSSAI() {}

//------------------------------------------------------------------------------
NSSAI::~NSSAI() {}

//------------------------------------------------------------------------------
void NSSAI::setS_NAASI(uint8_t SST) {}

//------------------------------------------------------------------------------
void NSSAI::getValue(std::vector<struct SNSSAI_s>& nssai) {
  nssai.assign(S_NSSAI.begin(), S_NSSAI.end());
}

//------------------------------------------------------------------------------
int NSSAI::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding NSSAI IEI (0x%x)", _iei);
  if (len < length) {
    Logger::nas_mm().error("len is less than %d", length);
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = length;
    encoded_size++;

    for (int i = 0; i < S_NSSAI.size(); i++) {
      int len_s_nssai = 1;
      encoded_size++;
      *(buf + encoded_size) = S_NSSAI.at(i).sst;
      encoded_size++;
      if (S_NSSAI.at(i).sd != -1) {
        len_s_nssai += 3;
        *(buf + encoded_size) = (S_NSSAI.at(i).sd & 0x00ff0000) >> 16;
        encoded_size++;
        Logger::nas_mm().debug(
            "Encoded NSSAI len (%x)", *(buf + encoded_size - 1));
        *(buf + encoded_size) = (S_NSSAI.at(i).sd & 0x0000ff00) >> 8;
        encoded_size++;
        Logger::nas_mm().debug(
            "Encoded NSSAI len (%x)", *(buf + encoded_size - 1));
        *(buf + encoded_size) = S_NSSAI.at(i).sd & 0x000000ff;
        encoded_size++;
        Logger::nas_mm().debug(
            "Encoded NSSAI len (%x)", *(buf + encoded_size - 1));
      }
      if (S_NSSAI.at(i).mHplmnSst != -1) {
        len_s_nssai += 1;
        *(buf + encoded_size) = S_NSSAI.at(i).mHplmnSst;
        encoded_size++;
      }
      if (S_NSSAI.at(i).mHplmnSd != -1) {
        len_s_nssai += 3;
        *(buf + encoded_size) = (S_NSSAI.at(i).mHplmnSd & 0x00ff0000) >> 16;
        encoded_size++;
        *(buf + encoded_size) = (S_NSSAI.at(i).mHplmnSd & 0x0000ff00) >> 8;
        encoded_size++;
        *(buf + encoded_size) = S_NSSAI.at(i).mHplmnSd & 0x000000ff;
        encoded_size++;
      }
      *(buf + encoded_size - len_s_nssai - 1) = len_s_nssai;
    }
  } else {
    //     *(buf + encoded_size) = length - 1; encoded_size++;
    //   *(buf + encoded_size) = _value; encoded_size++; encoded_size++;
  }
  Logger::nas_mm().debug("Encoded NSSAI len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int NSSAI::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding NSSAI IEI (0x%x)", *buf);
  int decoded_size = 0;
  SNSSAI_s a       = {0, 0, 0, 0};
  if (is_option) {
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  int length_tmp = length;
  while (length_tmp) {
    switch (*(buf + decoded_size)) {
      case 1: {
        decoded_size++;  // snssai—leagth
        length_tmp--;
        a.sst = *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd        = -1;
        a.mHplmnSst = -1;
        a.mHplmnSd  = -1;
      } break;
      case 4: {
        decoded_size++;
        length_tmp--;
        a.sst = *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd << 8;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd << 8;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.mHplmnSst = -1;
        a.mHplmnSd  = -1;
      } break;
      case 5: {
        decoded_size++;
        length_tmp--;
        a.sst = *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd << 8;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd << 8;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.mHplmnSst = *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.mHplmnSd = -1;
      } break;
      case 8: {
        decoded_size++;
        length_tmp--;
        a.sst = *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd << 8;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.sd << 8;
        a.sd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.mHplmnSst = *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.mHplmnSd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.mHplmnSd << 16;
        a.mHplmnSd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
        a.mHplmnSd << 8;
        a.mHplmnSd |= *(buf + decoded_size);
        decoded_size++;
        length_tmp--;
      } break;
    }

    S_NSSAI.insert(S_NSSAI.end(), a);
    a = {0, 0, 0, 0};
  }

  for (int i = 0; i < S_NSSAI.size(); i++) {
    Logger::nas_mm().debug(
        "Decoded NSSAI SST (0x%x) SD (0x%x) hplmnSST (0x%x) hplmnSD (%d)",
        S_NSSAI.at(i).sst, S_NSSAI.at(i).sd, S_NSSAI.at(i).mHplmnSst,
        S_NSSAI.at(i).mHplmnSd);
  }
  Logger::nas_mm().debug("Decoded NSSAI len (%d)", decoded_size);
  return decoded_size;
}
