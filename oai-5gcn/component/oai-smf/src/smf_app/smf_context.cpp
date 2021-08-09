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

/*! \file smf_context.cpp
 \brief
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr
 */

#include "smf_context.hpp"

#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "3gpp_24.501.h"
#include "3gpp_29.500.h"
#include "3gpp_29.502.h"
#include "SmContextCreatedData.h"
#include "itti.hpp"
#include "logger.hpp"
#include "smf_app.hpp"
#include "smf_config.hpp"
#include "smf_event.hpp"
#include "smf_n1.hpp"
#include "smf_sbi.hpp"
#include "smf_n2.hpp"
#include "smf_paa_dynamic.hpp"
#include "smf_pfcp_association.hpp"
#include "smf_procedure.hpp"
#include "3gpp_conversions.hpp"
#include "string.hpp"

extern "C" {
#include "Ngap_AssociatedQosFlowItem.h"
#include "Ngap_GTPTunnel.h"
#include "Ngap_PDUSessionResourceModifyResponseTransfer.h"
#include "Ngap_PDUSessionResourceReleaseResponseTransfer.h"
#include "Ngap_PDUSessionResourceSetupResponseTransfer.h"
#include "Ngap_PDUSessionResourceSetupUnsuccessfulTransfer.h"
#include "Ngap_QosFlowAddOrModifyResponseItem.h"
#include "Ngap_QosFlowAddOrModifyResponseList.h"
#include "dynamic_memory_check.h"
}

using namespace smf;

extern itti_mw* itti_inst;
extern smf::smf_app* smf_app_inst;
extern smf::smf_config smf_cfg;

//------------------------------------------------------------------------------
void smf_qos_flow::mark_as_released() {
  released = true;
}

//------------------------------------------------------------------------------
std::string smf_qos_flow::toString() const {
  std::string s = {};
  s.append("QoS Flow:\n");
  s.append("\tQFI:\t\t\t\t")
      .append(std::to_string((uint8_t) qfi.qfi))
      .append("\n");
  s.append("\tUL FTEID:\t\t").append(ul_fteid.toString()).append("\n");
  s.append("\tDL FTEID:\t\t").append(dl_fteid.toString()).append("\n");
  s.append("\tPDR ID UL:\t\t\t")
      .append(std::to_string(pdr_id_ul.rule_id))
      .append("\n");
  s.append("\tPDR ID DL:\t\t\t")
      .append(std::to_string(pdr_id_dl.rule_id))
      .append("\n");

  s.append("\tPrecedence:\t\t\t")
      .append(std::to_string(precedence.precedence))
      .append("\n");
  if (far_id_ul.first) {
    s.append("\tFAR ID UL:\t\t\t")
        .append(std::to_string(far_id_ul.second.far_id))
        .append("\n");
  }
  if (far_id_dl.first) {
    s.append("\tFAR ID DL:\t\t\t")
        .append(std::to_string(far_id_dl.second.far_id))
        .append("\n");
  }
  return s;
}
//------------------------------------------------------------------------------
void smf_qos_flow::deallocate_ressources() {
  clear();
  Logger::smf_app().info(
      "Resources associated with this QoS Flow (%d) have been released",
      (uint8_t) qfi.qfi);
}

//------------------------------------------------------------------------------
void smf_pdu_session::set(const paa_t& paa) {
  switch (paa.pdu_session_type.pdu_session_type) {
    case PDU_SESSION_TYPE_E_IPV4:
      ipv4                              = true;
      ipv6                              = false;
      ipv4_address                      = paa.ipv4_address;
      pdu_session_type.pdu_session_type = paa.pdu_session_type.pdu_session_type;
      break;
    case PDU_SESSION_TYPE_E_IPV6:
      ipv4                              = false;
      ipv6                              = true;
      ipv6_address                      = paa.ipv6_address;
      pdu_session_type.pdu_session_type = paa.pdu_session_type.pdu_session_type;
      break;
    case PDU_SESSION_TYPE_E_IPV4V6:
      ipv4                              = true;
      ipv6                              = true;
      ipv4_address                      = paa.ipv4_address;
      ipv6_address                      = paa.ipv6_address;
      pdu_session_type.pdu_session_type = paa.pdu_session_type.pdu_session_type;
      break;
    case PDU_SESSION_TYPE_E_UNSTRUCTURED:
    case PDU_SESSION_TYPE_E_ETHERNET:
    case PDU_SESSION_TYPE_E_RESERVED:
      ipv4                              = false;
      ipv6                              = false;
      pdu_session_type.pdu_session_type = paa.pdu_session_type.pdu_session_type;
      break;
    default:
      Logger::smf_app().error(
          "smf_pdu_session::set(paa_t) Unknown PDN type %d",
          paa.pdu_session_type.pdu_session_type);
  }
}

//------------------------------------------------------------------------------
void smf_pdu_session::get_paa(paa_t& paa) {
  switch (pdu_session_type.pdu_session_type) {
    case PDU_SESSION_TYPE_E_IPV4:
      ipv4             = true;
      ipv6             = false;
      paa.ipv4_address = ipv4_address;
      break;
    case PDU_SESSION_TYPE_E_IPV6:
      ipv4             = false;
      ipv6             = true;
      paa.ipv6_address = ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_IPV4V6:
      ipv4             = true;
      ipv6             = true;
      paa.ipv4_address = ipv4_address;
      paa.ipv6_address = ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_UNSTRUCTURED:
    case PDU_SESSION_TYPE_E_ETHERNET:
    case PDU_SESSION_TYPE_E_RESERVED:
      ipv4 = false;
      ipv6 = false;
      break;
    default:
      Logger::smf_app().error(
          "smf_pdu_session::get_paa (paa_t) Unknown PDN type %d",
          pdu_session_type.pdu_session_type);
  }
  paa.pdu_session_type.pdu_session_type = pdu_session_type.pdu_session_type;
}

//------------------------------------------------------------------------------
void smf_pdu_session::add_qos_flow(const smf_qos_flow& flow) {
  if ((flow.qfi.qfi >= QOS_FLOW_IDENTIFIER_FIRST) and
      (flow.qfi.qfi <= QOS_FLOW_IDENTIFIER_LAST)) {
    Logger::smf_app().trace(
        "QoS Flow (flow Id %d) has been added successfully", flow.qfi.qfi);
    std::unique_lock lock(m_pdu_session_mutex);
    qos_flows.erase(flow.qfi.qfi);
    qos_flows.insert(
        std::pair<uint8_t, smf_qos_flow>((uint8_t) flow.qfi.qfi, flow));
  } else {
    Logger::smf_app().error(
        "Failed to add QoS flow (flow Id %d), invalid QFI", flow.qfi.qfi);
  }
}

