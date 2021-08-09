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

/*! \file nrf_profile.hpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#ifndef FILE_NRF_PROFILE_HPP_SEEN
#define FILE_NRF_PROFILE_HPP_SEEN

#include <arpa/inet.h>
#include <netinet/in.h>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <shared_mutex>
#include <utility>
#include <vector>

#include "3gpp_29.510.h"
#include "logger.hpp"
#include "nrf.h"
#include "nrf_event.hpp"

namespace oai {
namespace nrf {
namespace app {

using namespace std;

class nrf_profile : public std::enable_shared_from_this<nrf_profile> {
 public:
  nrf_profile(nrf_event& ev)
      : m_event_sub(ev),
        nf_type(NF_TYPE_UNKNOWN),
        heartBeat_timer(0),
        snssais(),
        ipv4_addresses(),
        priority(0),
        capacity(0),
        nf_services(),
        heartbeart_mutex() {
    nf_instance_name = "";
    nf_status        = "";
    json_data        = {};
    first_update     = true;
    is_updated       = false;
  }
  nrf_profile(nrf_event& ev, const nf_type_t type)
      : m_event_sub(ev),
        nf_type(type),
        heartBeat_timer(0),
        snssais(),
        ipv4_addresses(),
        priority(0),
        capacity(0),
        nf_services(),
        heartbeart_mutex() {
    nf_instance_name = "";
    nf_status        = "";
    json_data        = {};
    first_update     = true;
    is_updated       = false;
  }

  nrf_profile(nrf_event& ev, const std::string& id)
      : m_event_sub(ev),
        nf_instance_id(id),
        heartBeat_timer(0),
        snssais(),
        ipv4_addresses(),
        priority(0),
        capacity(0),
        nf_services(),
        nf_type(NF_TYPE_UNKNOWN),
        heartbeart_mutex() {
    nf_instance_name = "";
    nf_status        = "";
    json_data        = {};
    first_update     = true;
    is_updated       = false;
  }

  nrf_profile(nrf_profile& b) = delete;

  virtual ~nrf_profile() {
    Logger::nrf_app().debug("Delete NRF Profile instance...");
    if (task_connection.connected()) task_connection.disconnect();
    if (first_hb_connection.connected()) first_hb_connection.disconnect();
  }

  /*
   * Set NF instance ID
   * @param [const std::string &] instance_id: instance id
   * @return void
   */
  void set_nf_instance_id(const std::string& instance_id);

  /*
   * Get NF instance ID
   * @param [std::string &] instance_id: store instance id
   * @return void:
   */
  void get_nf_instance_id(std::string& instance_id) const;

  /*
   * Get NF instance ID
   * @param [std::string &] instance_id: store instance id
   * @return void:
   */
  std::string get_nf_instance_id() const;

  /*
   * Set NF instance name
   * @param [const std::string &] instance_name: instance name
   * @return void
   */
  void set_nf_instance_name(const std::string& instance_name);

  /*
   * Get NF instance ID
   * @param [std::string &] instance_name: store instance name
   * @return void:
   */
  void get_nf_instance_name(std::string& instance_name) const;

  /*
   * Get NF instance name
   * @param
   * @return [std::string] instance name
   */
  std::string get_nf_instance_name() const;

  /*
   * Set NF instance status
   * @param [const std::string &] status: instance status
   * @return void
   */
  void set_nf_status(const std::string& status);

  /*
   * Get NF instance status
   * @param [std::string &] status: store instance status
   * @return void:
   */
  void get_nf_status(std::string& status) const;

  /*
   * Get NF status
   * @param
   * @return [std::string] instance status
   */
  std::string get_nf_status() const;

  /*
   * Get NF type
   * @param
   * @return [std::string] nf type
   */
  nf_type_t get_nf_type() const;

  /*
   * Set NF type
   * @param [const nf_type_t &] type: nf type
   * @return void
   */
  void set_nf_type(const nf_type_t& type);

  /*
   * Set NF instance heartBeat_timer
   * @param [const std::string &] timer: heartBeat_timer
   * @return void
   */
  void set_nf_heartBeat_timer(const int32_t& timer);

  /*
   * Get NF instance heartBeat_timer
   * @param [std::string &] timer: store heartBeat_timer
   * @return void:
   */
  void get_nf_heartBeat_timer(int32_t& timer) const;

  /*
   * Get NF heartBeat_timer
   * @param void
   * @return heartBeat_timer
   */
  int32_t get_nf_heartBeat_timer() const;

  /*
   * Set NF instance priority
   * @param [const uint16_t] p: instance priority
   * @return void
   */
  void set_nf_priority(const uint16_t& p);

  /*
   * Get NF instance priority
   * @param [uint16_t] p: store instance priority
   * @return void:
   */
  void get_nf_priority(uint16_t& p) const;

  /*
   * Get NF instance priority
   * @param void
   * @return [uint16_t] instance priority
   */
  uint16_t get_nf_priority() const;

  /*
   * Set NF instance capacity
   * @param [const uint16_t] c: instance capacity
   * @return void
   */
  void set_nf_capacity(const uint16_t& c);

  /*
   * Get NF instance priority
   * @param [uint16_t ] c: store instance capacity
   * @return void:
   */
  void get_nf_capacity(uint16_t& c) const;

  /*
   * Get NF instance priority
   * @param void
   * @return [uint16_t ] instance capacity
   */
  uint16_t get_nf_capacity() const;

  /*
   * Set NF instance SNSSAIs
   * @param [std::vector<snssai_t> &] s: SNSSAIs
   * @return void
   */
  void set_nf_snssais(const std::vector<snssai_t>& s);

  /*
   * Add SNSSAI
   * @param [snssai_t &] s: SNSSAI
   * @return void
   */
  void add_snssai(const snssai_t& s);

  /*
   * Get NF instance SNSSAIs
   * @param [std::vector<snssai_t> &] s: store instance's SNSSAIs
   * @return void:
   */
  void get_nf_snssais(std::vector<snssai_t>& s) const;

  /*
   * Set NF instance ipv4_addresses
   * @param [std::vector<struct in_addr> &] a: ipv4_addresses
   * @return void
   */
  void set_nf_ipv4_addresses(const std::vector<struct in_addr>& a);

  /*
   * Add an IPv4 address to the list of addresses
   * @param [const struct in_addr &] a: ipv4_address
   * @return void
   */
  void add_nf_ipv4_addresses(const struct in_addr& a);

  /*
   * Get NF instance ipv4_addresses
   * @param [std::vector<struct in_addr> &] a: store instance's ipv4_addresses
   * @return void:
   */
  void get_nf_ipv4_addresses(std::vector<struct in_addr>& a) const;

  /*
   * Set json data
   * @param [const nlohmann::json &] data: Json data to be set
   * @return void
   */
  void set_json_data(const nlohmann::json& data);

  /*
   * Get json data
   * @param [nlohmann::json &] data: Store json data
   * @return void
   */
  void get_json_data(nlohmann::json& data) const;

  /*
   * Set NF instance services
   * @param [std::vector<nf_service_t> &] n: nf_service
   * @return void
   */
  void set_nf_services(const std::vector<nf_service_t>& n);

  /*
   * Add nf service
   * @param [snssai_t &] n: nf service
   * @return void
   */
  void add_nf_service(const nf_service_t& n);

  /*
   * Get NF services
   * @param [std::vector<snssai_t> &] n: store instance's nf services
   * @return void:
   */
  void get_nf_services(std::vector<nf_service_t>& n) const;

  /*
   * Print related-information for NF profile
   * @param void
   * @return void:
   */
  virtual void display();

  /*
   * Update a new value for a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  virtual bool replace_profile_info(
      const std::string& path, const std::string& value);

  /*
   * Add a new value for a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  virtual bool add_profile_info(
      const std::string& path, const std::string& value);

  /*
   * Remove value of a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  virtual bool remove_profile_info(const std::string& path);

  /*
   * Represent NF profile as json object
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  virtual void to_json(nlohmann::json& data) const;

  /*
   * Subscribe to the HBT timeout event (after receiving NF Update)
   * @param [uint64_t] ms: current time
   * @return void
   */
  void subscribe_heartbeat_timeout_nfupdate(uint64_t ms);

  /*
   * Subscribe to the HBT timeout event (after receiving NF Registration)
   * @param [uint64_t] ms: current time
   * @return void
   */
  void subscribe_heartbeat_timeout_nfregistration(uint64_t ms);

  /*
   * Handle heartbeart timeout event
   * @param [uint64_t] ms: current time
   * @return void
   */
  void handle_heartbeart_timeout(uint64_t ms);

  /*
   * Handle heartbeart timeout event after NF Registration
   * @param [uint64_t] ms: current time
   * @return void
   */
  void handle_heartbeart_timeout_nfregistration(uint64_t ms);

  /*
   * Handle heartbeart timeout event after NF Update
   * @param [uint64_t] ms: current time
   * @return void
   */
  void handle_heartbeart_timeout_nfupdate(uint64_t ms);

  /*
   * Unubscribe to HBT event (after NF Registration)
   * @param void
   * @return void
   */
  bool unsubscribe_heartbeat_timeout_nfregistration();

  /*
   * Unubscribe to HBT event (after NF Update)
   * @param void
   * @return void
   */
  bool unsubscribe_heartbeat_timeout_nfupdate();

  /*
   * Set status updated to true
   * @param void
   * @return void
   */
  void set_status_updated(bool status);

 protected:
  nrf_event& m_event_sub;
  bs2::connection
      task_connection;  // connection for the HBT timeout (after NF Update)
  bs2::connection first_hb_connection;  // connection for first HBT timeout
                                        // (after NF Registration)
  bool first_update;
  bool is_updated;
  mutable std::shared_mutex heartbeart_mutex;

  // From NFProfile (Section 6.1.6.2.2@3GPP TS 29.510 V16.0.0 (2019-06))
  std::string nf_instance_id;
  std::string nf_instance_name;
  nf_type_t nf_type;
  std::string nf_status;
  int32_t heartBeat_timer;
  std::vector<snssai_t> snssais;
  std::vector<struct in_addr> ipv4_addresses;
  uint16_t priority;
  uint16_t capacity;
  nlohmann::json json_data;  // store extra json data
  std::vector<nf_service_t> nf_services;

  /*
   std::vector<PlmnId> m_PlmnList;
   bool m_PlmnListIsSet;
   std::vector<Snssai> m_SNssais;
   bool m_SNssaisIsSet;
   std::vector<PlmnSnssai> m_PerPlmnSnssaiList;
   bool m_PerPlmnSnssaiListIsSet;
   std::vector<std::string> m_NsiList;
   bool m_NsiListIsSet;
   std::string m_Fqdn;
   bool m_FqdnIsSet;
   std::vector<Ipv6Addr> m_Ipv6Addresses;
   bool m_Ipv6AddressesIsSet;
   int32_t m_Load;
   bool m_LoadIsSet;
   std::string m_Locality;
   bool m_LocalityIsSet;
   UdrInfo m_UdrInfo;
   bool m_UdrInfoIsSet;
   std::vector<UdrInfo> m_UdrInfoExt;
   bool m_UdrInfoExtIsSet;
   UdmInfo m_UdmInfo;
   bool m_UdmInfoIsSet;
   std::vector<UdmInfo> m_UdmInfoExt;
   bool m_UdmInfoExtIsSet;
   AusfInfo m_AusfInfo;
   bool m_AusfInfoIsSet;
   std::vector<AusfInfo> m_AusfInfoExt;
   bool m_AusfInfoExtIsSet;
   AmfInfo m_AmfInfo;
   bool m_AmfInfoIsSet;
   std::vector<AmfInfo> m_AmfInfoExt;
   bool m_AmfInfoExtIsSet;
   SmfInfo m_SmfInfo;
   bool m_SmfInfoIsSet;
   std::vector<SmfInfo> m_SmfInfoExt;
   bool m_SmfInfoExtIsSet;
   UpfInfo m_UpfInfo;
   bool m_UpfInfoIsSet;
   std::vector<UpfInfo> m_UpfInfoExt;
   bool m_UpfInfoExtIsSet;
   PcfInfo m_PcfInfo;
   bool m_PcfInfoIsSet;
   std::vector<PcfInfo> m_PcfInfoExt;
   bool m_PcfInfoExtIsSet;
   BsfInfo m_BsfInfo;
   bool m_BsfInfoIsSet;
   std::vector<BsfInfo> m_BsfInfoExt;
   bool m_BsfInfoExtIsSet;
   ChfInfo m_ChfInfo;
   bool m_ChfInfoIsSet;
   std::vector<ChfInfo> m_ChfInfoExt;
   bool m_ChfInfoExtIsSet;
   NwdafInfo m_NwdafInfo;
   bool m_NwdafInfoIsSet;
   std::string m_RecoveryTime;
   bool m_RecoveryTimeIsSet;
   bool m_NfServicePersistence;
   bool m_NfServicePersistenceIsSet;
   std::vector<NFService> m_NfServices;
   bool m_NfServicesIsSet;
   std::vector<DefaultNotificationSubscription>
   m_DefaultNotificationSubscriptions; bool
   m_DefaultNotificationSubscriptionsIsSet;
   */
};

