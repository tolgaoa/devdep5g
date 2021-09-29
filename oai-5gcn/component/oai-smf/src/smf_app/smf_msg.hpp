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

/*! \file smf_msg.hpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: tien-thinh.nguyen@eurecom.fr
 */
#ifndef FILE_SMF_MSG_HPP_SEEN
#define FILE_SMF_MSG_HPP_SEEN

#include "smf.h"
#include "3gpp_23.003.h"
#include "3gpp_24.007.h"
#include "3gpp_24.501.h"
#include "3gpp_29.244.h"
#include "3gpp_29.508.h"
#include "3gpp_29.571.h"
#include "NgRanTargetId.h"
#include "pistache/http.h"
#include "smf_profile.hpp"

extern "C" {
#include "QOSRules.h"
}

typedef enum {
  PDU_SESSION_MSG_TYPE_NONE             = -1,
  PDU_SESSION_MSG_TYPE_FIRST            = 0,
  PDU_SESSION_CREATE_SM_CONTEXT_REQUEST = PDU_SESSION_MSG_TYPE_FIRST,
  PDU_SESSION_CREATE_SM_CONTEXT_RESPONSE,
  PDU_SESSION_UPDATE_SM_CONTEXT_REQUEST,
  PDU_SESSION_UPDATE_SM_CONTEXT_RESPONSE,
  PDU_SESSION_RELEASE_SM_CONTEXT_REQUEST,
  PDU_SESSION_RELEASE_SM_CONTEXT_RESPONSE,
  PDU_SESSION_MODIFICATION_SMF_REQUESTED,
  PDU_SESSION_REPORT_RESPONSE,
  PDU_SESSION_MSG_TYPE_MAX
} pdu_session_msg_type_t;

namespace smf {

//-----------------------------------------------------------------------------
// QoS flow to be created/modified/removed
class qos_flow_context_updated {
 public:
  qos_flow_context_updated()
      : cause_value(),
        qfi(),
        ul_fteid(),
        dl_fteid(),
        qos_profile(),
        to_be_removed(false) {}

  void set_cause(const uint8_t cause);
  void set_qfi(const pfcp::qfi_t& q);
  void set_ul_fteid(const pfcp::fteid_t& teid);
  void set_dl_fteid(const pfcp::fteid_t& teid);
  void add_qos_rule(const QOSRulesIE& rule);
  void set_qos_profile(const qos_profile_t& profile);
  void set_priority_level(uint8_t p);

  uint8_t cause_value;
  pfcp::qfi_t qfi;
  pfcp::fteid_t ul_fteid;
  pfcp::fteid_t dl_fteid;
  std::map<uint8_t, QOSRulesIE> qos_rules;
  qos_profile_t qos_profile;
  bool to_be_removed;
};

//---------------------------------------------------------------------------------------
class pdu_session_msg {
 public:
  pdu_session_msg()
      : m_msg_type(),
        m_supi(),
        m_pdu_session_id(),
        m_dnn(),
        m_snssai(),
        m_pdu_session_type(0) {
    m_n1_sm_msg_is_set       = false;
    m_n2_sm_info_is_set      = false;
    m_n2_sm_info_type_is_set = false;
  };
  pdu_session_msg(pdu_session_msg_type_t msg_type)
      : m_msg_type(msg_type),
        m_supi(),
        m_pdu_session_id(),
        m_dnn(),
        m_snssai(),
        m_pdu_session_type(0) {
    m_n1_sm_msg_is_set       = false;
    m_n2_sm_info_is_set      = false;
    m_n2_sm_info_type_is_set = false;
  };
  pdu_session_msg(
      pdu_session_msg_type_t msg_type, supi_t supi, pdu_session_id_t pdi,
      std::string dnn, snssai_t snssai)
      : m_msg_type(msg_type),
        m_supi(supi),
        m_pdu_session_id(pdi),
        m_dnn(dnn),
        m_snssai(snssai),
        m_pdu_session_type(0) {
    m_n1_sm_msg_is_set       = false;
    m_n2_sm_info_is_set      = false;
    m_n2_sm_info_type_is_set = false;
  }

  virtual ~pdu_session_msg() = default;

