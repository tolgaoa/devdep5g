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

/*! \file smf_app.hpp
 \brief
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr
 */

#ifndef FILE_SMF_APP_HPP_SEEN
#define FILE_SMF_APP_HPP_SEEN

#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <future>
#include <map>
#include <set>
#include <shared_mutex>
#include <string>
#include <thread>

#include "3gpp_29.502.h"
#include "itti_msg_n11.hpp"
#include "itti_msg_n4.hpp"
#include "itti_msg_sbi.hpp"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#include "smf.h"
#include "smf_context.hpp"
#include "smf_msg.hpp"
#include "smf_pco.hpp"
#include "smf_profile.hpp"
#include "smf_subscription.hpp"
#include "ProblemDetails.h"

namespace smf {

#define TASK_SMF_APP_TRIGGER_T3591 (0)
#define TASK_SMF_APP_TIMEOUT_T3591 (1)
#define TASK_SMF_APP_TRIGGER_T3592 (2)
#define TASK_SMF_APP_TIMEOUT_T3592 (3)
#define TASK_SMF_APP_TIMEOUT_NRF_HEARTBEAT (4)
#define TASK_SMF_APP_TIMEOUT_NRF_DEREGISTRATION (5)

// Table 10.3.2 @3GPP TS 24.501 V16.1.0 (2019-06)
#define T3591_TIMER_VALUE_SEC 16
#define T3591_TIMER_MAX_RETRIES 4
#define T3592_TIMER_VALUE_SEC 16
#define T3592_TIMER_MAX_RETRIES 4

typedef enum {
  PDU_SESSION_ESTABLISHMENT = 1,
  PDU_SESSION_MODIFICATION  = 2,
  PDU_SESSION_RELEASE       = 3
} pdu_session_procedure_t;

class smf_config;

class smf_context_ref {
 public:
  smf_context_ref() { clear(); }

  void clear() {
    supi           = {};
    nssai          = {};
    dnn            = "";
    pdu_session_id = 0;
    amf_status_uri = "";
    amf_addr       = "";
    upf_node_id    = {};
  }

  supi_t supi;
  std::string dnn;
  pdu_session_id_t pdu_session_id;
  snssai_t nssai;
  std::string amf_status_uri;
  std::string amf_addr;
  pfcp::node_id_t upf_node_id;
};

class smf_app {
 private:
  std::thread::id thread_id;
  std::thread thread;

  // seid generator
  uint64_t seid_n4_generator;
  std::mutex m_seid_n4_generator;
  std::set<uint64_t> set_seid_n4;

  std::map<seid_t, std::shared_ptr<smf_context>> seid2smf_context;
  mutable std::shared_mutex m_seid2smf_context;

  std::map<supi64_t, std::shared_ptr<smf_context>> supi2smf_context;
  mutable std::shared_mutex m_supi2smf_context;

  util::uint_generator<uint32_t> sm_context_ref_generator;
  std::map<scid_t, std::shared_ptr<smf_context_ref>> scid2smf_context;

  util::uint_generator<uint32_t> evsub_id_generator;
  std::map<
      std::pair<evsub_id_t, smf_event_t>, std::shared_ptr<smf_subscription>>
      smf_event_subscriptions;

  mutable std::shared_mutex m_scid2smf_context;
  mutable std::shared_mutex m_smf_event_subscriptions;
  // Store promise IDs for Create/Update session
  mutable std::shared_mutex m_sm_context_create_promises;
  mutable std::shared_mutex m_sm_context_update_promises;
  mutable std::shared_mutex m_sm_context_release_promises;

  std::map<
      uint32_t,
      boost::shared_ptr<boost::promise<pdu_session_create_sm_context_response>>>
      sm_context_create_promises;
  std::map<
      uint32_t,
      boost::shared_ptr<boost::promise<pdu_session_update_sm_context_response>>>
      sm_context_update_promises;
  std::map<
      uint32_t, boost::shared_ptr<
                    boost::promise<pdu_session_release_sm_context_response>>>
      sm_context_release_promises;

