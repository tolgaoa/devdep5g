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

/*! \file smf_msg.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: tien-thinh.nguyen@eurecom.fr
 */
#include "smf_msg.hpp"

using namespace smf;

/*
 * class: QoS Flow Context Updated
 */
//-----------------------------------------------------------------------------
void qos_flow_context_updated::set_cause(const uint8_t cause) {
  cause_value = cause;
}

//-----------------------------------------------------------------------------
void qos_flow_context_updated::set_qfi(const pfcp::qfi_t& q) {
  qfi = q;
}

//-----------------------------------------------------------------------------
void qos_flow_context_updated::set_ul_fteid(const pfcp::fteid_t& teid) {
  ul_fteid = teid;
}

//-----------------------------------------------------------------------------
void qos_flow_context_updated::set_dl_fteid(const pfcp::fteid_t& teid) {
  dl_fteid = teid;
}

//-----------------------------------------------------------------------------
void qos_flow_context_updated::add_qos_rule(const QOSRulesIE& rule) {
  uint8_t rule_id = rule.qosruleidentifer;
  if ((rule_id >= QOS_RULE_IDENTIFIER_FIRST) and
      (rule_id <= QOS_RULE_IDENTIFIER_LAST)) {
    qos_rules.erase(rule_id);
    qos_rules.insert(std::pair<uint8_t, QOSRulesIE>(rule_id, rule));
    Logger::smf_app().trace(
        "qos_flow_context_updated::add_qos_rule(%d) success", rule_id);
  }
}

//-----------------------------------------------------------------------------

void qos_flow_context_updated::set_qos_profile(const qos_profile_t& profile) {
  qos_profile = profile;
}

//-----------------------------------------------------------------------------
void qos_flow_context_updated::set_priority_level(uint8_t p) {
  // priority_level = p;
  qos_profile.priority_level = p;
}

/*
 * class: PDU Session MSG
 */