  pdu_session_msg_type_t get_msg_type() const;
  void set_msg_type(const pdu_session_msg_type_t& value);
  supi_t get_supi() const;
  void set_supi(const supi_t& value);
  std::string get_supi_prefix() const;
  void set_supi_prefix(const std::string& value);
  pdu_session_id_t get_pdu_session_id() const;
  void set_pdu_session_id(const pdu_session_id_t value);
  std::string get_dnn() const;
  void set_dnn(const std::string& value);
  snssai_t get_snssai() const;
  void set_snssai(const snssai_t& value);
  void set_api_root(const std::string& value);
  std::string get_api_root() const;
  uint8_t get_pdu_session_type() const;
  void set_pdu_session_type(const uint8_t& pdu_session_type);
  procedure_transaction_id_t get_pti() const;
  void set_pti(const procedure_transaction_id_t& pti);
  std::string get_n2_sm_information() const;
  void set_n2_sm_information(const std::string& value);
  std::string get_n1_sm_message() const;
  void set_n1_sm_message(const std::string& value);
  bool n1_sm_msg_is_set() const;
  bool n2_sm_info_is_set() const;
  std::string get_n2_sm_info_type() const;
  void set_n2_sm_info_type(const std::string& value);
  bool n2_sm_info_type_is_set() const;

 private:
  pdu_session_msg_type_t m_msg_type;
  std::string m_api_root;
  supi_t m_supi;
  std::string m_supi_prefix;
  pdu_session_id_t m_pdu_session_id;
  std::string m_dnn;
  snssai_t m_snssai;
  uint8_t m_pdu_session_type;
  procedure_transaction_id_t m_pti;
  std::string m_n1_sm_message;
  bool m_n1_sm_msg_is_set;
  std::string m_n2_sm_information;
  bool m_n2_sm_info_is_set;
  std::string m_n2_sm_info_type;
  bool m_n2_sm_info_type_is_set;
};

//---------------------------------------------------------------------------------------
class pdu_session_sm_context_request : public pdu_session_msg {
 public:
  pdu_session_sm_context_request() : pdu_session_msg() {
    m_epd          = EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
    m_message_type = PDU_SESSION_MESSAGE_TYPE_UNKNOWN;
  };
  pdu_session_sm_context_request(pdu_session_msg_type_t msg_type)
      : pdu_session_msg(msg_type) {
    m_epd          = EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
    m_message_type = PDU_SESSION_MESSAGE_TYPE_UNKNOWN;
  };
  pdu_session_sm_context_request(
      pdu_session_msg_type_t msg_type, supi_t supi, pdu_session_id_t pdi,
      std::string dnn, snssai_t snssai)
      : pdu_session_msg(msg_type, supi, pdi, dnn, snssai) {
    m_epd          = EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
    m_message_type = PDU_SESSION_MESSAGE_TYPE_UNKNOWN;
  }

  extended_protocol_discriminator_t get_epd() const;
  void set_epd(const extended_protocol_discriminator_t& epd);
  uint8_t get_message_type() const;
  void set_message_type(const uint8_t& message_type);

 private:
  extended_protocol_discriminator_t m_epd;
  uint8_t m_message_type;
};

//---------------------------------------------------------------------------------------
class pdu_session_sm_context_response : public pdu_session_msg {
 public:
  pdu_session_sm_context_response(pdu_session_msg_type_t msg_type)
      : pdu_session_msg(msg_type) {
    m_cause       = 0;
    m_http_code   = 0;
    m_json_format = "application/json";
  }
  pdu_session_sm_context_response(
      pdu_session_msg_type_t msg_type, supi_t supi, pdu_session_id_t pdi,
      std::string dnn, snssai_t snssai)
      : pdu_session_msg(msg_type, supi, pdi, dnn, snssai) {
    m_cause       = 0;
    m_http_code   = 0;
    m_json_format = "application/json";
  }

  void set_cause(const uint8_t cause);
  uint8_t get_cause() const;
  void set_http_code(const uint32_t code);
  uint32_t get_http_code() const;
  void set_json_data(const nlohmann::json& data);
  void get_json_data(nlohmann::json& data) const;
  void set_json_format(const std::string& format);
  void get_json_format(std::string& format) const;

