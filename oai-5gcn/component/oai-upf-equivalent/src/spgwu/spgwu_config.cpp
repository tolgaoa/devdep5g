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

/*! \file spgwu_config.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "3gpp_29.274.hpp"
#include "async_shell_cmd.hpp"
#include "common_defs.h"
#include "conversions.hpp"
#include "get_gateway_netlink.hpp"
#include "if.hpp"
#include "logger.hpp"
#include "spgwu_config.hpp"
#include "string.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <cstdlib>
#include <iomanip>
#include <iostream>

using namespace std;
using namespace libconfig;
using namespace spgwu;

// C includes
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

//------------------------------------------------------------------------------
int spgwu_config::execute() {
  return RETURNok;
}

//------------------------------------------------------------------------------
int spgwu_config::get_pfcp_node_id(pfcp::node_id_t& node_id) {
  node_id = {};
  if (sx.addr4.s_addr) {
    node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
    node_id.u1.ipv4_address = sx.addr4;
    return RETURNok;
  }
  if (sx.addr6.s6_addr32[0] | sx.addr6.s6_addr32[1] | sx.addr6.s6_addr32[2] |
      sx.addr6.s6_addr32[3]) {
    node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV6_ADDRESS;
    node_id.u1.ipv6_address = sx.addr6;
    return RETURNok;
  }
  return RETURNerror;
}
//------------------------------------------------------------------------------
int spgwu_config::get_pfcp_fseid(pfcp::fseid_t& fseid) {
  int rc = RETURNerror;
  fseid  = {};
  if (sx.addr4.s_addr) {
    fseid.v4           = 1;
    fseid.ipv4_address = sx.addr4;
    rc                 = RETURNok;
  }
  if (sx.addr6.s6_addr32[0] | sx.addr6.s6_addr32[1] | sx.addr6.s6_addr32[2] |
      sx.addr6.s6_addr32[3]) {
    fseid.v6           = 1;
    fseid.ipv6_address = sx.addr6;
    rc                 = RETURNok;
  }
  return rc;
}
//------------------------------------------------------------------------------
int spgwu_config::load_thread_sched_params(
    const Setting& thread_sched_params_cfg, util::thread_sched_params& cfg) {
  try {
    thread_sched_params_cfg.lookupValue(
        SPGWU_CONFIG_STRING_THREAD_RD_CPU_ID, cfg.cpu_id);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }
  try {
    std::string thread_rd_sched_policy;
    thread_sched_params_cfg.lookupValue(
        SPGWU_CONFIG_STRING_THREAD_RD_SCHED_POLICY, thread_rd_sched_policy);
    util::trim(thread_rd_sched_policy);
    if (boost::iequals(thread_rd_sched_policy, "SCHED_OTHER")) {
      cfg.sched_policy = SCHED_OTHER;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_IDLE")) {
      cfg.sched_policy = SCHED_IDLE;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_BATCH")) {
      cfg.sched_policy = SCHED_BATCH;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_FIFO")) {
      cfg.sched_policy = SCHED_FIFO;
    } else if (boost::iequals(thread_rd_sched_policy, "SCHED_RR")) {
      cfg.sched_policy = SCHED_RR;
    } else {
      Logger::spgwu_app().error(
          "thread_rd_sched_policy: %s, unknown in config file",
          thread_rd_sched_policy.c_str());
      return RETURNerror;
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    thread_sched_params_cfg.lookupValue(
        SPGWU_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY, cfg.sched_priority);
    if ((cfg.sched_priority > 99) || (cfg.sched_priority < 1)) {
      Logger::spgwu_app().error(
          "thread_rd_sched_priority: %d, must be in interval [1..99] in config "
          "file",
          cfg.sched_priority);
      return RETURNerror;
    }
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  return RETURNok;
}
//------------------------------------------------------------------------------
int spgwu_config::load_itti(const Setting& itti_cfg, itti_cfg_t& cfg) {
  try {
    const Setting& sched_params_cfg =
        itti_cfg[SPGWU_CONFIG_STRING_ITTI_TIMER_SCHED_PARAMS];
    load_thread_sched_params(sched_params_cfg, cfg.itti_timer_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& s1u_sched_params_cfg =
        itti_cfg[SPGWU_CONFIG_STRING_S1U_SCHED_PARAMS];
    load_thread_sched_params(s1u_sched_params_cfg, cfg.s1u_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& sx_sched_params_cfg =
        itti_cfg[SPGWU_CONFIG_STRING_SX_SCHED_PARAMS];
    load_thread_sched_params(sx_sched_params_cfg, cfg.sx_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& spgwu_app_sched_params_cfg =
        itti_cfg[SPGWU_CONFIG_STRING_SX_SCHED_PARAMS];
    load_thread_sched_params(
        spgwu_app_sched_params_cfg, cfg.spgwu_app_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& async_cmd_sched_params_cfg =
        itti_cfg[SPGWU_CONFIG_STRING_ASYNC_CMD_SCHED_PARAMS];
    load_thread_sched_params(
        async_cmd_sched_params_cfg, cfg.async_cmd_sched_params);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  return RETURNok;
}

//------------------------------------------------------------------------------
int spgwu_config::load_interface(const Setting& if_cfg, interface_cfg_t& cfg) {
  if_cfg.lookupValue(SPGWU_CONFIG_STRING_INTERFACE_NAME, cfg.if_name);
  util::trim(cfg.if_name);
  if (not boost::iequals(cfg.if_name, "none")) {
    std::string address = {};
    if_cfg.lookupValue(SPGWU_CONFIG_STRING_IPV4_ADDRESS, address);
    util::trim(address);
    if (boost::iequals(address, "read")) {
      if (get_inet_addr_infos_from_iface(
              cfg.if_name, cfg.addr4, cfg.network4, cfg.mtu)) {
        Logger::spgwu_app().error(
            "Could not read %s network interface configuration", cfg.if_name);
        return RETURNerror;
      }
    } else {
      std::vector<std::string> words;
      boost::split(
          words, address, boost::is_any_of("/"), boost::token_compress_on);
      if (words.size() != 2) {
        Logger::spgwu_app().error(
            "Bad value " SPGWU_CONFIG_STRING_IPV4_ADDRESS
            " = %s in config file",
            address.c_str());
        return RETURNerror;
      }
      unsigned char buf_in_addr[sizeof(struct in6_addr)];  // you never know...
      if (inet_pton(AF_INET, util::trim(words.at(0)).c_str(), buf_in_addr) ==
          1) {
        memcpy(&cfg.addr4, buf_in_addr, sizeof(struct in_addr));
      } else {
        Logger::spgwu_app().error(
            "In conversion: Bad value " SPGWU_CONFIG_STRING_IPV4_ADDRESS
            " = %s in config file",
            util::trim(words.at(0)).c_str());
        return RETURNerror;
      }
      cfg.network4.s_addr = htons(
          ntohs(cfg.addr4.s_addr) &
          0xFFFFFFFF << (32 - std::stoi(util::trim(words.at(1)))));
    }
    if_cfg.lookupValue(SPGWU_CONFIG_STRING_PORT, cfg.port);
    try {
      const Setting& sched_params_cfg =
          if_cfg[SPGWU_CONFIG_STRING_SCHED_PARAMS];
      load_thread_sched_params(sched_params_cfg, cfg.thread_rd_sched_params);
    } catch (const SettingNotFoundException& nfex) {
      Logger::spgwu_app().info(
          "%s : %s, using defaults", nfex.what(), nfex.getPath());
    }
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int spgwu_config::load(const string& config_file) {
  Config cfg;
  unsigned char buf_in_addr[sizeof(struct in_addr) + 1];
  unsigned char buf_in6_addr[sizeof(struct in6_addr) + 1];

  // Read the file. If there is an error, report it and exit.
  try {
    cfg.readFile(config_file.c_str());
  } catch (const FileIOException& fioex) {
    Logger::spgwu_app().error(
        "I/O error while reading file %s - %s", config_file.c_str(),
        fioex.what());
    throw;
  } catch (const ParseException& pex) {
    Logger::spgwu_app().error(
        "Parse error at %s:%d - %s", pex.getFile(), pex.getLine(),
        pex.getError());
    throw;
  }

  const Setting& root = cfg.getRoot();

  try {
    const Setting& spgwu_cfg = root[SPGWU_CONFIG_STRING_SPGWU_CONFIG];
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().error("%s : %s", nfex.what(), nfex.getPath());
    return RETURNerror;
  }

  const Setting& spgwu_cfg = root[SPGWU_CONFIG_STRING_SPGWU_CONFIG];

  try {
    spgwu_cfg.lookupValue(SPGWU_CONFIG_STRING_INSTANCE, instance);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    spgwu_cfg.lookupValue(SPGWU_CONFIG_STRING_PID_DIRECTORY, pid_dir);
    util::trim(pid_dir);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& itti_cfg = spgwu_cfg[SPGWU_CONFIG_STRING_ITTI_TASKS];
    load_itti(itti_cfg, itti);
  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().info(
        "%s : %s, using defaults", nfex.what(), nfex.getPath());
  }

  try {
    const Setting& nw_if_cfg = spgwu_cfg[SPGWU_CONFIG_STRING_INTERFACES];

    const Setting& s1_up_cfg =
        nw_if_cfg[SPGWU_CONFIG_STRING_INTERFACE_S1U_S12_S4_UP];
    load_interface(s1_up_cfg, s1_up);

    const Setting& sx_cfg = nw_if_cfg[SPGWU_CONFIG_STRING_INTERFACE_SX];
    load_interface(sx_cfg, sx);

    const Setting& sgi_cfg = nw_if_cfg[SPGWU_CONFIG_STRING_INTERFACE_SGI];
    load_interface(sgi_cfg, sgi);

    if ((boost::iequals(sgi.if_name, "none")) ||
        (boost::iequals(sgi.if_name, "default_gateway"))) {
      if (util::get_gateway_and_iface(gateway, sgi.if_name)) {
        std::string copy = sgi.if_name;
        if (get_inet_addr_infos_from_iface(
                sgi.if_name, sgi.addr4, sgi.network4, sgi.mtu)) {
          Logger::spgwu_app().error(
              "Could not read %s network interface configuration from system",
              sgi.if_name);
          return RETURNerror;
        }
      } else {
        Logger::spgwu_app().error(
            "Could not find SGi interface and SGi gateway");
        return RETURNerror;
      }
    }

    const Setting& pdn_network_list_cfg =
        spgwu_cfg[SPGWU_CONFIG_STRING_PDN_NETWORK_LIST];
    int count = pdn_network_list_cfg.getLength();
    for (int i = 0; i < count; i++) {
      pdn_cfg_t pdn_cfg              = {};
      const Setting& pdn_network_cfg = pdn_network_list_cfg[i];

      string network_ipv4 = {};
      if (pdn_network_cfg.lookupValue(
              SPGWU_CONFIG_STRING_NETWORK_IPV4, network_ipv4)) {
        std::vector<std::string> ips = {};
        boost::split(
            ips, network_ipv4,
            boost::is_any_of(SPGWU_CONFIG_STRING_ADDRESS_PREFIX_DELIMITER),
            boost::token_compress_on);
        if (ips.size() != 2) {
          Logger::spgwu_app().error(
              "Bad value %s : %s in config file %s",
              SPGWU_CONFIG_STRING_ADDRESS_PREFIX_DELIMITER,
              network_ipv4.c_str(), config_file.c_str());
          throw(
              "Bad value %s : %s in config file %s",
              SPGWU_CONFIG_STRING_ADDRESS_PREFIX_DELIMITER,
              network_ipv4.c_str(), config_file.c_str());
        }

        if (inet_pton(AF_INET, util::trim(ips.at(0)).c_str(), buf_in_addr) ==
            1) {
          memcpy(&pdn_cfg.network_ipv4, buf_in_addr, sizeof(struct in_addr));
        } else {
          Logger::spgwu_app().error(
              "CONFIG: BAD IPV4 NETWORK ADDRESS "
              "in " SPGWU_CONFIG_STRING_PDN_NETWORK_LIST " PDN item %d",
              i);
          throw(
              "CONFIG: BAD NETWORK ADDRESS "
              "in " SPGWU_CONFIG_STRING_PDN_NETWORK_LIST);
        }
        pdn_cfg.prefix_ipv4 = std::stoul(ips.at(1), nullptr, 0);
      }

      string network_ipv6 = {};
      if (pdn_network_cfg.lookupValue(
              SPGWU_CONFIG_STRING_NETWORK_IPV6, network_ipv6)) {
        std::vector<std::string> ips = {};
        boost::split(
            ips, network_ipv6,
            boost::is_any_of(SPGWU_CONFIG_STRING_ADDRESS_PREFIX_DELIMITER),
            boost::token_compress_on);
        if (ips.size() != 2) {
          Logger::spgwu_app().error(
              "Bad value %s : %s in config file %s",
              SPGWU_CONFIG_STRING_ADDRESS_PREFIX_DELIMITER,
              network_ipv6.c_str(), config_file.c_str());
          throw(
              "Bad value %s : %s in config file %s",
              SPGWU_CONFIG_STRING_ADDRESS_PREFIX_DELIMITER,
              network_ipv6.c_str(), config_file.c_str());
        }

        if (inet_pton(AF_INET6, util::trim(ips.at(0)).c_str(), buf_in6_addr) ==
            1) {
          memcpy(&pdn_cfg.network_ipv6, buf_in6_addr, sizeof(struct in6_addr));
        } else {
          Logger::spgwu_app().error(
              "CONFIG: BAD IPV6 NETWORK ADDRESS "
              "in " SPGWU_CONFIG_STRING_PDN_NETWORK_LIST " PDN item %d",
              i);
          throw(
              "CONFIG: BAD IPV6 NETWORK ADDRESS "
              "in " SPGWU_CONFIG_STRING_PDN_NETWORK_LIST);
        }
        pdn_cfg.prefix_ipv6 = std::stoul(ips.at(1), nullptr, 0);
        Logger::spgwu_app().info(
            "  %s......: %s/%d", ips.at(0).c_str(),
            conv::toString(pdn_cfg.network_ipv6).c_str(), pdn_cfg.prefix_ipv6);
      }
      pdn_cfg.snat        = false;
      std::string astring = {};
      if (pdn_network_cfg.lookupValue(SPGWU_CONFIG_STRING_SNAT, astring)) {
        if (boost::iequals(astring, "yes")) {
          pdn_cfg.snat = true;
        }
      }
      pdns.push_back(pdn_cfg);
    }

    const Setting& spgwc_list_cfg = spgwu_cfg[SPGWU_CONFIG_STRING_SPGWC_LIST];
    count                         = spgwc_list_cfg.getLength();
    for (int i = 0; i < count; i++) {
      pdn_cfg_t pdn_cfg        = {};
      const Setting& spgwc_cfg = spgwc_list_cfg[i];

      string address = {};
      if (spgwc_cfg.lookupValue(SPGWU_CONFIG_STRING_IPV4_ADDRESS, address)) {
        pfcp::node_id_t n = {};
        n.node_id_type    = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;  // actually
        if (inet_pton(AF_INET, util::trim(address).c_str(), buf_in_addr) == 1) {
          memcpy(&n.u1.ipv4_address, buf_in_addr, sizeof(struct in_addr));
        } else {
          Logger::spgwu_app().error(
              "CONFIG: BAD IPV4 ADDRESS in " SPGWU_CONFIG_STRING_SPGWC_LIST
              " item %d",
              i);
          throw("CONFIG: BAD ADDRESS in " SPGWU_CONFIG_STRING_SPGWC_LIST);
        }
        spgwcs.push_back(n);
      } else {
        // TODO IPV6_ADDRESS, FQDN
        throw(
            "Bad value in section %s : item no %d in config file %s",
            SPGWU_CONFIG_STRING_SPGWC_LIST, i, config_file.c_str());
      }
    }

    // NRF
    const Setting& nrf_cfg = spgwu_cfg[SPGWU_CONFIG_STRING_NRF];
    struct in_addr nrf_ipv4_addr;
    unsigned int nrf_port = 0;
    std::string nrf_api_version;
    string nrf_address = {};
    nrf_cfg.lookupValue(SPGWU_CONFIG_STRING_NRF_IPV4_ADDRESS, nrf_address);
    IPV4_STR_ADDR_TO_INADDR(
        util::trim(nrf_address).c_str(), nrf_ipv4_addr,
        "BAD IPv4 ADDRESS FORMAT FOR NRF !");
    nrf_addr.ipv4_addr = nrf_ipv4_addr;
    if (!(nrf_cfg.lookupValue(SPGWU_CONFIG_STRING_NRF_PORT, nrf_port))) {
      Logger::spgwu_app().error(SPGWU_CONFIG_STRING_NRF_PORT "failed");
      throw(SPGWU_CONFIG_STRING_NRF_PORT "failed");
    }
    nrf_addr.port = nrf_port;

    if (!(nrf_cfg.lookupValue(
            SPGWU_CONFIG_STRING_API_VERSION, nrf_api_version))) {
      Logger::spgwu_app().error(SPGWU_CONFIG_STRING_API_VERSION "failed");
      throw(SPGWU_CONFIG_STRING_API_VERSION "failed");
    }
    nrf_addr.api_version = nrf_api_version;

    // Support features
    const Setting& support_features =
        spgwu_cfg[SPGWU_CONFIG_STRING_SUPPORT_FEATURES];
    string opt;
    support_features.lookupValue(
        SPGWU_CONFIG_STRING_SUPPORT_FEATURES_REGISTER_NRF, opt);
    if (boost::iequals(opt, "yes")) {
      register_nrf = true;
    } else {
      register_nrf = false;
    }

    // UPF info
    const Setting& upf_info_cfg = spgwu_cfg[SPGWU_CONFIG_STRING_UPF_INFO];

    count = upf_info_cfg.getLength();
    for (int i = 0; i < count; i++) {
      const Setting& upf_info_item_cfg = upf_info_cfg[i];

      unsigned int nssai_sst = 0;
      string nssai_sd        = {};
      string dnn             = {};

      upf_info_item_cfg.lookupValue(SPGWU_CONFIG_STRING_NSSAI_SST, nssai_sst);
      upf_info_item_cfg.lookupValue(SPGWU_CONFIG_STRING_NSSAI_SD, nssai_sd);
      upf_info_item_cfg.lookupValue(SPGWU_CONFIG_STRING_DNN, dnn);
      snssai_t snssai = {};
      snssai.sST      = nssai_sst;
      snssai.sD       = nssai_sd;
      upf_info.add_snssai(snssai, dnn);
    }

  } catch (const SettingNotFoundException& nfex) {
    Logger::spgwu_app().error("%s : %s", nfex.what(), nfex.getPath());
    return RETURNerror;
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
void spgwu_config::display() {
  Logger::spgwu_app().info(
      "==== EURECOM %s v%s ====", PACKAGE_NAME, PACKAGE_VERSION);
  Logger::spgwu_app().info("Configuration:");
  Logger::spgwu_app().info("- Instance ..............: %d", instance);
  Logger::spgwu_app().info("- PID dir ...............: %s", pid_dir.c_str());
  Logger::spgwu_app().info("- ITTI tasks:");
  Logger::spgwu_app().info("    ITTI Timer task:");
  Logger::spgwu_app().info(
      "      CPU ID .........: %d", itti.itti_timer_sched_params.cpu_id);
  Logger::spgwu_app().info(
      "      sched policy....: %d", itti.itti_timer_sched_params.sched_policy);
  Logger::spgwu_app().info(
      "      sched priority..: %d",
      itti.itti_timer_sched_params.sched_priority);
  Logger::spgwu_app().info("    SPGWU-S1U task:");
  Logger::spgwu_app().info(
      "      CPU ID .........: %d", itti.s1u_sched_params.cpu_id);
  Logger::spgwu_app().info(
      "      sched policy....: %d", itti.s1u_sched_params.sched_policy);
  Logger::spgwu_app().info(
      "      sched priority..: %d", itti.s1u_sched_params.sched_priority);
  Logger::spgwu_app().info("    SPGWU-SX task:");
  Logger::spgwu_app().info(
      "      CPU ID .........: %d", itti.sx_sched_params.cpu_id);
  Logger::spgwu_app().info(
      "      sched policy....: %d", itti.sx_sched_params.sched_policy);
  Logger::spgwu_app().info(
      "      sched priority..: %d", itti.sx_sched_params.sched_priority);
  Logger::spgwu_app().info("    SPGWU_APP task:");
  Logger::spgwu_app().info(
      "      CPU ID .........: %d", itti.spgwu_app_sched_params.cpu_id);
  Logger::spgwu_app().info(
      "      sched policy....: %d", itti.spgwu_app_sched_params.sched_policy);
  Logger::spgwu_app().info(
      "      sched priority..: %d", itti.spgwu_app_sched_params.sched_priority);
  Logger::spgwu_app().info("    ASYNC_SHELL_CMD task:");
  Logger::spgwu_app().info(
      "      CPU ID .........: %d", itti.async_cmd_sched_params.cpu_id);
  Logger::spgwu_app().info(
      "      sched policy....: %d", itti.async_cmd_sched_params.sched_policy);
  Logger::spgwu_app().info(
      "      sched priority..: %d", itti.async_cmd_sched_params.sched_priority);
  Logger::spgwu_app().info("- S1u_S12_S4:");
  Logger::spgwu_app().info("    iface ............: %s", s1_up.if_name.c_str());
  Logger::spgwu_app().info(
      "    ipv4.addr ........: %s", inet_ntoa(s1_up.addr4));
  Logger::spgwu_app().info(
      "    ipv4.mask ........: %s", inet_ntoa(s1_up.network4));
  Logger::spgwu_app().info("    mtu ..............: %d", s1_up.mtu);
  Logger::spgwu_app().info("    port .............: %d", s1_up.port);
  Logger::spgwu_app().info("    Reader thread:");
  Logger::spgwu_app().info(
      "      CPU ID .........: %d", s1_up.thread_rd_sched_params.cpu_id);
  Logger::spgwu_app().info(
      "      sched policy....: %d", s1_up.thread_rd_sched_params.sched_policy);
  Logger::spgwu_app().info(
      "      sched priority..: %d",
      s1_up.thread_rd_sched_params.sched_priority);
  Logger::spgwu_app().info("- SXA-SXB:");
  Logger::spgwu_app().info("    iface ............: %s", sx.if_name.c_str());
  Logger::spgwu_app().info("    ipv4.addr ........: %s", inet_ntoa(sx.addr4));
  Logger::spgwu_app().info(
      "    ipv4.mask ........: %s", inet_ntoa(sx.network4));
  Logger::spgwu_app().info("    mtu ..............: %d", sx.mtu);
  Logger::spgwu_app().info("    port .............: %u", sx.port);
  Logger::spgwu_app().info("    Reader thread:");
  Logger::spgwu_app().info(
      "      CPU ID .........: %d (TODO)", sx.thread_rd_sched_params.cpu_id);
  Logger::spgwu_app().info(
      "      sched policy....: %d (TODO)",
      sx.thread_rd_sched_params.sched_policy);
  Logger::spgwu_app().info(
      "      sched priority..: %d (TODO)",
      sx.thread_rd_sched_params.sched_priority);
  Logger::spgwu_app().info("- SGi:");
  Logger::spgwu_app().info("    iface ............: %s", sgi.if_name.c_str());
  Logger::spgwu_app().info("    ipv4.addr ........: %s", inet_ntoa(sgi.addr4));
  Logger::spgwu_app().info(
      "    ipv4.mask ........: %s", inet_ntoa(sgi.network4));
  Logger::spgwu_app().info("    mtu ..............: %d", sgi.mtu);
  Logger::spgwu_app().info("    gateway ..........: %s", gateway.c_str());
  Logger::spgwu_app().info("    Reader thread:");
  Logger::spgwu_app().info(
      "      CPU ID .........: %d", sgi.thread_rd_sched_params.cpu_id);
  Logger::spgwu_app().info(
      "      sched policy....: %d", sgi.thread_rd_sched_params.sched_policy);
  Logger::spgwu_app().info(
      "      sched priority..: %d", sgi.thread_rd_sched_params.sched_priority);
  Logger::spgwu_app().info("- PDN networks:");
  int i = 1;
  for (auto it : pdns) {
    Logger::spgwu_app().info(
        "    PDN %d ............: snat %s", i, (it.snat) ? "yes" : "no");
    if (it.prefix_ipv4) {
      Logger::spgwu_app().info(
          "       NW .............: %s/%d",
          conv::toString(it.network_ipv4).c_str(), it.prefix_ipv4);
    }
    if (it.prefix_ipv6) {
      Logger::spgwu_app().info(
          "       NW .............: %s/%d",
          conv::toString(it.network_ipv6).c_str(), it.prefix_ipv6);
    }
    i++;
  }
  if (register_nrf) {
    if (upf_info.snssai_upf_info_list.size() > 0) {
      Logger::spgwu_app().debug("- UPF Info:");
    }
    for (auto s : upf_info.snssai_upf_info_list) {
      Logger::spgwu_app().debug("\tParameters supported by the UPF:");
      Logger::spgwu_app().debug(
          "\t\tSNSSAI (SST %d, SD %s)", s.snssai.sST, s.snssai.sD.c_str());
      for (auto d : s.dnn_upf_info_list) {
        Logger::spgwu_app().debug("\t\tDNN %s", d.dnn.c_str());
      }
    }
  }
}
