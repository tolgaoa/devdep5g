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

/*! \file _5GSTrackingAreaIdList.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "_5GSTrackingAreaIdList.hpp"

#include "String2Value.hpp"

using namespace nas;

//------------------------------------------------------------------------------
_5GSTrackingAreaIdList::_5GSTrackingAreaIdList(
    uint8_t iei, std::vector<p_tai_t> tai_list) {
  m_tai_list = tai_list;
  m_iei      = iei;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode2buffer(uint8_t* buf, int len) {
  int encoded_size = 0, length = 0;
  if (m_iei) {
    *(buf + encoded_size) = m_iei;
    encoded_size++;
  }
  encoded_size++;  // for length octet
  for (int i = 0; i < m_tai_list.size(); i++) {
    int item_len = 0;
    switch (m_tai_list[i].type) {
      case 0x00: {
        item_len += encode_00_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);
      } break;
      case 0x01: {
        item_len += encode_01_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);
      } break;
      case 0x10: {
        item_len += encode_10_type(
            m_tai_list[i], buf + encoded_size, len - encoded_size);
      } break;
    }
    encoded_size += item_len;
    length += item_len;
  }
  *(buf + encoded_size - length - 1) = length;
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode_00_type(
    p_tai_t item, uint8_t* buf, int len) {
  int encoded_size      = 0;
  uint8_t octet         = 0x00 | ((item.tac_list.size() - 1) & 0x1f);
  *(buf + encoded_size) = octet;
  encoded_size++;
  encoded_size +=
      encode_mcc_mnc(item.plmn_list[0], buf + encoded_size, len - encoded_size);
  for (int i = 0; i < item.tac_list.size(); i++) {
    octet                 = (item.tac_list[i] & 0x00ff0000) >> 16;
    *(buf + encoded_size) = octet;
    encoded_size++;
    octet                 = (item.tac_list[i] & 0x0000ff00) >> 8;
    *(buf + encoded_size) = octet;
    encoded_size++;
    octet                 = (item.tac_list[i] & 0x000000ff) >> 0;
    *(buf + encoded_size) = octet;
    encoded_size++;
  }
  return encoded_size;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode_01_type(
    p_tai_t item, uint8_t* buf, int len) {
  return 1;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode_10_type(
    p_tai_t item, uint8_t* buf, int len) {
  return 1;
}

//------------------------------------------------------------------------------
int _5GSTrackingAreaIdList::encode_mcc_mnc(
    nas_plmn_t plmn, uint8_t* buf, int len) {
  int encoded_size = 0;
  int mcc          = fromString<int>(plmn.mcc);
  int mnc          = fromString<int>(plmn.mnc);
  *(buf + encoded_size) =
      (0x0f & (mcc / 100)) | ((0x0f & ((mcc % 100) / 10)) << 4);
  encoded_size += 1;
  if (!(mnc / 100)) {
    *(buf + encoded_size) = (0x0f & (mcc % 10)) | 0xf0;
    encoded_size += 1;
    *(buf + encoded_size) =
        (0x0f & ((mnc % 100) / 10)) | ((0x0f & (mnc % 10)) << 4);
    encoded_size += 1;
  } else {
    *(buf + encoded_size) = (0x0f & (mcc % 10)) | ((0x0f & (mnc % 10)) << 4);
    encoded_size += 1;
    *(buf + encoded_size) =
        ((0x0f & ((mnc % 100) / 10)) << 4) | (0x0f & (mnc / 100));
    encoded_size += 1;
  }
  return encoded_size;
}