  smf_profile nf_instance_profile;  // SMF profile
  std::string smf_instance_id;      // SMF instance id
  timer_id_t timer_nrf_heartbeat;

  /*
   * Apply the config from the configuration file for DNN pools
   * @param [const smf_config &cfg] cfg
   * @return
   */
  int apply_config(const smf_config& cfg);

  /*
   * pco_push_protocol_or_container_id
   * @param [protocol_configuration_options_t &] pco
   * @param [pco_protocol_or_container_id_t *const] proc_id
   * @return
   */
  int pco_push_protocol_or_container_id(
      protocol_configuration_options_t& pco,
      pco_protocol_or_container_id_t* const
          poc_id /* STOLEN_REF poc_id->contents*/);

  /*
   * process_pco_request_ipcp
   * @param [protocol_configuration_options_t &] pco_resp
   * @param [pco_protocol_or_container_id_t *const] proc_id
   * @return
   */
  int process_pco_request_ipcp(
      protocol_configuration_options_t& pco_resp,
      const pco_protocol_or_container_id_t* const poc_id);

  /*
   * process_pco_dns_server_request
   * @param [protocol_configuration_options_t &] pco_resp
   * @param [pco_protocol_or_container_id_t *const] proc_id
   * @return
   */
  int process_pco_dns_server_request(
      protocol_configuration_options_t& pco_resp,
      const pco_protocol_or_container_id_t* const poc_id);

  /*
   * process_pco_dns_server_v6_request
   * @param [protocol_configuration_options_t &] pco_resp
   * @param [pco_protocol_or_container_id_t *const] proc_id
   * @return
   */
  int process_pco_dns_server_v6_request(
      protocol_configuration_options_t& pco_resp,
      const pco_protocol_or_container_id_t* const poc_id);

  /*
   * process_pco_link_mtu_request
   * @param [protocol_configuration_options_t &] pco_resp
   * @param [pco_protocol_or_container_id_t *const] proc_id
   * @return
   */
  int process_pco_link_mtu_request(
      protocol_configuration_options_t& pco_resp,
      const pco_protocol_or_container_id_t* const poc_id);

 public:
  explicit smf_app(const std::string& config_file);
  smf_app(smf_app const&) = delete;

  virtual ~smf_app() {
    Logger::smf_app().debug("Delete SMF_APP instance...");
    // TODO: Unregister NRF
  }

  void operator=(smf_app const&) = delete;

  /*
   * Set the association between Seid and SM Context
   * @param [const seid_t &] seid: SessionID
   * @param [std::shared_ptr<smf_context> &] pc : Shared_ptr to a SMF context
   * @return
   */
  void set_seid_2_smf_context(
      const seid_t& seid, std::shared_ptr<smf_context>& pc);

  /*
   * Find SMF context associated with a Session ID
   * @param [const seid_t &] seid: SessionID
   * @param [std::shared_ptr<smf_context> &] pc : Shared_ptr to a SMF context
   * @return bool: True if SMF context found, otherwise return false
   */
  bool seid_2_smf_context(
      const seid_t& seid, std::shared_ptr<smf_context>& pc) const;

  /*
   * Delete the SMF Context
   * @param [std::shared_ptr<smf_context> &] pc : Shared_ptr to the SMF context
   * to be deleted
   * @return void
   */
  void delete_smf_context(std::shared_ptr<smf_context> spc);

  /*
   * static_paa_get_free_paa
   * @param [const std::string &] dnn
   * @param [paa_t &] paa
   * @return void
   */
  int static_paa_get_free_paa(const std::string& dnn, paa_t& paa);

  /*
   * static_paa_get_free_paa
   * @param [const std::string &] dnn
   * @param [struct in_addr &] addr
   * @return void
   */
  int static_paa_release_address(const std::string& dnn, struct in_addr& addr);

  /*
   * static_paa_get_num_ipv4_pool
   * @param void
   * @return void
   */
  int static_paa_get_num_ipv4_pool(void);