 private:
  uint8_t m_cause;
  nlohmann::json m_json_data;
  std::string m_json_format;
  uint32_t m_http_code;
};

//---------------------------------------------------------------------------------------
class pdu_session_create_sm_context_request
    : public pdu_session_sm_context_request {
 public:
  pdu_session_create_sm_context_request()
      : pdu_session_sm_context_request(PDU_SESSION_CREATE_SM_CONTEXT_REQUEST),
        m_unauthenticated_supi(true) {
    m_epco = {};
  }
  pdu_session_create_sm_context_request(
      supi_t supi, pdu_session_id_t pdi, std::string dnn, snssai_t snssai)
      : pdu_session_sm_context_request(
            PDU_SESSION_CREATE_SM_CONTEXT_REQUEST, supi, pdi, dnn, snssai),
        m_unauthenticated_supi(true) {
    m_epco = {};
  }

  std::string get_serving_nf_id() const;
  void set_serving_nf_id(const std::string& value);
  std::string get_request_type() const;
  void set_request_type(const std::string& value);
  void set_dnn_selection_mode(const std::string& value);
  std::string get_dnn_selection_mode() const;
  void set_sm_context_status_uri(const std::string& value);
  std::string get_sm_context_status_uri() const;
  void set_epco(const protocol_configuration_options_t& p);
  void get_epco(protocol_configuration_options_t& p) const;
  void set_plmn(const plmn_t p);
  void get_plmn(plmn_t& p) const;
  plmn_t get_plmn() const;
  void set_an_type(const std::string& an_type);
  void get_an_type(std::string& an_type) const;
  void set_guami(const guami_5g_t& guami);
  void get_guami(guami_5g_t& guami) const;

 private:
  bool m_unauthenticated_supi;
  std::string m_serving_nf_id;  // AMF Id
  std::string m_request_type;
  std::string m_rat_type;
  std::string m_presence_in_ladn;
  std::string m_an_type;
  std::string m_dnn_selection_mode;  // SelMode
  std::string m_sm_context_status_uri;
  protocol_configuration_options_t m_epco;
  plmn_t m_serving_network;
  guami_5g_t m_guami;
};

//---------------------------------------------------------------------------------------
class pdu_session_create_sm_context_response
    : public pdu_session_sm_context_response {
 public:
  pdu_session_create_sm_context_response()
      : pdu_session_sm_context_response(
            PDU_SESSION_CREATE_SM_CONTEXT_RESPONSE) {
    m_paa              = {};
    m_qos_flow_context = {};
    m_epco             = {};
  }
  pdu_session_create_sm_context_response(
      supi_t supi, pdu_session_id_t pdi, std::string dnn, snssai_t snssai)
      : pdu_session_sm_context_response(
            PDU_SESSION_CREATE_SM_CONTEXT_RESPONSE, supi, pdi, dnn, snssai) {
    m_paa              = {};
    m_qos_flow_context = {};
    m_epco             = {};
  }

  void set_paa(const paa_t& paa);
  paa_t get_paa() const;
  void set_qos_flow_context(const qos_flow_context_updated& qos_flow);
  qos_flow_context_updated get_qos_flow_context() const;
  void set_amf_url(const std::string& value);
  std::string get_amf_url() const;
  void set_smf_context_uri(const std::string& value);
  std::string get_smf_context_uri() const;
  void set_epco(const protocol_configuration_options_t& p);
  void get_epco(protocol_configuration_options_t& p) const;

 private:
  paa_t m_paa;
  qos_flow_context_updated m_qos_flow_context;
  std::string m_amf_url;
  std::string m_smf_context_uri;
  protocol_configuration_options_t m_epco;
};

//---------------------------------------------------------------------------------------
// see SmContextUpdateData (TS29502_Nsmf_PDUSession.yaml)
class pdu_session_update_sm_context_request
    : public pdu_session_sm_context_request {
 public:
  pdu_session_update_sm_context_request()
      : pdu_session_sm_context_request(PDU_SESSION_UPDATE_SM_CONTEXT_REQUEST) {
    m_5gMm_cause_value   = 0;
    m_data_forwarding    = false;
    m_upCnx_state_is_set = false;
    qfis                 = {};
    dl_fteid             = {};
    m_release            = false;
    m_release_is_set     = false;
    m_an_type_is_set     = false;
    m_rat_type_is_set    = false;
  };

  void add_qfi(const pfcp::qfi_t& qfi);
  void add_qfi(const uint8_t& qfi);
  void get_qfis(std::vector<pfcp::qfi_t>& q);
  void set_dl_fteid(const pfcp::fteid_t& t);
  void get_dl_fteid(pfcp::fteid_t& t);
  void set_upCnx_state(const std::string& value);
  bool upCnx_state_is_set() const;
  void set_rat_type(const std::string& value);
  bool rat_type_is_set() const;
  void set_an_type(const std::string& value);
  bool an_type_is_set() const;
  bool release_is_set() const;
  void set_release(bool value);

 private:
  std::vector<pfcp::qfi_t> qfis;
  pfcp::fteid_t dl_fteid;  // AN Tunnel Info
  std::string m_nf_instanceId;
  std::string m_an_type;
  bool m_an_type_is_set;
  std::string m_rat_type;
  bool m_rat_type_is_set;
  std::string m_upCnx_state;
  bool m_upCnx_state_is_set;
  std::string m_target_serving_nfId;
  std::string m_sm_context_status_uri;
  bool m_data_forwarding;
  uint8_t m_5gMm_cause_value;
  bool m_release_is_set;
  bool m_release;
};

//---------------------------------------------------------------------------------------
// for PDU session update response
class pdu_session_update_sm_context_response
    : public pdu_session_sm_context_response {
 public:
  pdu_session_update_sm_context_response()
      : pdu_session_sm_context_response(
            PDU_SESSION_UPDATE_SM_CONTEXT_RESPONSE){};

  pdu_session_update_sm_context_response(pdu_session_msg_type_t type)
      : pdu_session_sm_context_response(type){};

  void add_qos_flow_context_updated(const qos_flow_context_updated& qos_flow);
  bool get_qos_flow_context_updated(
      const pfcp::qfi_t& qfi, qos_flow_context_updated& qos_flow);
  void get_all_qos_flow_context_updateds(
      std::map<uint8_t, qos_flow_context_updated>& all_flows);
  void remove_all_qos_flow_context_updateds();
  void set_smf_context_uri(const std::string& value);
  std::string get_smf_context_uri() const;

 private:
  std::map<uint8_t, qos_flow_context_updated> qos_flow_context_updateds;
  std::string m_smf_context_uri;
};

//---------------------------------------------------------------------------------------
class pdu_session_release_sm_context_request : public pdu_session_msg {
 public:
  pdu_session_release_sm_context_request()
      : pdu_session_msg(PDU_SESSION_RELEASE_SM_CONTEXT_REQUEST){};

