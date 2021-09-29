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

/*! \file spgwu_config.hpp
 * \brief
 * \author Lionel Gauthier
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_SPGWU_CONFIG_HPP_SEEN
#define FILE_SPGWU_CONFIG_HPP_SEEN

#include "3gpp_29.244.h"
#include "3gpp_29.510.h"
#include "gtpv1u.hpp"
#include "pfcp.hpp"
#include "thread_sched.hpp"
#include <libconfig.h++>
#include <mutex>
#include <netinet/in.h>
#include <stdint.h>
#include <stdbool.h>
#include <string>

namespace spgwu {

#define SPGWU_CONFIG_STRING_SPGWU_CONFIG "SPGW-U"
#define SPGWU_CONFIG_STRING_PID_DIRECTORY "PID_DIRECTORY"
#define SPGWU_CONFIG_STRING_INSTANCE "INSTANCE"
#define SPGWU_CONFIG_STRING_INTERFACES "INTERFACES"
#define SPGWU_CONFIG_STRING_INTERFACE_NAME "INTERFACE_NAME"
#define SPGWU_CONFIG_STRING_IPV4_ADDRESS "IPV4_ADDRESS"
#define SPGWU_CONFIG_STRING_PORT "PORT"
#define SPGWU_CONFIG_STRING_SCHED_PARAMS "SCHED_PARAMS"
#define SPGWU_CONFIG_STRING_THREAD_RD_CPU_ID "CPU_ID"
#define SPGWU_CONFIG_STRING_THREAD_RD_SCHED_POLICY "SCHED_POLICY"
#define SPGWU_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY "SCHED_PRIORITY"
#define SPGWU_CONFIG_STRING_INTERFACE_SGI "SGI"
#define SPGWU_CONFIG_STRING_INTERFACE_SX "SX"
#define SPGWU_CONFIG_STRING_INTERFACE_S1U_S12_S4_UP "S1U_S12_S4_UP"
#define SPGWU_CONFIG_STRING_PDN_NETWORK_LIST "PDN_NETWORK_LIST"
#define SPGWU_CONFIG_STRING_NETWORK_IPV4 "NETWORK_IPV4"
#define SPGWU_CONFIG_STRING_NETWORK_IPV6 "NETWORK_IPV6"
#define SPGWU_CONFIG_STRING_ADDRESS_PREFIX_DELIMITER "/"
#define SPGWU_CONFIG_STRING_SNAT "SNAT"
#define SPGWU_CONFIG_STRING_MAX_PFCP_SESSIONS "MAX_PFCP_SESSIONS"
#define SPGWU_CONFIG_STRING_SPGWC_LIST "SPGW-C_LIST"
#define SPGWU_CONFIG_STRING_NRF "NRF"
#define SPGWU_CONFIG_STRING_NRF_IPV4_ADDRESS "IPV4_ADDRESS"
#define SPGWU_CONFIG_STRING_NRF_PORT "PORT"
#define SPGWU_CONFIG_STRING_API_VERSION "API_VERSION"
#define SPGWU_CONFIG_STRING_ITTI_TASKS "ITTI_TASKS"
#define SPGWU_CONFIG_STRING_ITTI_TIMER_SCHED_PARAMS "ITTI_TIMER_SCHED_PARAMS"
#define SPGWU_CONFIG_STRING_S1U_SCHED_PARAMS "S1U_SCHED_PARAMS"
#define SPGWU_CONFIG_STRING_SX_SCHED_PARAMS "SX_SCHED_PARAMS"
#define SPGWU_CONFIG_STRING_SPGWU_APP_SCHED_PARAMS "SPGWU_APP_SCHED_PARAMS"
#define SPGWU_CONFIG_STRING_ASYNC_CMD_SCHED_PARAMS "ASYNC_CMD_SCHED_PARAMS"

#define SPGWU_CONFIG_STRING_SUPPORT_FEATURES "SUPPORT_FEATURES"
#define SPGWU_CONFIG_STRING_SUPPORT_FEATURES_REGISTER_NRF "REGISTER_NRF"

#define SPGWU_CONFIG_STRING_UPF_INFO "UPF_INFO"
#define SPGWU_CONFIG_STRING_NSSAI_SST "NSSAI_SST"
#define SPGWU_CONFIG_STRING_NSSAI_SD "NSSAI_SD"
#define SPGWU_CONFIG_STRING_DNN "DNN"

#define SPGW_ABORT_ON_ERROR true
#define SPGW_WARN_ON_ERROR false

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  unsigned int mtu;
  unsigned int port;
  util::thread_sched_params thread_rd_sched_params;
} interface_cfg_t;

typedef struct pdn_cfg_s {
  struct in_addr network_ipv4;
  int prefix_ipv4;
  struct in6_addr network_ipv6;
  int prefix_ipv6;
  bool snat;
} pdn_cfg_t;

typedef struct itti_cfg_s {
  util::thread_sched_params itti_timer_sched_params;
  util::thread_sched_params s1u_sched_params;
  util::thread_sched_params sx_sched_params;
  util::thread_sched_params spgwu_app_sched_params;
  util::thread_sched_params async_cmd_sched_params;
} itti_cfg_t;

class spgwu_config {
 private:
  int load_itti(const libconfig::Setting& itti_cfg, itti_cfg_t& cfg);
  int load_interface(const libconfig::Setting& if_cfg, interface_cfg_t& cfg);
  int load_thread_sched_params(
      const libconfig::Setting& thread_sched_params_cfg,
      util::thread_sched_params& cfg);

 public:
  /* Reader/writer lock for this configuration */
  std::mutex m_rw_lock;
  std::string pid_dir;
  unsigned int instance;
  interface_cfg_t s1_up;
  interface_cfg_t sgi;
  interface_cfg_t sx;
  itti_cfg_t itti;

  std::string gateway;

  uint32_t max_pfcp_sessions;

  std::vector<pdn_cfg_t> pdns;
  std::vector<pfcp::node_id_t> spgwcs;

  bool register_nrf;
  upf_info_t upf_info;

  struct {
    struct in_addr ipv4_addr;
    unsigned int port;
    std::string api_version;
  } nrf_addr;

  spgwu_config()
      : m_rw_lock(),
        pid_dir(),
        instance(0),
        s1_up(),
        sgi(),
        gateway(),
        sx(),
        itti(),
        pdns(),
        spgwcs(),
        max_pfcp_sessions(100) {
    itti.itti_timer_sched_params.sched_priority = 85;
    itti.s1u_sched_params.sched_priority        = 84;
    itti.sx_sched_params.sched_priority         = 84;
    itti.spgwu_app_sched_params.sched_priority  = 84;
    itti.async_cmd_sched_params.sched_priority  = 84;

    s1_up.thread_rd_sched_params.sched_priority = 98;
    s1_up.port                                  = gtpv1u::default_port;

    sgi.thread_rd_sched_params.sched_priority = 98;

    sx.thread_rd_sched_params.sched_priority = 95;
    sx.port                                  = pfcp::default_port;

    nrf_addr.ipv4_addr.s_addr = INADDR_ANY;
    nrf_addr.port             = 80;
    nrf_addr.api_version      = "v1";

    register_nrf = false;
    upf_info     = {};
  };
  void lock() { m_rw_lock.lock(); };
  void unlock() { m_rw_lock.unlock(); };
  int load(const std::string& config_file);
  int execute();
  void display();
  int get_pfcp_node_id(pfcp::node_id_t& node_id);
  int get_pfcp_fseid(pfcp::fseid_t& fseid);
};
}  // namespace spgwu

#endif /* FILE_SPGWU_CONFIG_HPP_SEEN */