  /*
   * Get paa pool
   * @param
   * @return pool index
   */
  int static_paa_get_ipv4_pool(
      const int pool_id, struct in_addr* const range_low,
      struct in_addr* const range_high, struct in_addr* const netaddr,
      struct in_addr* const netmask,
      std::vector<struct in_addr>::iterator& it_out_of_nw);

  /*
   * Get pool ID corresponding to an address
   * @param [const struct in_addr &] ue_addr
   * @return pool index
   */
  int static_paa_get_pool_id(const struct in_addr& ue_addr);

  /*
   * process_pco_request
   * @param [const protocol_configuration_options_t &] pco_req
   * @param [const protocol_configuration_options_t &] pco_resp
   * @param [const protocol_configuration_options_ids_t &] pco_ids
   * @return pool index
   */
  int process_pco_request(
      const protocol_configuration_options_t& pco_req,
      protocol_configuration_options_t& pco_resp,
      protocol_configuration_options_ids_t& pco_ids);

  /*
   * Handle ITTI message (N4 Session Establishment Response)
   * @param [itti_n4_session_modification_response&] sne
   * @return void
   */
  void handle_itti_msg(itti_n4_session_establishment_response& sne);

  /*
   * Handle ITTI message (N4 Session Modification Response)
   * @param [itti_n4_session_modification_response&] snm
   * @return void
   */
  void handle_itti_msg(itti_n4_session_modification_response& snm);

  /*
   * Handle ITTI message (N4 Session Report Request)
   * @param [itti_n4_association_setup_request&] snd
   * @return void
   */
  void handle_itti_msg(itti_n4_session_deletion_response& snd);

  /*
   * Handle ITTI message (N4 Session Report Request)
   * @param [itti_n4_association_setup_request&] snr
   * @return void
   */
  void handle_itti_msg(std::shared_ptr<itti_n4_session_report_request> snr);

  /*
   * Handle ITTI message (N4 Association Setup Request)
   * @param [itti_n4_association_setup_request&] sna
   * @return void
   */
  void handle_itti_msg(itti_n4_association_setup_request& sna);

  /*
   * Handle ITTI message (N4 Node Failure)
   * @param [itti_n4_node_failure&] snf
   * @return void
   */
  void handle_itti_msg(std::shared_ptr<itti_n4_node_failure> snf);

  /*
   * Handle ITTI message from N11 to update PDU session status
   * @param [itti_n11_update_pdu_session_status&] snu
   * @return void
   */
  void handle_itti_msg(itti_n11_update_pdu_session_status& snu);

  /*
   * Handle ITTI message N11 Create SM Context Response to trigger the response
   * to AMF
   * @param [itti_n11_create_sm_context_response&] snc
   * @return void
   */
  void handle_itti_msg(itti_n11_create_sm_context_response& snc);

  /*
   * Handle ITTI message N11 Update SM Context Response to trigger the response
   * to AMF
   * @param [itti_n11_update_sm_context_response&] m
   * @return void
   */
  void handle_itti_msg(itti_n11_update_sm_context_response& m);

  /*
   * Handle ITTI message N11 Release SM Context Response to trigger the response
   * to AMF
   * @param [itti_n11_release_sm_context_response&] m
   * @return void
   */
  void handle_itti_msg(itti_n11_release_sm_context_response& m);

  /*
   * Handle ITTI message from N11 (N1N2MessageTransfer Response)
   * @param [itti_n11_n1n2_message_transfer_response_status&] snm
   * @return void
   */
  void handle_itti_msg(itti_n11_n1n2_message_transfer_response_status& snm);

  /*
   * Handle ITTI message from N11 (NFRegiser Response)
   * @param [itti_n11_register_nf_instance_response&] r
   * @return void
   */
  void handle_itti_msg(itti_n11_register_nf_instance_response& r);

  /*
   * Handle ITTI message from N11 (NFUpdate Response)
   * @param [itti_n11_update_nf_instance_response&] u
   * @return void
   */
  void handle_itti_msg(itti_n11_update_nf_instance_response& u);