 private:
  // From smContextReleaseData
  // cause:
  // ngApCause:
  // 5gMmCauseValue:
  // ueLocation:
  // ueTimeZone:
  // addUeLocation:
  // vsmfReleaseOnly:
  // ismfReleaseOnly:
};

//---------------------------------------------------------------------------------------
class pdu_session_release_sm_context_response : public pdu_session_msg {
 public:
  pdu_session_release_sm_context_response()
      : pdu_session_msg(PDU_SESSION_RELEASE_SM_CONTEXT_RESPONSE) {
    m_cause     = 0;
    m_http_code = 0;
  };
  void set_cause(uint8_t cause);
  uint8_t get_cause();
  void set_http_code(const uint32_t code);
  uint32_t get_http_code() const;

 private:
  uint8_t m_cause;
  uint32_t m_http_code;
};

//---------------------------------------------------------------------------------------
class pdu_session_modification_network_requested
    : public pdu_session_sm_context_request {
 public:
  pdu_session_modification_network_requested()
      : pdu_session_sm_context_request(PDU_SESSION_MODIFICATION_SMF_REQUESTED) {
    m_json_data   = {};
    m_json_format = "application/json";
  }

  void set_amf_url(const std::string& value);
  std::string get_amf_url() const;
  void add_qfi(const pfcp::qfi_t& qfi);
  void add_qfi(const uint8_t& qfi);
  void get_qfis(std::vector<pfcp::qfi_t>& q);
  void set_json_data(const nlohmann::json& data);
  void get_json_data(nlohmann::json& data) const;
  void set_json_format(const std::string& format);
  void get_json_format(std::string& format) const;
  void add_qos_flow_context_updated(const qos_flow_context_updated& qos_flow);
  bool get_qos_flow_context_updated(
      const pfcp::qfi_t& qfi, qos_flow_context_updated& qos_flow);

 private:
  std::string amf_url;
  std::vector<pfcp::qfi_t> qfis;
  std::map<uint8_t, qos_flow_context_updated> qos_flow_context_updateds;
  nlohmann::json m_json_data;
  std::string m_json_format;
};

//---------------------------------------------------------------------------------------
class pdu_session_report_response : public pdu_session_sm_context_response {
 public:
  pdu_session_report_response()
      : pdu_session_sm_context_response(PDU_SESSION_REPORT_RESPONSE) {
    seid    = 0;
    trxn_id = 0;
  };

