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

#include "TransportLayerAddress.hpp"

#include <iostream>
#include <vector>

#include "String2Value.hpp"
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
TransportLayerAddress::TransportLayerAddress() {}

//------------------------------------------------------------------------------
TransportLayerAddress::~TransportLayerAddress() {}

//------------------------------------------------------------------------------
void TransportLayerAddress::setTransportLayerAddress(
    const std::string m_ipaddress) {
  // ipaddress = fromString<long>(m_ipaddress);
  ipaddress = m_ipaddress;
}

//------------------------------------------------------------------------------
bool TransportLayerAddress::getTransportLayerAddress(std::string& m_ipaddress) {
  // m_ipaddress = to_string(ipaddress);
  m_ipaddress = ipaddress;

  return true;
}

//------------------------------------------------------------------------------
vector<string> splite(const string& s, const string& c)  //分割字符用的
{
  std::string::size_type pos1, pos2;
  vector<string> v;
  pos2 = s.find(c);
  pos1 = 0;
  while (std::string::npos != pos2) {
    v.push_back(s.substr(pos1, pos2 - pos1));

    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if (pos1 != s.length()) {
    v.push_back(s.substr(pos1));
  }
  return v;
}

//------------------------------------------------------------------------------
bool TransportLayerAddress::encode2TransportLayerAddress(
    Ngap_TransportLayerAddress_t& transportLayerAddress) {
  transportLayerAddress.size        = sizeof(uint32_t);
  transportLayerAddress.bits_unused = 0;
  transportLayerAddress.buf = (uint8_t*) calloc(1, transportLayerAddress.size);
  if (!transportLayerAddress.buf) return false;

  vector<string> ip_address = splite(ipaddress, ".");

  for (int i = 0; i < transportLayerAddress.size; i++) {
    transportLayerAddress.buf[i] = fromString<int>(ip_address[i]);
  }

  return true;
}

//------------------------------------------------------------------------------
bool TransportLayerAddress::decodefromTransportLayerAddress(
    Ngap_TransportLayerAddress_t& transportLayerAddress) {
  if (!transportLayerAddress.buf) return false;

  ipaddress = to_string(transportLayerAddress.buf[0]);
  for (int i = 1; i < transportLayerAddress.size; i++) {
    ipaddress = ipaddress + '.' + to_string(transportLayerAddress.buf[i]);
  }

  return true;
}

}  // namespace ngap