  /*
   * Restore a N4 Session
   * @param [const seid_t &] seid: Session ID to be restored
   * @return void
   */
  void restore_n4_sessions(const seid_t& seid) const;

  /*
   * Generate a Session ID
   * @param [void]
   * @return uint64_t: Return Seid generated
   */
  uint64_t generate_seid();

  /*
   * Verify whether a session with a given ID exist
   * @param [const uint64_t &] s: Seid ID
   * @return bool: True if exist, otherwise false
   */
  bool is_seid_n4_exist(const uint64_t& s) const;

  /*
   * Free a Seid by its ID
   * @param [const uint64_t &] s: Seid ID
   * @return void
   */
  void free_seid_n4(const uint64_t& seid);

  /*
   * Generate a SMF Context Reference in a form of string
   * @param [std::string &] smf_ref: Store the generated reference
   * @return void
   */
  void generate_smf_context_ref(std::string& smf_ref);

  /*
   * Generate a SMF Context Reference
   * @param [void]
   * @return the generated reference
   */
  scid_t generate_smf_context_ref();

  /*
   * Generate an Event Exposure Subscription ID in a form of string
   * @param [std::string &] sub_id: Store the generated reference
   * @return void
   */
  void generate_ev_subscription_id(std::string& sub_id);

  /*
   * Generate an Event Exposure Subscription ID
   * @param [void]
   * @return the generated reference
   */
  evsub_id_t generate_ev_subscription_id();

  /*
   * Set the association betwen a SMF Context Reference and a SMF Context
   * @param [const scid_t &] id: SMF Context Reference Id
   * @param [std::shared_ptr<smf_context_ref>] scf: SMF Context
   * @return the generated reference
   */
  void set_scid_2_smf_context(
      const scid_t& id, std::shared_ptr<smf_context_ref> scf);

  /*
   * Find SMF Context Reference by its ID
   * @param [const scid_t &] scid: SM Context Reference ID
   * @return Shared_ptr to a SMF Context Reference if found, otherwise return
   * false
   */
  std::shared_ptr<smf_context_ref> scid_2_smf_context(const scid_t& scid) const;

  /*
   * Verify whether a SMF Context Reference with a given ID exist
   * @param [const scid_t &] scid: SM Context Reference ID
   * @return bool: True if a SMF Context Reference exist, otherwise return false
   */
  bool is_scid_2_smf_context(const scid_t& scid) const;

  /*
   * Verify whether a SMF Context Reference with a given ID exist
   * @param [const supi64_t &] supi64: Supi64
   * @param [const std::string &] dnn: DNN
   * @param [const snssai_t &] snssai: S-NSSAI
   * @param [const pdu_session_id_t &] pid: PDU Session ID
   *
   * @return bool: True if a SMF Context Reference exist, otherwise return false
   */
  bool is_scid_2_smf_context(
      const supi64_t& supi, const std::string& dnn, const snssai_t& snssai,
      const pdu_session_id_t& pid) const;

  /*
   * Find SMF Context Reference by its ID
   * @param [const scid_t &] scid: SM Context Reference ID
   * @param [std::shared_ptr<smf_context_ref> &] scf : Shared_ptr to a SMF
   * Context Reference
   * @return bool: True if SMF Context Reference found, otherwise return false
   */
  bool scid_2_smf_context(
      const scid_t& scid, std::shared_ptr<smf_context_ref>& scf) const;

  /*
   * Handle PDUSession_CreateSMContextRequest from AMF
   * @param [std::shared_ptr<itti_n11_create_sm_context_request>&] Request
   * message
   * @return void
   */
  void handle_pdu_session_create_sm_context_request(
      std::shared_ptr<itti_n11_create_sm_context_request> smreq);

  /*
   * Handle PDUSession_UpdateSMContextRequest from AMF
   * @param [std::shared_ptr<itti_n11_update_sm_context_request>&] Request
   * message
   * @return void
   */
  void handle_pdu_session_update_sm_context_request(
      std::shared_ptr<itti_n11_update_sm_context_request> smreq);
  /*
   * Handle PDUSession_ReleaseSMContextRequest from AMF
   * @param [std::shared_ptr<itti_n11_release_sm_context_request>&] Request
   * message
   * @return void
   */
  void handle_pdu_session_release_sm_context_request(
      std::shared_ptr<itti_n11_release_sm_context_request> smreq);