class amf_profile : public nrf_profile {
 public:
  amf_profile(nrf_event& ev) : nrf_profile(ev, NF_TYPE_AMF) { amf_info = {}; }

  amf_profile(nrf_event& ev, const std::string& id) : nrf_profile(ev, id) {
    nf_type  = NF_TYPE_AMF;
    amf_info = {};
  }

  amf_profile(amf_profile& b) = delete;

  ~amf_profile() {}

  /*
   * Add an AMF info
   * @param [const amf_info_t &] info: AMF info
   * @return void
   */
  void add_amf_info(const amf_info_t& info);

  /*
   * Get list of AMF infos an AMF info
   * @param [const amf_info_t &] info: AMF info
   * @return void
   */
  void get_amf_info(amf_info_t& info) const;

  /*
   * Print related-information for an AMF profile
   * @param void
   * @return void:
   */
  void display();

  /*
   * Update a new value for a member of AMF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return void
   */
  bool replace_profile_info(const std::string& path, const std::string& value);

  /*
   * Add a new value for a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  bool add_profile_info(const std::string& path, const std::string& value);

  /*
   * Remove value of a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  bool remove_profile_info(const std::string& path);

  /*
   * Represent NF profile as json object
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  void to_json(nlohmann::json& data) const;

 private:
  amf_info_t amf_info;
};

class smf_profile : public nrf_profile {
 public:
  smf_profile(nrf_event& ev) : nrf_profile(ev, NF_TYPE_SMF) { smf_info = {}; }

  smf_profile(nrf_event& ev, const std::string& id) : nrf_profile(ev, id) {
    nf_type  = NF_TYPE_SMF;
    smf_info = {};
  }

  smf_profile(smf_profile& b) = delete;

  /*
   * Add a SMF info
   * @param [const smf_info_t &] info: SMF info
   * @return void
   */
  void add_smf_info(const smf_info_t& info);

