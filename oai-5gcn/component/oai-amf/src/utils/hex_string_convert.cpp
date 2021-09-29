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

#include <iostream>
#include <string>

#include "bstrlib.h"
#include "conversions.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

void convert_string_2_hex(std::string& input_str, std::string& output_str) {
  unsigned char* data = (unsigned char*) malloc(input_str.length() + 1);
  memset(data, 0, input_str.length() + 1);
  memcpy((void*) data, (void*) input_str.c_str(), input_str.length());

  for (int i = 0; i < input_str.length(); i++) {
    printf("%02x ", data[i]);
  }
  printf("\n");
  char* datahex = (char*) malloc(input_str.length() * 2 + 1);
  memset(datahex, 0, input_str.length() * 2 + 1);

  for (int i = 0; i < input_str.length(); i++)
    sprintf(datahex + i * 2, "%02x", data[i]);

  output_str = reinterpret_cast<char*>(datahex);
}

// extern std::size_t callback(const char* in, std::size_t size, std::size_t
// num, std::string* out);
unsigned char* format_string_as_hex(std::string str) {
  unsigned int str_len     = str.length();
  unsigned char* datavalue = (unsigned char*) malloc(str_len / 2 + 1);

  unsigned char* data = (unsigned char*) malloc(str_len + 1);
  memset(data, 0, str_len + 1);

  memcpy((void*) data, (void*) str.c_str(), str_len);

  std::cout << "Data: " << data << " (" << str_len << " bytes)" << std::endl;

  std::cout << "Data (formatted): \n";
  for (int i = 0; i < str_len; i++) {
    char datatmp[3] = {0};
    memcpy(datatmp, &data[i], 2);
    // Ensure both characters are hexadecimal
    bool bBothDigits = true;

    for (int j = 0; j < 2; ++j) {
      if (!isxdigit(datatmp[j])) bBothDigits = false;
    }
    if (!bBothDigits) break;
    // Convert two hexadecimal characters into one character
    unsigned int nAsciiCharacter;
    sscanf(datatmp, "%x", &nAsciiCharacter);
    printf("%x ", nAsciiCharacter);
    // Concatenate this character onto the output
    datavalue[i / 2] = (unsigned char) nAsciiCharacter;

    // Skip the next character
    i++;
  }
  printf("\n");

  free_wrapper((void**) &data);
  return datavalue;
}

char* bstring2charString(bstring b) {
  char* buf      = (char*) calloc(1, blength(b) + 1);
  uint8_t* value = (uint8_t*) bdata(b);
  for (int i = 0; i < blength(b); i++) buf[i] = (char) value[i];
  buf[blength(b)] = '\0';
  free_wrapper((void**) &value);
  return buf;
}

void msg_str_2_msg_hex(std::string msg, bstring& b) {
  std::string msg_hex_str;
  convert_string_2_hex(msg, msg_hex_str);
  printf("tmp string: %s\n", msg_hex_str.c_str());
  unsigned int msg_len = msg_hex_str.length();
  char* data           = (char*) malloc(msg_len + 1);
  memset(data, 0, msg_len + 1);
  memcpy((void*) data, (void*) msg_hex_str.c_str(), msg_len);
  printf("data: %s\n", data);
  uint8_t* msg_hex = (uint8_t*) malloc(msg_len / 2 + 1);
  conv::ascii_to_hex(msg_hex, (const char*) data);
  b = blk2bstr(msg_hex, (msg_len / 2));
}
