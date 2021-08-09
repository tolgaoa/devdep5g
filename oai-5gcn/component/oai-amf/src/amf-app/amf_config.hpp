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

/*! \file amf_config.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _AMF_CONFIG_H_
#define _AMF_CONFIG_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <libconfig.h++>
#include <string>
#include <vector>

#include "amf_config.hpp"
#include "thread_sched.hpp"

#define AMF_CONFIG_STRING_AMF_CONFIG "AMF"
#define AMF_CONFIG_STRING_PID_DIRECTORY "PID_DIRECTORY"
#define AMF_CONFIG_STRING_INSTANCE_ID "INSTANCE_ID"
#define AMF_CONFIG_STRING_STATISTICS_TIMER_INTERVAL "STATISTICS_TIMER_INTERVAL"
#define AMF_CONFIG_STRING_INTERFACES "INTERFACES"
#define AMF_CONFIG_STRING_INTERFACE_NGAP_AMF "NGAP_AMF"

#define AMF_CONFIG_STRING_INTERFACE_NAME "INTERFACE_NAME"
#define AMF_CONFIG_STRING_IPV4_ADDRESS "IPV4_ADDRESS"
#define AMF_CONFIG_STRING_PORT "PORT"
#define AMF_CONFIG_STRING_PPID "PPID"

#define AMF_CONFIG_STRING_INTERFACE_N11 "N11"
#define AMF_CONFIG_STRING_SMF_INSTANCES_POOL "SMF_INSTANCES_POOL"
#define AMF_CONFIG_STRING_SMF_INSTANCE_ID "SMF_INSTANCE_ID"
#define AMF_CONFIG_STRING_SMF_INSTANCE_PORT "PORT"
#define AMF_CONFIG_STRING_SMF_INSTANCE_VERSION "VERSION"
#define AMF_CONFIG_STRING_SMF_INSTANCE_SELECTED "SELECTED"

#define AMF_CONFIG_STRING_NRF "NRF"
#define AMF_CONFIG_STRING_NRF_IPV4_ADDRESS "IPV4_ADDRESS"
#define AMF_CONFIG_STRING_NRF_PORT "PORT"
#define AMF_CONFIG_STRING_API_VERSION "API_VERSION"

#define AMF_CONFIG_STRING_AUSF "AUSF"

#define AMF_CONFIG_STRING_SCHED_PARAMS "SCHED_PARAMS"
#define AMF_CONFIG_STRING_THREAD_RD_CPU_ID "CPU_ID"
#define AMF_CONFIG_STRING_THREAD_RD_SCHED_POLICY "SCHED_POLICY"
#define AMF_CONFIG_STRING_THREAD_RD_SCHED_PRIORITY "SCHED_PRIORITY"

#define AMF_CONFIG_STRING_AMF_NAME "AMF_NAME"
#define AMF_CONFIG_STRING_GUAMI "GUAMI"
#define AMF_CONFIG_STRING_SERVED_GUAMI_LIST "SERVED_GUAMI_LIST"
#define AMF_CONFIG_STRING_TAC "TAC"
#define AMF_CONFIG_STRING_MCC "MCC"
#define AMF_CONFIG_STRING_MNC "MNC"
#define AMF_CONFIG_STRING_RegionID "RegionID"
#define AMF_CONFIG_STRING_AMFSetID "AMFSetID"
#define AMF_CONFIG_STRING_AMFPointer "AMFPointer"
#define AMF_CONFIG_STRING_RELATIVE_AMF_CAPACITY "RELATIVE_CAPACITY"
#define AMF_CONFIG_STRING_PLMN_SUPPORT_LIST "PLMN_SUPPORT_LIST"
#define AMF_CONFIG_STRING_SLICE_SUPPORT_LIST "SLICE_SUPPORT_LIST"
#define AMF_CONFIG_STRING_SST "SST"
#define AMF_CONFIG_STRING_SD "SD"
#define AMF_CONFIG_STRING_CORE_CONFIGURATION "CORE_CONFIGURATION"
#define AMF_CONFIG_STRING_EMERGENCY_SUPPORT "EMERGENCY_SUPPORT"
#define AMF_CONFIG_STRING_AUTHENTICATION "AUTHENTICATION"
#define AMF_CONFIG_STRING_AUTH_MYSQL_SERVER "MYSQL_server"
#define AMF_CONFIG_STRING_AUTH_MYSQL_USER "MYSQL_user"
#define AMF_CONFIG_STRING_AUTH_MYSQL_PASS "MYSQL_pass"
#define AMF_CONFIG_STRING_AUTH_MYSQL_DB "MYSQL_db"
#define AMF_CONFIG_STRING_AUTH_OPERATOR_KEY "OPERATOR_key"
#define AMF_CONFIG_STRING_AUTH_RANDOM "RANDOM"
#define AMF_CONFIG_STRING_NAS "NAS"
#define AMF_CONFIG_STRING_NAS_SUPPORTED_INTEGRITY_ALGORITHM_LIST               \
  "ORDERED_SUPPORTED_INTEGRITY_ALGORITHM_LIST"
#define AMF_CONFIG_STRING_NAS_SUPPORTED_CIPHERING_ALGORITHM_LIST               \
  "ORDERED_SUPPORTED_CIPHERING_ALGORITHM_LIST"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES "SUPPORT_FEATURES"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_NF_REGISTRATION "NF_REGISTRATION"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_SMF_SELECTION "SMF_SELECTION"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_AUSF "EXTERNAL_AUSF"
#define AMF_CONFIG_STRING_SUPPORT_FEATURES_EXTERNAL_UDM "EXTERNAL_UDM"

using namespace libconfig;

namespace config {

typedef struct {
  std::string mysql_server;
  std::string mysql_user;
  std::string mysql_pass;
  std::string mysql_db;
  std::string operator_key;
  std::string random;
} auth_conf;

typedef struct interface_cfg_s {
  std::string if_name;
  struct in_addr addr4;
  struct in_addr network4;
  struct in6_addr addr6;
  unsigned int mtu;
  unsigned int port;
} interface_cfg_t;

typedef struct itti_cfg_s {
  util::thread_sched_params itti_timer_sched_params;
  util::thread_sched_params sx_sched_params;
  util::thread_sched_params s5s8_sched_params;
  util::thread_sched_params pgw_app_sched_params;
  util::thread_sched_params async_cmd_sched_params;
} itti_cfg_t;

typedef struct guami_s {
  std::string mcc;
  std::string mnc;
  std::string regionID;
  std::string AmfSetID;
  std::string AmfPointer;
} guami_t;

typedef struct slice_s {
  std::string sST;
  std::string sD;
} slice_t;

typedef struct plmn_support_item_s {
  std::string mcc;
  std::string mnc;
  uint32_t tac;
  std::vector<slice_t> slice_list;
} plmn_item_t;

typedef struct {
  uint8_t prefered_integrity_algorithm[8];
  uint8_t prefered_ciphering_algorithm[8];
} nas_conf_t;

typedef struct {
  int id;
  std::string ipv4;
  std::string port;
  std::string version;
  bool selected;
} smf_inst_t;

class amf_config {
 public:
  amf_config();
  ~amf_config();
  int load(const std::string& config_file);
  int load_interface(const Setting& if_cfg, interface_cfg_t& cfg);

  void display();
  unsigned int instance;
  std::string pid_dir;
  interface_cfg_t n2;
  interface_cfg_t n11;
  itti_cfg_t itti;
  std::string sbi_api_version;
  unsigned int statistics_interval;
  std::string AMF_Name;
  guami_t guami;
  std::vector<guami_t> guami_list;
  unsigned int relativeAMFCapacity;
  std::vector<plmn_item_t> plmn_list;
  std::string is_emergency_support;
  auth_conf auth_para;
  nas_conf_t nas_cfg;
  std::vector<smf_inst_t> smf_pool;
  bool enable_nf_registration;
  bool enable_smf_selection;
  bool enable_external_ausf;
  bool enable_external_udm;

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
};

}  // namespace config

#endif