  /*
   * Handle Event Exposure Msg from AMF
   * @param [std::shared_ptr<itti_sbi_event_exposure_request>&] Request message
   * @return [evsub_id_t] ID of the created subscription
   */
  evsub_id_t handle_event_exposure_subscription(
      std::shared_ptr<itti_sbi_event_exposure_request> msg);

  bool handle_nf_status_notification(
      std::shared_ptr<itti_sbi_notification_data>& msg,
      oai::smf_server::model::ProblemDetails& problem_details,
      uint8_t& http_code);

  /*
   * Trigger pdu session modification
   * @param [const supi_t &] supi
   * @param [const std::string &] dnn
   * @param [const pdu_session_id_t] pdu_session_id
   * @param [const snssai_t &] snssai
   * @param [const pfcp::qfi_t &] qfi
   * @return void
   */
  void trigger_pdu_session_modification(
      const supi_t& supi, const std::string& dnn,
      const pdu_session_id_t pdu_session_id, const snssai_t& snssai,
      const pfcp::qfi_t& qfi, const uint8_t& http_version);

  /*
   * Verify if SM Context is existed for this Supi
   * @param [supi_t] supi
   * @return True if existed, otherwise false
   */
  bool is_supi_2_smf_context(const supi64_t& supi) const;

  /*
   * Create/Update SMF context with the corresponding supi
   * @param [const supi_t&] supi
   * @param [std::shared_ptr<smf_context>] sc Shared_ptr Pointer to an SMF
   * context
   * @return True if existed, otherwise false
   */
  void set_supi_2_smf_context(
      const supi64_t& supi, std::shared_ptr<smf_context> sc);

  /*
   * Get SM Context
   * @param [supi_t] Supi
   * @return Shared pointer to SM context
   */
  std::shared_ptr<smf_context> supi_2_smf_context(const supi64_t& supi) const;

  /*
   * Check whether SMF uses local configuration instead of retrieving Session
   * Management Data from UDM
   * @param [const std::string&] dnn_selection_mode
   * @return True if SMF uses the local configuration to check the validity of
   * the UE request, False otherwise
   */
  bool use_local_configuration_subscription_data(
      const std::string& dnn_selection_mode);

  /*
   * Verify whether the Session Management Data is existed
   * @param [const supi_t&] SUPI
   * @param [const std::string&] DNN
   * @param [const snssai_t&] S-NSSAI
   * @return True if SMF uses the local configuration to check the validity of
   * the UE request, False otherwise
   */
  bool is_supi_dnn_snssai_subscription_data(
      const supi_t& supi, const std::string& dnn, const snssai_t& snssai) const;

  /*
   * Get the Session Management Subscription data from local configuration
   * @param [const supi_t &] SUPI
   * @param [const std::string &] DNN
   * @param [const snssai_t &] S-NSSAI
   * @param [std::shared_ptr<session_management_subscription>] subscription:
   * store subscription data if exist
   * @return True if local configuration for this session management
   * subscription exists, False otherwise
   */
  bool get_session_management_subscription_data(
      const supi64_t& supi, const std::string& dnn, const snssai_t& snssai,
      std::shared_ptr<session_management_subscription> subscription);

  /*
   * Verify whether the UE request is valid according to the user subscription
   * and with local policies
   * @param [..]
   * @return True if the request is valid, otherwise False
   */
  bool is_create_sm_context_request_valid() const;

  /*
   * Convert a string to hex representing this string
   * @param [const std::string&] input_str Input string
   * @param [std::string&] output_str String represents string in hex format
   * @return void
   */
  void convert_string_2_hex(
      const std::string& input_str, std::string& output_str);

  /*
   * Update PDU session status
   * @param [const scid_t &] id SM Context ID
   * @param [const pdu_session_status_e &] status PDU Session Status
   * @return void
   */
  void update_pdu_session_status(
      const scid_t& id, const pdu_session_status_e& status);