//------------------------------------------------------------------------------
bool smf_pdu_session::get_qos_flow(
    const pfcp::pdr_id_t& pdr_id, smf_qos_flow& q) {
  std::shared_lock lock(m_pdu_session_mutex);
  for (auto it : qos_flows) {
    if (it.second.pdr_id_ul.rule_id == pdr_id.rule_id) {
      q = it.second;
      return true;
    }
    if (it.second.pdr_id_dl.rule_id == pdr_id.rule_id) {
      q = it.second;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
bool smf_pdu_session::get_qos_flow(
    const pfcp::far_id_t& far_id, smf_qos_flow& q) {
  std::shared_lock lock(m_pdu_session_mutex);
  for (auto it : qos_flows) {
    if ((it.second.far_id_ul.first) &&
        (it.second.far_id_ul.second.far_id == far_id.far_id)) {
      q = it.second;
      return true;
    }
    if ((it.second.far_id_dl.first) &&
        (it.second.far_id_dl.second.far_id == far_id.far_id)) {
      q = it.second;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
bool smf_pdu_session::get_qos_flow(const pfcp::qfi_t& qfi, smf_qos_flow& q) {
  std::shared_lock lock(m_pdu_session_mutex);
  for (auto it : qos_flows) {
    if (it.second.qfi == qfi) {
      q = it.second;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void smf_pdu_session::set_default_qos_flow(const pfcp::qfi_t& qfi) {
  default_qfi.qfi = qfi.qfi;
}

//------------------------------------------------------------------------------
bool smf_pdu_session::get_default_qos_flow(smf_qos_flow& flow) {
  Logger::smf_app().debug("Get default QoS Flow of this PDU session.");
  return get_qos_flow(default_qfi, flow);
}

//------------------------------------------------------------------------------
void smf_pdu_session::get_qos_flows(std::vector<smf_qos_flow>& flows) {
  std::shared_lock lock(m_pdu_session_mutex);
  flows.clear();
  for (auto it : qos_flows) {
    flows.push_back(it.second);
  }
}

//------------------------------------------------------------------------------
bool smf_pdu_session::find_qos_flow(
    const pfcp::pdr_id_t& pdr_id, smf_qos_flow& flow) {
  std::shared_lock lock(m_pdu_session_mutex);
  for (std::map<uint8_t, smf_qos_flow>::iterator it = qos_flows.begin();
       it != qos_flows.end(); ++it) {
    if ((it->second.pdr_id_ul == pdr_id) || (it->second.pdr_id_dl == pdr_id)) {
      flow = it->second;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void smf_pdu_session::remove_qos_flow(const pfcp::qfi_t& qfi) {
  std::unique_lock lock(m_pdu_session_mutex);
  smf_qos_flow& flow = qos_flows[qfi.qfi];
  flow.deallocate_ressources();
  qos_flows.erase(qfi.qfi);
}

//------------------------------------------------------------------------------
void smf_pdu_session::remove_qos_flow(smf_qos_flow& flow) {
  std::unique_lock lock(m_pdu_session_mutex);
  pfcp::qfi_t qfi = {.qfi = flow.qfi.qfi};
  flow.deallocate_ressources();
  qos_flows.erase(qfi.qfi);
}

//------------------------------------------------------------------------------
void smf_pdu_session::remove_qos_flows() {
  std::unique_lock lock(m_pdu_session_mutex);
  for (std::map<uint8_t, smf_qos_flow>::iterator it = qos_flows.begin();
       it != qos_flows.end(); ++it) {
    it->second.deallocate_ressources();
  }
  qos_flows.clear();
}

//------------------------------------------------------------------------------
void smf_pdu_session::deallocate_ressources(const std::string& dnn) {
  for (std::map<uint8_t, smf_qos_flow>::iterator it = qos_flows.begin();
       it != qos_flows.end(); ++it) {
    // TODO: release FAR_ID, PDR_ID
    // release_pdr_id(it->second.pdr_id_dl);
    // release_pdr_id(it->second.pdr_id_ul);
    // release_far_id(it->second.far_id_dl.second);
    // release_far_id(it->second.far_id_ul.second);
    it->second.deallocate_ressources();
  }
  if (ipv4) {
    paa_dynamic::get_instance().release_paa(dnn, ipv4_address);
  }
  clear();  // including qos_flows.clear()
  Logger::smf_app().info(
      "Resources associated with this PDU Session have been released");
}

//------------------------------------------------------------------------------
void smf_pdu_session::generate_seid() {}

void smf_pdu_session::set_seid(const uint64_t& s) {
  seid = s;
}

//------------------------------------------------------------------------------
// TODO check if far_id should be uniq in the UPF or in the context of a pdn
// connection
void smf_pdu_session::generate_far_id(pfcp::far_id_t& far_id) {
  far_id.far_id = far_id_generator.get_uid();
}

//------------------------------------------------------------------------------
void smf_pdu_session::release_far_id(const pfcp::far_id_t& far_id) {
  far_id_generator.free_uid(far_id.far_id);
}

//------------------------------------------------------------------------------
// TODO check if prd_id should be uniq in the UPF or in the context of a pdn
// connection
void smf_pdu_session::generate_pdr_id(pfcp::pdr_id_t& pdr_id) {
  pdr_id.rule_id = pdr_id_generator.get_uid();
}

//------------------------------------------------------------------------------
// TODO check if prd_id should be uniq in the UPF or in the context of a pdn
// connection
void smf_pdu_session::release_pdr_id(const pfcp::pdr_id_t& pdr_id) {
  pdr_id_generator.free_uid(pdr_id.rule_id);
}

//------------------------------------------------------------------------------
void smf_pdu_session::generate_qos_rule_id(uint8_t& rule_id) {
  rule_id = qos_rule_id_generator.get_uid();
}

//------------------------------------------------------------------------------
void smf_pdu_session::release_qos_rule_id(const uint8_t& rule_id) {
  qos_rule_id_generator.free_uid(rule_id);
}

//------------------------------------------------------------------------------
std::string smf_pdu_session::toString() const {
  std::string s     = {};
  smf_qos_flow flow = {};

  s.append("PDN CONNECTION:\n");
  s.append("\tPDN type:\t\t\t")
      .append(pdu_session_type.toString())
      .append("\n");
  if (ipv4)
    s.append("\tPAA IPv4:\t\t\t")
        .append(conv::toString(ipv4_address))
        .append("\n");
  if (ipv6)
    s.append("\tPAA IPv6:\t\t\t")
        .append(conv::toString(ipv6_address))
        .append("\n");
  if (default_qfi.qfi) {
    s.append("\tDefault QFI:\t\t\t")
        .append(std::to_string(default_qfi.qfi))
        .append("\n");
  } else {
    s.append("\tDefault QFI:\t\t\t").append("No QFI available").append("\n");
  }

  s.append("\tSEID:\t\t\t\t").append(std::to_string(seid)).append("\n");

  if (default_qfi.qfi) {
    s.append("Default ");
    for (auto it : qos_flows) {
      if (it.second.qfi == default_qfi.qfi) {
        s.append(it.second.toString());
      }
    }
  }
  return s;
}

//------------------------------------------------------------------------------
void smf_pdu_session::set_pdu_session_status(
    const pdu_session_status_e& status) {
  // TODO: Should consider congestion handling
  Logger::smf_app().info(
      "Set PDU Session Status to %s",
      pdu_session_status_e2str.at(static_cast<int>(status)).c_str());
  std::unique_lock lock(m_pdu_session_mutex);
  pdu_session_status = status;
}

//------------------------------------------------------------------------------
pdu_session_status_e smf_pdu_session::get_pdu_session_status() const {
  std::shared_lock lock(m_pdu_session_mutex);
  return pdu_session_status;
}

//------------------------------------------------------------------------------
void smf_pdu_session::set_upCnx_state(const upCnx_state_e& state) {
  Logger::smf_app().info(
      "Set upCnxState to %s",
      upCnx_state_e2str.at(static_cast<int>(state)).c_str());
  std::unique_lock lock(m_pdu_session_mutex);
  upCnx_state = state;
}

//------------------------------------------------------------------------------
upCnx_state_e smf_pdu_session::get_upCnx_state() const {
  std::shared_lock lock(m_pdu_session_mutex);
  return upCnx_state;
}

//------------------------------------------------------------------------------
pdu_session_type_t smf_pdu_session::get_pdu_session_type() const {
  return pdu_session_type;
}

//------------------------------------------------------------------------------
void smf_pdu_session::get_qos_rules_to_be_synchronised(
    std::vector<QOSRulesIE>& rules) const {
  std::shared_lock lock(m_pdu_session_mutex);
  for (auto it : qos_rules_to_be_synchronised) {
    if (qos_rules.count(it) > 0) rules.push_back(qos_rules.at(it));
  }
}

//------------------------------------------------------------------------------
void smf_pdu_session::get_qos_rules(
    const pfcp::qfi_t& qfi, std::vector<QOSRulesIE>& rules) const {
  Logger::smf_app().info(
      "Get QoS Rules associated with Flow with QFI %d", qfi.qfi);
  std::shared_lock lock(m_pdu_session_mutex);
  for (auto it : qos_rules) {
    if (it.second.qosflowidentifer == qfi.qfi)
      rules.push_back(qos_rules.at(it.first));
  }
}

//------------------------------------------------------------------------------
bool smf_pdu_session::get_default_qos_rule(QOSRulesIE& qos_rule) const {
  Logger::smf_app().info(
      "Get default QoS Rule this PDU Session (ID %d)", pdu_session_id);
  std::shared_lock lock(m_pdu_session_mutex);
  for (auto it : qos_rules) {
    if (it.second.dqrbit == THE_QOS_RULE_IS_DEFAULT_QOS_RULE) {
      qos_rule = it.second;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
bool smf_pdu_session::get_qos_rule(
    const uint8_t rule_id, QOSRulesIE& qos_rule) const {
  Logger::smf_app().info("Find QoS Rule with Rule Id %d", (uint8_t) rule_id);
  std::shared_lock lock(m_pdu_session_mutex);
  if (qos_rules.count(rule_id) > 0) {
    qos_rule = qos_rules.at(rule_id);
  }
  return false;
}

//------------------------------------------------------------------------------
void smf_pdu_session::update_qos_rule(const QOSRulesIE& qos_rule) {
  std::unique_lock lock(
      m_pdu_session_mutex,
      std::defer_lock);  // Do not lock it first

  Logger::smf_app().info(
      "Update QoS Rule with Rule Id %d", (uint8_t) qos_rule.qosruleidentifer);
  uint8_t rule_id = qos_rule.qosruleidentifer;
  if ((rule_id >= QOS_RULE_IDENTIFIER_FIRST) and
      (rule_id <= QOS_RULE_IDENTIFIER_LAST)) {
    if (qos_rules.count(rule_id) > 0) {
      lock.lock();  // Lock it here
      qos_rules.erase(rule_id);
      qos_rules.insert(std::pair<uint8_t, QOSRulesIE>(rule_id, qos_rule));
      // marked to be synchronised with UE
      qos_rules_to_be_synchronised.push_back(rule_id);
      Logger::smf_app().trace("Update QoS rule (%d) success", rule_id);
    } else {
      Logger::smf_app().error(
          "Update QoS Rule (%d) failed, rule does not existed", rule_id);
    }

  } else {
    Logger::smf_app().error(
        "Update QoS rule (%d) failed, invalid Rule Id", rule_id);
  }
}

//------------------------------------------------------------------------------
void smf_pdu_session::mark_qos_rule_to_be_synchronised(const uint8_t rule_id) {
  std::unique_lock lock(
      m_pdu_session_mutex,
      std::defer_lock);  // Do not lock it first
  if ((rule_id >= QOS_RULE_IDENTIFIER_FIRST) and
      (rule_id <= QOS_RULE_IDENTIFIER_LAST)) {
    if (qos_rules.count(rule_id) > 0) {
      lock.lock();  // Lock it here
      qos_rules_to_be_synchronised.push_back(rule_id);
      Logger::smf_app().trace(
          "smf_pdu_session::mark_qos_rule_to_be_synchronised(%d) success",
          rule_id);
    } else {
      Logger::smf_app().error(
          "smf_pdu_session::mark_qos_rule_to_be_synchronised(%d) failed, rule "
          "does not existed",
          rule_id);
    }

  } else {
    Logger::smf_app().error(
        "smf_pdu_session::mark_qos_rule_to_be_synchronised(%d) failed, invalid "
        "Rule Id",
        rule_id);
  }
}

//------------------------------------------------------------------------------
void smf_pdu_session::add_qos_rule(const QOSRulesIE& qos_rule) {
  std::unique_lock lock(
      m_pdu_session_mutex,
      std::defer_lock);  // Do not lock it first
  Logger::smf_app().info(
      "Add QoS Rule with Rule Id %d", (uint8_t) qos_rule.qosruleidentifer);
  uint8_t rule_id = qos_rule.qosruleidentifer;

  if ((rule_id >= QOS_RULE_IDENTIFIER_FIRST) and
      (rule_id <= QOS_RULE_IDENTIFIER_LAST)) {
    if (qos_rules.count(rule_id) > 0) {
      Logger::smf_app().error(
          "Failed to add rule (Id %d), rule existed", rule_id);
    } else {
      lock.lock();  // Lock it here
      qos_rules.insert(std::pair<uint8_t, QOSRulesIE>(rule_id, qos_rule));
      Logger::smf_app().trace(
          "Rule (Id %d) has been added successfully", rule_id);
    }

  } else {
    Logger::smf_app().error(
        "Failed to add rule (Id %d) failed: invalid rule Id", rule_id);
  }
}

//------------------------------------------------------------------------------
void smf_pdu_session::set_amf_addr(const std::string& addr) {
  amf_addr = addr;
}

//------------------------------------------------------------------------------
void smf_pdu_session::get_amf_addr(std::string& addr) const {
  addr = amf_addr;
}

//------------------------------------------------------------------------------
std::string smf_pdu_session::get_amf_addr() const {
  return amf_addr;
}

//------------------------------------------------------------------------------
void session_management_subscription::insert_dnn_configuration(
    const std::string& dnn,
    std::shared_ptr<dnn_configuration_t>& dnn_configuration) {
  std::unique_lock lock(m_mutex);
  dnn_configurations.insert(
      std::pair<std::string, std::shared_ptr<dnn_configuration_t>>(
          dnn, dnn_configuration));
}

//------------------------------------------------------------------------------
void session_management_subscription::find_dnn_configuration(
    const std::string& dnn,
    std::shared_ptr<dnn_configuration_t>& dnn_configuration) const {
  Logger::smf_app().info("Find DNN configuration with DNN %s", dnn.c_str());
  std::shared_lock lock(m_mutex);
  if (dnn_configurations.count(dnn) > 0) {
    dnn_configuration = dnn_configurations.at(dnn);
  }
}

//------------------------------------------------------------------------------
bool session_management_subscription::dnn_configuration(
    const std::string& dnn) const {
  std::shared_lock lock(m_mutex);
  if (dnn_configurations.count(dnn) > 0) {
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
void smf_context::insert_procedure(std::shared_ptr<smf_procedure>& sproc) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  pending_procedures.push_back(sproc);
}

//------------------------------------------------------------------------------
bool smf_context::find_procedure(
    const uint64_t& trxn_id, std::shared_ptr<smf_procedure>& proc) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  auto found = std::find_if(
      pending_procedures.begin(), pending_procedures.end(),
      [trxn_id](const std::shared_ptr<smf_procedure>& i) -> bool {
        return i->trxn_id == trxn_id;
      });
  if (found != pending_procedures.end()) {
    proc = *found;
    return true;
  }
  return false;
}

//------------------------------------------------------------------------------
void smf_context::remove_procedure(smf_procedure* proc) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  auto found = std::find_if(
      pending_procedures.begin(), pending_procedures.end(),
      [proc](const std::shared_ptr<smf_procedure>& i) {
        return i.get() == proc;
      });
  if (found != pending_procedures.end()) {
    pending_procedures.erase(found);
  }
}

//------------------------------------------------------------------------------
void smf_context::handle_itti_msg(
    itti_n4_session_establishment_response& seresp) {
  std::shared_ptr<smf_procedure> proc = {};
  if (find_procedure(seresp.trxn_id, proc)) {
    Logger::smf_app().debug(
        "Received N4 Session Establishment Response sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 "",
        seresp.seid, seresp.trxn_id);
    proc->handle_itti_msg(seresp, shared_from_this());
    remove_procedure(proc.get());
  } else {
    Logger::smf_app().debug(
        "Received N4 Session Establishment Response sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", smf_procedure not found, discarded!",
        seresp.seid, seresp.trxn_id);
  }
}

//------------------------------------------------------------------------------
void smf_context::handle_itti_msg(
    itti_n4_session_modification_response& smresp) {
  std::shared_ptr<smf_procedure> proc = {};
  if (find_procedure(smresp.trxn_id, proc)) {
    Logger::smf_app().debug(
        "Received N4 Session Modification Response sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 " ",
        smresp.seid, smresp.trxn_id);
    proc->handle_itti_msg(smresp, shared_from_this());
    remove_procedure(proc.get());
  } else {
    Logger::smf_app().debug(
        "Received N4 Session Modification Response sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", smf_procedure not found, discarded!",
        smresp.seid, smresp.trxn_id);
  }
  Logger::smf_app().info(
      "Handle N4 Session Modification Response with SMF context %s",
      toString().c_str());
}

//------------------------------------------------------------------------------
void smf_context::handle_itti_msg(itti_n4_session_deletion_response& sdresp) {
  std::shared_ptr<smf_procedure> proc = {};
  if (find_procedure(sdresp.trxn_id, proc)) {
    Logger::smf_app().debug(
        "Received N4 Session Deletion Response sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 " ",
        sdresp.seid, sdresp.trxn_id);
    proc->handle_itti_msg(sdresp, shared_from_this());
    remove_procedure(proc.get());
  } else {
    Logger::smf_app().debug(
        "Received N4 Session Deletion Response sender teid " TEID_FMT
        "  pfcp_tx_id %" PRIX64 ", smf_procedure not found, discarded!",
        sdresp.seid, sdresp.trxn_id);
  }

  Logger::smf_app().info(
      "Handle N4 Session Deletion Response with SMF context %s",
      toString().c_str());
}

//------------------------------------------------------------------------------
void smf_context::handle_itti_msg(
    std::shared_ptr<itti_n4_session_report_request>& req) {
  pfcp::report_type_t report_type;
  if (req->pfcp_ies.get(report_type)) {
    pfcp::pdr_id_t pdr_id;
    // Downlink Data Report
    if (report_type.dldr) {
      pfcp::downlink_data_report data_report;
      if (req->pfcp_ies.get(data_report)) {
        pfcp::pdr_id_t pdr_id;
        if (data_report.get(pdr_id)) {
          std::shared_ptr<dnn_context> sd     = {};
          std::shared_ptr<smf_pdu_session> sp = {};
          pfcp::qfi_t qfi                     = {};
          if (find_pdu_session(pdr_id, qfi, sd, sp)) {
            // Step 1. send N4 Data Report Ack to UPF
            pfcp::node_id_t up_node_id = {};
            scid_t scid                = get_scid();
            // Get UPF node
            std::shared_ptr<smf_context_ref> scf = {};
            if (smf_app_inst->is_scid_2_smf_context(scid)) {
              scf        = smf_app_inst->scid_2_smf_context(scid);
              up_node_id = scf.get()->upf_node_id;
            } else {
              Logger::smf_app().warn(
                  "SM Context associated with this id " SCID_FMT
                  " does not exit!",
                  scid);
              return;
            }

            itti_n4_session_report_response* n4_ser =
                new itti_n4_session_report_response(TASK_SMF_APP, TASK_SMF_N4);
            n4_ser->seid    = req->seid;
            n4_ser->trxn_id = req->trxn_id;
            n4_ser->r_endpoint =
                endpoint(up_node_id.u1.ipv4_address, pfcp::default_port);
            std::shared_ptr<itti_n4_session_report_response> n4_report_ack =
                std::shared_ptr<itti_n4_session_report_response>(n4_ser);

            Logger::smf_app().info(
                "Sending ITTI message %s to task TASK_SMF_N4",
                n4_ser->get_msg_name());
            int ret = itti_inst->send_msg(n4_report_ack);
            if (RETURNok != ret) {
              Logger::smf_app().error(
                  "Could not send ITTI message %s to task TASK_SMF_N4",
                  n4_ser->get_msg_name());
              return;
            }

            // Step 2. Send N1N2MessageTranfer to AMF
            pdu_session_report_response session_report_msg = {};
            // set the required IEs
            session_report_msg.set_supi(supi);
            session_report_msg.set_snssai(sd.get()->nssai);
            session_report_msg.set_dnn(sd.get()->dnn_in_use);
            session_report_msg.set_pdu_session_type(
                sp.get()->get_pdu_session_type().pdu_session_type);
            // get supi and put into URL
            std::string supi_prefix = {};
            get_supi_prefix(supi_prefix);
            std::string supi_str = supi_prefix + "-" + smf_supi_to_string(supi);
            std::string url =
                // std::string(inet_ntoa(
                //    *((struct in_addr*) &smf_cfg.amf_addr.ipv4_addr))) +
                //":" + std::to_string(smf_cfg.amf_addr.port) +
                sp.get()->get_amf_addr() + NAMF_COMMUNICATION_BASE +
                smf_cfg.amf_addr.api_version +
                fmt::format(
                    NAMF_COMMUNICATION_N1N2_MESSAGE_TRANSFER_URL,
                    supi_str.c_str());
            session_report_msg.set_amf_url(url);
            // seid and trxn_id to be used in Failure indication
            session_report_msg.set_seid(req->seid);
            session_report_msg.set_trxn_id(req->trxn_id);

            // QFIs, QoS profiles, CN Tunnel
            smf_qos_flow flow = {};
            sp.get()->get_qos_flow(qfi, flow);
            // ADD QoS Flow to be updated
            qos_flow_context_updated qcu = {};
            qcu.set_qfi(qfi);
            qcu.set_ul_fteid(flow.ul_fteid);
            qcu.set_qos_profile(flow.qos_profile);
            session_report_msg.add_qos_flow_context_updated(qcu);

            // Create N2 SM Information: PDU Session Resource Setup Request
            // Transfer IE
            // N2 SM Information
            std::string n2_sm_info, n2_sm_info_hex;
            smf_n2::get_instance()
                .create_n2_pdu_session_resource_setup_request_transfer(
                    session_report_msg, n2_sm_info_type_e::PDU_RES_SETUP_REQ,
                    n2_sm_info);

            smf_app_inst->convert_string_2_hex(n2_sm_info, n2_sm_info_hex);
            session_report_msg.set_n2_sm_information(n2_sm_info_hex);

            // Fill the json part
            nlohmann::json json_data = {};
            json_data["n2InfoContainer"]["n2InformationClass"] =
                N1N2_MESSAGE_CLASS;
            json_data["n2InfoContainer"]["smInfo"]["PduSessionId"] =
                session_report_msg.get_pdu_session_id();
            // N2InfoContent (section 6.1.6.2.27@3GPP TS 29.518)
            json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]
                     ["ngapIeType"] = "PDU_RES_SETUP_REQ";  // NGAP message type
            json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapData"]
                     ["contentId"] = N2_SM_CONTENT_ID;  // NGAP part
            json_data["n2InfoContainer"]["smInfo"]["sNssai"]["sst"] =
                session_report_msg.get_snssai().sST;
            json_data["n2InfoContainer"]["smInfo"]["sNssai"]["sd"] =
                session_report_msg.get_snssai().sD;

            session_report_msg.set_json_data(json_data);

            itti_n11_session_report_request* itti_n11 =
                new itti_n11_session_report_request(TASK_SMF_APP, TASK_SMF_SBI);
            itti_n11->http_version = 1;  // use HTTPv1 for the moment
            std::shared_ptr<itti_n11_session_report_request> itti_n11_report =
                std::shared_ptr<itti_n11_session_report_request>(itti_n11);
            itti_n11_report->res = session_report_msg;
            // send ITTI message to N11 interface to trigger N1N2MessageTransfer
            // towards AMFs
            Logger::smf_app().info(
                "Sending ITTI message %s to task TASK_SMF_SBI",
                itti_n11_report->get_msg_name());

            ret = itti_inst->send_msg(itti_n11_report);
            if (RETURNok != ret) {
              Logger::smf_app().error(
                  "Could not send ITTI message %s to task TASK_SMF_SBI",
                  itti_n11_report->get_msg_name());
            }
          }
        }
      }
    }
    // Usage Report
    if (report_type.usar) {
      // TODO
      Logger::smf_app().debug("PFCP_SESSION_REPORT_REQUEST/Usage Report");
    }
    // Error Indication Report
    if (report_type.erir) {
      // TODO
      Logger::smf_app().debug(
          "PFCP_SESSION_REPORT_REQUEST/Error Indication Report");
    }
    // User Plane Inactivity Report
    if (report_type.upir) {
      // TODO
      Logger::smf_app().debug(
          "PFCP_SESSION_REPORT_REQUEST/User Plane Inactivity Report");
    }
  }
}

//------------------------------------------------------------------------------
std::string smf_context::toString() const {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  std::string s = {};
  s.append("\n");
  s.append("SMF CONTEXT:\n");
  s.append("\tSUPI:\t\t\t\t")
      .append(smf_supi_to_string(supi).c_str())
      .append("\n");
  for (auto it : dnns) {
    s.append(it->toString());
  }
  return s;
}

//------------------------------------------------------------------------------
void smf_context::get_default_qos(
    const snssai_t& snssai, const std::string& dnn,
    subscribed_default_qos_t& default_qos) {
  Logger::smf_app().info(
      "Get default QoS for a PDU Session, key %d", (uint8_t) snssai.sST);
  // get the default QoS profile
  std::shared_ptr<session_management_subscription> ss = {};
  std::shared_ptr<dnn_configuration_t> sdc            = {};
  find_dnn_subscription(snssai, ss);

  if (nullptr != ss.get()) {
    ss.get()->find_dnn_configuration(dnn, sdc);
    if (nullptr != sdc.get()) {
      default_qos = sdc.get()->_5g_qos_profile;
    }
  }
}

//------------------------------------------------------------------------------
void smf_context::get_default_qos_rule(
    QOSRulesIE& qos_rule, uint8_t pdu_session_type) {
  // TODO, update according to PDU Session type
  Logger::smf_app().info(
      "Get default QoS rule for a PDU Session (PDU session type %d)",
      pdu_session_type);
  // see section 9.11.4.13 @ 3GPP TS 24.501 and section 5.7.1.4 @ 3GPP TS 23.501
  qos_rule.qosruleidentifer  = 0x01;  // be updated later on
  qos_rule.ruleoperationcode = CREATE_NEW_QOS_RULE;
  qos_rule.dqrbit            = THE_QOS_RULE_IS_DEFAULT_QOS_RULE;
  if ((pdu_session_type == PDU_SESSION_TYPE_E_IPV4) or
      (pdu_session_type == PDU_SESSION_TYPE_E_IPV4V6) or
      (pdu_session_type == PDU_SESSION_TYPE_E_IPV6) or
      (pdu_session_type == PDU_SESSION_TYPE_E_ETHERNET)) {
    qos_rule.numberofpacketfilters = 1;
    qos_rule.packetfilterlist.create_modifyandadd_modifyandreplace =
        (Create_ModifyAndAdd_ModifyAndReplace*) calloc(
            1, sizeof(Create_ModifyAndAdd_ModifyAndReplace));
    qos_rule.packetfilterlist.create_modifyandadd_modifyandreplace[0]
        .packetfilterdirection = 0b11;  // bi-directional
    qos_rule.packetfilterlist.create_modifyandadd_modifyandreplace[0]
        .packetfilteridentifier = 1;
    qos_rule.packetfilterlist.create_modifyandadd_modifyandreplace[0]
        .packetfiltercontents.component_type = QOS_RULE_MATCHALL_TYPE;
    // qos_rule.packetfilterlist.create_modifyandadd_modifyandreplace[0].packetfiltercontents.component_value
    // = bfromcstralloc(2, "\0");
    qos_rule.qosruleprecedence = 0xff;
  }

  if (pdu_session_type == PDU_SESSION_TYPE_E_UNSTRUCTURED) {
    qos_rule.numberofpacketfilters = 0;
    qos_rule.qosruleprecedence     = 0xff;
  }

  qos_rule.segregation      = SEGREGATION_NOT_REQUESTED;
  qos_rule.qosflowidentifer = DEFAULT_QFI;

  Logger::smf_app().debug(
      "Default QoSRules: %x %x %x %x %x %x %x %x %x", qos_rule.qosruleidentifer,
      qos_rule.ruleoperationcode, qos_rule.dqrbit,
      qos_rule.numberofpacketfilters,
      qos_rule.packetfilterlist.create_modifyandadd_modifyandreplace
          ->packetfilterdirection,
      qos_rule.packetfilterlist.create_modifyandadd_modifyandreplace
          ->packetfilteridentifier,
      qos_rule.packetfilterlist.create_modifyandadd_modifyandreplace
          ->packetfiltercontents.component_type,
      qos_rule.qosruleprecedence, qos_rule.segregation,
      qos_rule.qosflowidentifer);
}

//------------------------------------------------------------------------------
void smf_context::get_default_qos_flow_description(
    QOSFlowDescriptionsContents& qos_flow_description, uint8_t pdu_session_type,
    const pfcp::qfi_t& qfi) {
  // TODO, update according to PDU Session type
  Logger::smf_app().info(
      "Get default QoS Flow Description (PDU session type %d)",
      pdu_session_type);
  qos_flow_description.qfi                = qfi.qfi;
  qos_flow_description.operationcode      = CREATE_NEW_QOS_FLOW_DESCRIPTION;
  qos_flow_description.e                  = PARAMETERS_LIST_IS_INCLUDED;
  qos_flow_description.numberofparameters = 1;
  qos_flow_description.parameterslist =
      (ParametersList*) calloc(3, sizeof(ParametersList));
  qos_flow_description.parameterslist[0].parameteridentifier =
      PARAMETER_IDENTIFIER_5QI;
  qos_flow_description.parameterslist[0].parametercontents._5qi = qfi.qfi;
  /*
   qos_flow_description.parameterslist[1].parameteridentifier =
   PARAMETER_IDENTIFIER_GFBR_UPLINK;
   qos_flow_description.parameterslist[1].parametercontents
   .gfbrormfbr_uplinkordownlink.uint =
   GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS;
   qos_flow_description.parameterslist[1].parametercontents
   .gfbrormfbr_uplinkordownlink.value = 0x10;
   qos_flow_description.parameterslist[2].parameteridentifier =
   PARAMETER_IDENTIFIER_GFBR_DOWNLINK;
   qos_flow_description.parameterslist[2].parametercontents
   .gfbrormfbr_uplinkordownlink.uint =
   GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS;
   qos_flow_description.parameterslist[2].parametercontents
   .gfbrormfbr_uplinkordownlink.value = 0x10;
   */

  Logger::smf_app().debug(
      "Default Qos Flow Description: %x %x %x %x %x %x",
      qos_flow_description.qfi, qos_flow_description.operationcode,
      qos_flow_description.e, qos_flow_description.numberofparameters,
      qos_flow_description.parameterslist[0].parameteridentifier,
      qos_flow_description.parameterslist[0].parametercontents._5qi
      /*      qos_flow_description.parameterslist[1].parameteridentifier,
       qos_flow_description.parameterslist[1].parametercontents
       .gfbrormfbr_uplinkordownlink.uint,
       qos_flow_description.parameterslist[1].parametercontents
       .gfbrormfbr_uplinkordownlink.value,
       qos_flow_description.parameterslist[2].parameteridentifier,
       qos_flow_description.parameterslist[2].parametercontents
       .gfbrormfbr_uplinkordownlink.uint,
       qos_flow_description.parameterslist[2].parametercontents
       .gfbrormfbr_uplinkordownlink.value
       */);
}

//------------------------------------------------------------------------------
void smf_context::get_session_ambr(
    SessionAMBR& session_ambr, const snssai_t& snssai, const std::string& dnn) {
  Logger::smf_app().debug(
      "Get AMBR info from the subscription information (DNN %s)", dnn.c_str());

  std::shared_ptr<session_management_subscription> ss = {};
  std::shared_ptr<dnn_configuration_t> sdc            = {};
  find_dnn_subscription(snssai, ss);
  if (nullptr != ss.get()) {
    ss.get()->find_dnn_configuration(dnn, sdc);
    if (nullptr != sdc.get()) {
      Logger::smf_app().debug(
          "Default AMBR info from the subscription information, downlink %s, "
          "uplink %s",
          (sdc.get()->session_ambr).downlink.c_str(),
          (sdc.get()->session_ambr).uplink.c_str());

      // Downlink
      size_t leng_of_session_ambr_dl =
          (sdc.get()->session_ambr).downlink.length();
      try {
        std::string session_ambr_dl_unit =
            (sdc.get()->session_ambr)
                .downlink.substr(
                    leng_of_session_ambr_dl -
                    4);  // 4 last characters stand for mbps, kbps, ..
        if (session_ambr_dl_unit.compare("Kbps") == 0)
          session_ambr.uint_for_session_ambr_for_downlink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1KBPS;
        if (session_ambr_dl_unit.compare("Mbps") == 0)
          session_ambr.uint_for_session_ambr_for_downlink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS;
        if (session_ambr_dl_unit.compare("Gbps") == 0)
          session_ambr.uint_for_session_ambr_for_downlink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1GBPS;
        if (session_ambr_dl_unit.compare("Tbps") == 0)
          session_ambr.uint_for_session_ambr_for_downlink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1TBPS;
        if (session_ambr_dl_unit.compare("Pbps") == 0)
          session_ambr.uint_for_session_ambr_for_downlink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1PBPS;

        session_ambr.session_ambr_for_downlink =
            std::stoi((sdc.get()->session_ambr)
                          .downlink.substr(0, leng_of_session_ambr_dl - 4));
      } catch (const std::exception& e) {
        Logger::smf_app().warn("Undefined error: %s", e.what());
        // assign default value
        session_ambr.session_ambr_for_downlink = 1;
        session_ambr.uint_for_session_ambr_for_downlink =
            AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS;
      }

      // Uplink
      size_t leng_of_session_ambr_ul =
          (sdc.get()->session_ambr).uplink.length();
      try {
        std::string session_ambr_ul_unit =
            (sdc.get()->session_ambr)
                .uplink.substr(
                    leng_of_session_ambr_ul -
                    4);  // 4 last characters stand for mbps, kbps, ..
        if (session_ambr_ul_unit.compare("Kbps") == 0)
          session_ambr.uint_for_session_ambr_for_uplink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1KBPS;
        if (session_ambr_ul_unit.compare("Mbps") == 0)
          session_ambr.uint_for_session_ambr_for_uplink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS;
        if (session_ambr_ul_unit.compare("Gbps") == 0)
          session_ambr.uint_for_session_ambr_for_uplink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1GBPS;
        if (session_ambr_ul_unit.compare("Tbps") == 0)
          session_ambr.uint_for_session_ambr_for_uplink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1TBPS;
        if (session_ambr_ul_unit.compare("Pbps") == 0)
          session_ambr.uint_for_session_ambr_for_uplink =
              AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1PBPS;

        session_ambr.session_ambr_for_uplink =
            std::stoi((sdc.get()->session_ambr)
                          .uplink.substr(0, leng_of_session_ambr_ul - 4));
      } catch (const std::exception& e) {
        Logger::smf_app().warn("Undefined error: %s", e.what());
        // assign default value
        session_ambr.session_ambr_for_uplink = 1;
        session_ambr.uint_for_session_ambr_for_uplink =
            AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS;
      }
    }
  } else {
    Logger::smf_app().debug(
        "Could not get default info from the subscription information, use "
        "default value instead.");
    // use default value
    session_ambr.session_ambr_for_downlink = 1;
    session_ambr.uint_for_session_ambr_for_downlink =
        AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS;
    session_ambr.session_ambr_for_uplink = 1;
    session_ambr.uint_for_session_ambr_for_uplink =
        AMBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS;
  }
}

//------------------------------------------------------------------------------
void smf_context::get_session_ambr(
    Ngap_PDUSessionAggregateMaximumBitRate_t& session_ambr,
    const snssai_t& snssai, const std::string& dnn) {
  std::shared_ptr<session_management_subscription> ss = {};
  std::shared_ptr<dnn_configuration_t> sdc            = {};
  find_dnn_subscription(snssai, ss);

  uint32_t bit_rate_dl = {110000000};  // TODO: to be updated
  uint32_t bit_rate_ul = {110000000};  // TODO: to be updated

  session_ambr.pDUSessionAggregateMaximumBitRateDL.size = 4;
  session_ambr.pDUSessionAggregateMaximumBitRateDL.buf =
      (uint8_t*) calloc(4, sizeof(uint8_t));
  session_ambr.pDUSessionAggregateMaximumBitRateUL.size = 4;
  session_ambr.pDUSessionAggregateMaximumBitRateUL.buf =
      (uint8_t*) calloc(4, sizeof(uint8_t));

  if (nullptr != ss.get()) {
    ss.get()->find_dnn_configuration(dnn, sdc);

    if (nullptr != sdc.get()) {
      Logger::smf_app().debug(
          "Default AMBR info from the DNN configuration, uplink %s, downlink "
          "%s",
          (sdc.get()->session_ambr).uplink.c_str(),
          (sdc.get()->session_ambr).downlink.c_str());
      // Downlink
      size_t leng_of_session_ambr_dl =
          (sdc.get()->session_ambr).downlink.length();
      try {
        bit_rate_dl =
            std::stoi((sdc.get()->session_ambr)
                          .downlink.substr(0, leng_of_session_ambr_dl - 4));
        std::string session_ambr_dl_unit =
            (sdc.get()->session_ambr)
                .downlink.substr(
                    leng_of_session_ambr_dl -
                    4);  // 4 last characters stand for mbps, kbps, ..
        if (session_ambr_dl_unit.compare("Kbps") == 0) bit_rate_dl *= 1000;
        if (session_ambr_dl_unit.compare("Mbps") == 0) bit_rate_dl *= 1000000;
        if (session_ambr_dl_unit.compare("Gbps") == 0)
          bit_rate_dl *= 1000000000;
        INT32_TO_BUFFER(
            bit_rate_dl, session_ambr.pDUSessionAggregateMaximumBitRateDL.buf);
      } catch (const std::exception& e) {
        Logger::smf_app().warn("Undefined error: %s", e.what());
        // assign default value
        bit_rate_dl = 1;
        INT32_TO_BUFFER(
            bit_rate_dl, session_ambr.pDUSessionAggregateMaximumBitRateDL.buf);
      }

      // Uplink
      size_t leng_of_session_ambr_ul =
          (sdc.get()->session_ambr).uplink.length();
      try {
        bit_rate_ul =
            std::stoi((sdc.get()->session_ambr)
                          .uplink.substr(0, leng_of_session_ambr_ul - 4));
        std::string session_ambr_ul_unit =
            (sdc.get()->session_ambr)
                .uplink.substr(
                    leng_of_session_ambr_ul -
                    4);  // 4 last characters stand for mbps, kbps, ..
        if (session_ambr_ul_unit.compare("Kbps") == 0) bit_rate_ul *= 1000;
        if (session_ambr_ul_unit.compare("Mbps") == 0) bit_rate_ul *= 1000000;
        if (session_ambr_ul_unit.compare("Gbps") == 0)
          bit_rate_ul *= 1000000000;
        INT32_TO_BUFFER(
            bit_rate_ul, session_ambr.pDUSessionAggregateMaximumBitRateUL.buf);
      } catch (const std::exception& e) {
        Logger::smf_app().warn("Undefined error: %s", e.what());
        // assign default value
        bit_rate_ul = 1;
        INT32_TO_BUFFER(
            bit_rate_ul, session_ambr.pDUSessionAggregateMaximumBitRateUL.buf);
      }
    }
  } else {
    INT32_TO_BUFFER(
        bit_rate_dl, session_ambr.pDUSessionAggregateMaximumBitRateDL.buf);
    INT32_TO_BUFFER(
        bit_rate_ul, session_ambr.pDUSessionAggregateMaximumBitRateUL.buf);
  }

  Logger::smf_app().debug(
      "Get AMBR info from the subscription information (DNN %s), uplink %d "
      "downlink %d",
      dnn.c_str(), bit_rate_ul, bit_rate_dl);
}

//------------------------------------------------------------------------------
void smf_context::handle_pdu_session_create_sm_context_request(
    std::shared_ptr<itti_n11_create_sm_context_request> smreq) {
  Logger::smf_app().info(
      "Handle a PDU Session Create SM Context Request message from AMF (HTTP "
      "version %d)",
      smreq->http_version);

  std::string n1_sm_message, n1_sm_msg_hex;
  bool request_accepted = true;

  // Step 1. get necessary information
  std::string dnn         = smreq->req.get_dnn();
  snssai_t snssai         = smreq->req.get_snssai();
  supi64_t supi64         = smf_supi_to_u64(smreq->req.get_supi());
  uint32_t pdu_session_id = smreq->req.get_pdu_session_id();

  // Step 2. check the validity of the UE request, if valid send PDU Session
  // Accept, otherwise send PDU Session Reject to AMF
  if (!verify_sm_context_request(smreq)) {
    Logger::smf_app().warn(
        "Received a PDU Session Create SM Context Request, the request is not "
        "valid!");
    if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
            smreq->req, n1_sm_message,
            cause_value_5gsm_e::
                CAUSE_29_USER_AUTHENTICATION_OR_AUTHORIZATION_FAILED)) {
      smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_msg_hex);
      // trigger to send reply to AMF
      smf_app_inst->trigger_create_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_401_UNAUTHORIZED,
          PDU_SESSION_APPLICATION_ERROR_SUBSCRIPTION_DENIED, n1_sm_msg_hex,
          smreq->pid);
    } else {
      smf_app_inst->trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
          smreq->pid, N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
    }
    // TODO:
    // SMF unsubscribes to the modifications of Session Management Subscription
    // data for (SUPI, DNN, S-NSSAI)  using Nudm_SDM_Unsubscribe()
    return;
  }

  // store HttpResponse and session-related information to be used when
  // receiving the response from UPF
  itti_n11_create_sm_context_response* sm_context_resp =
      new itti_n11_create_sm_context_response(
          TASK_SMF_APP, TASK_SMF_SBI, smreq->pid);
  std::shared_ptr<itti_n11_create_sm_context_response> sm_context_resp_pending =
      std::shared_ptr<itti_n11_create_sm_context_response>(sm_context_resp);

  // Assign necessary information for the response
  xgpp_conv::create_sm_context_response_from_ct_request(
      smreq, sm_context_resp_pending);

  // Step 3. find pdu_session
  std::shared_ptr<dnn_context> sd = {};
  bool find_dnn                   = find_dnn_context(snssai, dnn, sd);

  // step 3.1. create dnn context if not exist
  // At this step, this context should be existed
  if (nullptr == sd.get()) {
    Logger::smf_app().debug(
        "DNN context (dnn_in_use %s) is not existed yet!", dnn.c_str());
    sd                   = std::shared_ptr<dnn_context>(new dnn_context());
    sd.get()->in_use     = true;
    sd.get()->dnn_in_use = dnn;
    sd.get()->nssai      = snssai;
    insert_dnn(sd);
  } else {
    sd.get()->dnn_in_use = dnn;
    Logger::smf_app().debug(
        "DNN context (dnn_in_use %s) is already existed", dnn.c_str());
  }

  // step 3.2. create pdu session if not exist
  std::shared_ptr<smf_pdu_session> sp = {};
  bool find_pdu = sd.get()->find_pdu_session(pdu_session_id, sp);

  if (nullptr == sp.get()) {
    Logger::smf_app().debug("Create a new PDU session");
    sp = std::shared_ptr<smf_pdu_session>(new smf_pdu_session());
    sp.get()->pdu_session_type.pdu_session_type =
        smreq->req.get_pdu_session_type();
    sp.get()->pdu_session_id = pdu_session_id;
    sp.get()->amf_id         = smreq->req.get_serving_nf_id();  // amf id
    sd->insert_pdu_session(sp);
  } else {
    Logger::smf_app().warn("PDU session is already existed!");
  }

  // TODO: if "Integrity Protection is required", check UE Integrity Protection
  // Maximum Data Rate
  // TODO: (Optional) Secondary authentication/authorization

  // TODO: Step 5. PCF selection
  // TODO: Step 5.1. SM Policy Association Establishment to get default PCC
  // rules for this PDU session from PCF  For the moment, SMF uses the local
  // policy (e.g., default QoS rule)

  // Step 6. PCO
  // section 6.2.4.2, TS 24.501
  // If the UE wants to use DHCPv4 for IPv4 address assignment, it shall
  // indicate that to the network within the Extended  protocol configuration
  // options IE in the PDU SESSION ESTABLISHMENT REQUEST  Extended protocol
  // configuration options: See subclause 10.5.6.3A in 3GPP TS 24.008.

  protocol_configuration_options_t pco_req = {};
  smreq->req.get_epco(pco_req);
  protocol_configuration_options_t pco_resp    = {};
  protocol_configuration_options_ids_t pco_ids = {
      .pi_ipcp                                     = 0,
      .ci_dns_server_ipv4_address_request          = 0,
      .ci_ip_address_allocation_via_nas_signalling = 0,
      .ci_ipv4_address_allocation_via_dhcpv4       = 0,
      .ci_ipv4_link_mtu_request                    = 0,
      .ci_dns_server_ipv6_address_request          = 0};

  smf_app_inst->process_pco_request(pco_req, pco_resp, pco_ids);
  sm_context_resp_pending->res.set_epco(pco_resp);

  // Step 7. Address allocation based on PDN type, IP Address pool is controlled
  // by SMF
  bool set_paa = false;
  paa_t paa    = {};
  Logger::smf_app().debug("UE Address Allocation");

  switch (sp->pdu_session_type.pdu_session_type) {
    case PDU_SESSION_TYPE_E_IPV4V6: {
      Logger::smf_app().debug(
          "PDU Session Type IPv4v6, select PDU Session Type IPv4");
      bool paa_res = false;
      // TODO: Verified if use default session type or requested session type
      std::shared_ptr<session_management_subscription> ss = {};
      std::shared_ptr<dnn_configuration_t> sdc            = {};
      find_dnn_subscription(snssai, ss);
      if (nullptr != ss.get()) {
        ss.get()->find_dnn_configuration(sd->dnn_in_use, sdc);
        if (nullptr != sdc.get()) {
          paa.pdu_session_type.pdu_session_type =
              sdc.get()
                  ->pdu_session_types.default_session_type.pdu_session_type;
        }
      }
      // TODO: use requested PDU Session Type?
      //     paa.pdu_session_type.pdu_session_type = PDU_SESSION_TYPE_E_IPV4V6;
      if ((not paa_res) || (not paa.is_ip_assigned())) {
        bool success =
            paa_dynamic::get_instance().get_free_paa(sd->dnn_in_use, paa);
        if (success) {
          set_paa = true;
        } else {
          // ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED;
          set_paa          = false;
          request_accepted = false;
          sm_context_resp->res.set_cause(static_cast<uint8_t>(
              cause_value_5gsm_e::CAUSE_26_INSUFFICIENT_RESOURCES));
        }
        // TODO: Static IP address allocation
      } else if ((paa_res) && (paa.is_ip_assigned())) {
        set_paa = true;
      }
      Logger::smf_app().info(
          "PAA, Ipv4 Address: %s",
          inet_ntoa(*((struct in_addr*) &paa.ipv4_address)));

      char str_addr6[INET6_ADDRSTRLEN];
      if (inet_ntop(
              AF_INET6, &paa.ipv6_address, str_addr6, sizeof(str_addr6))) {
        Logger::smf_app().info("PAA, IPv6 Address: %s", str_addr6);
      }

    }; break;
    case PDU_SESSION_TYPE_E_IPV4: {
      Logger::smf_app().debug("PDU Session Type IPv4");
      if (!pco_ids.ci_ipv4_address_allocation_via_dhcpv4) {
        // use SM NAS signalling
        // static or dynamic address allocation
        bool paa_res = false;  // how to define static or dynamic
        // depend of subscription information: staticIpAddress in DNN
        // Configuration
        // TODO: check static IP address is available in the subscription
        // information (SessionManagementSubscription) or in DHCP/DN-AAA

        std::shared_ptr<session_management_subscription> ss = {};
        std::shared_ptr<dnn_configuration_t> sdc            = {};
        find_dnn_subscription(snssai, ss);
        if (nullptr != ss.get()) {
          ss.get()->find_dnn_configuration(sd->dnn_in_use, sdc);
          if (nullptr != sdc.get()) {
            paa.pdu_session_type.pdu_session_type =
                sdc.get()
                    ->pdu_session_types.default_session_type
                    .pdu_session_type;  // TODO: Verified if use default session
                                        // type or requested session type

            // TODO: static ip address
          }
        }

        if ((not paa_res) || (not paa.is_ip_assigned())) {
          bool success =
              paa_dynamic::get_instance().get_free_paa(sd->dnn_in_use, paa);
          if (success) {
            set_paa = true;
          } else {
            // ALL_DYNAMIC_ADDRESSES_ARE_OCCUPIED;
            set_paa          = false;
            request_accepted = false;
            sm_context_resp->res.set_cause(static_cast<uint8_t>(
                cause_value_5gsm_e::CAUSE_26_INSUFFICIENT_RESOURCES));
          }
          // Static IP address allocation
        } else if ((paa_res) && (paa.is_ip_assigned())) {
          set_paa = true;
        }
        Logger::smf_app().info(
            "PAA, Ipv4 Address: %s",
            inet_ntoa(*((struct in_addr*) &paa.ipv4_address)));
      } else {  // use DHCP
        Logger::smf_app().info(
            "UE requests to use DHCPv4 for IPv4 address assignment, this "
            "feature has not been supported yet!");
        // TODO
      }

    } break;

    case PDU_SESSION_TYPE_E_IPV6: {
      // TODO:
      Logger::smf_app().debug("IPv6 has not been supported yet!");
    } break;

    default: {
      Logger::smf_app().error(
          "Unknown PDN type %d", sp->pdu_session_type.pdu_session_type);
      // PDU Session Establishment Reject
      if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
              smreq->req, n1_sm_message,
              cause_value_5gsm_e::CAUSE_28_UNKNOWN_PDU_SESSION_TYPE)) {
        smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_msg_hex);
        // trigger to send reply to AMF
        smf_app_inst->trigger_create_context_error_response(
            http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
            PDU_SESSION_APPLICATION_ERROR_PDUTYPE_DENIED, n1_sm_msg_hex,
            smreq->pid);
      } else {
        smf_app_inst->trigger_http_response(
            http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
            smreq->pid, N11_SESSION_CREATE_SM_CONTEXT_RESPONSE);
      }
      // sm_context_resp->res.set_cause(static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_28_UNKNOWN_PDU_SESSION_TYPE));
      request_accepted = false;
    }
  }

  // TODO: Step 8. SMF-initiated SM Policy Modification (with PCF)

  // Step 9. Create session establishment procedure and run the procedure
  // if request is accepted
  if (request_accepted) {
    if (set_paa) {
      sm_context_resp_pending->res.set_paa(paa);
      sp->set(paa);
    } else {
      // TODO:
    }

    // Store AMF callback URI and subscribe to the status notification: AMF will
    // be notified when SM context changes
    std::shared_ptr<smf_context_ref> scf = {};
    if (smf_app_inst->is_scid_2_smf_context(smreq->scid)) {
      scf = smf_app_inst->scid_2_smf_context(smreq->scid);
    } else {
      Logger::smf_app().warn(
          "SM Context associated with this id " SCID_FMT " does not exit!",
          smreq->scid);
      // TODO: return;
    }
    scf.get()->amf_status_uri = smreq->req.get_sm_context_status_uri();

    // Get and Store AMF Addr if available
    std::vector<std::string> split_result;
    std::string amf_addr_str = {};
    amf_addr_str             = std::string(inet_ntoa(
                       *((struct in_addr*) &smf_cfg.amf_addr.ipv4_addr))) +
                   ":" + std::to_string(smf_cfg.amf_addr.port);

    boost::split(
        split_result, scf.get()->amf_status_uri, boost::is_any_of("/"));
    if (split_result.size() >= 3) {
      std::string addr = split_result[2];
      struct in_addr amf_ipv4_addr;
      if (inet_aton(util::trim(addr).c_str(), &amf_ipv4_addr) == 0) {
        Logger::smf_api_server().warn("Bad IPv4 for AMF");
      } else {
        amf_addr_str = addr;
        Logger::smf_api_server().debug("AMF IP Addr %s", amf_addr_str.c_str());
      }
    }
    scf.get()->amf_addr = amf_addr_str;
    sp.get()->set_amf_addr(amf_addr_str);

    // Trigger SMF APP to send response to SMF-HTTP-API-SERVER (Step
    // 5, 4.3.2.2.1 TS 23.502)
    Logger::smf_app().debug(
        "Send ITTI msg to SMF APP to trigger the response of Server");
    std::shared_ptr<itti_n11_create_sm_context_response> itti_msg =
        std::make_shared<itti_n11_create_sm_context_response>(
            TASK_SMF_SBI, TASK_SMF_APP, smreq->pid);

    pdu_session_create_sm_context_response sm_context_response = {};
    std::string smContextRef = std::to_string(smreq->scid);
    // headers: Location: contains the URI of the newly created resource,
    // according to the structure:
    // {apiRoot}/nsmf-pdusession/{apiVersion}/sm-contexts/{smContextRef}
    std::string smf_context_uri =
        smreq->req.get_api_root() + "/" + smContextRef.c_str();
    sm_context_response.set_smf_context_uri(smf_context_uri);
    sm_context_response.set_cause(static_cast<uint8_t>(
        cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));  // TODO

    nlohmann::json json_data = {};
    json_data["cause"]       = 0;
    sm_context_response.set_json_data(json_data);
    sm_context_response.set_http_code(
        http_status_code_e::HTTP_STATUS_CODE_201_CREATED);
    itti_msg->res = sm_context_response;

    int ret = itti_inst->send_msg(itti_msg);
    if (RETURNok != ret) {
      Logger::smf_app().error(
          "Could not send ITTI message %s to task TASK_SMF_APP",
          itti_msg->get_msg_name());
    }

    // TODO: PDU Session authentication/authorization (Optional)
    // see section 4.3.2.3@3GPP TS 23.502 and section 6.3.1@3GPP TS 24.501

    Logger::smf_app().info("Create a procedure to process this message.");
    session_create_sm_context_procedure* proc =
        new session_create_sm_context_procedure(sp);
    std::shared_ptr<smf_procedure> sproc = std::shared_ptr<smf_procedure>(proc);

    insert_procedure(sproc);
    if (proc->run(smreq, sm_context_resp_pending, shared_from_this())) {
      // error !
      Logger::smf_app().info(
          "PDU Session Establishment Request: Create SM Context Request "
          "procedure failed");
      remove_procedure(proc);
      // Set cause to error to trigger PDU session establishment reject (step
      // 10)
      sm_context_resp->res.set_cause(
          PDU_SESSION_APPLICATION_ERROR_PEER_NOT_RESPONDING);
    }
  } else {  // if request is rejected
    // TODO:
    // un-subscribe to the modifications of Session Management Subscription data
    // for (SUPI, DNN, S-NSSAI)
  }

  // Step 10. if error when establishing the pdu session,
  // send ITTI message to APP to trigger N1N2MessageTransfer towards AMFs (PDU
  // Session Establishment Reject)
  if (sm_context_resp->res.get_cause() !=
      static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED)) {
    // clear pco, ambr

    // free paa
    paa_t free_paa = {};
    free_paa       = sm_context_resp->res.get_paa();
    if (free_paa.is_ip_assigned()) {
      switch (sp->pdu_session_type.pdu_session_type) {
        case PDU_SESSION_TYPE_E_IPV4:
        case PDU_SESSION_TYPE_E_IPV4V6:
          // paa_dynamic::get_instance().release_paa(
          //    sd->dnn_in_use, free_paa.ipv4_address);
          // break;
        case PDU_SESSION_TYPE_E_IPV6:
          paa_dynamic::get_instance().release_paa(sd->dnn_in_use, free_paa);
          break;
        case PDU_SESSION_TYPE_E_UNSTRUCTURED:
        case PDU_SESSION_TYPE_E_ETHERNET:
        case PDU_SESSION_TYPE_E_RESERVED:
        default:;
      }
    }
    // clear the created context??
    // TODO:

    // Create PDU Session Establishment Reject and embedded in
    // Namf_Communication_N1N2MessageTransfer Request
    Logger::smf_app().debug("Create PDU Session Establishment Reject");
    cause_value_5gsm_e cause_n1 =
        static_cast<cause_value_5gsm_e>(sm_context_resp->res.get_cause());

    smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
        sm_context_resp_pending->res, n1_sm_message, cause_n1);
    smf_app_inst->convert_string_2_hex(n1_sm_message, n1_sm_msg_hex);
    sm_context_resp_pending->res.set_n1_sm_message(n1_sm_msg_hex);

    // get supi and put into URL
    std::string supi_str;
    supi_t supi = sm_context_resp_pending->res.get_supi();
    supi_str    = sm_context_resp_pending->res.get_supi_prefix() + "-" +
               smf_supi_to_string(supi);
    std::string url =
        //     std::string(
        //    inet_ntoa(*((struct in_addr*) &smf_cfg.amf_addr.ipv4_addr))) +
        // ":" + std::to_string(smf_cfg.amf_addr.port) + NAMF_COMMUNICATION_BASE
        // +
        sp.get()->get_amf_addr() + NAMF_COMMUNICATION_BASE +
        smf_cfg.amf_addr.api_version +
        fmt::format(
            NAMF_COMMUNICATION_N1N2_MESSAGE_TRANSFER_URL, supi_str.c_str());
    sm_context_resp_pending->res.set_amf_url(url);

    // Fill the json part
    nlohmann::json json_data                          = {};
    json_data["n1MessageContainer"]["n1MessageClass"] = N1N2_MESSAGE_CLASS;
    json_data["n1MessageContainer"]["n1MessageContent"]["contentId"] =
        N1_SM_CONTENT_ID;
    json_data["pduSessionId"] =
        sm_context_resp_pending->res.get_pdu_session_id();
    sm_context_resp_pending->res.set_json_data(json_data);

    // send ITTI message to N11 to trigger N1N2MessageTransfer towards AMFs
    Logger::smf_app().info(
        "Sending ITTI message %s to task TASK_SMF_SBI",
        sm_context_resp_pending->get_msg_name());
    int ret = itti_inst->send_msg(sm_context_resp_pending);
    if (RETURNok != ret) {
      Logger::smf_app().error(
          "Could not send ITTI message %s to task TASK_SMF_SBI",
          sm_context_resp_pending->get_msg_name());
    }
  }
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_modification_request(
    nas_message_t& nas_msg,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request,
    std::shared_ptr<itti_n11_update_sm_context_response>& sm_context_resp,
    std::shared_ptr<smf_pdu_session>& sp) {
  Logger::smf_app().debug("PDU_SESSION_MODIFICATION_REQUEST");

  sm_context_resp.get()->session_procedure_type =
      session_management_procedures_type_e::
          PDU_SESSION_MODIFICATION_UE_INITIATED_STEP1;

  // check if the PDU Session Release Command is already sent for this
  // message (see section 6.3.3.5 @3GPP TS 24.501)
  if (sp.get()->get_pdu_session_status() ==
      pdu_session_status_e::PDU_SESSION_INACTIVE_PENDING) {
    // Ignore the message
    Logger::smf_app().info(
        "A PDU Session Release Command has been sent for this session "
        "(session ID %d), ignore the message!",
        nas_msg.plain.sm.header.pdu_session_identity);
    return false;
  }

  // check if the session is in state Modification pending, SMF will
  // ignore this message (see section 6.3.2.5 @3GPP TS 24.501)
  if (sp.get()->get_pdu_session_status() ==
      pdu_session_status_e::PDU_SESSION_MODIFICATION_PENDING) {
    // Ignore the message
    Logger::smf_app().info(
        "This PDU session is in MODIFICATION_PENDING State (session ID "
        "%d), ignore the message!",
        nas_msg.plain.sm.header.pdu_session_identity);
    return false;
  }

  // See section 6.4.2 - UE-requested PDU Session modification procedure@
  // 3GPP TS 24.501  Verify PDU Session Identity
  if (sm_context_request.get()->req.get_pdu_session_id() !=
      nas_msg.plain.sm.header.pdu_session_identity) {
    // TODO: PDU Session ID mismatch
  }

  // PTI
  Logger::smf_app().info(
      "PTI %d", nas_msg.plain.sm.header.procedure_transaction_identity);
  procedure_transaction_id_t pti = {
      .procedure_transaction_id =
          nas_msg.plain.sm.header.procedure_transaction_identity};
  sm_context_resp.get()->res.set_pti(pti);

  // Message Type
  // TODO: _5GSMCapability
  // TODO: Cause
  // TODO: maximum_number_of_supported_packet_filters
  // TODO: AlwaysonPDUSessionRequested
  // TODO: IntergrityProtectionMaximumDataRate

  // Process QoS rules and Qos Flow descriptions
  update_qos_info(sp, sm_context_resp.get()->res, nas_msg);

  // TODO: MappedEPSBearerContexts
  // TODO: ExtendedProtocolConfigurationOptions

  // section 6.3.2. Network-requested PDU Session modification procedure @
  // 3GPP TS 24.501  requested QoS rules (including packet filters) and/or
  // requested QoS flow descriptions  session-AMBR, session TMBR
  // PTI or UE capability

  // Create a N1 SM (PDU Session Modification Command) and N2 SM (PDU
  // Session Resource Modify Request Transfer IE)
  std::string n1_sm_msg_to_be_created, n1_sm_msg_hex_to_be_created;
  std::string n2_sm_info_to_be_created, n2_sm_info_hex_to_be_created;
  // N1 SM (PDU Session Modification Command)
  if (not smf_n1::get_instance().create_n1_pdu_session_modification_command(
          sm_context_resp.get()->res, n1_sm_msg_to_be_created,
          cause_value_5gsm_e::CAUSE_0_UNKNOWN) or
      // N2 SM (PDU Session Resource Modify Request Transfer IE)
      not smf_n2::get_instance()
              .create_n2_pdu_session_resource_modify_request_transfer(
                  sm_context_resp.get()->res,
                  n2_sm_info_type_e::PDU_RES_MOD_REQ,
                  n2_sm_info_to_be_created)) {
    smf_app_inst->trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
        sm_context_request.get()->pid, N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE);

    free_wrapper((void**) &nas_msg.plain.sm.pdu_session_modification_request
                     .qosflowdescriptions.qosflowdescriptionscontents);
    return false;
  }

  smf_app_inst->convert_string_2_hex(
      n1_sm_msg_to_be_created, n1_sm_msg_hex_to_be_created);
  smf_app_inst->convert_string_2_hex(
      n2_sm_info_to_be_created, n2_sm_info_hex_to_be_created);

  sm_context_resp.get()->res.set_n1_sm_message(n1_sm_msg_hex_to_be_created);
  sm_context_resp.get()->res.set_n2_sm_information(
      n2_sm_info_hex_to_be_created);
  sm_context_resp.get()->res.set_n2_sm_info_type("PDU_RES_MOD_REQ");

  // Fill the json part
  nlohmann::json json_data = {};
  // N1SM
  json_data["n1MessageContainer"]["n1MessageClass"] = N1N2_MESSAGE_CLASS;
  json_data["n1MessageContainer"]["n1MessageContent"]["contentId"] =
      N1_SM_CONTENT_ID;
  json_data["n2InfoContainer"]["n2InformationClass"] = N1N2_MESSAGE_CLASS;
  json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapIeType"] =
      "PDU_RES_MOD_REQ";  // NGAP message
  json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapData"]
           ["contentId"] = N2_SM_CONTENT_ID;
  json_data["n2InfoContainer"]["smInfo"]["PduSessionId"] =
      sm_context_resp.get()->res.get_pdu_session_id();

  sm_context_resp.get()->res.set_json_data(json_data);
  // Update PDU Session status
  sp.get()->set_pdu_session_status(
      pdu_session_status_e::PDU_SESSION_MODIFICATION_PENDING);
  // start timer T3591
  // get smf_pdu_session and set the corresponding timer
  sp.get()->timer_T3591 = itti_inst->timer_setup(
      T3591_TIMER_VALUE_SEC, 0, TASK_SMF_APP, TASK_SMF_APP_TRIGGER_T3591,
      sm_context_request.get()->req.get_pdu_session_id());

  free_wrapper((void**) &nas_msg.plain.sm.pdu_session_modification_request
                   .qosflowdescriptions.qosflowdescriptionscontents);
  return true;
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_modification_complete(
    nas_message_t& nas_msg,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request,
    std::shared_ptr<itti_n11_update_sm_context_response>& sm_context_resp,
    std::shared_ptr<smf_pdu_session>& sp) {
  /* see section 6.3.2.3@3GPP TS 24.501 V16.1.0
   Upon receipt of a PDU SESSION MODIFICATION COMPLETE message, the SMF
   shall stop timer T3591 and shall consider the PDU session as modified.
   If the selected SSC mode of the PDU session is "SSC mode 3" and the PDU
   SESSION MODIFICATION COMMAND message included 5GSM cause #39
   "reactivation requested", the SMF shall start timer T3593. If the PDU
   Session Address Lifetime value is sent to the UE in the PDU SESSION
   MODIFICATION COMMAND message then timer T3593 shall be started with the
   same value, otherwise it shall use a default value.
   */
  // Update PDU Session status -> ACTIVE
  sp.get()->set_pdu_session_status(pdu_session_status_e::PDU_SESSION_ACTIVE);
  // stop T3591
  itti_inst->timer_remove(sp.get()->timer_T3591);

  sm_context_resp.get()->session_procedure_type =
      session_management_procedures_type_e::
          PDU_SESSION_MODIFICATION_UE_INITIATED_STEP3;
  return true;
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_modification_command_reject(
    nas_message_t& nas_msg,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request,
    std::shared_ptr<itti_n11_update_sm_context_response>& sm_context_resp,
    std::shared_ptr<smf_pdu_session>& sp) {
  sm_context_resp.get()->session_procedure_type =
      session_management_procedures_type_e::
          PDU_SESSION_MODIFICATION_UE_INITIATED_STEP3;

  // Verify PDU Session Identity
  if (sm_context_request.get()->req.get_pdu_session_id() !=
      nas_msg.plain.sm.header.pdu_session_identity) {
    // TODO: PDU Session ID mismatch
  }

  Logger::smf_app().info(
      "PTI %d", nas_msg.plain.sm.header.procedure_transaction_identity);
  procedure_transaction_id_t pti = {
      .procedure_transaction_id =
          nas_msg.plain.sm.header.procedure_transaction_identity};
  sm_context_resp.get()->res.set_pti(pti);

  // Message Type
  //_5GSMCause
  // presence
  // ExtendedProtocolConfigurationOptions

  if (nas_msg.plain.sm.pdu_session_modification_command_reject._5gsmcause ==
      static_cast<uint8_t>(
          cause_value_5gsm_e::CAUSE_43_INVALID_PDU_SESSION_IDENTITY)) {
    // Update PDU Session status -> INACTIVE
    sp.get()->set_pdu_session_status(
        pdu_session_status_e::PDU_SESSION_INACTIVE);
    // TODO: Release locally the existing PDU Session (see
    // section 6.3.2.5@3GPP TS 24.501)
  } else if (
      sp.get()->get_pdu_session_status() ==
      pdu_session_status_e::PDU_SESSION_MODIFICATION_PENDING) {
    // Update PDU Session status -> ACTIVE
    sp.get()->set_pdu_session_status(pdu_session_status_e::PDU_SESSION_ACTIVE);
  }

  // stop T3591
  itti_inst->timer_remove(sp.get()->timer_T3591);

  return true;
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_release_request(
    nas_message_t& nas_msg,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request,
    std::shared_ptr<itti_n11_update_sm_context_response>& sm_context_resp,
    std::shared_ptr<smf_pdu_session>& sp) {
  std::string n1_sm_msg, n1_sm_msg_hex;
  std::string n2_sm_info, n2_sm_info_hex;

  sm_context_resp.get()->session_procedure_type =
      session_management_procedures_type_e::
          PDU_SESSION_RELEASE_UE_REQUESTED_STEP1;

  // verify PDU Session ID
  if (sm_context_request.get()->req.get_pdu_session_id() !=
      nas_msg.plain.sm.header.pdu_session_identity) {
    // TODO: PDU Session ID mismatch
  }

  // Abnormal cases in network side (see section 6.4.3.6 @3GPP TS 24.501)
  if (sp.get()->get_pdu_session_status() ==
      pdu_session_status_e::PDU_SESSION_INACTIVE) {
    Logger::smf_app().warn(
        "PDU Session status: INACTIVE, send PDU Session Release Reject "
        "to UE!");
    if (smf_n1::get_instance().create_n1_pdu_session_release_reject(
            sm_context_request.get()->req, n1_sm_msg,
            cause_value_5gsm_e::CAUSE_43_INVALID_PDU_SESSION_IDENTITY)) {
      smf_app_inst->convert_string_2_hex(n1_sm_msg, n1_sm_msg_hex);
      // trigger to send reply to AMF
      smf_app_inst->trigger_update_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
          PDU_SESSION_APPLICATION_ERROR_NETWORK_FAILURE, n1_sm_msg_hex,
          sm_context_request.get()->pid);
    } else {
      smf_app_inst->trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
          sm_context_request.get()->pid,
          N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE);
    }
    return false;
  }
  // Abnormal cases in network side (see section 6.3.3.5 @3GPP TS 24.501)
  if (sp.get()->get_pdu_session_status() ==
      pdu_session_status_e::PDU_SESSION_INACTIVE_PENDING) {
    // Ignore the message
    Logger::smf_app().info(
        "A PDU Session Release Command has been sent for this session "
        "(session ID %d), ignore the message!",
        nas_msg.plain.sm.header.pdu_session_identity);
    return false;
  }

  Logger::smf_app().info(
      "PTI %d", nas_msg.plain.sm.header.procedure_transaction_identity);
  procedure_transaction_id_t pti = {
      .procedure_transaction_id =
          nas_msg.plain.sm.header.procedure_transaction_identity};
  sm_context_resp.get()->res.set_pti(pti);

  // Message Type
  // Presence
  // 5GSM Cause
  // Extended Protocol Configuration Options

  // Release the resources related to this PDU Session (in Procedure)

  // get the associated QoS flows: to be used for PFCP Session
  // Modification procedure
  std::vector<smf_qos_flow> qos_flows;
  sp.get()->get_qos_flows(qos_flows);
  for (auto i : qos_flows) {
    sm_context_request.get()->req.add_qfi(i.qfi.qfi);
  }

  return true;
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_release_complete(
    nas_message_t& nas_msg,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request,
    std::shared_ptr<itti_n11_update_sm_context_response>& sm_context_resp,
    std::shared_ptr<smf_pdu_session>& sp) {
  sm_context_resp.get()->session_procedure_type =
      session_management_procedures_type_e::
          PDU_SESSION_RELEASE_UE_REQUESTED_STEP3;

  // verify PDU Session ID
  if (sm_context_request.get()->req.get_pdu_session_id() !=
      nas_msg.plain.sm.header.pdu_session_identity) {
    // TODO: PDU Session ID mismatch
  }

  Logger::smf_app().info(
      "PTI %d", nas_msg.plain.sm.header.procedure_transaction_identity);
  procedure_transaction_id_t pti = {
      .procedure_transaction_id =
          nas_msg.plain.sm.header.procedure_transaction_identity};
  if (nas_msg.plain.sm.header.message_type != PDU_SESSION_RELEASE_COMPLETE) {
    // TODO: Message Type mismatch
  }
  // 5GSM Cause
  // Extended Protocol Configuration Options

  // Update PDU Session status -> INACTIVE
  sp.get()->set_pdu_session_status(pdu_session_status_e::PDU_SESSION_INACTIVE);
  // Stop timer T3592
  itti_inst->timer_remove(sp.get()->timer_T3592);

  // TODO: SMF invokes Nsmf_PDUSession_SMContextStatusNotify to notify AMF
  // that the SM context for this PDU Session is released
  scid_t scid = {};
  try {
    scid = std::stoi(sm_context_request.get()->scid);
  } catch (const std::exception& err) {
    Logger::smf_app().warn(
        "Received a PDU Session Update SM Context Request, couldn't "
        "retrieve the corresponding SMF context, ignore message!");
    // TODO: return;
  }

  Logger::smf_app().debug("Signal the SM Context Status Change");
  std::string status = "RELEASED";
  event_sub.sm_context_status(
      scid, status, sm_context_request.get()->http_version);

  // Trigger PDU Session Release event notification
  supi64_t supi64 = smf_supi_to_u64(sm_context_request.get()->req.get_supi());
  Logger::smf_app().debug("Signal the PDU Session Release Event notification");
  event_sub.ee_pdu_session_release(
      supi64, sm_context_request.get()->req.get_pdu_session_id(),
      sm_context_request.get()->http_version);

  // TODO: if dynamic PCC applied, SMF invokes an SM Policy Association
  // Termination
  // TODO: SMF un-subscribes from Session Management Subscription data
  // changes notification from UDM by invoking Numd_SDM_Unsubscribe

  // TODO: should check if sd context exist
  std::shared_ptr<dnn_context> sd = {};
  find_dnn_context(
      sm_context_request.get()->req.get_snssai(),
      sm_context_request.get()->req.get_dnn(), sd);

  if (sd.get() != nullptr) {
    if (sd.get()->get_number_pdu_sessions() == 0) {
      Logger::smf_app().debug(
          "Unsubscribe from Session Management Subscription data changes "
          "notification from UDM");
      // TODO: unsubscribes from Session Management Subscription data
      // changes notification from UDM
    }
  }
  // TODO: Invoke Nudm_UECM_Deregistration
  return true;
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_resource_setup_response_transfer(
    std::string& n2_sm_information,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request) {
  std::string n1_sm_msg, n1_sm_msg_hex;

  // Ngap_PDUSessionResourceSetupResponseTransfer
  std::shared_ptr<Ngap_PDUSessionResourceSetupResponseTransfer_t> decoded_msg =
      std::make_shared<Ngap_PDUSessionResourceSetupResponseTransfer_t>();
  int decode_status = smf_n2::get_instance().decode_n2_sm_information(
      decoded_msg, n2_sm_information);
  if (decode_status == RETURNerror) {
    // error, send error to AMF
    Logger::smf_app().warn(
        "Decode N2 SM (Ngap_PDUSessionResourceSetupResponseTransfer) "
        "failed!");
    // PDU Session Establishment Reject
    // 24.501: response with a 5GSM STATUS message including cause "#95
    // Semantically incorrect message"
    if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
            sm_context_request.get()->req, n1_sm_msg,
            cause_value_5gsm_e::CAUSE_95_SEMANTICALLY_INCORRECT_MESSAGE)) {
      smf_app_inst->convert_string_2_hex(n1_sm_msg, n1_sm_msg_hex);
      // trigger to send reply to AMF
      smf_app_inst->trigger_update_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
          PDU_SESSION_APPLICATION_ERROR_N2_SM_ERROR,
          sm_context_request.get()->pid);

    } else {
      smf_app_inst->trigger_http_response(
          http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
          sm_context_request.get()->pid,
          N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE);
    }
    return false;
  }

  // store AN Tunnel Info + list of accepted QFIs
  pfcp::fteid_t dl_teid = {};
  memcpy(
      &dl_teid.teid,
      decoded_msg->dLQosFlowPerTNLInformation.uPTransportLayerInformation.choice
          .gTPTunnel->gTP_TEID.buf,
      TEID_GRE_KEY_LENGTH);
  memcpy(
      &dl_teid.ipv4_address,
      decoded_msg->dLQosFlowPerTNLInformation.uPTransportLayerInformation.choice
          .gTPTunnel->transportLayerAddress.buf,
      4);

  dl_teid.teid = ntohl(dl_teid.teid);
  dl_teid.v4   = 1;  // Only V4 for now
  sm_context_request.get()->req.set_dl_fteid(dl_teid);

  Logger::smf_app().debug(
      "DL GTP F-TEID (AN F-TEID) "
      "0x%" PRIx32 " ",
      dl_teid.teid);
  Logger::smf_app().debug(
      "uPTransportLayerInformation (AN IP Addr) %s",
      conv::toString(dl_teid.ipv4_address).c_str());

  for (int i = 0;
       i <
       decoded_msg->dLQosFlowPerTNLInformation.associatedQosFlowList.list.count;
       i++) {
    pfcp::qfi_t qfi((uint8_t)(decoded_msg->dLQosFlowPerTNLInformation
                                  .associatedQosFlowList.list.array[i])
                        ->qosFlowIdentifier);
    sm_context_request.get()->req.add_qfi(qfi);
    Logger::smf_app().debug(
        "QoSFlowPerTNLInformation, AssociatedQosFlowList, QFI %d",
        (decoded_msg->dLQosFlowPerTNLInformation.associatedQosFlowList.list
             .array[i])
            ->qosFlowIdentifier);
  }
  return true;
}
//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_resource_setup_unsuccessful_transfer(
    std::string& n2_sm_information,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request) {
  std::string n1_sm_msg, n1_sm_msg_hex;

  // Ngap_PDUSessionResourceSetupUnsuccessfulTransfer
  std::shared_ptr<Ngap_PDUSessionResourceSetupUnsuccessfulTransfer_t>
      decoded_msg = std::make_shared<
          Ngap_PDUSessionResourceSetupUnsuccessfulTransfer_t>();
  int decode_status = smf_n2::get_instance().decode_n2_sm_information(
      decoded_msg, n2_sm_information);

  if (decode_status == RETURNerror) {
    Logger::smf_app().warn(
        "Decode N2 SM (Ngap_PDUSessionResourceSetupUnsuccessfulTransfer) "
        "failed!");
    // trigger to send reply to AMF
    smf_app_inst->trigger_update_context_error_response(
        http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
        PDU_SESSION_APPLICATION_ERROR_N2_SM_ERROR,
        sm_context_request.get()->pid);
    return false;
  }

  // PDU Session Establishment Reject, 24.501 cause "#26 Insufficient resources"
  if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
          sm_context_request.get()->req, n1_sm_msg,
          cause_value_5gsm_e::CAUSE_26_INSUFFICIENT_RESOURCES)) {
    smf_app_inst->convert_string_2_hex(n1_sm_msg, n1_sm_msg_hex);
    // trigger to send reply to AMF
    smf_app_inst->trigger_update_context_error_response(
        http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
        PDU_SESSION_APPLICATION_ERROR_UE_NOT_RESPONDING, n1_sm_msg_hex,
        sm_context_request.get()->pid);

    // TODO: Need release established resources?
  } else {
    smf_app_inst->trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
        sm_context_request.get()->pid, N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE);
  }
  return true;
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_resource_modify_response_transfer(
    std::string& n2_sm_information,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request) {
  std::string n1_sm_msg, n1_sm_msg_hex;

  // Ngap_PDUSessionResourceModifyResponseTransfer
  std::shared_ptr<Ngap_PDUSessionResourceModifyResponseTransfer_t> decoded_msg =
      std::make_shared<Ngap_PDUSessionResourceModifyResponseTransfer_t>();
  int decode_status = smf_n2::get_instance().decode_n2_sm_information(
      decoded_msg, n2_sm_information);

  if (decode_status == RETURNerror) {
    Logger::smf_app().warn(
        "Decode N2 SM (Ngap_PDUSessionResourceModifyResponseTransfer) "
        "failed!");
    // trigger to send reply to AMF
    smf_app_inst->trigger_update_context_error_response(
        http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
        PDU_SESSION_APPLICATION_ERROR_N2_SM_ERROR,
        sm_context_request.get()->pid);
    return false;
  }

  // see section 8.2.3 (PDU Session Resource Modify) @3GPP TS 38.413
  // if dL_NGU_UP_TNLInformation is included, it shall be considered as
  // the new DL transport layer addr for the PDU session (should be
  // verified)
  // TODO: may include uL_NGU_UP_TNLInformation (mapping between each new
  // DL transport layer address and the corresponding UL transport layer
  // address)
  pfcp::fteid_t dl_teid;
  memcpy(
      &dl_teid.teid,
      decoded_msg->dL_NGU_UP_TNLInformation->choice.gTPTunnel->gTP_TEID.buf,
      TEID_GRE_KEY_LENGTH);
  memcpy(
      &dl_teid.ipv4_address,
      decoded_msg->dL_NGU_UP_TNLInformation->choice.gTPTunnel
          ->transportLayerAddress.buf,
      4);

  dl_teid.teid = ntohl(dl_teid.teid);
  dl_teid.v4   = 1;  // Only v4 for now
  sm_context_request.get()->req.set_dl_fteid(dl_teid);

  // list of Qos Flows which have been successfully setup or modified
  if (decoded_msg->qosFlowAddOrModifyResponseList) {
    for (int i = 0; i < decoded_msg->qosFlowAddOrModifyResponseList->list.count;
         i++) {
      sm_context_request.get()->req.add_qfi(
          (decoded_msg->qosFlowAddOrModifyResponseList->list.array[i])
              ->qosFlowIdentifier);
    }
  }

  // TODO: list of QoS Flows which have failed to be modified,
  // qosFlowFailedToAddOrModifyList
  // TODO: additionalDLQosFlowPerTNLInformation
  return true;
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_pdu_session_resource_release_response_transfer(
    std::string& n2_sm_information,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request) {
  std::string n1_sm_msg, n1_sm_msg_hex;

  // TODO: SMF does nothing (Step 7, section 4.3.4.2@3GPP TS 23.502)
  // Ngap_PDUSessionResourceReleaseResponseTransfer
  std::shared_ptr<Ngap_PDUSessionResourceReleaseResponseTransfer_t>
      decoded_msg =
          std::make_shared<Ngap_PDUSessionResourceReleaseResponseTransfer_t>();
  int decode_status = smf_n2::get_instance().decode_n2_sm_information(
      decoded_msg, n2_sm_information);
  if (decode_status == RETURNerror) {
    Logger::smf_app().warn(
        "Decode N2 SM (Ngap_PDUSessionResourceReleaseResponseTransfer) "
        "failed!");
    // trigger to send reply to AMF
    smf_app_inst->trigger_update_context_error_response(
        http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
        PDU_SESSION_APPLICATION_ERROR_N2_SM_ERROR,
        sm_context_request.get()->pid);

    return false;
  }

  smf_app_inst->trigger_http_response(
      http_status_code_e::HTTP_STATUS_CODE_200_OK,
      sm_context_request.get()->pid, N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE);

  return true;
}

//-------------------------------------------------------------------------------------
bool smf_context::handle_service_request(
    std::string& n2_sm_information,
    std::shared_ptr<itti_n11_update_sm_context_request>& sm_context_request,
    std::shared_ptr<itti_n11_update_sm_context_response>& sm_context_resp,
    std::shared_ptr<smf_pdu_session>& sp) {
  std::string n2_sm_info, n2_sm_info_hex;

  // Update upCnxState
  sp.get()->set_upCnx_state(upCnx_state_e::UPCNX_STATE_ACTIVATING);

  // get QFIs associated with PDU session ID
  std::vector<smf_qos_flow> qos_flows = {};
  sp.get()->get_qos_flows(qos_flows);
  for (auto i : qos_flows) {
    sm_context_request.get()->req.add_qfi(i.qfi.qfi);

    qos_flow_context_updated qcu = {};
    qcu.set_cause(
        static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
    qcu.set_qfi(i.qfi);
    qcu.set_ul_fteid(i.ul_fteid);
    qcu.set_qos_profile(i.qos_profile);
    sm_context_resp.get()->res.add_qos_flow_context_updated(qcu);
  }

  sm_context_resp.get()->session_procedure_type =
      session_management_procedures_type_e::SERVICE_REQUEST_UE_TRIGGERED_STEP1;

  // Create N2 SM Information: PDU Session Resource Setup Request Transfer IE
  // N2 SM Information
  smf_n2::get_instance().create_n2_pdu_session_resource_setup_request_transfer(
      sm_context_resp.get()->res, n2_sm_info_type_e::PDU_RES_SETUP_REQ,
      n2_sm_info);

  smf_app_inst->convert_string_2_hex(n2_sm_info, n2_sm_info_hex);
  sm_context_resp.get()->res.set_n2_sm_information(n2_sm_info_hex);

  // fill the content of SmContextUpdatedData
  nlohmann::json json_data                           = {};
  json_data["n2InfoContainer"]["n2InformationClass"] = N1N2_MESSAGE_CLASS;
  json_data["n2InfoContainer"]["smInfo"]["PduSessionId"] =
      sm_context_resp.get()->res.get_pdu_session_id();
  json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapData"]
           ["contentId"] = N2_SM_CONTENT_ID;
  json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapIeType"] =
      "PDU_RES_SETUP_REQ";  // NGAP message
  json_data["upCnxState"] = "ACTIVATING";
  sm_context_resp.get()->res.set_json_data(json_data);

  // Update upCnxState to ACTIVATING
  sp.get()->set_upCnx_state(upCnx_state_e::UPCNX_STATE_ACTIVATING);

  // TODO: If new UPF is used, need to send N4 Session Modification
  // Request/Response to new/old UPF

  // Accept the activation of UP connection and continue to using the current
  // UPF
  // TODO: Accept the activation of UP connection and select a new UPF
  // Reject the activation of UP connection
  // SMF fails to find a suitable I-UPF: i) trigger re-establishment of PDU
  // Session;  or ii) keep PDU session but reject the activation of UP
  // connection;  or iii) release PDU session

  return true;
}

//-------------------------------------------------------------------------------------
void smf_context::handle_pdu_session_update_sm_context_request(
    std::shared_ptr<itti_n11_update_sm_context_request> smreq) {
  Logger::smf_app().info(
      "Handle a PDU Session Update SM Context Request message from an AMF "
      "(HTTP version %d)",
      smreq->http_version);
  pdu_session_update_sm_context_request sm_context_req_msg = smreq->req;
  std::string n1_sm_msg, n1_sm_msg_hex;
  std::string n2_sm_info, n2_sm_info_hex;
  bool update_upf = false;
  session_management_procedures_type_e procedure_type(
      session_management_procedures_type_e::
          PDU_SESSION_ESTABLISHMENT_UE_REQUESTED);

  // Step 1. get DNN, SMF PDU session context. At this stage, dnn_context and
  // pdu_session must be existed
  std::shared_ptr<dnn_context> sd     = {};
  std::shared_ptr<smf_pdu_session> sp = {};
  bool find_dnn                       = find_dnn_context(
      sm_context_req_msg.get_snssai(), sm_context_req_msg.get_dnn(), sd);
  bool find_pdu = false;
  if (find_dnn) {
    find_pdu =
        sd.get()->find_pdu_session(sm_context_req_msg.get_pdu_session_id(), sp);
  }
  if (!find_dnn or !find_pdu) {
    // error, send reply to AMF with error code "Context Not Found"
    Logger::smf_app().warn("DNN or PDU session context does not exist!");
    // trigger to send reply to AMF
    smf_app_inst->trigger_update_context_error_response(
        http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND,
        PDU_SESSION_APPLICATION_ERROR_CONTEXT_NOT_FOUND, smreq->pid);
    return;
  }

  // we need to store HttpResponse and session-related information to be used
  // when receiving the response from UPF
  itti_n11_update_sm_context_response* n11_sm_context_resp =
      new itti_n11_update_sm_context_response(
          TASK_SMF_SBI, TASK_SMF_APP, smreq->pid);
  std::shared_ptr<itti_n11_update_sm_context_response> sm_context_resp_pending =
      std::shared_ptr<itti_n11_update_sm_context_response>(n11_sm_context_resp);

  n11_sm_context_resp->res.set_pdu_session_type(
      sp.get()->get_pdu_session_type().pdu_session_type);

  // Assign necessary information for the response
  xgpp_conv::update_sm_context_response_from_ct_request(
      smreq, sm_context_resp_pending);

  // Step 2.1. Decode N1 (if content is available)
  if (sm_context_req_msg.n1_sm_msg_is_set()) {
    nas_message_t decoded_nas_msg = {};

    // Decode NAS and get the necessary information
    n1_sm_msg = sm_context_req_msg.get_n1_sm_message();
    memset(&decoded_nas_msg, 0, sizeof(nas_message_t));

    int decoder_rc = smf_n1::get_instance().decode_n1_sm_container(
        decoded_nas_msg, n1_sm_msg);
    if (decoder_rc != RETURNok) {
      // error, send reply to AMF with error code!!
      Logger::smf_app().warn("N1 SM container cannot be decoded correctly!");
      smf_app_inst->trigger_update_context_error_response(
          http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
          PDU_SESSION_APPLICATION_ERROR_N1_SM_ERROR, smreq->pid);
      return;
    }

    uint8_t message_type = decoded_nas_msg.plain.sm.header.message_type;
    switch (message_type) {
      case PDU_SESSION_MODIFICATION_REQUEST: {
        // PDU Session Modification procedure (UE-initiated, step 1.a,
        // Section 4.3.3.2@3GPP TS 23.502)  UE initiated PDU session
        // modification request (Step 1)

        procedure_type = session_management_procedures_type_e::
            PDU_SESSION_MODIFICATION_UE_INITIATED_STEP1;
        if (!handle_pdu_session_modification_request(
                decoded_nas_msg, smreq, sm_context_resp_pending, sp))
          return;
        // don't need to create a procedure to update UPF
      } break;

      case PDU_SESSION_MODIFICATION_COMPLETE: {
        // PDU Session Modification
        // procedure (UE-initiated/Network-requested) (step 3)  PDU Session
        // Modification Command Complete
        Logger::smf_app().debug("PDU_SESSION_MODIFICATION_COMPLETE");

        procedure_type = session_management_procedures_type_e::
            PDU_SESSION_MODIFICATION_UE_INITIATED_STEP3;
        if (!handle_pdu_session_modification_complete(
                decoded_nas_msg, smreq, sm_context_resp_pending, sp))
          return;
        // don't need to create a procedure to update UPF
      } break;

      case PDU_SESSION_MODIFICATION_COMMAND_REJECT: {
        // PDU_SESSION_MODIFICATION_COMMAND_REJECT
        // PDU Session Modification procedure (Section 4.3.3.2@3GPP TS 23.502)
        Logger::smf_app().debug("PDU_SESSION_MODIFICATION_COMMAND_REJECT");

        procedure_type = session_management_procedures_type_e::
            PDU_SESSION_MODIFICATION_UE_INITIATED_STEP3;

        if (!handle_pdu_session_modification_command_reject(
                decoded_nas_msg, smreq, sm_context_resp_pending, sp))
          return;
        // don't need to create a procedure to update UPF
      } break;

      case PDU_SESSION_RELEASE_REQUEST: {
        // PDU Session Release procedure (Section 4.3.4@3GPP TS 23.502)
        // PDU Session Release UE-Initiated (Step 1)

        Logger::smf_app().debug("PDU_SESSION_RELEASE_REQUEST");
        Logger::smf_app().info(
            "PDU Session Release (UE-Initiated), processing N1 SM Information");
        procedure_type = session_management_procedures_type_e::
            PDU_SESSION_RELEASE_UE_REQUESTED_STEP1;

        if (!handle_pdu_session_release_request(
                decoded_nas_msg, smreq, sm_context_resp_pending, sp))
          return;
        // need to update UPF accordingly
        update_upf = true;
      } break;

      case PDU_SESSION_RELEASE_COMPLETE: {
        // PDU Session Release procedure
        // PDU Session Release UE-Initiated (Step 3)

        Logger::smf_app().debug("PDU_SESSION_RELEASE_COMPLETE");
        Logger::smf_app().info(
            "PDU Session Release Complete (UE-Initiated), processing N1 SM "
            "Information");
        procedure_type = session_management_procedures_type_e::
            PDU_SESSION_RELEASE_UE_REQUESTED_STEP3;

        if (!handle_pdu_session_release_complete(
                decoded_nas_msg, smreq, sm_context_resp_pending, sp))
          return;
        // don't need to create a procedure to update UPF
      } break;

        // To be verified
      case PDU_SESSION_RELEASE_COMMAND: {
        // PDU Session Release procedure (Section 4.3.4@3GPP TS 23.502)
        // TODO:

      } break;

      default: {
        Logger::smf_app().warn("Unknown message type %d", message_type);
        // TODO:
      }
    }  // end switch
  }

  // Step 2.2. Decode N2 (if content is available)
  if (sm_context_req_msg.n2_sm_info_is_set()) {
    // get necessary information (N2 SM information)
    std::string n2_sm_info_type_str = smreq->req.get_n2_sm_info_type();
    std::string n2_sm_information   = smreq->req.get_n2_sm_information();
    n2_sm_info_type_e n2_sm_info_type =
        smf_app_inst->n2_sm_info_type_str2e(n2_sm_info_type_str);

    // decode N2 SM Info
    switch (n2_sm_info_type) {
      case n2_sm_info_type_e::PDU_RES_SETUP_RSP: {
        // PDU Session Resource Setup Response Transfer is included in the
        // following procedures:  1 - UE-Requested PDU Session Establishment
        // procedure (Section 4.3.2.2.1@3GPP TS 23.502)  2 - UE Triggered
        // Service Request Procedure (step 2)

        Logger::smf_app().info("PDU Session Resource Setup Response Transfer");

        if (!handle_pdu_session_resource_setup_response_transfer(
                n2_sm_information, smreq))
          return;

        if (sm_context_req_msg.rat_type_is_set() and
            sm_context_req_msg.an_type_is_set()) {
          procedure_type = session_management_procedures_type_e::
              SERVICE_REQUEST_UE_TRIGGERED_STEP2;
          Logger::smf_app().info(
              "UE-Triggered Service Request, processing N2 SM Information");
        } else {
          procedure_type = session_management_procedures_type_e::
              PDU_SESSION_ESTABLISHMENT_UE_REQUESTED;
          Logger::smf_app().info(
              "PDU Session Establishment Request, processing N2 SM "
              "Information");
        }

        // need to update UPF accordingly
        update_upf = true;
      } break;

      case n2_sm_info_type_e::PDU_RES_SETUP_FAIL: {
        // PDU Session Establishment procedure
        // PDU Session Resource Setup Unsuccessful Transfer

        Logger::smf_app().info(
            "PDU Session Resource Setup Unsuccessful Transfer");

        if (!handle_pdu_session_resource_setup_unsuccessful_transfer(
                n2_sm_information, smreq))
          return;
        // don't need to update UPF
      } break;

      case n2_sm_info_type_e::PDU_RES_MOD_RSP: {
        // PDU Session Modification procedure (UE-initiated,
        // Section 4.3.3.2@3GPP TS 23.502 or SMF-Requested)(Step 2)

        Logger::smf_app().info(
            "PDU Session Modification Procedure, processing N2 SM Information");
        procedure_type = session_management_procedures_type_e::
            PDU_SESSION_MODIFICATION_UE_INITIATED_STEP2;

        if (!handle_pdu_session_resource_modify_response_transfer(
                n2_sm_information, smreq))
          return;

        // need to update UPF accordingly
        update_upf = true;
      } break;

      case n2_sm_info_type_e::PDU_RES_MOD_FAIL: {
        // PDU Session Modification procedure

        Logger::smf_app().info("PDU_RES_MOD_FAIL");
        // TODO: To be completed
      } break;

      case n2_sm_info_type_e::PDU_RES_REL_RSP: {
        // PDU Session Release procedure (UE-initiated, Section 4.3.4.2@3GPP
        // TS 23.502 or SMF-Requested)(Step 2)
        Logger::smf_app().info(
            "PDU Session Release (UE-initiated), processing N2 SM Information");

        procedure_type = session_management_procedures_type_e::
            PDU_SESSION_RELEASE_UE_REQUESTED_STEP2;

        if (!handle_pdu_session_resource_release_response_transfer(
                n2_sm_information, smreq))
          return;

        sm_context_resp_pending->session_procedure_type =
            session_management_procedures_type_e::
                PDU_SESSION_RELEASE_UE_REQUESTED_STEP2;
        // don't need to create a procedure to update UPF
      } break;

      default: {
        Logger::smf_app().warn("Unknown N2 SM info type %d", n2_sm_info_type);
      }

    }  // end switch
  }

  // Step 3. For Service Request
  if (!sm_context_req_msg.n1_sm_msg_is_set() and
      !sm_context_req_msg.n2_sm_info_is_set() and
      sm_context_req_msg.upCnx_state_is_set()) {
    Logger::smf_app().info("Service Request (UE-triggered, step 1)");
    procedure_type = session_management_procedures_type_e::
        SERVICE_REQUEST_UE_TRIGGERED_STEP1;

    if (!handle_service_request(n2_sm_info, smreq, sm_context_resp_pending, sp))
      return;

    // do not need update UPF
    update_upf = false;
  }

  // Step 4. For AMF-initiated Session Release (with release indication)
  if (sm_context_req_msg.release_is_set()) {
    procedure_type =
        session_management_procedures_type_e::PDU_SESSION_RELEASE_AMF_INITIATED;
    // get QFIs associated with PDU session ID
    std::vector<smf_qos_flow> qos_flows = {};
    sp.get()->get_qos_flows(qos_flows);
    for (auto i : qos_flows) {
      smreq->req.add_qfi(i.qfi.qfi);
    }
    // need update UPF
    update_upf = true;
  }

  // Step 5. Create a procedure for update SM context and let the procedure
  // handle the request if necessary
  if (update_upf) {
    session_update_sm_context_procedure* proc =
        new session_update_sm_context_procedure(sp);
    std::shared_ptr<smf_procedure> sproc = std::shared_ptr<smf_procedure>(proc);
    proc->session_procedure_type         = procedure_type;

    insert_procedure(sproc);
    if (proc->run(smreq, sm_context_resp_pending, shared_from_this())) {
      // error !
      Logger::smf_app().info(
          "PDU Update SM Context Request procedure failed (session procedure "
          "type %s)",
          session_management_procedures_type_e2str
              .at(static_cast<int>(procedure_type))
              .c_str());
      remove_procedure(proc);

      // send error to AMF according to the procedure
      switch (procedure_type) {
        case session_management_procedures_type_e::
            PDU_SESSION_ESTABLISHMENT_UE_REQUESTED: {
          // PDU Session Establishment Reject
          if (smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
                  sm_context_req_msg, n1_sm_msg,
                  cause_value_5gsm_e::CAUSE_38_NETWORK_FAILURE)) {
            smf_app_inst->convert_string_2_hex(n1_sm_msg, n1_sm_msg_hex);
            // trigger to send reply to AMF
            smf_app_inst->trigger_update_context_error_response(
                http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
                PDU_SESSION_APPLICATION_ERROR_PEER_NOT_RESPONDING, smreq->pid);
          } else {
            smf_app_inst->trigger_http_response(
                http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
                smreq->pid, N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE);
          }
        } break;

        case session_management_procedures_type_e::
            SERVICE_REQUEST_UE_TRIGGERED_STEP1:
        case session_management_procedures_type_e::
            PDU_SESSION_MODIFICATION_SMF_REQUESTED:
        case session_management_procedures_type_e::
            PDU_SESSION_MODIFICATION_AN_REQUESTED:
        case session_management_procedures_type_e::
            PDU_SESSION_MODIFICATION_UE_INITIATED_STEP2:
        case session_management_procedures_type_e::
            PDU_SESSION_RELEASE_AMF_INITIATED:
        case session_management_procedures_type_e::
            PDU_SESSION_RELEASE_UE_REQUESTED_STEP1: {
          // trigger to send reply to AMF
          smf_app_inst->trigger_update_context_error_response(
              http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
              PDU_SESSION_APPLICATION_ERROR_PEER_NOT_RESPONDING, smreq->pid);
        } break;

        default: {
          // trigger to send reply to AMF
          smf_app_inst->trigger_update_context_error_response(
              http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
              PDU_SESSION_APPLICATION_ERROR_PEER_NOT_RESPONDING, smreq->pid);
        }
      }
      return;
    }
  } else {
    Logger::smf_app().info(
        "Sending ITTI message %s to task TASK_SMF_APP to trigger response",
        sm_context_resp_pending->get_msg_name());
    int ret = itti_inst->send_msg(sm_context_resp_pending);
    if (RETURNok != ret) {
      Logger::smf_app().error(
          "Could not send ITTI message %s to task TASK_SMF_SBI",
          sm_context_resp_pending->get_msg_name());
    }
  }

  // TODO, Step 6
  /*  If the PDU Session establishment is not successful, the SMF informs the
   AMF by invoking Nsmf_PDUSession_SMContextStatusNotify (Release). The SMF also
   releases any N4 session(s) created, any PDU Session address if allocated
   (e.g. IP address) and releases the association with PCF, if any. In this
   case, step 19 is skipped. see step 18, section 4.3.2.2.1@3GPP TS 23.502)
   */
}

//-------------------------------------------------------------------------------------
void smf_context::handle_pdu_session_release_sm_context_request(
    std::shared_ptr<itti_n11_release_sm_context_request> smreq) {
  Logger::smf_app().info(
      "Handle a PDU Session Release SM Context Request message from AMF");
  bool update_upf = false;

  // Step 1. get DNN, SMF PDU session context. At this stage, dnn_context and
  // pdu_session must be existed
  std::shared_ptr<dnn_context> sd     = {};
  std::shared_ptr<smf_pdu_session> sp = {};
  bool find_dnn =
      find_dnn_context(smreq->req.get_snssai(), smreq->req.get_dnn(), sd);
  bool find_pdu = false;
  if (find_dnn) {
    find_pdu = sd.get()->find_pdu_session(smreq->req.get_pdu_session_id(), sp);
  }
  if (!find_dnn or !find_pdu) {
    // error, send reply to AMF with error code "Context Not Found"
    Logger::smf_app().warn("DNN or PDU session context does not exist!");
    // trigger to send reply to AMF
    smf_app_inst->trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_404_NOT_FOUND, smreq->pid,
        N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE);
    return;
  }

  itti_n11_release_sm_context_response* n11_sm_context_resp =
      new itti_n11_release_sm_context_response(
          TASK_SMF_SBI, TASK_SMF_APP, smreq->pid);

  std::shared_ptr<itti_n11_release_sm_context_response>
      sm_context_resp_pending =
          std::shared_ptr<itti_n11_release_sm_context_response>(
              n11_sm_context_resp);

  n11_sm_context_resp->res.set_http_code(
      http_status_code_e::HTTP_STATUS_CODE_200_OK);
  n11_sm_context_resp->res.set_supi(smreq->req.get_supi());
  n11_sm_context_resp->res.set_supi_prefix(smreq->req.get_supi_prefix());
  n11_sm_context_resp->res.set_cause(
      static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
  n11_sm_context_resp->res.set_pdu_session_id(smreq->req.get_pdu_session_id());
  n11_sm_context_resp->res.set_snssai(smreq->req.get_snssai());
  n11_sm_context_resp->res.set_dnn(smreq->req.get_dnn());

  session_release_sm_context_procedure* proc =
      new session_release_sm_context_procedure(sp);
  std::shared_ptr<smf_procedure> sproc = std::shared_ptr<smf_procedure>(proc);

  insert_procedure(sproc);
  if (proc->run(smreq, sm_context_resp_pending, shared_from_this())) {
    Logger::smf_app().info("PDU Release SM Context Request procedure failed");
    // trigger to send reply to AMF
    smf_app_inst->trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR,
        smreq->pid, N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE);
    return;
  }
}

//------------------------------------------------------------------------------
void smf_context::handle_pdu_session_modification_network_requested(
    std::shared_ptr<itti_nx_trigger_pdu_session_modification> itti_msg) {
  Logger::smf_app().info(
      "Handle a PDU Session Modification Request (SMF-Requested)");

  std::string n1_sm_msg, n1_sm_msg_hex;
  std::string n2_sm_info, n2_sm_info_hex;

  // Step 1. get DNN, SMF PDU session context. At this stage, dnn_context and
  // pdu_session must be existed
  std::shared_ptr<dnn_context> sd     = {};
  std::shared_ptr<smf_pdu_session> sp = {};
  bool find_dnn =
      find_dnn_context(itti_msg->msg.get_snssai(), itti_msg->msg.get_dnn(), sd);
  bool find_pdu = false;
  if (find_dnn) {
    find_pdu =
        sd.get()->find_pdu_session(itti_msg->msg.get_pdu_session_id(), sp);
  }
  if (!find_dnn or !find_pdu) {
    Logger::smf_app().warn("DNN or PDU session context does not exist!");
    return;
  }

  std::vector<pfcp::qfi_t> list_qfis_to_be_updated;
  itti_msg->msg.get_qfis(list_qfis_to_be_updated);

  // add QFI(s), QoS Profile(s), QoS Rules
  for (auto it : list_qfis_to_be_updated) {
    Logger::smf_app().debug("QFI to be updated: %d", it.qfi);

    std::vector<QOSRulesIE> qos_rules;
    sp.get()->get_qos_rules(it, qos_rules);
    // mark QoS rule to be updated for all rules associated with the QFIs
    for (auto r : qos_rules) {
      sp.get()->mark_qos_rule_to_be_synchronised(r.qosruleidentifer);
    }

    // list of QFIs and QoS profiles
    smf_qos_flow flow = {};
    if (sp.get()->get_qos_flow(it, flow)) {
      qos_flow_context_updated qcu = {};
      qcu.set_qfi(flow.qfi);
      qcu.set_qos_profile(flow.qos_profile);
      qcu.set_ul_fteid(flow.ul_fteid);
      qcu.set_dl_fteid(flow.dl_fteid);
      itti_msg->msg.add_qos_flow_context_updated(qcu);
    }
  }

  // Step 2. prepare information for N1N2MessageTransfer to send to AMF
  Logger::smf_app().debug(
      "Prepare N1N2MessageTransfer message and send to AMF");

  // TODO: handle encode N1, N2 failure
  // N1: PDU_SESSION_MODIFICATION_COMMAND
  smf_n1::get_instance().create_n1_pdu_session_modification_command(
      itti_msg->msg, n1_sm_msg, cause_value_5gsm_e::CAUSE_0_UNKNOWN);
  smf_app_inst->convert_string_2_hex(n1_sm_msg, n1_sm_msg_hex);
  itti_msg->msg.set_n1_sm_message(n1_sm_msg_hex);

  // N2: PDU Session Resource Modify Response Transfer
  smf_n2::get_instance().create_n2_pdu_session_resource_modify_request_transfer(
      itti_msg->msg, n2_sm_info_type_e::PDU_RES_MOD_REQ, n2_sm_info);

  smf_app_inst->convert_string_2_hex(n2_sm_info, n2_sm_info_hex);
  itti_msg->msg.set_n2_sm_information(n2_sm_info_hex);

  // Fill N1N2MesasgeTransferRequestData
  // get supi and put into URL
  supi_t supi = itti_msg->msg.get_supi();
  std::string supi_str =
      itti_msg->msg.get_supi_prefix() + "-" + smf_supi_to_string(supi);
  std::string url =
      // std::string(inet_ntoa(*((struct in_addr*)
      // &smf_cfg.amf_addr.ipv4_addr))) +
      //":" + std::to_string(smf_cfg.amf_addr.port) + NAMF_COMMUNICATION_BASE +
      sp.get()->get_amf_addr() + NAMF_COMMUNICATION_BASE +
      smf_cfg.amf_addr.api_version +
      fmt::format(
          NAMF_COMMUNICATION_N1N2_MESSAGE_TRANSFER_URL, supi_str.c_str());
  itti_msg->msg.set_amf_url(url);
  Logger::smf_app().debug(
      "N1N2MessageTransfer will be sent to AMF with URL: %s", url.c_str());

  // Fill the json part
  nlohmann::json json_data = {};
  // N1SM
  json_data["n1MessageContainer"]["n1MessageClass"] = N1N2_MESSAGE_CLASS;
  json_data["n1MessageContainer"]["n1MessageContent"]["contentId"] =
      N1_SM_CONTENT_ID;  // NAS part
  // N2SM
  json_data["n2InfoContainer"]["n2InformationClass"] = N1N2_MESSAGE_CLASS;
  json_data["n2InfoContainer"]["smInfo"]["PduSessionId"] =
      itti_msg->msg.get_pdu_session_id();
  // N2InfoContent (section 6.1.6.2.27@3GPP TS 29.518)
  json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapIeType"] =
      "PDU_RES_MOD_REQ";  // NGAP message type
  json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapData"]
           ["contentId"] = N2_SM_CONTENT_ID;  // NGAP part
  json_data["n2InfoContainer"]["smInfo"]["sNssai"]["sst"] =
      itti_msg->msg.get_snssai().sST;
  json_data["n2InfoContainer"]["smInfo"]["sNssai"]["sd"] =
      itti_msg->msg.get_snssai().sD;
  json_data["n2InfoContainer"]["ranInfo"] = "SM";

  json_data["pduSessionId"] = itti_msg->msg.get_pdu_session_id();
  itti_msg->msg.set_json_data(json_data);

  // Step 3. Send ITTI message to N11 interface to trigger N1N2MessageTransfer
  // towards AMFs
  Logger::smf_app().info(
      "Sending ITTI message %s to task TASK_SMF_SBI", itti_msg->get_msg_name());

  int ret = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void smf_context::insert_dnn_subscription(
    const snssai_t& snssai,
    std::shared_ptr<session_management_subscription>& ss) {
  std::unique_lock<std::recursive_mutex> lock(m_context);

  dnn_subscriptions[(uint8_t) snssai.sST] = ss;
  Logger::smf_app().info(
      "Inserted DNN Subscription, key: %d", (uint8_t) snssai.sST);
}

//------------------------------------------------------------------------------
void smf_context::insert_dnn_subscription(
    const snssai_t& snssai, const std::string& dnn,
    std::shared_ptr<session_management_subscription>& ss) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  if (dnn_subscriptions.count((uint8_t) snssai.sST) > 0) {
    std::shared_ptr<session_management_subscription> old_ss =
        dnn_subscriptions.at((uint8_t) snssai.sST);

    std::shared_ptr<dnn_configuration_t> dnn_configuration = {};
    ss.get()->find_dnn_configuration(dnn, dnn_configuration);
    if (dnn_configuration != nullptr) {
      old_ss.get()->insert_dnn_configuration(dnn, dnn_configuration);
    }

  } else {
    dnn_subscriptions[(uint8_t) snssai.sST] = ss;
  }
  Logger::smf_app().info(
      "Inserted DNN Subscription, key: %d, dnn %s", (uint8_t) snssai.sST,
      dnn.c_str());
}

//------------------------------------------------------------------------------
bool smf_context::is_dnn_snssai_subscription_data(
    const std::string& dnn, const snssai_t& snssai) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  if (dnn_subscriptions.count((uint8_t) snssai.sST) > 0) {
    std::shared_ptr<session_management_subscription> ss =
        dnn_subscriptions.at((uint8_t) snssai.sST);
    if (ss.get()->dnn_configuration(dnn))
      return true;
    else
      return false;
  }
  return false;
}

