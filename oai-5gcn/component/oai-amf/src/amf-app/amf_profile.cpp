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

/*! \file amf_profile.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#include "amf_profile.hpp"

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "logger.hpp"
#include "string.hpp"

using namespace std;
using namespace amf_application;

//------------------------------------------------------------------------------
void nf_profile::set_nf_instance_id(const std::string& instance_id) {
  nf_instance_id = instance_id;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_instance_id(std::string& instance_id) const {
  instance_id = nf_instance_id;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_nf_instance_id() const {
  return nf_instance_id;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_instance_name(const std::string& instance_name) {
  nf_instance_name = instance_name;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_instance_name(std::string& instance_name) const {
  instance_name = nf_instance_name;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_nf_instance_name() const {
  return nf_instance_name;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_type(const std::string& type) {
  nf_type = type;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_nf_type() const {
  return nf_type;
}
//------------------------------------------------------------------------------
void nf_profile::set_nf_status(const std::string& status) {
  nf_status = status;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_status(std::string& status) const {
  status = nf_status;
}

//------------------------------------------------------------------------------
std::string nf_profile::get_nf_status() const {
  return nf_status;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_heartBeat_timer(const int32_t& timer) {
  heartBeat_timer = timer;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_heartBeat_timer(int32_t& timer) const {
  timer = heartBeat_timer;
}

//------------------------------------------------------------------------------
int32_t nf_profile::get_nf_heartBeat_timer() const {
  return heartBeat_timer;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_priority(const uint16_t& p) {
  priority = p;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_priority(uint16_t& p) const {
  p = priority;
}

//------------------------------------------------------------------------------
uint16_t nf_profile::get_nf_priority() const {
  return priority;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_capacity(const uint16_t& c) {
  capacity = c;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_capacity(uint16_t& c) const {
  c = capacity;
}

//------------------------------------------------------------------------------
uint16_t nf_profile::get_nf_capacity() const {
  return capacity;
}

//------------------------------------------------------------------------------
void nf_profile::set_nf_snssais(const std::vector<snssai_t>& s) {
  snssais = s;
}

//------------------------------------------------------------------------------
void nf_profile::get_nf_snssais(std::vector<snssai_t>& s) const {
  s = snssais;
}

//------------------------------------------------------------------------------
void nf_profile::add_snssai(const snssai_t& s) {
  snssais.push_back(s);
}
//------------------------------------------------------------------------------
void nf_profile::set_nf_ipv4_addresses(const std::vector<struct in_addr>& a) {
  ipv4_addresses = a;
}

//------------------------------------------------------------------------------
void nf_profile::add_nf_ipv4_addresses(const struct in_addr& a) {
  ipv4_addresses.push_back(a);
}
//------------------------------------------------------------------------------
void nf_profile::get_nf_ipv4_addresses(std::vector<struct in_addr>& a) const {
  a = ipv4_addresses;
}

//------------------------------------------------------------------------------
void nf_profile::display() const {
  Logger::amf_app().debug("NF instance info");
  Logger::amf_app().debug("\tInstance ID: %s", nf_instance_id.c_str());

  Logger::amf_app().debug("\tInstance name: %s", nf_instance_name.c_str());
  Logger::amf_app().debug("\tInstance type: %s", nf_type.c_str());
  Logger::amf_app().debug("\tStatus: %s", nf_status.c_str());
  Logger::amf_app().debug("\tHeartBeat timer: %d", heartBeat_timer);
  Logger::amf_app().debug("\tPriority: %d", priority);
  Logger::amf_app().debug("\tCapacity: %d", capacity);
  // SNSSAIs
  if (snssais.size() > 0) {
    Logger::amf_app().debug("\tSNSSAI:");
  }
  for (auto s : snssais) {
    Logger::amf_app().debug("\t\t SST, SD: %d, %s", s.sST, s.sD.c_str());
  }

  // IPv4 Addresses
  if (ipv4_addresses.size() > 0) {
    Logger::amf_app().debug("\tIPv4 Addr:");
  }
  for (auto address : ipv4_addresses) {
    Logger::amf_app().debug("\t\t %s", inet_ntoa(address));
  }
}

//------------------------------------------------------------------------------
void nf_profile::to_json(nlohmann::json& data) const {
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
    data["ipv4Addresses"].push_back(inet_ntoa(address));
  }

  data["priority"] = priority;
  data["capacity"] = capacity;

  // Logger::amf_app().debug("AMF profile to json:\n %s", data.dump().c_str());
}

//------------------------------------------------------------------------------
void nf_profile::from_json(const nlohmann::json& data) {
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
      // Logger::amf_app().debug("Added SNSSAI (SST %d, SD %s)", s.sST,
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
        Logger::amf_app().warn(
            "Address conversion: Bad value %s", util::trim(address).c_str());
      }
      // Logger::amf_app().debug("\tIPv4 Addr: %s", address.c_str());
      add_nf_ipv4_addresses(addr4);
    }
  }

  if (data.find("priority") != data.end()) {
    priority = data["priority"].get<int>();
  }

  if (data.find("capacity") != data.end()) {
    capacity = data["capacity"].get<int>();
  }
}

//------------------------------------------------------------------------------
void amf_profile::set_nf_services(const std::vector<nf_service_t>& n) {
  nf_services = n;
}

//------------------------------------------------------------------------------
void amf_profile::add_nf_service(const nf_service_t& n) {
  nf_services.push_back(n);
}

//------------------------------------------------------------------------------
void amf_profile::get_nf_services(std::vector<nf_service_t>& n) const {
  n = nf_services;
}

//------------------------------------------------------------------------------
void amf_profile::set_custom_info(const nlohmann::json& c) {
  custom_info = c;
}

//------------------------------------------------------------------------------
void amf_profile::get_custom_info(nlohmann::json& c) const {
  c = custom_info;
}

//------------------------------------------------------------------------------
void amf_profile::set_amf_info(const amf_info_t& s) {
  amf_info = s;
}

//------------------------------------------------------------------------------
void amf_profile::get_amf_info(amf_info_t& s) const {
  s = amf_info;
}

//------------------------------------------------------------------------------
void amf_profile::display() const {
  // display NF part
  nf_profile::display();

  // NF services
  if (nf_services.size() > 0) {
    Logger::amf_app().debug("\tNF Service");
  }
  for (auto service : nf_services) {
    Logger::amf_app().debug("\t\t%s", service.to_string().c_str());
  }

  if (!custom_info.empty()) {
    Logger::amf_app().debug("\tCustom info: %s", custom_info.dump().c_str());
  }

  // AMF info
  Logger::amf_app().debug("\tAMF Info");
  Logger::amf_app().debug(
      "\t\tAMF Set ID: %s, AMF Region ID: %s", amf_info.amf_set_id.c_str(),
      amf_info.amf_region_id.c_str());

  for (auto g : amf_info.guami_list) {
    Logger::amf_app().debug("\t\tAMF GUAMI List, AMF_ID: %s", g.amf_id.c_str());
    Logger::amf_app().debug(
        "\t\tAMF GUAMI List, PLMN (MCC: %s, MNC: %s)", g.plmn.mcc.c_str(),
        g.plmn.mnc.c_str());
  }
}

//------------------------------------------------------------------------------
void amf_profile::to_json(nlohmann::json& data) const {
  nf_profile::to_json(data);

  // NF services
  data["nfServices"] = nlohmann::json::array();
  for (auto service : nf_services) {
    nlohmann::json srv_tmp       = {};
    srv_tmp["serviceInstanceId"] = service.service_instance_id;
    srv_tmp["serviceName"]       = service.service_name;
    srv_tmp["versions"]          = nlohmann::json::array();
    for (auto v : service.versions) {
      nlohmann::json v_tmp     = {};
      v_tmp["apiVersionInUri"] = v.api_version_in_uri;
      v_tmp["apiFullVersion"]  = v.api_full_version;
      srv_tmp["versions"].push_back(v_tmp);
    }
    srv_tmp["scheme"]          = service.scheme;
    srv_tmp["nfServiceStatus"] = service.nf_service_status;
    // IP endpoints
    srv_tmp["ipEndPoints"] = nlohmann::json::array();
    for (auto endpoint : service.ip_endpoints) {
      nlohmann::json ep_tmp = {};
      ep_tmp["ipv4Address"] = nlohmann::json::array();
      for (auto address : endpoint.ipv4_addresses) {
        ep_tmp["ipv4Address"].push_back(inet_ntoa(address));
      }
      ep_tmp["transport"] = endpoint.transport;
      ep_tmp["port"]      = endpoint.port;
      srv_tmp["ipEndPoints"].push_back(ep_tmp);
    }

    data["nfServices"].push_back(srv_tmp);
  }

  data["custom_info"] = custom_info;

  // AMF info
  data["amfInfo"]["amfSetId"]    = amf_info.amf_set_id;
  data["amfInfo"]["amfRegionId"] = amf_info.amf_region_id;
  // guamiList
  data["amfInfo"]["guamiList"] = nlohmann::json::array();
  for (auto guami : amf_info.guami_list) {
    nlohmann::json tmp   = {};
    tmp["amfId"]         = guami.amf_id;
    tmp["plmnId"]["mnc"] = guami.plmn.mnc;
    tmp["plmnId"]["mcc"] = guami.plmn.mcc;
    data["amfInfo"]["guamiList"].push_back(tmp);
  }

  Logger::amf_app().debug("AMF profile to json:\n %s", data.dump().c_str());
}

//------------------------------------------------------------------------------
void amf_profile::from_json(const nlohmann::json& data) {
  nf_profile::from_json(data);

  // TODO: custom_info;

  // AMF info
  if (data.find("amfInfo") != data.end()) {
    nlohmann::json info    = data["amfInfo"];
    amf_info.amf_set_id    = info["amfSetId"].get<std::string>();
    amf_info.amf_region_id = info["amfRegionId"].get<std::string>();

    nlohmann::json guami_list = data["amfInfo"]["guamiList"];

    for (auto it : guami_list) {
      guami_5g_t guami = {};

      if (it.find("amfId") != it.end()) {
        guami.amf_id = it["amfId"].get<std::string>();
      }
      if (it.find("plmnId") != it.end()) {
        nlohmann::json plmn = it["plmnId"];

        if (plmn.find("mnc") != plmn.end()) {
          guami.plmn.mnc = plmn["mnc"].get<std::string>();
        }

        if (plmn.find("mcc") != plmn.end()) {
          guami.plmn.mcc = plmn["mcc"].get<std::string>();
        }
      }
      amf_info.guami_list.push_back(guami);
    }
  }

  display();
}

//------------------------------------------------------------------------------
void amf_profile::handle_heartbeart_timeout(uint64_t ms) {
  Logger::amf_app().info(
      "Handle heartbeart timeout profile %s, time %d", nf_instance_id.c_str(),
      ms);
  set_nf_status("SUSPENDED");
}