  /*
   * Convert N2 Info type representing by a string to  n2_sm_info_type_e
   * @param [const std::string] n2_info_type
   * @return representing of N2 info type in a form of emum
   */
  n2_sm_info_type_e n2_sm_info_type_str2e(
      const std::string& n2_info_type) const;

  /*
   * Update PDU session UpCnxState
   * @param [const scid_t] id SM Context ID
   * @param [const upCnx_state_e] status PDU Session UpCnxState
   * @return void
   */
  void update_pdu_session_upCnx_state(
      const scid_t& scid, const upCnx_state_e& state);

  /*
   * will be executed when timer T3591 expires
   * @param [timer_id_t] timer_id
   * @param [uint64_t] arg2_user
   * @return void
   */
  void timer_t3591_timeout(timer_id_t timer_id, uint64_t arg2_user);

  /*
   * will be executed when NRF Heartbeat timer expires
   * @param [timer_id_t] timer_id
   * @param [uint64_t] arg2_user
   * @return void
   */
  void timer_nrf_heartbeat_timeout(timer_id_t timer_id, uint64_t arg2_user);

  void timer_nrf_deregistration(timer_id_t timer_id, uint64_t arg2_user);
  /*
   * To start an association with a UPF (SMF-initiated association)
   * @param [const pfcp::node_id_t] node_id: UPF Node ID
   * @return void
   */
  void start_upf_association(const pfcp::node_id_t& node_id);

  /*
   * To start an association with a UPF (SMF-initiated association)
   * @param [const pfcp::node_id_t] node_id: UPF Node ID
   * @return void
   */
  void start_upf_association(
      const pfcp::node_id_t& node_id, const upf_profile& profile);

  /*
   * To store a promise of a PDU Session Create SM Contex Response to be
   * triggered when the result is ready
   * @param [uint32_t] id: promise id
   * @param [boost::shared_ptr<
   * boost::promise<pdu_session_create_sm_context_response> >&] p: pointer to
   * the promise
   * @return void
   */
  void add_promise(
      uint32_t id,
      boost::shared_ptr<boost::promise<pdu_session_create_sm_context_response>>&
          p);

  /*
   * To store a promise of a PDU Session Update SM Contex Response to be
   * triggered when the result is ready
   * @param [uint32_t] id: promise id
   * @param [boost::shared_ptr<
   * boost::promise<pdu_session_update_sm_context_response> >&] p: pointer to
   * the promise
   * @return void
   */
  void add_promise(
      uint32_t id,
      boost::shared_ptr<boost::promise<pdu_session_update_sm_context_response>>&
          p);

  /*
   * To store a promise of a PDU Session Release SM Context Response to be
   * triggered when the result is ready
   * @param [uint32_t] id: promise id
   * @param [boost::shared_ptr<
   * boost::promise<pdu_session_release_sm_context_response> >&] p: pointer to
   * the promise
   * @return void
   */
  void add_promise(
      uint32_t id,
      boost::shared_ptr<
          boost::promise<pdu_session_release_sm_context_response>>& p);

  /*
   * To trigger the response to the HTTP server by set the value of the
   * corresponding promise to ready
   * @param [const uint32_t &] http_code: Status code of HTTP response
   * @param [const uint8_t&] cause: Error cause
   * @param [const std::string &] n1_sm_msg: N1 SM message
   * @param [uint32_t &] promise_id: Promise Id
   * @return void
   */
  void trigger_create_context_error_response(
      const uint32_t& http_code, const uint8_t& cause,
      const std::string& n1_sm_msg, uint32_t& promise_id);

  /*
   * To trigger the response to the HTTP server by set the value of the
   * corresponding promise to ready
   * @param [const uint32_t &] http_code: Status code of HTTP response
   * @param [const uint8_t &] cause: Error cause
   * @param [uint32_t &] promise_id: Promise Id
   * @param [uint8_t] msg_type: Type of HTTP message (Update/Release)
   * @return void
   */
  void trigger_update_context_error_response(
      const uint32_t& http_code, const uint8_t& cause, uint32_t& promise_id);