  void set_amf_url(std::string const& value);
  std::string get_amf_url() const;
  void add_qos_flow_context_updated(const qos_flow_context_updated& qos_flow);
  bool get_qos_flow_context_updated(
      const pfcp::qfi_t& qfi, qos_flow_context_updated& qos_flow);
  void get_all_qos_flow_context_updateds(
      std::map<uint8_t, qos_flow_context_updated>& all_flows);
  void set_seid(const seid_t& s);
  void set_trxn_id(const uint64_t& t);
  seid_t get_seid() const;
  uint64_t get_trxn_id() const;

 private:
  std::string amf_url;
  std::map<uint8_t, qos_flow_context_updated> qos_flow_context_updateds;
  seid_t seid;
  uint64_t trxn_id;
};

class event_exposure_msg {
 public:
  supi_t get_supi() const;
  void set_supi(const supi_t& value);
  bool is_supi_is_set() const;
  std::string get_supi_prefix() const;
  void set_supi_prefix(const std::string& value);
  void set_pdu_session_id(const pdu_session_id_t value);
  pdu_session_id_t get_pdu_session_id() const;
  bool is_psi_is_set() const;
  void set_sub_id(std::string const& value);
  std::string get_sub_id() const;
  bool is_sub_id_is_set() const;
  void set_notif_uri(std::string const& value);
  std::string get_notif_uri() const;
  void set_notif_id(std::string const& value);
  std::string get_notif_id() const;
  std::vector<event_subscription_t> get_event_subs() const;
  void set_event_subs(std::vector<event_subscription_t> const& value);

 private:
  supi_t m_supi;
  bool m_supi_is_set;
  std::string m_supi_prefix;
  pdu_session_id_t m_pdu_session_id;  // m_PduSeId;
  bool m_psi_is_set;

  std::string m_sub_id;  // m_SubId;
  bool m_sub_id_is_set;
  std::string m_notif_uri;                         // m_NotifUri;
  std::string m_notif_id;                          // m_NotifId;
  std::vector<event_subscription_t> m_event_subs;  // m_EventSubs;

  // NotificationMethod m_NotifMethod;
  // bool m_NotifMethodIsSet;
  // int32_t m_MaxReportNbr;
  // bool m_MaxReportNbrIsSet;
  // std::string m_Expiry;
  // bool m_ExpiryIsSet;
  // int32_t m_RepPeriod;
  // bool m_RepPeriodIsSet;
  // Guami m_Guami;
  // bool m_GuamiIsSet;
  // std::string m_ServiveName;
  // bool m_ServiveNameIsSet;
  // std::vector<std::string> m_AltNotifIpv4Addrs;
  // bool m_AltNotifIpv4AddrsIsSet;
  // std::vector<Ipv6Addr> m_AltNotifIpv6Addrs;
  // bool m_AltNotifIpv6AddrsIsSet;
  // bool m_AnyUeInd;
  // bool m_AnyUeIndIsSet;
  // std::string m_Gpsi;
  // bool m_GpsiIsSet;
  // std::string m_GroupId;
  // bool m_GroupIdIsSet;
  // bool m_ImmeRep;
  // bool m_ImmeRepIsSet;
  // std::string m_SupportedFeatures;
  // bool m_SupportedFeaturesIsSet;
};

class event_notification {
 public:
  void set_smf_event(const smf_event_t& ev);
  smf_event_t get_smf_event() const;

