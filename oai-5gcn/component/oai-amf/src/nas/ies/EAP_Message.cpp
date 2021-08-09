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

#include "EAP_Message.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
EAP_Message::EAP_Message(uint8_t iei) : EAP() {
  _iei   = iei;
  length = 0;
}

//------------------------------------------------------------------------------
EAP_Message::EAP_Message(const uint8_t iei, bstring eap) {
  _iei   = iei;
  EAP    = bstrcpy(eap);
  length = blength(eap) + 3;
}

//------------------------------------------------------------------------------
EAP_Message::EAP_Message() : _iei(), EAP(), length() {}

//------------------------------------------------------------------------------
EAP_Message::~EAP_Message() {}

//------------------------------------------------------------------------------
void EAP_Message::getValue(bstring& eap) {
  eap = bstrcpy(EAP);
}

//------------------------------------------------------------------------------
int EAP_Message::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding EAP_Message iei(0x%x)", _iei);
  if (len < length) {
    Logger::nas_mm().error("len is less than %d", length);
    return 0;
  }
  int encoded_size      = 0;
  *(buf + encoded_size) = _iei;
  encoded_size++;
  *(buf + encoded_size) = (length - 3) & 0x00ff;
  encoded_size++;
  *(buf + encoded_size) = ((length - 3) & 0xff00) >> 8;
  encoded_size++;
  int size = encode_bstring(EAP, (buf + encoded_size), len - encoded_size);
  encoded_size += size;

  Logger::nas_mm().debug("encoded EAP_Message len (%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int EAP_Message::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  Logger::nas_mm().debug("decoding EAP_Message iei (0x%x)", *buf);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }
  length = 0;
  length |= *(buf + decoded_size);
  decoded_size++;
  length |= (*(buf + decoded_size)) << 8;
  decoded_size++;
  decode_bstring(&EAP, length, (buf + decoded_size), len - decoded_size);
  decoded_size += length;
  for (int i = 0; i < length; i++) {
    Logger::nas_mm().debug(
        "decoded EAP_Message value (0x%x)", (uint8_t) EAP->data[i]);
  }
  Logger::nas_mm().debug("decoded EAP_Message len (%d)", decoded_size);
  return decoded_size;
}
