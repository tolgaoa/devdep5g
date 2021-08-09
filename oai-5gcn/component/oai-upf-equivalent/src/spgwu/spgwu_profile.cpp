/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

/*! \file spgwu_profile.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2021
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "logger.hpp"
#include "spgwu_profile.hpp"
#include "string.hpp"

using namespace std;
using namespace spgwu;

//------------------------------------------------------------------------------
void spgwu_profile::set_nf_instance_id(const std::string& instance_id) {
  nf_instance_id = instance_id;
}

//------------------------------------------------------------------------------
void spgwu_profile::get_nf_instance_id(std::string& instance_id) const {
  instance_id = nf_instance_id;
}

//------------------------------------------------------------------------------
std::string spgwu_profile::get_nf_instance_id() const {
  return nf_instance_id;
}

//------------------------------------------------------------------------------
void spgwu_profile::set_nf_instance_name(const std::string& instance_name) {
  nf_instance_name = instance_name;
}

//------------------------------------------------------------------------------
void spgwu_profile::get_nf_instance_name(std::string& instance_name) const {
  instance_name = nf_instance_name;
}

//------------------------------------------------------------------------------
std::string spgwu_profile::get_nf_instance_name() const {
  return nf_instance_name;
}

//------------------------------------------------------------------------------
void spgwu_profile::set_nf_type(const std::string& type) {
  nf_type = type;
}

//------------------------------------------------------------------------------
std::string spgwu_profile::get_nf_type() const {
  return nf_type;
}
//------------------------------------------------------------------------------
void spgwu_profile::set_nf_status(const std::string& status) {
  nf_status = status;
}

//------------------------------------------------------------------------------
void spgwu_profile::get_nf_status(std::string& status) const {
  status = nf_status;
}

//------------------------------------------------------------------------------
std::string spgwu_profile::get_nf_status() const {
  return nf_status;
}

//------------------------------------------------------------------------------
void spgwu_profile::set_nf_heartBeat_timer(const int32_t& timer) {
  heartBeat_timer = timer;
}

//------------------------------------------------------------------------------
void spgwu_profile::get_nf_heartBeat_timer(int32_t& timer) const {
  timer = heartBeat_timer;
}

//------------------------------------------------------------------------------
int32_t spgwu_profile::get_nf_heartBeat_timer() const {
  return heartBeat_timer;
}

//------------------------------------------------------------------------------
void spgwu_profile::set_nf_priority(const uint16_t& p) {
  priority = p;
}

//------------------------------------------------------------------------------
void spgwu_profile::get_nf_priority(uint16_t& p) const {
  p = priority;
}

//------------------------------------------------------------------------------
uint16_t spgwu_profile::get_nf_priority() const {
  return priority;
}

//------------------------------------------------------------------------------
void spgwu_profile::set_nf_capacity(const uint16_t& c) {
  capacity = c;
}

//------------------------------------------------------------------------------
void spgwu_profile::get_nf_capacity(uint16_t& c) const {
  c = capacity;
}

//------------------------------------------------------------------------------
uint16_t spgwu_profile::get_nf_capacity() const {
  return capacity;
}

//------------------------------------------------------------------------------
void spgwu_profile::set_nf_snssais(const std::vector<snssai_t>& s) {
  snssais = s;
}

//------------------------------------------------------------------------------
void spgwu_profile::get_nf_snssais(std::vector<snssai_t>& s) const {
  s = snssais;
}

//------------------------------------------------------------------------------
void spgwu_profile::add_snssai(const snssai_t& s) {
  snssais.push_back(s);
}
//------------------------------------------------------------------------------
void spgwu_profile::set_nf_ipv4_addresses(
    const std::vector<struct in_addr>& a) {
  ipv4_addresses = a;
}

//------------------------------------------------------------------------------
void spgwu_profile::add_nf_ipv4_addresses(const struct in_addr& a) {
  ipv4_addresses.push_back(a);
}
//------------------------------------------------------------------------------
void spgwu_profile::get_nf_ipv4_addresses(
    std::vector<struct in_addr>& a) const {
  a = ipv4_addresses;
}

//------------------------------------------------------------------------------
void spgwu_profile::set_upf_info(const upf_info_t& s) {
  upf_info = s;
}

//------------------------------------------------------------------------------
void spgwu_profile::add_upf_info_item(const snssai_upf_info_item_t& s) {
  upf_info.snssai_upf_info_list.push_back(s);
}

//------------------------------------------------------------------------------
void spgwu_profile::get_upf_info(upf_info_t& s) const {
  s = upf_info;
}

//------------------------------------------------------------------------------
void spgwu_profile::display() const {
  Logger::spgwu_app().debug("NF instance info");

  Logger::spgwu_app().debug("\tInstance ID: %s", nf_instance_id.c_str());

  Logger::spgwu_app().debug("\tInstance name: %s", nf_instance_name.c_str());
  Logger::spgwu_app().debug("\tInstance type: %s", nf_type.c_str());
  Logger::spgwu_app().debug("\tStatus: %s", nf_status.c_str());
  Logger::spgwu_app().debug("\tHeartBeat timer: %d", heartBeat_timer);
  Logger::spgwu_app().debug("\tPriority: %d", priority);
  Logger::spgwu_app().debug("\tCapacity: %d", capacity);
  // SNSSAIs
  if (snssais.size() > 0) {
    Logger::spgwu_app().debug("\tSNSSAI:");
  }
  for (auto s : snssais) {
    Logger::spgwu_app().debug("\t\t SST, SD: %d, %s", s.sST, s.sD.c_str());
  }

  // IPv4 Addresses
  if (ipv4_addresses.size() > 0) {
    Logger::spgwu_app().debug("\tIPv4 Addr:");
  }
  for (auto address : ipv4_addresses) {
    Logger::spgwu_app().debug("\t\t %s", inet_ntoa(address));
  }

  // UPF info
  if (upf_info.snssai_upf_info_list.size() > 0) {
    Logger::spgwu_app().debug("\tUPF Info:");
  }
  for (auto s : upf_info.snssai_upf_info_list) {
    Logger::spgwu_app().debug("\t\tParameters supported by the UPF:");
    Logger::spgwu_app().debug(
        "\t\t\tSNSSAI (SST %d, SD %s)", s.snssai.sST, s.snssai.sD.c_str());
    for (auto d : s.dnn_upf_info_list) {
      Logger::spgwu_app().debug("\t\t\tDNN %s", d.dnn.c_str());
    }
  }
}

//------------------------------------------------------------------------------
void spgwu_profile::to_json(nlohmann::json& data) const {
  data["nfInstanceId"]   = nf_instance_id;
  data["nfInstanceName"] = nf_instance_name;
  data["nfType"]         = nf_type;
  data["nfStatus"]       = nf_status;
  data["heartBeatTimer"] = heartBeat_timer;
  // SNSSAIs
  data["sNssais"] = nlohmann::json::array();
  for (auto s : snssais) {
    nlohmann::json tmp = {};
    tmp["sst"]         = s.sST;
    tmp["sd"]          = s.sD;
    ;
    data["sNssais"].push_back(tmp);
  }
  // ipv4_addresses
  data["ipv4Addresses"] = nlohmann::json::array();
  for (auto address : ipv4_addresses) {
    nlohmann::json tmp = inet_ntoa(address);
    data["ipv4Addresses"].push_back(tmp);
  }

  data["priority"] = priority;
  data["capacity"] = capacity;

  // UPF info
  data["upfInfo"]                      = {};
  data["upfInfo"]["sNssaiUpfInfoList"] = nlohmann::json::array();
  for (auto s : upf_info.snssai_upf_info_list) {
    nlohmann::json tmp    = {};
    tmp["sNssai"]["sst"]  = s.snssai.sST;
    tmp["sNssai"]["sd"]   = s.snssai.sD;
    tmp["dnnUpfInfoList"] = nlohmann::json::array();
    for (auto d : s.dnn_upf_info_list) {
      nlohmann::json dnn_json = {};
      dnn_json["dnn"]         = d.dnn;
      tmp["dnnUpfInfoList"].push_back(dnn_json);
    }
    data["upfInfo"]["sNssaiUpfInfoList"].push_back(tmp);
  }

  Logger::spgwu_app().debug("UPF profile to json:\n %s", data.dump().c_str());
}

//------------------------------------------------------------------------------
void spgwu_profile::from_json(const nlohmann::json& data) {
  if (data.find("nfInstanceId") != data.end()) {
    nf_instance_id = data["nfInstanceId"].get<std::string>();
  }

  if (data.find("nfInstanceName") != data.end()) {
    nf_instance_name = data["nfInstanceName"].get<std::string>();
  }

  if (data.find("nfType") != data.end()) {
    nf_type = data["nfType"].get<std::string>();
  }

  if (data.find("nfStatus") != data.end()) {
    nf_status = data["nfStatus"].get<std::string>();
  }

  if (data.find("heartBeatTimer") != data.end()) {
    heartBeat_timer = data["heartBeatTimer"].get<int>();
  }
  // sNssais
  if (data.find("sNssais") != data.end()) {
    for (auto it : data["sNssais"]) {
      snssai_t s = {};
      s.sST      = it["sst"].get<int>();
      s.sD       = it["sd"].get<std::string>();
      snssais.push_back(s);
      // Logger::spgwu_app().debug("Added SNSSAI (SST %d, SD %s)", s.sST,
      // s.sD.c_str());
    }
  }

  if (data.find("ipv4Addresses") != data.end()) {
    nlohmann::json addresses = data["ipv4Addresses"];

    for (auto it : addresses) {
      struct in_addr addr4 = {};
      std::string address  = it.get<std::string>();
      unsigned char buf_in_addr[sizeof(struct in_addr)];
      if (inet_pton(AF_INET, util::trim(address).c_str(), buf_in_addr) == 1) {
        memcpy(&addr4, buf_in_addr, sizeof(struct in_addr));
      } else {
        Logger::spgwu_app().warn(
            "Address conversion: Bad value %s", util::trim(address).c_str());
      }
      // Logger::spgwu_app().debug("\tIPv4 Addr: %s", address.c_str());
      add_nf_ipv4_addresses(addr4);
    }
  }

  if (data.find("priority") != data.end()) {
    priority = data["priority"].get<int>();
  }

  if (data.find("capacity") != data.end()) {
    capacity = data["capacity"].get<int>();
  }

  // UPF info
  if (data.find("upfInfo") != data.end()) {
    nlohmann::json info = data["upfInfo"];

    dnn_upf_info_item_t dnn_item = {};

    if (info.find("sNssaiUpfInfoList") != info.end()) {
      nlohmann::json snssai_upf_info_list =
          data["upfInfo"]["sNssaiUpfInfoList"];

      for (auto it : snssai_upf_info_list) {
        snssai_upf_info_item_t upf_info_item = {};
        if (it.find("sNssai") != it.end()) {
          if (it["sNssai"].find("sst") != it["sNssai"].end())
            upf_info_item.snssai.sST = it["sNssai"]["sst"].get<int>();
          if (it["sNssai"].find("sd") != it["sNssai"].end())
            upf_info_item.snssai.sD = it["sNssai"]["sd"].get<std::string>();
        }
        if (it.find("dnnUpfInfoList") != it.end()) {
          for (auto d : it["dnnUpfInfoList"]) {
            if (d.find("dnn") != d.end()) {
              dnn_item.dnn = d["dnn"].get<std::string>();
              upf_info_item.dnn_upf_info_list.push_back(dnn_item);
            }
          }
        }
        upf_info.snssai_upf_info_list.push_back(upf_info_item);
      }
    }
  }

  display();
}

//------------------------------------------------------------------------------
void spgwu_profile::handle_heartbeart_timeout(uint64_t ms) {
  Logger::spgwu_app().info(
      "Handle heartbeart timeout profile %s, time %d", nf_instance_id.c_str(),
      ms);
  set_nf_status("SUSPENDED");
}
