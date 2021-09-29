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

/*! \file amf_config.cpp
 \brief
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#include "amf_config.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>
#include <libconfig.h++>

#include "3gpp_ts24501.hpp"
#include "amf_app.hpp"
#include "if.hpp"
#include "logger.hpp"
#include "string.hpp"
#include "thread_sched.hpp"

extern "C" {
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>

#include "common_defs.h"
}

using namespace libconfig;
using namespace amf_application;

namespace config {

//------------------------------------------------------------------------------
amf_config::amf_config() {
  nrf_addr.ipv4_addr.s_addr = INADDR_ANY;
  nrf_addr.port             = 80;
  nrf_addr.api_version      = "v1";
  enable_nf_registration    = false;
  enable_smf_selection      = false;
  enable_external_ausf      = false;
  enable_external_udm       = false;
  instance                  = 0;
  n2                        = {};
  n11                       = {};
  statistics_interval       = 0;
  guami                     = {};
  guami_list                = {};
  relativeAMFCapacity       = 0;
  plmn_list                 = {};
  auth_conf auth_para       = {};
  nas_cfg                   = {};
  smf_pool                  = {};
  enable_nf_registration    = false;
  enable_smf_selection      = false;
  enable_external_ausf      = false;
  enable_external_udm       = false;

  struct {
    struct in_addr ipv4_addr;
    unsigned int port;
    std::string api_version;
  } nrf_addr;

  struct {
    struct in_addr ipv4_addr;
    unsigned int port;
    std::string api_version;
  } ausf_addr;

  // TODO:
}

//------------------------------------------------------------------------------
amf_config::~amf_config() {}

//------------------------------------------------------------------------------
int amf_config::load(const std::string& config_file) {
  Logger::amf_app().debug(
      "\nLoad AMF system configuration file(%s)", config_file.c_str());
  Config cfg;
  unsigned char buf_in6_addr[sizeof(struct in6_addr)];
  try {
    cfg.readFile(config_file.c_str());
  } catch (const FileIOException& fioex) {
    Logger::amf_app().error(
        "I/O error while reading file %s - %s", config_file.c_str(),
        fioex.what());
    throw;
  } catch (const ParseException& pex) {
    Logger::amf_app().error(
        "Parse error at %s:%d - %s", pex.getFile(), pex.getLine(),
        pex.getError());
    throw;
  }
  const Setting& root = cfg.getRoot();
  try {
    const Setting& amf_cfg = root[AMF_CONFIG_STRING_AMF_CONFIG];
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error("%s : %s", nfex.what(), nfex.getPath());
    return -1;
  }
  const Setting& amf_cfg = root[AMF_CONFIG_STRING_AMF_CONFIG];
  try {
    amf_cfg.lookupValue(AMF_CONFIG_STRING_INSTANCE_ID, instance);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    amf_cfg.lookupValue(
        AMF_CONFIG_STRING_STATISTICS_TIMER_INTERVAL, statistics_interval);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    amf_cfg.lookupValue(AMF_CONFIG_STRING_PID_DIRECTORY, pid_dir);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    amf_cfg.lookupValue(AMF_CONFIG_STRING_AMF_NAME, AMF_Name);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    const Setting& guami_cfg = amf_cfg[AMF_CONFIG_STRING_GUAMI];
    guami_cfg.lookupValue(AMF_CONFIG_STRING_MCC, guami.mcc);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_MNC, guami.mnc);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_RegionID, guami.regionID);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_AMFSetID, guami.AmfSetID);
    guami_cfg.lookupValue(AMF_CONFIG_STRING_AMFPointer, guami.AmfPointer);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    const Setting& guami_list_cfg =
        amf_cfg[AMF_CONFIG_STRING_SERVED_GUAMI_LIST];
    int count = guami_list_cfg.getLength();
    for (int i = 0; i < count; i++) {
      guami_t guami;
      const Setting& guami_item = guami_list_cfg[i];
      guami_item.lookupValue(AMF_CONFIG_STRING_MCC, guami.mcc);
      guami_item.lookupValue(AMF_CONFIG_STRING_MNC, guami.mnc);
      guami_item.lookupValue(AMF_CONFIG_STRING_RegionID, guami.regionID);
      guami_item.lookupValue(AMF_CONFIG_STRING_AMFSetID, guami.AmfSetID);
      guami_item.lookupValue(AMF_CONFIG_STRING_AMFPointer, guami.AmfPointer);
      guami_list.push_back(guami);
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    amf_cfg.lookupValue(
        AMF_CONFIG_STRING_RELATIVE_AMF_CAPACITY, relativeAMFCapacity);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    const Setting& plmn_list_cfg = amf_cfg[AMF_CONFIG_STRING_PLMN_SUPPORT_LIST];
    int count                    = plmn_list_cfg.getLength();
    for (int i = 0; i < count; i++) {
      plmn_item_t plmn_item;
      const Setting& item = plmn_list_cfg[i];
      item.lookupValue(AMF_CONFIG_STRING_MCC, plmn_item.mcc);
      item.lookupValue(AMF_CONFIG_STRING_MNC, plmn_item.mnc);
      item.lookupValue(AMF_CONFIG_STRING_TAC, plmn_item.tac);
      const Setting& slice_list_cfg =
          plmn_list_cfg[i][AMF_CONFIG_STRING_SLICE_SUPPORT_LIST];
      int numOfSlice = slice_list_cfg.getLength();
      for (int j = 0; j < numOfSlice; j++) {
        slice_t slice;
        const Setting& slice_item = slice_list_cfg[j];
        slice_item.lookupValue(AMF_CONFIG_STRING_SST, slice.sST);
        slice_item.lookupValue(AMF_CONFIG_STRING_SD, slice.sD);
        plmn_item.slice_list.push_back(slice);
      }
      plmn_list.push_back(plmn_item);
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    const Setting& new_if_cfg = amf_cfg[AMF_CONFIG_STRING_INTERFACES];
    const Setting& n2_amf_cfg =
        new_if_cfg[AMF_CONFIG_STRING_INTERFACE_NGAP_AMF];
    load_interface(n2_amf_cfg, n2);
    const Setting& n11_cfg = new_if_cfg[AMF_CONFIG_STRING_INTERFACE_N11];
    load_interface(n11_cfg, n11);
    // SBI API VERSION
    if (!(n11_cfg.lookupValue(
            AMF_CONFIG_STRING_API_VERSION, sbi_api_version))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
      throw(AMF_CONFIG_STRING_API_VERSION "failed");
    }

    const Setting& smf_addr_pool =
        n11_cfg[AMF_CONFIG_STRING_SMF_INSTANCES_POOL];
    int count = smf_addr_pool.getLength();
    for (int i = 0; i < count; i++) {
      const Setting& smf_addr_item = smf_addr_pool[i];
      smf_inst_t smf_inst;
      std::string selected;
      smf_addr_item.lookupValue(AMF_CONFIG_STRING_SMF_INSTANCE_ID, smf_inst.id);
      smf_addr_item.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, smf_inst.ipv4);
      smf_addr_item.lookupValue(
          AMF_CONFIG_STRING_SMF_INSTANCE_PORT, smf_inst.port);
      smf_addr_item.lookupValue(
          AMF_CONFIG_STRING_SMF_INSTANCE_VERSION, smf_inst.version);
      smf_addr_item.lookupValue(
          AMF_CONFIG_STRING_SMF_INSTANCE_SELECTED, selected);
      if (!selected.compare("true"))
        smf_inst.selected = true;
      else
        smf_inst.selected = false;
      smf_pool.push_back(smf_inst);
    }

    // NRF
    const Setting& nrf_cfg = new_if_cfg[AMF_CONFIG_STRING_NRF];
    struct in_addr nrf_ipv4_addr;
    unsigned int nrf_port = 0;
    std::string nrf_api_version;
    string address;
    nrf_cfg.lookupValue(AMF_CONFIG_STRING_NRF_IPV4_ADDRESS, address);
    IPV4_STR_ADDR_TO_INADDR(
        util::trim(address).c_str(), nrf_ipv4_addr,
        "BAD IPv4 ADDRESS FORMAT FOR NRF !");
    nrf_addr.ipv4_addr = nrf_ipv4_addr;
    if (!(nrf_cfg.lookupValue(AMF_CONFIG_STRING_NRF_PORT, nrf_port))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_NRF_PORT "failed");
      throw(AMF_CONFIG_STRING_NRF_PORT "failed");
    }
    nrf_addr.port = nrf_port;

    if (!(nrf_cfg.lookupValue(
            AMF_CONFIG_STRING_API_VERSION, nrf_api_version))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
      throw(AMF_CONFIG_STRING_API_VERSION "failed");
    }
    nrf_addr.api_version = nrf_api_version;

    // AUSF
    const Setting& ausf_cfg = new_if_cfg[AMF_CONFIG_STRING_AUSF];
    struct in_addr ausf_ipv4_addr;
    unsigned int ausf_port = 0;
    std::string ausf_api_version;
    // string address;
    ausf_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
    IPV4_STR_ADDR_TO_INADDR(
        util::trim(address).c_str(), ausf_ipv4_addr,
        "BAD IPv4 ADDRESS FORMAT FOR AUSF !");
    ausf_addr.ipv4_addr = ausf_ipv4_addr;
    if (!(ausf_cfg.lookupValue(AMF_CONFIG_STRING_PORT, ausf_port))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_PORT "failed");
      throw(AMF_CONFIG_STRING_PORT "failed");
    }
    ausf_addr.port = ausf_port;

    if (!(ausf_cfg.lookupValue(
            AMF_CONFIG_STRING_API_VERSION, ausf_api_version))) {
      Logger::amf_app().error(AMF_CONFIG_STRING_API_VERSION "failed");
      throw(AMF_CONFIG_STRING_API_VERSION "failed");
    }
    ausf_addr.api_version = ausf_api_version;

  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return -1;
  }
  try {
    const Setting& core_config = amf_cfg[AMF_CONFIG_STRING_CORE_CONFIGURATION];
    core_config.lookupValue(
        AMF_CONFIG_STRING_EMERGENCY_SUPPORT, is_emergency_support);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return -1;
  }
  try {
    const Setting& auth = amf_cfg[AMF_CONFIG_STRING_AUTHENTICATION];
    auth.lookupValue(
        AMF_CONFIG_STRING_AUTH_MYSQL_SERVER, auth_para.mysql_server);
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_USER, auth_para.mysql_user);
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_PASS, auth_para.mysql_pass);
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_MYSQL_DB, auth_para.mysql_db);
    auth.lookupValue(
        AMF_CONFIG_STRING_AUTH_OPERATOR_KEY, auth_para.operator_key);
    auth.lookupValue(AMF_CONFIG_STRING_AUTH_RANDOM, auth_para.random);
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return -1;
  }
  try {
    const Setting& nas = amf_cfg[AMF_CONFIG_STRING_NAS];
    const Setting& intAlg =
        nas[AMF_CONFIG_STRING_NAS_SUPPORTED_INTEGRITY_ALGORITHM_LIST];

    int intCount = intAlg.getLength();
    for (int i = 0; i < intCount; i++) {
      std::string intAlgStr = intAlg[i];
      if (!intAlgStr.compare("NIA0"))
        nas_cfg.prefered_integrity_algorithm[i] = IA0_5G;
      if (!intAlgStr.compare("NIA1"))
        nas_cfg.prefered_integrity_algorithm[i] = IA1_128_5G;
      if (!intAlgStr.compare("NIA2"))
        nas_cfg.prefered_integrity_algorithm[i] = IA2_128_5G;
    }
    for (int i = intCount; i < 8; i++) {
      nas_cfg.prefered_integrity_algorithm[i] = IA0_5G;
    }
    const Setting& encAlg =
        nas[AMF_CONFIG_STRING_NAS_SUPPORTED_CIPHERING_ALGORITHM_LIST];
    int encCount = encAlg.getLength();
    for (int i = 0; i < encCount; i++) {
      std::string encAlgStr = encAlg[i];
      if (!encAlgStr.compare("NEA0"))
        nas_cfg.prefered_ciphering_algorithm[i] = EA0_5G;
      if (!encAlgStr.compare("NEA1"))
        nas_cfg.prefered_ciphering_algorithm[i] = EA1_128_5G;
      if (!encAlgStr.compare("NEA2"))
        nas_cfg.prefered_ciphering_algorithm[i] = EA2_128_5G;
    }
    for (int i = encCount; i < 8; i++) {
      nas_cfg.prefered_ciphering_algorithm[i] = EA0_5G;
    }

  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return -1;
  }

  try {
    const Setting& support_features =
        amf_cfg[AMF_CONFIG_STRING_SUPPORT_FEATURES];
    string opt;
    support_features.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_NF_REGISTRATION, opt);
    if (boost::iequals(opt, "yes")) {
      enable_nf_registration = true;
    } else {
      enable_nf_registration = false;
    }

    support_features.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_SMF_SELECTION, opt);
    if (boost::iequals(opt, "yes")) {
      enable_smf_selection = true;
    } else {
      enable_smf_selection = false;
    }

    support_features.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_AUSF, opt);
    if (boost::iequals(opt, "yes")) {
      enable_external_ausf = true;
    } else {
      enable_external_ausf = false;
    }

    support_features.lookupValue(
        AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_UDM, opt);
    if (boost::iequals(opt, "yes")) {
      enable_external_udm = true;
    } else {
      enable_external_udm = false;
    }

  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
    return -1;
  }
  return 1;
}

//------------------------------------------------------------------------------
void amf_config::display() {
  Logger::config().info(
      "======================    AMF   =====================");
  Logger::config().info("Configuration AMF:");
  Logger::config().info("- Instance ................: %d", instance);
  Logger::config().info("- PID dir .................: %s", pid_dir.c_str());
  Logger::config().info("- AMF NAME.................: %s", AMF_Name.c_str());
  Logger::config().info(
      "- GUAMI (MCC, MNC, Region ID, AMF Set ID, AMF pointer): ");
  Logger::config().info(
      "   (%s, %s, %s, %s, %s )", guami.mcc.c_str(), guami.mnc.c_str(),
      guami.regionID.c_str(), guami.AmfSetID.c_str(), guami.AmfPointer.c_str());
  Logger::config().info("- Served_Guami_List .......: ");
  for (int i = 0; i < guami_list.size(); i++) {
    Logger::config().info(
        "   (%s, %s, %s , %s, %s)", guami_list[i].mcc.c_str(),
        guami_list[i].mnc.c_str(), guami_list[i].regionID.c_str(),
        guami_list[i].AmfSetID.c_str(), guami_list[i].AmfPointer.c_str());
  }
  Logger::config().info("- Relative Capacity .......: %d", relativeAMFCapacity);
  Logger::config().info("- PLMN Support ............: ");
  for (int i = 0; i < plmn_list.size(); i++) {
    Logger::config().info(
        "       MCC, MNC ...........: %s, %s", plmn_list[i].mcc.c_str(),
        plmn_list[i].mnc.c_str());
    Logger::config().info("       TAC ................: %d", plmn_list[i].tac);
    Logger::config().info("       Slice Support ......:");
    for (int j = 0; j < plmn_list[i].slice_list.size(); j++) {
      Logger::config().info(
          "           SST, SD ........: %s, %s",
          plmn_list[i].slice_list[j].sST.c_str(),
          plmn_list[i].slice_list[j].sD.c_str());
    }
  }
  Logger::config().info(
      "- Emergency Support .......: %s", is_emergency_support.c_str());
  Logger::config().info(
      "- MySQL Server Addr .......: %s", auth_para.mysql_server.c_str());
  Logger::config().info(
      "- MySQL user ..............: %s", auth_para.mysql_user.c_str());
  Logger::config().info(
      "- MySQL pass ..............: %s", auth_para.mysql_pass.c_str());
  Logger::config().info(
      "- MySQL DB ................: %s", auth_para.mysql_db.c_str());
  Logger::config().info(
      "- operator key ............: %s", auth_para.operator_key.c_str());
  Logger::config().info(
      "- random ..................: %s", auth_para.random.c_str());

  Logger::config().info("- N2 Networking:");
  Logger::config().info("    iface .................: %s", n2.if_name.c_str());
  Logger::config().info("    ip ....................: %s", inet_ntoa(n2.addr4));
  Logger::config().info("    port ..................: %d", n2.port);

  Logger::config().info("- SBI Networking:");
  Logger::config().info("    iface .................: %s", n11.if_name.c_str());
  Logger::config().info(
      "    ip ....................: %s", inet_ntoa(n11.addr4));
  Logger::config().info("    port ..................: %d", n11.port);
  Logger::config().info(
      "    API version............: %s", sbi_api_version.c_str());

  if (enable_nf_registration or enable_smf_selection) {
    Logger::config().info("- NRF:");
    Logger::config().info(
        "    IP addr ...............: %s", inet_ntoa(nrf_addr.ipv4_addr));
    Logger::config().info("    Port ..................: %d", nrf_addr.port);
    Logger::config().info(
        "    Api version ...........: %s", nrf_addr.api_version.c_str());
  }

  if (enable_external_ausf) {
    Logger::config().info("- AUSF:");
    Logger::config().info(
        "    IP addr ...............: %s", inet_ntoa(ausf_addr.ipv4_addr));
    Logger::config().info("    Port ..................: %d", ausf_addr.port);
    Logger::config().info(
        "    Api version ...........: %s", ausf_addr.api_version.c_str());
  }

  Logger::config().info("- Remote SMF Pool.........: ");
  for (int i = 0; i < smf_pool.size(); i++) {
    std::string selected;
    if (smf_pool[i].selected)
      selected = "true";
    else
      selected = "false";
    Logger::config().info(
        "    SMF_INSTANCE_ID %d (%s:%s, version %s) is selected: %s",
        smf_pool[i].id, smf_pool[i].ipv4.c_str(), smf_pool[i].port.c_str(),
        smf_pool[i].version.c_str(), selected.c_str());
  }

  Logger::config().info("- Supported Features:");
  Logger::config().info(
      "    NF Registration .......: %s", enable_nf_registration ? "Yes" : "No");
  Logger::config().info(
      "    SMF Selection .........: %s", enable_smf_selection ? "Yes" : "No");
  Logger::config().info(
      "    External AUSF .........: %s", enable_external_ausf ? "Yes" : "No");
  Logger::config().info(
      "    External UDM ..........: %s", enable_external_udm ? "Yes" : "No");
}

//------------------------------------------------------------------------------
int amf_config::load_interface(
    const libconfig::Setting& if_cfg, interface_cfg_t& cfg) {
  if_cfg.lookupValue(AMF_CONFIG_STRING_INTERFACE_NAME, cfg.if_name);
  util::trim(cfg.if_name);
  if (not boost::iequals(cfg.if_name, "none")) {
    std::string address = {};
    if_cfg.lookupValue(AMF_CONFIG_STRING_IPV4_ADDRESS, address);
    util::trim(address);
    if (boost::iequals(address, "read")) {
      if (get_inet_addr_infos_from_iface(
              cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
        Logger::amf_app().error(
            "Could not read %s network interface configuration", cfg.if_name);
        return RETURNerror;
      }
    } else {
      std::vector<std::string> words;
      boost::split(
          words, address, boost::is_any_of("/"), boost::token_compress_on);
      if (words.size() != 2) {
        Logger::amf_app().error(
            "Bad value " AMF_CONFIG_STRING_IPV4_ADDRESS " = %s in config file",
            address.c_str());
        return RETURNerror;
      }
      unsigned char buf_in_addr[sizeof(struct in6_addr)];  // you never know...
      if (inet_pton(AF_INET, util::trim(words.at(0)).c_str(), buf_in_addr) ==
          1) {
        memcpy(&cfg.addr4, buf_in_addr, sizeof(struct in_addr));
      } else {
        Logger::amf_app().error(
            "In conversion: Bad value " AMF_CONFIG_STRING_IPV4_ADDRESS
            " = %s in config file",
            util::trim(words.at(0)).c_str());
        return RETURNerror;
      }
      cfg.network4.s_addr = htons(
          ntohs(cfg.addr4.s_addr) &
          0xFFFFFFFF << (32 - std::stoi(util::trim(words.at(1)))));
    }
    if_cfg.lookupValue(AMF_CONFIG_STRING_PORT, cfg.port);
  }
  return RETURNok;
}

}  // namespace config
