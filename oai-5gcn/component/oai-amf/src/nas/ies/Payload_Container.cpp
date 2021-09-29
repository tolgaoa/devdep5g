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

#include "Payload_Container.hpp"

#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
Payload_Container::Payload_Container(uint8_t iei) : content() {
  _iei    = iei;
  length  = 0;
  CONTENT = {};
}

//------------------------------------------------------------------------------
Payload_Container::Payload_Container(uint8_t iei, bstring b) {
  _iei    = iei;
  content = b;
  CONTENT = {};
}

//------------------------------------------------------------------------------
Payload_Container::Payload_Container(
    const uint8_t iei, std::vector<PayloadContainerEntry> content) {
  _iei = iei;
  if (_iei) {
    length = 4 + content.size() * 2;
  } else {
    length = 3 + content.size() * 2;
  }
  CONTENT.assign(content.begin(), content.end());
  for (int i = 0; i < content.size(); i++) {
    length = length + content.at(i).optionalIE.size() * 2;
    for (int j = 0; j < content.at(i).optionalIE.size(); j++) {
      length += blength(content.at(i).optionalIE.at(j).ie_value);
    }
  }
}

//------------------------------------------------------------------------------
Payload_Container::Payload_Container() : content() {
  _iei    = 0;
  CONTENT = {};
}

//------------------------------------------------------------------------------
Payload_Container::~Payload_Container() {}

//------------------------------------------------------------------------------
void Payload_Container::setValue(uint8_t iei, uint8_t value) {
  _iei = iei;
  //_value = value;
}

//------------------------------------------------------------------------------
void Payload_Container::getValue(std::vector<PayloadContainerEntry>& content) {
  content.assign(CONTENT.begin(), CONTENT.end());
}

//------------------------------------------------------------------------------
void Payload_Container::getValue(bstring& cnt) {
  cnt = content;
}

//------------------------------------------------------------------------------
int Payload_Container::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("Encoding Payload_Container iei(0x%x)", _iei);
  if (len < length) {
    // Logger::nas_mm().error("len is less than %d", length);
    // return 0;
  }
  int encoded_size = 0;
  if (_iei) {
    *(buf + encoded_size) = _iei;
    encoded_size++;
  }
  *(buf + encoded_size) = (blength(content) & 0xff00) >> 8;
  encoded_size++;
  *(buf + encoded_size) = (blength(content) & 0x00ff);
  encoded_size++;
  memcpy(buf + encoded_size, (uint8_t*) bdata(content), blength(content));
  encoded_size += blength(content);

#if 0
	if (_iei) {
		*(buf + encoded_size) = _iei; encoded_size++;
		*(buf + encoded_size) = (length - 2)&0x00ff; encoded_size++;
		*(buf + encoded_size) = (length - 2) & 0xff00; encoded_size++;
		*(buf + encoded_size) = CONTENT.size(); encoded_size++;
		for (int i = 0; i < CONTENT.size(); i++) {
			/*Length of Payload container entry*/
			*(buf + encoded_size) = CONTENT.at(i).optionalIE.size() * 2 + 1;
				for (int j = 0; j < CONTENT.at(i).optionalIE.size(); j++) {
					*(buf + encoded_size) += blength(CONTENT.at(i).optionalIE.at(j).ie_value);
				}
			encoded_size++;
			/*Length of Payload container entry*/
			*(buf + encoded_size) = ((CONTENT.at(i).optionalIE.size()&0x0f)<<4)| CONTENT.at(i).payloadContainerType;
			encoded_size++;
			for (int j = 0; j < CONTENT.at(i).optionalIE.size(); j++) {
				*(buf + encoded_size) += CONTENT.at(i).optionalIE.at(j).ie_type; encoded_size++;
				*(buf + encoded_size) += CONTENT.at(i).optionalIE.at(j).ie_len; encoded_size++;
				int size = encode_bstring(CONTENT.at(i).optionalIE.at(j).ie_value, (buf + encoded_size), len - encoded_size);
				encoded_size += size;
			}
		}
	}
	else {
		*(buf + encoded_size) = (length - 2)&0x00ff; encoded_size++;
		*(buf + encoded_size) = (length - 2) & 0xff00; encoded_size++;
		*(buf + encoded_size) = CONTENT.size(); encoded_size++;
		for (int i = 0; i < CONTENT.size(); i++) {
			/*Length of Payload container entry*/
			*(buf + encoded_size) = CONTENT.at(i).optionalIE.size() * 2 + 1;
				for (int j = 0; j < CONTENT.at(i).optionalIE.size(); j++) {
					*(buf + encoded_size) += blength(CONTENT.at(i).optionalIE.at(j).ie_value);
				}
			encoded_size++;
			/*Length of Payload container entry*/
			*(buf + encoded_size) = ((CONTENT.at(i).optionalIE.size()&0x0f)<<4)| CONTENT.at(i).payloadContainerType;
			encoded_size++;
			for (int j = 0; j < CONTENT.at(i).optionalIE.size(); j++) {
				*(buf + encoded_size) += CONTENT.at(i).optionalIE.at(j).ie_type; encoded_size++;
				*(buf + encoded_size) += CONTENT.at(i).optionalIE.at(j).ie_len; encoded_size++;
				int size = encode_bstring(CONTENT.at(i).optionalIE.at(j).ie_value, (buf + encoded_size), len - encoded_size);
				encoded_size += size;
			}
		}
	}
#endif
  Logger::nas_mm().debug("Encoded Payload_Container len(%d)", encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------

int Payload_Container::decodefrombuffer(uint8_t* buf, int len, bool is_option) {
  return 0;
}

//------------------------------------------------------------------------------
int Payload_Container::decodefrombuffer(
    uint8_t* buf, int len, bool is_option, uint8_t type) {
  Logger::nas_mm().debug("Decoding Payload_Container iei (0x%x)", _iei);
  int decoded_size = 0;
  if (is_option) {
    decoded_size++;
  }

  if (type != 0x0f) {  // not multiple payload
    uint8_t octet = *(buf + decoded_size);
    decoded_size++;
    length = 0;
    length |= (octet << 8);
    octet = *(buf + decoded_size);
    decoded_size++;
    length |= octet;
    content = blk2bstr(buf + decoded_size, length);
    decoded_size += length;
    return decoded_size;
  }

  uint8_t num_entries;
  uint8_t num_optional;
  IE_t value;
  PayloadContainerEntry payloadcontainerentry;
  length = 0x00;
  length |= *(buf + decoded_size);
  decoded_size++;
  length |= (*(buf + decoded_size)) << 8;
  decoded_size++;
  num_entries = *(buf + decoded_size);
  decoded_size++;
  while (num_entries) {
    uint8_t Length_entry = *(buf + decoded_size);
    decoded_size++;
    payloadcontainerentry.payloadContainerType = *(buf + decoded_size) & 0x0f;
    num_optional = (*(buf + decoded_size) & 0xf0) >> 4;
    decoded_size++;
    while (num_optional) {
      value.ie_type = *(buf + decoded_size);
      decoded_size++;
      value.ie_len = *(buf + decoded_size);
      decoded_size++;
      decode_bstring(
          &value.ie_value, value.ie_len, (buf + decoded_size),
          len - decoded_size);
      decoded_size += value.ie_len;
      payloadcontainerentry.optionalIE.insert(
          payloadcontainerentry.optionalIE.end(), value);
      num_optional--;
    }
    CONTENT.insert(CONTENT.end(), payloadcontainerentry);
    num_entries--;
  }
  Logger::nas_mm().debug("Decoded Payload_Container len (%d)", decoded_size);
  return decoded_size;
}