//-----------------------------------------------------------------------------
pdu_session_msg_type_t pdu_session_msg::get_msg_type() const {
  return m_msg_type;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_msg_type(const pdu_session_msg_type_t& msg_type) {
  m_msg_type = msg_type;
}

//-----------------------------------------------------------------------------
supi_t pdu_session_msg::get_supi() const {
  return m_supi;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_supi(const supi_t& supi) {
  m_supi = supi;
}

//-----------------------------------------------------------------------------
std::string pdu_session_msg::get_supi_prefix() const {
  return m_supi_prefix;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_supi_prefix(const std::string& prefix) {
  m_supi_prefix = prefix;
}

//-----------------------------------------------------------------------------
pdu_session_id_t pdu_session_msg::get_pdu_session_id() const {
  return m_pdu_session_id;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_pdu_session_id(
    const pdu_session_id_t pdu_session_id) {
  m_pdu_session_id = pdu_session_id;
}

//-----------------------------------------------------------------------------
std::string pdu_session_msg::get_dnn() const {
  return m_dnn;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_dnn(const std::string& dnn) {
  m_dnn = dnn;
}

//-----------------------------------------------------------------------------
snssai_t pdu_session_msg::get_snssai() const {
  return m_snssai;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_snssai(const snssai_t& snssai) {
  m_snssai = snssai;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_api_root(const std::string& value) {
  m_api_root = value;
}

std::string pdu_session_msg::get_api_root() const {
  return m_api_root;
}

//-----------------------------------------------------------------------------
uint8_t pdu_session_msg::get_pdu_session_type() const {
  return m_pdu_session_type;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_pdu_session_type(const uint8_t& pdu_session_type) {
  m_pdu_session_type = pdu_session_type;
}

//-----------------------------------------------------------------------------
procedure_transaction_id_t pdu_session_msg::get_pti() const {
  return m_pti;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_pti(const procedure_transaction_id_t& pti) {
  m_pti = pti;
}

//-----------------------------------------------------------------------------
std::string pdu_session_msg::get_n1_sm_message() const {
  return m_n1_sm_message;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_n1_sm_message(const std::string& value) {
  m_n1_sm_message    = value;
  m_n1_sm_msg_is_set = true;
}

//-----------------------------------------------------------------------------
bool pdu_session_msg::n1_sm_msg_is_set() const {
  return m_n1_sm_msg_is_set;
}

//-----------------------------------------------------------------------------
std::string pdu_session_msg::get_n2_sm_information() const {
  return m_n2_sm_information;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_n2_sm_information(const std::string& value) {
  m_n2_sm_information = value;
  m_n2_sm_info_is_set = true;
}

//-----------------------------------------------------------------------------
bool pdu_session_msg::n2_sm_info_is_set() const {
  return m_n2_sm_info_is_set;
}

//-----------------------------------------------------------------------------
std::string pdu_session_msg::get_n2_sm_info_type() const {
  return m_n2_sm_info_type;
}

//-----------------------------------------------------------------------------
void pdu_session_msg::set_n2_sm_info_type(const std::string& value) {
  m_n2_sm_info_type        = value;
  m_n2_sm_info_type_is_set = true;
}

//-----------------------------------------------------------------------------
bool pdu_session_msg::n2_sm_info_type_is_set() const {
  return m_n2_sm_info_type_is_set;
}

/*
 * class: PDU Session SM Context Request
 */
//-----------------------------------------------------------------------------
extended_protocol_discriminator_t pdu_session_sm_context_request::get_epd()
    const {
  return m_epd;
}

//-----------------------------------------------------------------------------
void pdu_session_sm_context_request::set_epd(
    const extended_protocol_discriminator_t& epd) {
  m_epd = epd;
}

//-----------------------------------------------------------------------------
uint8_t pdu_session_sm_context_request::get_message_type() const {
  return m_message_type;
}

//-----------------------------------------------------------------------------
void pdu_session_sm_context_request::set_message_type(
    const uint8_t& message_type) {
  m_message_type = message_type;
}

/*
 * class: PDU Session SM Context Response
 */
//-----------------------------------------------------------------------------
void pdu_session_sm_context_response::set_cause(const uint8_t cause) {
  m_cause = cause;
}

//-----------------------------------------------------------------------------
uint8_t pdu_session_sm_context_response::get_cause() const {
  return m_cause;
}

//-----------------------------------------------------------------------------
void pdu_session_sm_context_response::set_http_code(const uint32_t code) {
  m_http_code = code;
}

//-----------------------------------------------------------------------------
uint32_t pdu_session_sm_context_response::get_http_code() const {
  return m_http_code;
}

//-----------------------------------------------------------------------------
void pdu_session_sm_context_response::set_json_data(
    const nlohmann::json& data) {
  m_json_data = data;
}

//-----------------------------------------------------------------------------
void pdu_session_sm_context_response::get_json_data(
    nlohmann::json& data) const {
  data = m_json_data;
}

//-----------------------------------------------------------------------------
void pdu_session_sm_context_response::set_json_format(
    const std::string& format) {
  m_json_format = format;
}

//-----------------------------------------------------------------------------
void pdu_session_sm_context_response::get_json_format(
    std::string& format) const {
  format = m_json_format;
}

/*
 * class: PDU Session Create SM Context Request
 */
//-----------------------------------------------------------------------------
std::string pdu_session_create_sm_context_request::get_serving_nf_id() const {
  return m_serving_nf_id;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::set_serving_nf_id(
    const std::string& serving_nf_id) {
  m_serving_nf_id = serving_nf_id;
}

//-----------------------------------------------------------------------------
std::string pdu_session_create_sm_context_request::get_request_type() const {
  return m_request_type;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::set_request_type(
    const std::string& request_type) {
  m_request_type = request_type;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::set_dnn_selection_mode(
    const std::string& dnn_selection_mode) {
  m_dnn_selection_mode = dnn_selection_mode;
}

//-----------------------------------------------------------------------------
std::string pdu_session_create_sm_context_request::get_dnn_selection_mode()
    const {
  return m_dnn_selection_mode;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::set_sm_context_status_uri(
    const std::string& value) {
  m_sm_context_status_uri = value;
}

//-----------------------------------------------------------------------------
std::string pdu_session_create_sm_context_request::get_sm_context_status_uri()
    const {
  return m_sm_context_status_uri;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::set_epco(
    const protocol_configuration_options_t& p) {
  m_epco = p;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::get_epco(
    protocol_configuration_options_t& p) const {
  p = m_epco;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::set_plmn(const plmn_t p) {
  m_serving_network = p;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::get_plmn(plmn_t& p) const {
  p = m_serving_network;
}

//-----------------------------------------------------------------------------
plmn_t pdu_session_create_sm_context_request::get_plmn() const {
  return m_serving_network;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::set_an_type(
    const std::string& an_type) {
  m_an_type = an_type;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::get_an_type(
    std::string& an_type) const {
  an_type = m_an_type;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::set_guami(const guami_5g_t& guami) {
  m_guami = guami;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_request::get_guami(guami_5g_t& guami) const {
  guami = m_guami;
}

/*
 * class: PDU Session Create SM Context Response
 */
//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_response::set_paa(const paa_t& paa) {
  m_paa = paa;
}

//-----------------------------------------------------------------------------
paa_t pdu_session_create_sm_context_response::get_paa() const {
  return m_paa;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_response::set_qos_flow_context(
    const qos_flow_context_updated& qos_flow) {
  m_qos_flow_context = qos_flow;
}

//-----------------------------------------------------------------------------
qos_flow_context_updated
pdu_session_create_sm_context_response::get_qos_flow_context() const {
  return m_qos_flow_context;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_response::set_amf_url(
    const std::string& value) {
  m_amf_url = value;
}

//-----------------------------------------------------------------------------
std::string pdu_session_create_sm_context_response::get_amf_url() const {
  return m_amf_url;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_response::set_smf_context_uri(
    const std::string& value) {
  m_smf_context_uri = value;
}

//-----------------------------------------------------------------------------
std::string pdu_session_create_sm_context_response::get_smf_context_uri()
    const {
  return m_smf_context_uri;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_response::set_epco(
    const protocol_configuration_options_t& p) {
  m_epco = p;
}

//-----------------------------------------------------------------------------
void pdu_session_create_sm_context_response::get_epco(
    protocol_configuration_options_t& p) const {
  p = m_epco;
}

/*
 * class: PDU Session Update SM Context Request
 */

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::add_qfi(const pfcp::qfi_t& qfi) {
  qfis.push_back(qfi);
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::add_qfi(const uint8_t& q) {
  pfcp::qfi_t qfi(q);
  qfis.push_back(qfi);
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::get_qfis(
    std::vector<pfcp::qfi_t>& q) {
  for (auto qfi : qfis) {
    q.push_back(qfi);
  }
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::set_dl_fteid(
    const pfcp::fteid_t& t) {
  dl_fteid = t;
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::get_dl_fteid(pfcp::fteid_t& t) {
  t = dl_fteid;
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::set_upCnx_state(
    const std::string& value) {
  m_upCnx_state        = value;
  m_upCnx_state_is_set = true;
}

//-----------------------------------------------------------------------------
bool pdu_session_update_sm_context_request::upCnx_state_is_set() const {
  return m_upCnx_state_is_set;
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::set_rat_type(
    const std::string& value) {
  m_rat_type        = value;
  m_rat_type_is_set = true;
}

//-----------------------------------------------------------------------------
bool pdu_session_update_sm_context_request::rat_type_is_set() const {
  return m_rat_type_is_set;
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::set_an_type(
    const std::string& value) {
  m_an_type        = value;
  m_an_type_is_set = true;
}

//-----------------------------------------------------------------------------
bool pdu_session_update_sm_context_request::an_type_is_set() const {
  return m_an_type_is_set;
}

//-----------------------------------------------------------------------------
bool pdu_session_update_sm_context_request::release_is_set() const {
  return m_release_is_set;
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_request::set_release(bool value) {
  m_release        = value;
  m_release_is_set = true;
}

/*
 * class: PDU Session Update SM Context Response
 */
//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_response::add_qos_flow_context_updated(
    const qos_flow_context_updated& flow) {
  if ((flow.qfi.qfi >= QOS_FLOW_IDENTIFIER_FIRST) and
      (flow.qfi.qfi <= QOS_FLOW_IDENTIFIER_LAST)) {
    qos_flow_context_updateds.erase(flow.qfi.qfi);
    qos_flow_context_updateds.insert(
        std::pair<uint8_t, qos_flow_context_updated>(
            (uint8_t) flow.qfi.qfi, flow));
    Logger::smf_app().trace(
        "A QoS Flow Context (QFI %d) has been added successfully",
        flow.qfi.qfi);
  } else {
    Logger::smf_app().error(
        "Failed to add a QoS Flow Context (QFI %d), invalid QFI", flow.qfi.qfi);
  }
}

//-----------------------------------------------------------------------------
bool pdu_session_update_sm_context_response::get_qos_flow_context_updated(
    const pfcp::qfi_t& qfi, qos_flow_context_updated& flow) {
  for (auto it : qos_flow_context_updateds) {
    if (it.second.qfi == qfi) {
      flow = it.second;
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_response::get_all_qos_flow_context_updateds(
    std::map<uint8_t, qos_flow_context_updated>& all_flows) {
  for (auto it : qos_flow_context_updateds) {
    all_flows.insert(std::pair<uint8_t, qos_flow_context_updated>(
        (uint8_t) it.first, it.second));
  }
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_response::
    remove_all_qos_flow_context_updateds() {
  qos_flow_context_updateds.clear();
}

//-----------------------------------------------------------------------------
void pdu_session_update_sm_context_response::set_smf_context_uri(
    const std::string& value) {
  m_smf_context_uri = value;
}

//-----------------------------------------------------------------------------
std::string pdu_session_update_sm_context_response::get_smf_context_uri()
    const {
  return m_smf_context_uri;
}

/*
 * class: PDU Session Release SM Context Response
 */

//-----------------------------------------------------------------------------
void pdu_session_release_sm_context_response::set_cause(uint8_t cause) {
  m_cause = cause;
}

//-----------------------------------------------------------------------------
uint8_t pdu_session_release_sm_context_response::get_cause() {
  return m_cause;
}

//-----------------------------------------------------------------------------
void pdu_session_release_sm_context_response::set_http_code(
    const uint32_t code) {
  m_http_code = code;
}

//-----------------------------------------------------------------------------
uint32_t pdu_session_release_sm_context_response::get_http_code() const {
  return m_http_code;
}

/*
 * class: PDU Session Modification Network Requested
 */
//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::set_amf_url(
    const std::string& value) {
  amf_url = value;
}

//-----------------------------------------------------------------------------
std::string pdu_session_modification_network_requested::get_amf_url() const {
  return amf_url;
}

//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::add_qfi(
    const pfcp::qfi_t& qfi) {
  qfis.push_back(qfi);
}

//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::add_qfi(const uint8_t& q) {
  pfcp::qfi_t qfi(q);
  qfis.push_back(qfi);
}

//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::get_qfis(
    std::vector<pfcp::qfi_t>& q) {
  for (auto qfi : qfis) {
    q.push_back(qfi);
  }
}

//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::set_json_data(
    const nlohmann::json& data) {
  m_json_data = data;
}

//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::get_json_data(
    nlohmann::json& data) const {
  data = m_json_data;
}

//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::set_json_format(
    const std::string& format) {
  m_json_format = format;
}

//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::get_json_format(
    std::string& format) const {
  format = m_json_format;
}

//-----------------------------------------------------------------------------
void pdu_session_modification_network_requested::add_qos_flow_context_updated(
    const qos_flow_context_updated& flow) {
  if ((flow.qfi.qfi >= QOS_FLOW_IDENTIFIER_FIRST) and
      (flow.qfi.qfi <= QOS_FLOW_IDENTIFIER_LAST)) {
    qos_flow_context_updateds.erase(flow.qfi.qfi);
    qos_flow_context_updateds.insert(
        std::pair<uint8_t, qos_flow_context_updated>(
            (uint8_t) flow.qfi.qfi, flow));
    Logger::smf_app().trace(
        "A QoS Flow Context (QFI %d) has been added successfully",
        flow.qfi.qfi);
  } else {
    Logger::smf_app().error(
        "Failed to add a QoS Flow Context (QFI %d), invalid QFI", flow.qfi.qfi);
  }
}

//-----------------------------------------------------------------------------
bool pdu_session_modification_network_requested::get_qos_flow_context_updated(
    const pfcp::qfi_t& qfi, qos_flow_context_updated& flow) {
  for (auto it : qos_flow_context_updateds) {
    if (it.second.qfi == qfi) {
      flow = it.second;
      return true;
    }
  }
  return false;
}

/*
 * class: PDU Session Report Response
 */
//-----------------------------------------------------------------------------
void pdu_session_report_response::set_amf_url(const std::string& value) {
  amf_url = value;
}

//-----------------------------------------------------------------------------
std::string pdu_session_report_response::get_amf_url() const {
  return amf_url;
}

//-----------------------------------------------------------------------------
void pdu_session_report_response::add_qos_flow_context_updated(
    const qos_flow_context_updated& flow) {
  if ((flow.qfi.qfi >= QOS_FLOW_IDENTIFIER_FIRST) and
      (flow.qfi.qfi <= QOS_FLOW_IDENTIFIER_LAST)) {
    qos_flow_context_updateds.erase(flow.qfi.qfi);
    qos_flow_context_updateds.insert(
        std::pair<uint8_t, qos_flow_context_updated>(
            (uint8_t) flow.qfi.qfi, flow));
    Logger::smf_app().trace(
        "A QoS Flow Context (QFI %d) has been added successfully",
        flow.qfi.qfi);
  } else {
    Logger::smf_app().error(
        "Failed to add a QoS Flow Context (QFI %d), invalid QFI", flow.qfi.qfi);
  }
}

//-----------------------------------------------------------------------------
bool pdu_session_report_response::get_qos_flow_context_updated(
    const pfcp::qfi_t& qfi, qos_flow_context_updated& flow) {
  for (auto it : qos_flow_context_updateds) {
    if (it.second.qfi == qfi) {
      flow = it.second;
      return true;
    }
  }
  return false;
}

//-----------------------------------------------------------------------------
void pdu_session_report_response::get_all_qos_flow_context_updateds(
    std::map<uint8_t, qos_flow_context_updated>& all_flows) {
  for (auto it : qos_flow_context_updateds) {
    all_flows.insert(std::pair<uint8_t, qos_flow_context_updated>(
        (uint8_t) it.first, it.second));
  }
}

//-----------------------------------------------------------------------------
void pdu_session_report_response::set_seid(const seid_t& s) {
  seid = s;
}

//-----------------------------------------------------------------------------
void pdu_session_report_response::set_trxn_id(const uint64_t& t) {
  trxn_id = t;
}

//-----------------------------------------------------------------------------
seid_t pdu_session_report_response::get_seid() const {
  return seid;
}

//-----------------------------------------------------------------------------
uint64_t pdu_session_report_response::get_trxn_id() const {
  return trxn_id;
}

/*
 * class: Event Exposure
 */
//-----------------------------------------------------------------------------
supi_t event_exposure_msg::get_supi() const {
  return m_supi;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_supi(const supi_t& value) {
  m_supi        = value;
  m_supi_is_set = true;
}

//-----------------------------------------------------------------------------
bool event_exposure_msg::is_supi_is_set() const {
  return m_supi_is_set;
}

//-----------------------------------------------------------------------------
std::string event_exposure_msg::get_supi_prefix() const {
  return m_supi_prefix;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_supi_prefix(const std::string& prefix) {
  m_supi_prefix = prefix;
}
//-----------------------------------------------------------------------------
pdu_session_id_t event_exposure_msg::get_pdu_session_id() const {
  return m_pdu_session_id;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_pdu_session_id(const pdu_session_id_t value) {
  m_pdu_session_id = value;
  m_psi_is_set     = true;
}

//-----------------------------------------------------------------------------
bool event_exposure_msg::is_psi_is_set() const {
  return m_psi_is_set;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_sub_id(std::string const& value) {
  m_sub_id        = value;
  m_sub_id_is_set = true;
}

//-----------------------------------------------------------------------------
std::string event_exposure_msg::get_sub_id() const {
  return m_sub_id;
}

//-----------------------------------------------------------------------------
bool event_exposure_msg::is_sub_id_is_set() const {
  return m_sub_id_is_set;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_notif_uri(std::string const& value) {
  m_notif_uri = value;
}

//-----------------------------------------------------------------------------
std::string event_exposure_msg::get_notif_uri() const {
  return m_notif_uri;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_notif_id(std::string const& value) {
  m_notif_id = value;
}

//-----------------------------------------------------------------------------
std::string event_exposure_msg::get_notif_id() const {
  return m_notif_id;
}

//-----------------------------------------------------------------------------
std::vector<event_subscription_t> event_exposure_msg::get_event_subs() const {
  return m_event_subs;
}

//-----------------------------------------------------------------------------
void event_exposure_msg::set_event_subs(
    std::vector<event_subscription_t> const& value) {
  m_event_subs.clear();
  for (auto it : value) {
    m_event_subs.push_back(it);
  }
}

/*
 * class: Event Notification
 */
//-----------------------------------------------------------------------------

void event_notification::set_smf_event(const smf_event_t& ev) {
  m_event = ev;
}
//-----------------------------------------------------------------------------
smf_event_t event_notification::get_smf_event() const {
  return m_event;
}

//-----------------------------------------------------------------------------
supi64_t event_notification::get_supi() const {
  return m_supi;
}

//-----------------------------------------------------------------------------
void event_notification::set_supi(const supi64_t& value) {
  m_supi        = value;
  m_supi_is_set = true;
}

//-----------------------------------------------------------------------------
bool event_notification::is_supi_is_set() const {
  return m_supi_is_set;
}

//-----------------------------------------------------------------------------
void event_notification::set_ad_ipv4_addr(std::string const& value) {
  m_ad_ipv4_addr        = value;
  m_ad_ipv4_addr_is_set = true;
}

//-----------------------------------------------------------------------------
std::string event_notification::get_ad_ipv4_addr() const {
  return m_ad_ipv4_addr;
}

//-----------------------------------------------------------------------------
bool event_notification::is_ad_ipv4_addr_is_set() const {
  return m_ad_ipv4_addr_is_set;
}

//-----------------------------------------------------------------------------
void event_notification::set_re_ipv4_addr(std::string const& value) {
  m_re_ipv4_addr        = value;
  m_re_ipv4_addr_is_set = true;
}

//-----------------------------------------------------------------------------
std::string event_notification::get_re_ipv4_addr() const {
  return m_re_ipv4_addr;
}

//-----------------------------------------------------------------------------
bool event_notification::is_re_ipv4_addr_is_set() const {
  return m_re_ipv4_addr_is_set;
}

//-----------------------------------------------------------------------------
void event_notification::set_pdu_session_id(const pdu_session_id_t value) {
  m_pdu_session_id = value;
  m_psi_is_set     = true;
}

//-----------------------------------------------------------------------------
pdu_session_id_t event_notification::get_pdu_session_id() const {
  return m_pdu_session_id;
}

//-----------------------------------------------------------------------------
bool event_notification::is_psi_is_set() const {
  return m_psi_is_set;
}

//-----------------------------------------------------------------------------
void event_notification::set_notif_uri(std::string const& value) {
  m_notif_uri = value;
}

//-----------------------------------------------------------------------------
std::string event_notification::get_notif_uri() const {
  return m_notif_uri;
}

//-----------------------------------------------------------------------------
void event_notification::set_notif_id(std::string const& value) {
  m_notif_id = value;
}

//-----------------------------------------------------------------------------
std::string event_notification::get_notif_id() const {
  return m_notif_id;
}

//-----------------------------------------------------------------------------
void data_notification_msg::set_notification_event_type(
    const std::string& type) {
  notification_event_type = type;
}

//-----------------------------------------------------------------------------
void data_notification_msg::get_notification_event_type(
    std::string& type) const {
  type = notification_event_type;
}

//-----------------------------------------------------------------------------
void data_notification_msg::set_nf_instance_uri(const std::string& uri) {
  nf_instance_uri = uri;
}

//-----------------------------------------------------------------------------
void data_notification_msg::get_nf_instance_uri(std::string& uri) const {
  uri = nf_instance_uri;
}

//-----------------------------------------------------------------------------
void data_notification_msg::set_profile(const std::shared_ptr<nf_profile>& p) {
  profile = p;
}

//-----------------------------------------------------------------------------
void data_notification_msg::get_profile(std::shared_ptr<nf_profile>& p) const {
  p = profile;
}