  /*
   * To trigger the response to the HTTP server by set the value of the
   * corresponding promise to ready
   * @param [const uint32_t &] http_code: Status code of HTTP response
   * @param [const uint8_t &] cause: cause
   * @param [const std::string &] n1_sm_msg: N1 SM message
   * @param [uint32_t &] promise_id: Promise Id
   * @param [uint8_t] msg_type: Type of HTTP message (Update/Release)
   * @return void
   */
  void trigger_update_context_error_response(
      const uint32_t& http_code, const uint8_t& cause,
      const std::string& n1_sm_msg, uint32_t& promise_id);

  /*
   * To trigger the response to the HTTP server by set the value of the
   * corresponding promise to ready
   * @param [const uint32_t &] http_code: Status code of HTTP response
   * @param [uint32_t &] promise_id: Promise Id
   * @param [uint8_t] msg_type: Type of HTTP message (Create/Update/Release)
   * @return void
   */
  void trigger_http_response(
      const uint32_t& http_code, uint32_t& promise_id, uint8_t msg_type);

  /*
   * Add an Event Subscription to the list
   * @param [const evsub_id_t&] sub_id: Subscription ID
   * @param [smf_event_t] ev: Event type
   * @param [std::shared_ptr<smf_subscription>] ss: a shared pointer stored
   * information of the subscription
   * @return void
   */
  void add_event_subscription(
      evsub_id_t sub_id, smf_event_t ev, std::shared_ptr<smf_subscription> ss);

  /*
   * Get a list of subscription associated with a particular event
   * @param [smf_event_t] ev: Event type
   * @param [std::vector<std::shared_ptr<smf_subscription>>&] subscriptions:
   * store the list of the subscription associated with this event type
   * @return void
   */
  void get_ee_subscriptions(
      smf_event_t ev,
      std::vector<std::shared_ptr<smf_subscription>>& subscriptions);

  /*
   * Get a list of subscription associated with a particular event
   * @param [evsub_id_t] sub_id: Subscription ID
   * @param [std::vector<std::shared_ptr<smf_subscription>>&] subscriptions:
   * store the list of the subscription associated with this event type
   * @return void
   */
  void get_ee_subscriptions(
      evsub_id_t sub_id,
      std::vector<std::shared_ptr<smf_subscription>>& subscriptions);

  /*
   * Get a list of subscription associated with a particular event
   * @param [smf_event_t] ev: Event type
   * @param [supi64_t] supi: SUPI
   * @param [pdu_session_id_t] pdu_session_id: PDU Session ID
   * @param [std::vector<std::shared_ptr<smf_subscription>>&] subscriptions:
   * store the list of the subscription associated with this event type
   * @return void
   */
  void get_ee_subscriptions(
      smf_event_t ev, supi64_t supi, pdu_session_id_t pdu_session_id,
      std::vector<std::shared_ptr<smf_subscription>>& subscriptions);

  /*
   * Trigger NF instance registration to NRF
   * @param [void]
   * @return void
   */
  void register_to_nrf();

  /*
   * Generate a random UUID for SMF instance
   * @param [void]
   * @return void
   */
  void generate_uuid();

  /*
   * Generate a SMF profile for this instance
   * @param [void]
   * @return void
   */
  void generate_smf_profile();

  /*
   * Send request to N11 task to trigger NF instance registration to NRF
   * @param [void]
   * @return void
   */
  void trigger_nf_registration_request();

  /*
   * Send request to N11 task to trigger NF instance deregistration to NRF
   * @param [void]
   * @return void
   */
  void trigger_nf_deregistration();

  /*
   * Send request to N11 task to trigger NFSubscribeStatus to NRF
   * @param [void]
   * @return void
   */
  void trigger_upf_status_notification_subscribe();
};
}  // namespace smf
#include "smf_config.hpp"

#endif /* FILE_SMF_APP_HPP_SEEN */
