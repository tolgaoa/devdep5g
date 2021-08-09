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

#include "DNN.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
DNN::DNN(uint8_t iei) : _DNN() {
  _iei   = iei;
  length = 0;
}

//------------------------------------------------------------------------------
DNN::DNN(const uint8_t iei, bstring dnn) {
  _iei   = iei;
  _DNN   = bstrcpy(dnn);
  length = blength(dnn) + 2;
}

//------------------------------------------------------------------------------
DNN::DNN() : _iei(), _DNN(), length() {}

//------------------------------------------------------------------------------
DNN::~DNN() {}

//------------------------------------------------------------------------------
void DNN::getValue(bstring& dnn) {
  // dnn = _DNN;
  // dnn = bstrcpy(_DNN);
  dnn = blk2bstr((uint8_t*) bdata(_DNN) + 1, blength(_DNN) - 1);
}

//------------------------------------------------------------------------------
int DNN::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding DNN IEI (0x%x)", _iei);
  if (len < length) {
    Logger::nas_mm().error("len is less than %d", length);
    return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
    *(buf + encoded_size) = (length - 2);
    encoded_size++;
    int size = encode_bstring(_DNN, (buf + encoded_size), len - encoded_size);
    encoded_size += size;

  } else {
    *(buf + encoded_size) = (length - 1);
    encoded_size++;
    int size = encode_bstring(_DNN, (buf + encoded_size), len - encoded_size);
    encoded_size += size;
  }
  Logger::nas_mm().debug("encoded DNN len: %d", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int DNN::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("Decoding DNN IEI (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  length = *(buf + decoded_size);
  decoded_size++;
  decode_bstring(&_DNN, length, (buf + decoded_size), len - decoded_size);
  decoded_size += length;
  for (int i = 0; i < blength(_DNN); i++) {
    Logger::nas_mm().debug("Decoded DNN value: 0x%x", (uint8_t) bdata(_DNN)[i]);
  }
  Logger::nas_mm().debug("Decoded DNN len: %d", decoded_size);
  return decoded_size;
}