//------------------------------------------------------------------------------
bool smf_context::find_dnn_subscription(
    const snssai_t& snssai,
    std::shared_ptr<session_management_subscription>& ss) {
  Logger::smf_app().info(
      "Find a DNN Subscription with key: %d, map size %d", (uint8_t) snssai.sST,
      dnn_subscriptions.size());
  std::unique_lock<std::recursive_mutex> lock(m_context);
  if (dnn_subscriptions.count((uint8_t) snssai.sST) > 0) {
    ss = dnn_subscriptions.at((uint8_t) snssai.sST);
    return true;
  }

  Logger::smf_app().info(
      "DNN subscription (SNSSAI %d) not found", (uint8_t) snssai.sST);
  return false;
}

//------------------------------------------------------------------------------
bool smf_context::find_dnn_context(
    const snssai_t& nssai, const std::string& dnn,
    std::shared_ptr<dnn_context>& dnn_context) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  for (auto it : dnns) {
    if ((0 == dnn.compare(it->dnn_in_use)) and
        ((uint8_t) nssai.sST) == (uint8_t)(it->nssai.sST)) {
      dnn_context = it;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void smf_context::insert_dnn(std::shared_ptr<dnn_context>& sd) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  dnns.push_back(sd);
}

//------------------------------------------------------------------------------
bool smf_context::verify_sm_context_request(
    std::shared_ptr<itti_n11_create_sm_context_request> smreq) {
  // check the validity of the UE request according to the user subscription or
  // local policies
  // TODO: need to be implemented
  return true;
}

//-----------------------------------------------------------------------------
supi_t smf_context::get_supi() const {
  return supi;
}

//-----------------------------------------------------------------------------
void smf_context::set_supi(const supi_t& s) {
  supi = s;
}

//-----------------------------------------------------------------------------
std::size_t smf_context::get_number_dnn_contexts() const {
  return dnns.size();
}

//-----------------------------------------------------------------------------
void smf_context::set_scid(const scid_t& id) {
  scid = id;
}

//-----------------------------------------------------------------------------
scid_t smf_context::get_scid() const {
  return scid;
}

//-----------------------------------------------------------------------------
void smf_context::get_supi_prefix(std::string& prefix) const {
  prefix = supi_prefix;
}

//-----------------------------------------------------------------------------
void smf_context::set_supi_prefix(std::string const& prefix) {
  supi_prefix = prefix;
}

//-----------------------------------------------------------------------------
bool smf_context::find_pdu_session(
    const pfcp::pdr_id_t& pdr_id, pfcp::qfi_t& qfi,
    std::shared_ptr<dnn_context>& sd, std::shared_ptr<smf_pdu_session>& sp) {
  std::unique_lock<std::recursive_mutex> lock(m_context);
  for (auto it : dnns) {
    for (auto session : it.get()->pdu_sessions) {
      smf_qos_flow flow = {};
      if (session->find_qos_flow(pdr_id, flow)) {
        qfi.qfi = flow.qfi.qfi;
        sp      = session;
        sd      = it;
        return true;
      }
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void smf_context::handle_sm_context_status_change(
    scid_t scid, const std::string& status, uint8_t http_version) {
  Logger::smf_app().debug(
      "Send request to N11 to triger SM Context Status Notification to AMF, "
      "SMF Context ID " SCID_FMT " ",
      scid);
  std::shared_ptr<smf_context_ref> scf = {};

  if (smf_app_inst->is_scid_2_smf_context(scid)) {
    scf = smf_app_inst->scid_2_smf_context(scid);
  } else {
    Logger::smf_app().warn(
        "SM Context associated with this id " SCID_FMT " does not exit!", scid);
    return;
  }

  // Send request to N11 to trigger the notification
  Logger::smf_app().debug(
      "Send ITTI msg to SMF N11 to trigger the status notification");
  std::shared_ptr<itti_n11_notify_sm_context_status> itti_msg =
      std::make_shared<itti_n11_notify_sm_context_status>(
          TASK_SMF_APP, TASK_SMF_SBI);
  itti_msg->scid              = scid;
  itti_msg->sm_context_status = status;
  itti_msg->amf_status_uri    = scf.get()->amf_status_uri;
  itti_msg->http_version      = http_version;

  int ret = itti_inst->send_msg(itti_msg);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_SBI",
        itti_msg->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void smf_context::handle_ee_pdu_session_release(
    supi64_t supi, pdu_session_id_t pdu_session_id, uint8_t http_version) {
  Logger::smf_app().debug(
      "Send request to N11 to triger PDU Session Release Notification (Event "
      "Exposure), SUPI " SUPI_64_FMT " , PDU Session ID %d, HTTP version  %d",
      supi, pdu_session_id, http_version);

  std::vector<std::shared_ptr<smf_subscription>> subscriptions = {};
  smf_app_inst->get_ee_subscriptions(
      smf_event_t::SMF_EVENT_PDU_SES_REL, subscriptions);

  if (subscriptions.size() > 0) {
    // Send request to N11 to trigger the notification to the subscribed event
    Logger::smf_app().debug(
        "Send ITTI msg to SMF N11 to trigger the event notification");
    std::shared_ptr<itti_n11_notify_subscribed_event> itti_msg =
        std::make_shared<itti_n11_notify_subscribed_event>(
            TASK_SMF_APP, TASK_SMF_SBI);

    for (auto i : subscriptions) {
      event_notification ev_notif = {};
      ev_notif.set_supi(supi);
      ev_notif.set_pdu_session_id(pdu_session_id);
      ev_notif.set_smf_event(smf_event_t::SMF_EVENT_PDU_SES_REL);
      ev_notif.set_supi(supi);
      ev_notif.set_notif_uri(i.get()->notif_uri);
      ev_notif.set_notif_id(i.get()->notif_id);
      itti_msg->event_notifs.push_back(ev_notif);
    }

    itti_msg->http_version = http_version;

    int ret = itti_inst->send_msg(itti_msg);
    if (RETURNok != ret) {
      Logger::smf_app().error(
          "Could not send ITTI message %s to task TASK_SMF_SBI",
          itti_msg->get_msg_name());
    }
  } else {
    Logger::smf_app().debug("No subscription available for this event");
  }
}

//------------------------------------------------------------------------------
void smf_context::update_qos_info(
    std::shared_ptr<smf_pdu_session>& sp,
    smf::pdu_session_update_sm_context_response& res,
    const nas_message_t& nas_msg) {
  // Process QoS rules and Qos Flow descriptions
  uint16_t length_of_rule_ie = nas_msg.plain.sm.pdu_session_modification_request
                                   .qosrules.lengthofqosrulesie;

  pfcp::qfi_t generated_qfi = {.qfi = 0};

  // QOSFlowDescriptions
  uint8_t number_of_flow_descriptions =
      nas_msg.plain.sm.pdu_session_modification_request.qosflowdescriptions
          .qosflowdescriptionsnumber;
  QOSFlowDescriptionsContents qos_flow_description_content = {};

  // Only one flow description for new requested QoS Flow
  if (number_of_flow_descriptions > 0) {
    for (int i = 0; i < number_of_flow_descriptions; i++) {
      if (nas_msg.plain.sm.pdu_session_modification_request.qosflowdescriptions
              .qosflowdescriptionscontents[i]
              .qfi == NO_QOS_FLOW_IDENTIFIER_ASSIGNED) {
        // TODO: generate new QFI
        generated_qfi.qfi = (uint8_t) 60;  // hardcoded for now
        qos_flow_description_content =
            nas_msg.plain.sm.pdu_session_modification_request
                .qosflowdescriptions.qosflowdescriptionscontents[i];
        qos_flow_description_content.qfi = generated_qfi.qfi;
        break;
      }
    }
  }

  int i              = 0;
  int length_of_rule = 0;
  while (length_of_rule_ie > 0) {
    QOSRulesIE qos_rules_ie = {};
    qos_rules_ie = nas_msg.plain.sm.pdu_session_modification_request.qosrules
                       .qosrulesie[i];
    uint8_t rule_id       = {0};
    pfcp::qfi_t qfi       = {};
    smf_qos_flow qos_flow = {};
    length_of_rule        = qos_rules_ie.LengthofQoSrule;

    // If UE requested a new GBR flow
    if ((qos_rules_ie.ruleoperationcode == CREATE_NEW_QOS_RULE) and
        (qos_rules_ie.segregation == SEGREGATION_REQUESTED)) {
      // Add a new QoS Flow
      if (qos_rules_ie.qosruleidentifer == NO_QOS_RULE_IDENTIFIER_ASSIGNED) {
        // Generate a new QoS rule
        sp.get()->generate_qos_rule_id(rule_id);
        Logger::smf_app().info("Create a new QoS rule (rule Id %d)", rule_id);
        qos_rules_ie.qosruleidentifer = rule_id;
      }
      sp.get()->add_qos_rule(qos_rules_ie);

      qfi.qfi      = generated_qfi.qfi;
      qos_flow.qfi = generated_qfi.qfi;

      // set qos_profile from qos_flow_description_content
      qos_flow.qos_profile = {};

      for (int j = 0; j < qos_flow_description_content.numberofparameters;
           j++) {
        if (qos_flow_description_content.parameterslist[j]
                .parameteridentifier == PARAMETER_IDENTIFIER_5QI) {
          qos_flow.qos_profile._5qi =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents._5qi;
        } else if (
            qos_flow_description_content.parameterslist[j]
                .parameteridentifier == PARAMETER_IDENTIFIER_GFBR_UPLINK) {
          qos_flow.qos_profile.parameter.qos_profile_gbr.gfbr.uplink.unit =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.uint;
          qos_flow.qos_profile.parameter.qos_profile_gbr.gfbr.uplink.value =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.value;
        } else if (
            qos_flow_description_content.parameterslist[j]
                .parameteridentifier == PARAMETER_IDENTIFIER_GFBR_DOWNLINK) {
          qos_flow.qos_profile.parameter.qos_profile_gbr.gfbr.donwlink.unit =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.uint;
          qos_flow.qos_profile.parameter.qos_profile_gbr.gfbr.donwlink.value =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.value;
        } else if (
            qos_flow_description_content.parameterslist[j]
                .parameteridentifier == PARAMETER_IDENTIFIER_MFBR_UPLINK) {
          qos_flow.qos_profile.parameter.qos_profile_gbr.mfbr.uplink.unit =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.uint;
          qos_flow.qos_profile.parameter.qos_profile_gbr.mfbr.uplink.value =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.value;
        } else if (
            qos_flow_description_content.parameterslist[j]
                .parameteridentifier == PARAMETER_IDENTIFIER_MFBR_DOWNLINK) {
          qos_flow.qos_profile.parameter.qos_profile_gbr.mfbr.donwlink.unit =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.uint;
          qos_flow.qos_profile.parameter.qos_profile_gbr.mfbr.donwlink.value =
              qos_flow_description_content.parameterslist[j]
                  .parametercontents.gfbrormfbr_uplinkordownlink.value;
        }
      }

      Logger::smf_app().debug(
          "Add new QoS Flow with new QRI %d", qos_rules_ie.qosruleidentifer);
      // mark this rule to be synchronised with the UE
      sp.get()->update_qos_rule(qos_rules_ie);
      // Add new QoS flow
      sp.get()->add_qos_flow(qos_flow);

      // ADD QoS Flow to be updated
      qos_flow_context_updated qcu = {};
      qcu.set_qfi(pfcp::qfi_t(qos_flow.qfi));
      // qcu.set_ul_fteid(flow.ul_fteid);
      // qcu.set_dl_fteid(flow.dl_fteid);
      qcu.set_qos_profile(qos_flow.qos_profile);
      res.add_qos_flow_context_updated(qcu);

    } else {  // update existing QRI
      Logger::smf_app().debug(
          "Update existing QRI %d", qos_rules_ie.qosruleidentifer);
      qfi.qfi = qos_rules_ie.qosflowidentifer;
      if (sp.get()->get_qos_flow(qfi, qos_flow)) {
        sp.get()->update_qos_rule(qos_rules_ie);
        // update QoS flow
        sp.get()->add_qos_flow(qos_flow);

        // ADD QoS Flow to be updated
        qos_flow_context_updated qcu = {};
        qcu.set_qfi(pfcp::qfi_t(qos_flow.qfi));
        qcu.set_ul_fteid(qos_flow.ul_fteid);
        qcu.set_dl_fteid(qos_flow.dl_fteid);
        qcu.set_qos_profile(qos_flow.qos_profile);
        res.add_qos_flow_context_updated(qcu);
      }
    }
    length_of_rule_ie -= (length_of_rule + 3);  // 2 for Length of QoS
                                                // rules IE and 1 for QoS
                                                // rule identifier
    i++;
  }
}

//------------------------------------------------------------------------------
void smf_context::set_amf_addr(const std::string& addr) {
  amf_addr = addr;
}

//------------------------------------------------------------------------------
void smf_context::get_amf_addr(std::string& addr) const {
  addr = amf_addr;
}

//------------------------------------------------------------------------------
void smf_context::set_plmn(const plmn_t& plmn) {
  this->plmn = plmn;
}

//------------------------------------------------------------------------------
void smf_context::get_plmn(plmn_t& plmn) const {
  plmn = this->plmn;
}

//------------------------------------------------------------------------------
bool dnn_context::find_pdu_session(
    const uint32_t pdu_session_id,
    std::shared_ptr<smf_pdu_session>& pdu_session) {
  pdu_session = {};
  std::shared_lock lock(m_context);
  for (auto it : pdu_sessions) {
    if (pdu_session_id == it->pdu_session_id) {
      pdu_session = it;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void dnn_context::insert_pdu_session(std::shared_ptr<smf_pdu_session>& sp) {
  std::unique_lock lock(m_context);
  pdu_sessions.push_back(sp);
}

//------------------------------------------------------------------------------
bool dnn_context::remove_pdu_session(const uint32_t pdu_session_id) {
  std::unique_lock lock(m_context);
  for (auto it = pdu_sessions.begin(); it != pdu_sessions.end(); ++it) {
    if (pdu_session_id == (*it).get()->pdu_session_id) {
      (*it).get()->remove_qos_flows();
      (*it).get()->clear();
      pdu_sessions.erase(it);
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
size_t dnn_context::get_number_pdu_sessions() const {
  std::shared_lock lock(m_context);
  return pdu_sessions.size();
}

//------------------------------------------------------------------------------
std::string dnn_context::toString() const {
  std::string s = {};
  s.append("DNN CONTEXT:\n");
  s.append("\tIn use:\t\t\t\t").append(std::to_string(in_use)).append("\n");
  s.append("\tDNN:\t\t\t\t").append(dnn_in_use).append("\n");
  for (auto it : pdu_sessions) {
    s.append(it->toString());
  }
  return s;
}