  /*
   * Get list of SMF infos a SMF info
   * @param [const smf_info_t &] info: SMF info
   * @return void
   */
  void get_smf_info(smf_info_t& infos) const;

  /*
   * Print related-information for a SMF profile
   * @param void
   * @return void:
   */
  void display();

  /*
   * Update a new value for a member of SMF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return void
   */
  bool replace_profile_info(const std::string& path, const std::string& value);

  /*
   * Add a new value for a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  bool add_profile_info(const std::string& path, const std::string& value);

  /*
   * Remove value of a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  bool remove_profile_info(const std::string& path);

  /*
   * Represent NF profile as json object
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  void to_json(nlohmann::json& data) const;

 private:
  smf_info_t smf_info;
};

class upf_profile : public nrf_profile {
 public:
  upf_profile(nrf_event& ev) : nrf_profile(ev, NF_TYPE_UPF) { upf_info = {}; }

  upf_profile(nrf_event& ev, const std::string& id) : nrf_profile(ev, id) {
    nf_type  = NF_TYPE_UPF;
    upf_info = {};
  }

  upf_profile(upf_profile& b) = delete;

  /*
   * Add a UPF info
   * @param [const upf_info_t &] info: UPF info
   * @return void
   */
  void add_upf_info(const upf_info_t& info);

  /*
   * Get list of UPF infos a UPF info
   * @param [const upf_info_t &] info: UPF info
   * @return void
   */
  void get_upf_info(upf_info_t& infos) const;

  /*
   * Print related-information for a UPF profile
   * @param void
   * @return void:
   */
  void display();

  /*
   * Update a new value for a member of UPF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return void
   */
  bool replace_profile_info(const std::string& path, const std::string& value);

  /*
   * Add a new value for a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  bool add_profile_info(const std::string& path, const std::string& value);

  /*
   * Remove value of a member of NF profile
   * @param [const std::string &] path: member name
   * @param [const std::string &] value: new value
   * @return true if success, otherwise false
   */
  bool remove_profile_info(const std::string& path);

  /*
   * Represent NF profile as json object
   * @param [nlohmann::json &] data: Json data
   * @return void
   */
  void to_json(nlohmann::json& data) const;

 private:
  upf_info_t upf_info;
};
}  // namespace app
}  // namespace nrf
}  // namespace oai

#endif