  void set_supi(const supi64_t& supi);
  supi64_t get_supi() const;
  bool is_supi_is_set() const;
  // m_AdIpv4Addr
  void set_ad_ipv4_addr(std::string const& value);
  std::string get_ad_ipv4_addr() const;
  bool is_ad_ipv4_addr_is_set() const;
  // m_ReIpv4Addr
  void set_re_ipv4_addr(std::string const& value);
  std::string get_re_ipv4_addr() const;
  bool is_re_ipv4_addr_is_set() const;

  void set_pdu_session_id(const pdu_session_id_t value);
  pdu_session_id_t get_pdu_session_id() const;
  bool is_psi_is_set() const;

  void set_notif_uri(std::string const& value);
  std::string get_notif_uri() const;
  void set_notif_id(std::string const& value);
  std::string get_notif_id() const;

 private:
  std::string m_notif_uri;  // m_NotifUri;
  std::string m_notif_id;   // m_NotifId;

  smf_event_t m_event;  // SmfEvent
  // std::string m_TimeStamp;

  supi64_t m_supi;
  bool m_supi_is_set;

  // for a UE IP address change
  std::string m_ad_ipv4_addr;  // m_AdIpv4Addr
  bool m_ad_ipv4_addr_is_set;  // m_AdIpv4AddrIsSet;
  std::string m_re_ipv4_addr;  // m_ReIpv4Addr;
  bool m_re_ipv4_addr_is_set;  // m_ReIpv4AddrIsSet;

  // for a PLMN Change
  // PlmnId m_PlmnId;
  // bool m_PlmnIdIsSet;

  // for an access type change
  // AccessType m_AccType;
  // bool m_AccTypeIsSet;

  // for a PDU Session Release
  pdu_session_id_t m_pdu_session_id;  // m_PduSeId;
  bool m_psi_is_set;

  // std::string m_Gpsi;
  // bool m_GpsiIsSet;
  // std::string m_SourceDnai;
  // bool m_SourceDnaiIsSet;
  // std::string m_TargetDnai;
  // bool m_TargetDnaiIsSet;
  // DnaiChangeType m_DnaiChgType;
  // bool m_DnaiChgTypeIsSet;
  // std::string m_TargetUeIpv4Addr;
  // bool m_TargetUeIpv4AddrIsSet;
  // std::string m_SourceUeIpv4Addr;
  // bool m_SourceUeIpv4AddrIsSet;
  // Ipv6Prefix m_SourceUeIpv6Prefix;
  // bool m_SourceUeIpv6PrefixIsSet;
  // Ipv6Prefix m_TargetUeIpv6Prefix;
  // bool m_TargetUeIpv6PrefixIsSet;
  // RouteToLocation m_SourceTraRouting;
  // bool m_SourceTraRoutingIsSet;
  // RouteToLocation m_TargetTraRouting;
  // bool m_TargetTraRoutingIsSet;
  // std::string m_UeMac;
  // bool m_UeMacIsSet;
  // Ipv6Prefix m_AdIpv6Prefix;
  // bool m_AdIpv6PrefixIsSet;
  // Ipv6Prefix m_ReIpv6Prefix;
  // bool m_ReIpv6PrefixIsSet;
  // DddStatus m_DddStatus;
  // bool m_DddStatusIsSet;
  // std::string m_MaxWaitTime;
  // bool m_MaxWaitTimeIsSet;
};

class data_notification_msg {
 public:
  void set_notification_event_type(const std::string& type);
  void get_notification_event_type(std::string& type) const;
  void set_nf_instance_uri(const std::string& uri);
  void get_nf_instance_uri(std::string& uri) const;
  void set_profile(const std::shared_ptr<nf_profile>& p);
  void get_profile(std::shared_ptr<nf_profile>& p) const;

 private:
  std::string notification_event_type;
  std::string nf_instance_uri;
  std::shared_ptr<nf_profile> profile;
  // bool m_NfProfileIsSet;
  // std::vector<ChangeItem> m_ProfileChanges;
  // bool m_ProfileChangesIsSet;
};
}  // namespace smf

#endif
