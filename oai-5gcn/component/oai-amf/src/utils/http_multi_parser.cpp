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

#include "mime_parser.hpp"

uint8_t multipart_parser(
    std::string input, std::string& jsonData, std::string& n1sm,
    std::string& n2sm) {
  // simple parser
  mime_parser sp = {};
  sp.parse(input);

  std::vector<mime_part> parts = {};
  sp.get_mime_parts(parts);
  uint8_t size = parts.size();
  // at least 2 parts for Json data and N1 (+ N2)
  if (size < 2) {
    return size;
  }

  jsonData     = parts[0].body;
  n1sm         = parts[1].body;
  bool is_ngap = false;
  if (size > 2) {
    n2sm = parts[2].body;
  } else {
    n2sm = "null";
  }

  return size;
}
