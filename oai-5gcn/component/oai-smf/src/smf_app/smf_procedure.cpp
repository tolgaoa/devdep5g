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

/*! \file smf_procedure.cpp
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr
 */

#include "smf_procedure.hpp"

#include <algorithm>  // std::search

#include "3gpp_29.244.h"
#include "3gpp_29.500.h"
#include "3gpp_29.502.h"
#include "3gpp_conversions.hpp"
#include "SmContextCreatedData.h"
#include "common_defs.h"
#include "conversions.hpp"
#include "itti.hpp"
#include "itti_msg_n4_restore.hpp"
#include "logger.hpp"
#include "smf_app.hpp"
#include "smf_config.hpp"
#include "smf_context.hpp"
#include "smf_n1.hpp"
#include "smf_sbi.hpp"
#include "smf_n2.hpp"
#include "smf_pfcp_association.hpp"
#include "ProblemDetails.h"
#include "3gpp_24.501.h"

using namespace pfcp;
using namespace smf;
using namespace std;

extern itti_mw* itti_inst;
extern smf::smf_app* smf_app_inst;
extern smf::smf_config smf_cfg;

//------------------------------------------------------------------------------
int n4_session_restore_procedure::run() {
  if (pending_sessions.size()) {
    itti_n4_restore* itti_msg = nullptr;
    for (std::set<pfcp::fseid_t>::iterator it = pending_sessions.begin();
         it != pending_sessions.end(); ++it) {
      if (!itti_msg) {
        itti_msg = new itti_n4_restore(TASK_SMF_N4, TASK_SMF_APP);
      }
      itti_msg->sessions.insert(*it);
      if (itti_msg->sessions.size() >= 64) {
        std::shared_ptr<itti_n4_restore> i =
            std::shared_ptr<itti_n4_restore>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (RETURNok != ret) {
          Logger::smf_n4().error(
              "Could not send ITTI message %s to task TASK_SMF_APP",
              i->get_msg_name());
        }
        itti_msg = nullptr;
      }
    }
    if (itti_msg) {
      std::shared_ptr<itti_n4_restore> i =
          std::shared_ptr<itti_n4_restore>(itti_msg);
      int ret = itti_inst->send_msg(i);
      if (RETURNok != ret) {
        Logger::smf_n4().error(
            "Could not send ITTI message %s to task TASK_SMF_APP",
            i->get_msg_name());
        return RETURNerror;
      }
    }
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
int session_create_sm_context_procedure::run(
    std::shared_ptr<itti_n11_create_sm_context_request> sm_context_req,
    std::shared_ptr<itti_n11_create_sm_context_response> sm_context_resp,
    std::shared_ptr<smf::smf_context> sc) {
  Logger::smf_app().info("Perform a procedure - Create SM Context Request");
  // TODO check if compatible with ongoing procedures if any
  pfcp::node_id_t up_node_id = {};
  snssai_t snssai            = sm_context_req->req.get_snssai();
  std::string dnn            = sm_context_req->req.get_dnn();

  // if (not pfcp_associations::get_instance().select_up_node(
  //        up_node_id, NODE_SELECTION_CRITERIA_MIN_PFCP_SESSIONS)) {
  if (not pfcp_associations::get_instance().select_up_node(
          up_node_id, snssai, dnn)) {
    sm_context_resp->res.set_cause(
        PDU_SESSION_APPLICATION_ERROR_PEER_NOT_RESPONDING);
    return RETURNerror;
  } else {
    // Store UPF node
    std::shared_ptr<smf_context_ref> scf = {};
    if (smf_app_inst->is_scid_2_smf_context(sm_context_req->scid)) {
      scf = smf_app_inst->scid_2_smf_context(sm_context_req->scid);
      scf.get()->upf_node_id = up_node_id;
    } else {
      Logger::smf_app().warn(
          "SM Context associated with this id " SCID_FMT " does not exit!",
          sm_context_req->scid);
      // TODO:
    }
  }

  //-------------------
  n11_trigger           = sm_context_req;
  n11_triggered_pending = sm_context_resp;
  uint64_t seid         = smf_app_inst->generate_seid();
  sps->set_seid(seid);
  itti_n4_session_establishment_request* n4_ser =
      new itti_n4_session_establishment_request(TASK_SMF_APP, TASK_SMF_N4);
  n4_ser->seid       = 0;
  n4_ser->trxn_id    = this->trxn_id;
  n4_ser->r_endpoint = endpoint(up_node_id.u1.ipv4_address, pfcp::default_port);
  n4_triggered = std::shared_ptr<itti_n4_session_establishment_request>(n4_ser);

  //-------------------
  // IE node_id_t
  //-------------------
  pfcp::node_id_t node_id = {};
  smf_cfg.get_pfcp_node_id(node_id);
  n4_ser->pfcp_ies.set(node_id);

  //-------------------
  // IE fseid_t
  //-------------------
  pfcp::fseid_t cp_fseid = {};
  smf_cfg.get_pfcp_fseid(cp_fseid);
  cp_fseid.seid = sps->seid;
  n4_ser->pfcp_ies.set(cp_fseid);

  //*******************
  // UPLINK
  //*******************
  //-------------------
  // IE create_far (Forwarding Action Rules)
  //-------------------
  pfcp::create_far create_far                       = {};
  pfcp::far_id_t far_id                             = {};  // rule ID
  pfcp::apply_action_t apply_action                 = {};
  pfcp::forwarding_parameters forwarding_parameters = {};

  // forwarding_parameters IEs
  pfcp::destination_interface_t destination_interface = {};

  sps->generate_far_id(far_id);
  apply_action.forw = 1;  // forward the packets

  destination_interface.interface_value =
      pfcp::INTERFACE_VALUE_CORE;  // ACCESS is for downlink, CORE for uplink
  forwarding_parameters.set(destination_interface);
  // TODO: Network Instance
  // TODO: Redirect Information
  // TODO: Outer Header Creation (e.g., in case of N9)

  create_far.set(far_id);
  create_far.set(apply_action);
  create_far.set(
      forwarding_parameters);  // should check since destination
                               // interface is directly set to FAR (as
                               // described in Table 5.8.2.11.6-1)

  //-------------------
  // IE create_pdr (section 5.8.2.11.3@TS 23.501)
  //-------------------
  pfcp::create_pdr create_pdr   = {};
  pfcp::pdr_id_t pdr_id         = {};  // rule ID?
  pfcp::precedence_t precedence = {};
  pfcp::pdi pdi                 = {};  // packet detection information
  pfcp::outer_header_removal_t outer_header_removal = {};
  // pdi IEs
  pfcp::source_interface_t source_interface          = {};
  pfcp::fteid_t local_fteid                          = {};
  pfcp::ue_ip_address_t ue_ip_address                = {};
  pfcp::sdf_filter_t sdf_filter                      = {};
  pfcp::application_id_t application_id              = {};
  pfcp::qfi_t qfi                                    = {};
  pfcp::_3gpp_interface_type_t source_interface_type = {};

  // DOIT simple
  // shall uniquely identify the PDR among all the PDRs configured for that PFCP
  // session.
  sps->generate_pdr_id(pdr_id);

  // get the default QoS profile
  subscribed_default_qos_t default_qos                = {};
  std::shared_ptr<session_management_subscription> ss = {};
  sc.get()->get_default_qos(
      sm_context_req->req.get_snssai(), sm_context_req->req.get_dnn(),
      default_qos);
  // Default QoS Flow - Non GBR, qfi = 5qi
  qfi.qfi = default_qos._5qi;
  Logger::smf_app().info("Default qfi %d", qfi.qfi);

  // Packet detection information (see Table 7.5.2.2-2: PDI IE within PFCP
  // Session Establishment Request, 3GPP TS 29.244 V16.0.0)  source interface
  source_interface.interface_value = pfcp::INTERFACE_VALUE_ACCESS;
  pdi.set(source_interface);
  // CN tunnel info
  local_fteid.ch =
      1;  // SMF requests the UPF to assign a local F-TEID to the PDR
  // TODO required?: local_fteid.v4 = 1;
  // local_fteid.chid = 1;

  // TS 29.244 R16 8.2.3 -> At least one of the V4 and V6 flags shall be set to
  // "1", and both may be set to "1" for scenarios when the UP function is
  // requested to allocate the F-TEID, i.e. when CHOOSE bit is set to "1", and
  // the IPv4 address and IPv6 address fields are not present.
  local_fteid.v4 = 1;
  pdi.set(local_fteid);
  // TODO: Network Instance
  // UE IP address
  xgpp_conv::paa_to_pfcp_ue_ip_address(
      sm_context_resp->res.get_paa(), ue_ip_address);
  pdi.set(ue_ip_address);
  // TODO: Traffic Endpoint ID
  // TODO: SDF Filter
  // TODO: Application ID
  // TODO: Ethernet PDU Session Information
  // TODO: Ethernet Packet Filter
  pdi.set(qfi);  // QFI - QoS Flow ID
  // TODO: Framed Route Information
  // TODO: Framed-Routing
  // TODO: Framed-IPv6-Route
  // Source Interface Type - N3
  source_interface_type.interface_type_value = pfcp::_3GPP_INTERFACE_TYPE_N3;
  pdi.set(source_interface_type);

  outer_header_removal.outer_header_removal_description =
      OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4;

  create_pdr.set(pdr_id);
  create_pdr.set(precedence);
  create_pdr.set(pdi);
  create_pdr.set(outer_header_removal);

  create_pdr.set(far_id);
  // TODO: list of Usage reporting Rule IDs
  // TODO: list of QoS Enforcement Rule IDs

  //-------------------
  // ADD IEs to message
  //-------------------
  n4_ser->pfcp_ies.set(create_pdr);
  n4_ser->pfcp_ies.set(create_far);

  // TODO: verify whether N4 SessionID should be included in PDR and FAR
  // (Section 5.8.2.11@3GPP TS 23.501)

  // Create default QoS (Non-GBR) and associate far id and pdr id to this flow
  smf_qos_flow flow     = {};
  flow.far_id_ul.first  = true;
  flow.far_id_ul.second = far_id;
  flow.pdr_id_ul        = pdr_id;
  flow.pdu_session_id   = sm_context_req->req.get_pdu_session_id();
  // default QoS profile
  flow.qfi.qfi                    = default_qos._5qi;
  flow.qos_profile._5qi           = default_qos._5qi;
  flow.qos_profile.arp            = default_qos.arp;
  flow.qos_profile.priority_level = default_qos.priority_level;

  // assign default QoS rule for this flow
  QOSRulesIE qos_rule = {};
  sc.get()->get_default_qos_rule(
      qos_rule, sm_context_req->req.get_pdu_session_type());
  uint8_t rule_id = {0};
  sps->generate_qos_rule_id(rule_id);
  qos_rule.qosruleidentifer = rule_id;
  qos_rule.qosflowidentifer = flow.qfi.qfi;
  sps->add_qos_rule(qos_rule);

  sps->add_qos_flow(flow);
  sps->set_default_qos_flow(flow.qfi);

  // for finding procedure when receiving response
  smf_app_inst->set_seid_2_smf_context(cp_fseid.seid, sc);

  Logger::smf_app().info(
      "Sending ITTI message %s to task TASK_SMF_N4", n4_ser->get_msg_name());
  int ret = itti_inst->send_msg(n4_triggered);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_N4",
        n4_ser->get_msg_name());
    return RETURNerror;
  }

  return RETURNok;
}

//------------------------------------------------------------------------------
void session_create_sm_context_procedure::handle_itti_msg(
    itti_n4_session_establishment_response& resp,
    std::shared_ptr<smf::smf_context> sc) {
  Logger::smf_app().info(
      "Handle N4 Session Establishment Response (PDU Session Id %d)",
      n11_trigger.get()->req.get_pdu_session_id());

  cause_value_5gsm_e cause_n1 = {cause_value_5gsm_e::CAUSE_0_UNKNOWN};
  pfcp::cause_t cause         = {};
  resp.pfcp_ies.get(cause);
  if (cause.cause_value == pfcp::CAUSE_VALUE_REQUEST_ACCEPTED) {
    resp.pfcp_ies.get(sps->up_fseid);
    n11_triggered_pending->res.set_cause(
        static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
  }

  for (auto it : resp.pfcp_ies.created_pdrs) {
    pfcp::pdr_id_t pdr_id = {};
    pfcp::far_id_t far_id = {};
    if (it.get(pdr_id)) {
      smf_qos_flow flow = {};
      if (sps->get_qos_flow(pdr_id, flow)) {
        // pfcp::fteid_t local_up_fteid = { };
        if (it.get(flow.ul_fteid)) {
          // Update Qos Flow
          sps->add_qos_flow(flow);
        }
      } else {
        Logger::smf_app().error(
            "Could not get QoS Flow for created_pdr %d", pdr_id.rule_id);
      }
    } else {
      Logger::smf_app().error(
          "Could not get pdr_id for created_pdr in %s",
          resp.pfcp_ies.get_msg_name());
    }
  }

  smf_qos_flow default_qos_flow = {};
  // flow_updated info will be used to construct N1,N2 container
  qos_flow_context_updated flow_updated = {};
  QOSRulesIE qos_rule                   = {};

  flow_updated.set_cause(
      static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
  if (not sps->get_default_qos_flow(default_qos_flow)) {
    flow_updated.set_cause(static_cast<uint8_t>(
        cause_value_5gsm_e::CAUSE_31_REQUEST_REJECTED_UNSPECIFIED));
  } else {
    if (default_qos_flow.ul_fteid.is_zero()) {
      flow_updated.set_cause(static_cast<uint8_t>(
          cause_value_5gsm_e::CAUSE_31_REQUEST_REJECTED_UNSPECIFIED));
    } else {
      flow_updated.set_ul_fteid(default_qos_flow.ul_fteid);  // tunnel info
    }
    if (sps->get_default_qos_rule(qos_rule)) {
      flow_updated.add_qos_rule(qos_rule);
    }
    flow_updated.set_qfi(default_qos_flow.qfi);
    qos_profile_t profile = {};
    profile               = default_qos_flow.qos_profile;
    flow_updated.set_qos_profile(profile);
    flow_updated.set_priority_level(
        default_qos_flow.qos_profile.priority_level);
  }

  // TODO: Set RQA (optional)

  n11_triggered_pending->res.set_qos_flow_context(flow_updated);

  // fill content for N1N2MessageTransfer (including N1, N2 SM)
  // Create N1 SM container & N2 SM Information
  std::string n1_sm_msg, n1_sm_msg_hex;
  std::string n2_sm_info, n2_sm_info_hex;

  if (n11_triggered_pending->res.get_cause() !=
      static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED)) {
    // PDU Session Establishment Reject
    Logger::smf_app().debug(
        "Prepare a PDU Session Establishment Reject message and send to UE");
    cause_n1 = cause_value_5gsm_e::CAUSE_38_NETWORK_FAILURE;
    // TODO: Support IPv4 only for now
    if (n11_triggered_pending->res.get_pdu_session_type() ==
        PDU_SESSION_TYPE_E_IPV6) {
      n11_triggered_pending->res.set_pdu_session_type(PDU_SESSION_TYPE_E_IPV4);
      cause_n1 =
          cause_value_5gsm_e::CAUSE_50_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED;
    }

    smf_n1::get_instance().create_n1_pdu_session_establishment_reject(
        n11_triggered_pending->res, n1_sm_msg, cause_n1);
    smf_app_inst->convert_string_2_hex(n1_sm_msg, n1_sm_msg_hex);
    n11_triggered_pending->res.set_n1_sm_message(n1_sm_msg_hex);

  } else {  // PDU Session Establishment Accept
    Logger::smf_app().debug(
        "Prepare a PDU Session Establishment Accept message and send to UE");

    // TODO: Support IPv4 only for now
    if (n11_triggered_pending->res.get_pdu_session_type() ==
        PDU_SESSION_TYPE_E_IPV6) {
      n11_triggered_pending->res.set_pdu_session_type(PDU_SESSION_TYPE_E_IPV4);
      cause_n1 =
          cause_value_5gsm_e::CAUSE_50_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED;
    }

    smf_n1::get_instance().create_n1_pdu_session_establishment_accept(
        n11_triggered_pending->res, n1_sm_msg, cause_n1);
    smf_app_inst->convert_string_2_hex(n1_sm_msg, n1_sm_msg_hex);
    n11_triggered_pending->res.set_n1_sm_message(n1_sm_msg_hex);
    // N2 SM Information (Step 11, section 4.3.2.2.1 @ 3GPP TS 23.502):
    // PDUSessionRessourceSetupRequestTransfer IE
    smf_n2::get_instance()
        .create_n2_pdu_session_resource_setup_request_transfer(
            n11_triggered_pending->res, n2_sm_info_type_e::PDU_RES_SETUP_REQ,
            n2_sm_info);

    smf_app_inst->convert_string_2_hex(n2_sm_info, n2_sm_info_hex);
    n11_triggered_pending->res.set_n2_sm_information(n2_sm_info_hex);
  }

  // Fill N1N2MesasgeTransferRequestData
  // get supi and put into URL
  supi_t supi          = n11_triggered_pending->res.get_supi();
  std::string supi_str = n11_triggered_pending->res.get_supi_prefix() + "-" +
                         smf_supi_to_string(supi);
  std::string url =
      // std::string(inet_ntoa(*((struct in_addr*)
      // &smf_cfg.amf_addr.ipv4_addr))) +
      //":" + std::to_string(smf_cfg.amf_addr.port) + NAMF_COMMUNICATION_BASE +
      sps.get()->get_amf_addr() + NAMF_COMMUNICATION_BASE +
      smf_cfg.amf_addr.api_version +
      fmt::format(
          NAMF_COMMUNICATION_N1N2_MESSAGE_TRANSFER_URL, supi_str.c_str());
  n11_triggered_pending->res.set_amf_url(url);
  Logger::smf_app().debug(
      "N1N2MessageTransfer will be sent to AMF with URL: %s", url.c_str());

  // Fill the json part
  nlohmann::json json_data = {};
  // N1SM
  json_data["n1MessageContainer"]["n1MessageClass"] = N1N2_MESSAGE_CLASS;
  json_data["n1MessageContainer"]["n1MessageContent"]["contentId"] =
      N1_SM_CONTENT_ID;  // NAS part
  // N2SM
  if (n11_triggered_pending->res.get_cause() ==
      static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED)) {
    json_data["n2InfoContainer"]["n2InformationClass"] = N1N2_MESSAGE_CLASS;
    json_data["n2InfoContainer"]["smInfo"]["pduSessionId"] =
        n11_triggered_pending->res.get_pdu_session_id();
    // N2InfoContent (section 6.1.6.2.27@3GPP TS 29.518)
    json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapIeType"] =
        "PDU_RES_SETUP_REQ";  // NGAP message type
    json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapData"]
             ["contentId"] = N2_SM_CONTENT_ID;  // NGAP part
    json_data["n2InfoContainer"]["smInfo"]["sNssai"]["sst"] =
        n11_triggered_pending->res.get_snssai().sST;
    json_data["n2InfoContainer"]["smInfo"]["sNssai"]["sd"] =
        n11_triggered_pending->res.get_snssai().sD;
    json_data["n2InfoContainer"]["ranInfo"] = "SM";

    // N1N2MsgTxfrFailureNotification
    std::string callback_uri =
        // std::string(
        //    inet_ntoa(*((struct in_addr*) &smf_cfg.amf_addr.ipv4_addr))) +
        //":" + std::to_string(smf_cfg.amf_addr.port) + NSMF_PDU_SESSION_BASE +
        sps.get()->get_amf_addr() + NSMF_PDU_SESSION_BASE +
        smf_cfg.sbi_api_version +
        fmt::format(
            NSMF_CALLBACK_N1N2_MESSAGE_TRANSFER_FAILURE, supi_str.c_str());
    json_data["n1n2FailureTxfNotifURI"] = callback_uri.c_str();
  }
  // Others information
  // n11_triggered_pending->res.n1n2_message_transfer_data["pti"] = 1;  //Don't
  // need this info for the moment
  json_data["pduSessionId"] = n11_triggered_pending->res.get_pdu_session_id();

  n11_triggered_pending->res.set_json_data(json_data);

  // send ITTI message to APP to trigger N1N2MessageTransfer towards AMFs
  Logger::smf_app().info(
      "Sending ITTI message %s to task TASK_SMF_APP",
      n11_triggered_pending->get_msg_name());

  int ret = itti_inst->send_msg(n11_triggered_pending);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_SBI",
        n11_triggered_pending->get_msg_name());
  }
}

//------------------------------------------------------------------------------
int session_update_sm_context_procedure::run(
    std::shared_ptr<itti_n11_update_sm_context_request> sm_context_req,
    std::shared_ptr<itti_n11_update_sm_context_response> sm_context_resp,
    std::shared_ptr<smf::smf_context> sc) {
  // Handle SM update sm context request
  // The SMF initiates an N4 Session Modification procedure with the UPF. The
  // SMF provides AN Tunnel Info to the UPF as well as the corresponding
  // forwarding rules

  bool send_n4 = false;
  Logger::smf_app().info("Perform a procedure - Update SM Context Request");
  // TODO check if compatible with ongoing procedures if any
  pfcp::node_id_t up_node_id = {};
  // Get UPF node
  std::shared_ptr<smf_context_ref> scf = {};
  scid_t scid                          = {};
  try {
    scid = std::stoi(sm_context_req->scid);
  } catch (const std::exception& err) {
    Logger::smf_app().warn(
        "SM Context associated with this id %s does not exit!",
        sm_context_req->scid);
  }
  if (smf_app_inst->is_scid_2_smf_context(scid)) {
    scf        = smf_app_inst->scid_2_smf_context(scid);
    up_node_id = scf.get()->upf_node_id;
  } else {
    Logger::smf_app().warn(
        "SM Context associated with this id " SCID_FMT " does not exit!", scid);
    // TODO:
  }

  /*  if (not pfcp_associations::get_instance().select_up_node(
            up_node_id, NODE_SELECTION_CRITERIA_MIN_PFCP_SESSIONS)) {
      sm_context_resp->res.set_cause(
          PDU_SESSION_APPLICATION_ERROR_PEER_NOT_RESPONDING);
      Logger::smf_app().info("[SMF Procedure] REMOTE_PEER_NOT_RESPONDING");
      return RETURNerror;
    }
  */

  //-------------------
  n11_trigger           = sm_context_req;
  n11_triggered_pending = sm_context_resp;
  uint64_t seid         = smf_app_inst->generate_seid();
  sps->set_seid(seid);
  itti_n4_session_modification_request* n4_ser =
      new itti_n4_session_modification_request(TASK_SMF_APP, TASK_SMF_N4);
  n4_ser->seid       = sps->up_fseid.seid;
  n4_ser->trxn_id    = this->trxn_id;
  n4_ser->r_endpoint = endpoint(up_node_id.u1.ipv4_address, pfcp::default_port);
  n4_triggered = std::shared_ptr<itti_n4_session_modification_request>(n4_ser);

  // qos Flow to be modified
  pdu_session_update_sm_context_request sm_context_req_msg =
      sm_context_req->req;
  std::vector<pfcp::qfi_t> list_of_qfis_to_be_modified = {};
  sm_context_req_msg.get_qfis(list_of_qfis_to_be_modified);

  for (auto i : list_of_qfis_to_be_modified) {
    Logger::smf_app().debug("QoS Flow to be modified (QFI %d)", i.qfi);
  }

  Logger::smf_app().debug(
      "Session procedure type: %s",
      session_management_procedures_type_e2str
          .at(static_cast<int>(session_procedure_type))
          .c_str());

  switch (session_procedure_type) {
    case session_management_procedures_type_e::
        PDU_SESSION_ESTABLISHMENT_UE_REQUESTED:
    case session_management_procedures_type_e::
        SERVICE_REQUEST_UE_TRIGGERED_STEP2:
    case session_management_procedures_type_e::
        PDU_SESSION_MODIFICATION_SMF_REQUESTED:
    case session_management_procedures_type_e::
        PDU_SESSION_MODIFICATION_AN_REQUESTED:
    case session_management_procedures_type_e::
        PDU_SESSION_MODIFICATION_UE_INITIATED_STEP2: {
      pfcp::fteid_t dl_fteid = {};
      sm_context_req_msg.get_dl_fteid(dl_fteid);  // eNB's fteid

      for (auto qfi : list_of_qfis_to_be_modified) {
        smf_qos_flow flow = {};
        if (!sps->get_qos_flow(qfi, flow)) {  // no QoS flow found
          Logger::smf_app().error(
              "could not found any QoS flow with QFI %d", qfi.qfi);
          // Set cause to SYSTEM_FAILURE and send response
          qos_flow_context_updated qcu = {};
          qcu.set_cause(static_cast<uint8_t>(
              cause_value_5gsm_e::CAUSE_31_REQUEST_REJECTED_UNSPECIFIED));
          qcu.set_qfi(qfi);
          n11_triggered_pending->res.add_qos_flow_context_updated(qcu);
          continue;
        }

        Logger::smf_app().debug(
            "FAR ID DL first %d,  FAR DL ID second "
            "0x%" PRIx32 " ",
            flow.far_id_dl.first, flow.far_id_dl.second.far_id);

        pfcp::far_id_t far_id = {};
        pfcp::pdr_id_t pdr_id = {};
        if ((dl_fteid == flow.dl_fteid) and (not flow.released)) {
          Logger::smf_app().debug("QFI %d dl_fteid unchanged", qfi.qfi);
          qos_flow_context_updated qcu = {};
          qcu.set_cause(static_cast<uint8_t>(
              cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
          qcu.set_qfi(qfi);
          n11_triggered_pending->res.add_qos_flow_context_updated(qcu);
          continue;
        } else if ((flow.far_id_dl.first) && (flow.far_id_dl.second.far_id)) {
          Logger::smf_app().debug(
              "Update FAR DL "
              "0x%" PRIx32 " ",
              flow.far_id_dl.second.far_id);
          // Update FAR
          far_id.far_id                     = flow.far_id_dl.second.far_id;
          pfcp::update_far update_far       = {};
          pfcp::apply_action_t apply_action = {};
          pfcp::outer_header_creation_t outer_header_creation             = {};
          pfcp::update_forwarding_parameters update_forwarding_parameters = {};

          update_far.set(flow.far_id_dl.second);
          outer_header_creation.outer_header_creation_description =
              OUTER_HEADER_CREATION_GTPU_UDP_IPV4;
          outer_header_creation.teid = dl_fteid.teid;
          outer_header_creation.ipv4_address.s_addr =
              dl_fteid.ipv4_address.s_addr;
          update_forwarding_parameters.set(outer_header_creation);
          update_far.set(update_forwarding_parameters);
          apply_action.forw = 1;  // forward the packets
          // apply_action.nocp = 1; //notify the CP function about the arrival
          // of a first DL packet
          update_far.set(apply_action);

          n4_ser->pfcp_ies.set(update_far);

          send_n4              = true;
          flow.far_id_dl.first = true;
          flow.dl_fteid        = dl_fteid;

        } else {
          Logger::smf_app().debug("Create FAR DL");
          // Create FAR
          pfcp::create_far create_far                       = {};
          pfcp::apply_action_t apply_action                 = {};
          pfcp::forwarding_parameters forwarding_parameters = {};
          // pfcp::duplicating_parameters      duplicating_parameters = {};
          // pfcp::bar_id_t                    bar_id = {};

          // forwarding_parameters IEs
          pfcp::destination_interface_t destination_interface = {};
          // pfcp::network_instance_t          network_instance = {};
          // pfcp::redirect_information_t      redirect_information = {};
          pfcp::outer_header_creation_t outer_header_creation = {};
          // pfcp::transport_level_marking_t   transport_level_marking = {};
          // pfcp::forwarding_policy_t         forwarding_policy = {};
          // pfcp::header_enrichment_t         header_enrichment = {};
          // pfcp::traffic_endpoint_id_t       linked_traffic_endpoint_id_t =
          // {};  pfcp::proxying_t                  proxying = {};

          sps->generate_far_id(far_id);
          apply_action.forw = 1;  // forward the packets
          // apply_action.nocp = 1; //notify the CP function about the arrival
          // of a first DL packet

          destination_interface.interface_value =
              pfcp::INTERFACE_VALUE_ACCESS;  // ACCESS is for downlink, CORE for
                                             // uplink
          forwarding_parameters.set(destination_interface);
          outer_header_creation.outer_header_creation_description =
              OUTER_HEADER_CREATION_GTPU_UDP_IPV4;
          outer_header_creation.teid = dl_fteid.teid;
          outer_header_creation.ipv4_address.s_addr =
              dl_fteid.ipv4_address.s_addr;
          forwarding_parameters.set(outer_header_creation);

          create_far.set(far_id);
          create_far.set(apply_action);
          create_far.set(forwarding_parameters);

          // Add IEs to message
          n4_ser->pfcp_ies.set(create_far);

          send_n4 = true;

          flow.far_id_dl.first         = true;
          flow.far_id_dl.second.far_id = far_id.far_id;
          flow.dl_fteid                = dl_fteid;
          Logger::smf_app().debug(
              "FAR DL ID "
              "0x%" PRIx32 " ",
              far_id.far_id);
        }

        if (not flow.pdr_id_dl.rule_id) {
          Logger::smf_app().debug("Create PDR DL");
          //-------------------
          // IE create_pdr
          //-------------------
          pfcp::create_pdr create_pdr   = {};
          pfcp::precedence_t precedence = {};
          pfcp::pdi pdi                 = {};
          //    pfcp::far_id_t                    far_id;
          //    pfcp::urr_id_t                    urr_id;
          //    pfcp::qer_id_t                    qer_id;
          //    pfcp::activate_predefined_rules_t activate_predefined_rules;
          // pdi IEs
          pfcp::source_interface_t source_interface = {};
          // pfcp::fteid_t                    local_fteid = {};
          // pfcp::network_instance_t         network_instance = {};
          pfcp::ue_ip_address_t ue_ip_address = {};
          // pfcp::traffic_endpoint_id_t      traffic_endpoint_id = {};
          pfcp::sdf_filter_t sdf_filter         = {};
          pfcp::application_id_t application_id = {};
          // pfcp::ethernet_packet_filter     ethernet_packet_filter = {};
          pfcp::qfi_t qfi = {};
          // pfcp::framed_route_t             framed_route = {};
          // pfcp::framed_routing_t           framed_routing = {};
          // pfcp::framed_ipv6_route_t        framed_ipv6_route = {};
          source_interface.interface_value = pfcp::INTERFACE_VALUE_CORE;

          // local_fteid.from_core_fteid(qos_flow.qos_flow.dl_fteid);
          if (sps->ipv4) {
            ue_ip_address.v4 = 1;
            // Bit 3 – S/D: TS 29.244 R16, 8.2.62 In the PDI IE, if this bit is
            // set to "0", this indicates a Source IP address; if this bit is
            // set to "1", this indicates a Destination IP address.
            ue_ip_address.sd                  = 1;
            ue_ip_address.ipv4_address.s_addr = sps->ipv4_address.s_addr;
          }
          if (sps->ipv6) {
            ue_ip_address.v6           = 1;
            ue_ip_address.ipv6_address = sps->ipv6_address;
          }

          // DOIT simple
          // shall uniquely identify the PDR among all the PDRs configured for
          // that PFCP session.
          sps->generate_pdr_id(pdr_id);
          precedence.precedence = flow.precedence.precedence;
          pdi.set(source_interface);
          // pdi.set(local_fteid);
          pdi.set(ue_ip_address);

          create_pdr.set(pdr_id);
          create_pdr.set(precedence);
          create_pdr.set(pdi);
          create_pdr.set(far_id);

          // Add IEs to message
          n4_ser->pfcp_ies.set(create_pdr);

          send_n4 = true;

          flow.pdr_id_dl.rule_id = pdr_id.rule_id;
          Logger::smf_app().debug(
              "PDR DL ID "
              "0x%" PRIx16 " ",
              pdr_id.rule_id);
        } else {
          Logger::smf_app().debug(
              "Update FAR, PDR DL Rule Id "
              "0x%" PRIx16 ", FAR ID 0x%" PRIx32 " ",
              flow.pdr_id_dl.rule_id, flow.far_id_dl.second.far_id);
          /*
           // Update FAR
           far_id.far_id = flow.far_id_ul.second.far_id;
           pfcp::update_far update_far = { };
           pfcp::apply_action_t apply_action = { };

           update_far.set(flow.far_id_ul.second);
           apply_action.forw = 1;
           update_far.set(apply_action);

           n4_ser->pfcp_ies.set(update_far);

           send_n4 = true;

           flow.far_id_dl.first = true;
           */

          pfcp::update_pdr update_pdr               = {};
          pfcp::precedence_t precedence             = {};
          pfcp::pdi pdi                             = {};
          pfcp::ue_ip_address_t ue_ip_address       = {};
          pfcp::source_interface_t source_interface = {};

          if (sps->ipv4) {
            ue_ip_address.v4 = 1;
            // Bit 3 – S/D: TS 29.244 R16, 8.2.62 In the PDI IE, if this bit is
            // set to "0", this indicates a Source IP address; if this bit is
            // set to "1", this indicates a Destination IP address.
            ue_ip_address.sd                  = 1;
            ue_ip_address.ipv4_address.s_addr = sps->ipv4_address.s_addr;
          }
          if (sps->ipv6) {
            ue_ip_address.v6           = 1;
            ue_ip_address.ipv6_address = sps->ipv6_address;
          }
          precedence.precedence = flow.precedence.precedence;

          source_interface.interface_value = pfcp::INTERFACE_VALUE_CORE;
          pdi.set(source_interface);
          pdi.set(ue_ip_address);

          update_pdr.set(flow.pdr_id_dl);
          update_pdr.set(precedence);
          update_pdr.set(pdi);
          update_pdr.set(flow.far_id_dl.second);

          // Add IEs to message
          n4_ser->pfcp_ies.set(update_pdr);

          send_n4 = true;
          Logger::smf_app().debug(
              "PDR DL ID  "
              "0x%" PRIx16 " updated",
              flow.pdr_id_dl.rule_id);
        }
        // after a release flows
        if (not flow.ul_fteid.is_zero()) {
        }

        if (not flow.dl_fteid.is_zero()) {
        }
        // may be modified
        sps->add_qos_flow(flow);

        qos_flow_context_updated qcu = {};
        qcu.set_cause(static_cast<uint8_t>(
            cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
        qcu.set_qfi(qfi);
        n11_triggered_pending->res.add_qos_flow_context_updated(qcu);
      }
    } break;

    case session_management_procedures_type_e::
        PDU_SESSION_RELEASE_AMF_INITIATED:
    case session_management_procedures_type_e::
        PDU_SESSION_RELEASE_UE_REQUESTED_STEP1: {
      for (auto qfi : list_of_qfis_to_be_modified) {
        smf_qos_flow flow = {};
        if (!sps->get_qos_flow(qfi, flow)) {  // no QoS flow found
          Logger::smf_app().error(
              "Update SM Context procedure: could not found QoS flow with QFI "
              "%d",
              qfi.qfi);
          // Set cause to SYSTEM_FAILURE and send response
          qos_flow_context_updated qcu = {};
          qcu.set_cause(static_cast<uint8_t>(
              cause_value_5gsm_e::CAUSE_31_REQUEST_REJECTED_UNSPECIFIED));
          qcu.set_qfi(qfi);
          n11_triggered_pending->res.add_qos_flow_context_updated(qcu);
          continue;
        }

        // for DL
        if (flow.far_id_dl.first) {
          pfcp::update_far far  = {};
          pfcp::far_id_t far_id = {};

          far_id.far_id = flow.far_id_dl.second.far_id;
          // apply_action.buff = 1;
          pfcp::apply_action_t apply_action = {};
          apply_action.nocp = 1;  // notify the CP function about the arrival of
                                  // a first DL packet

          far.set(far_id);
          far.set(apply_action);
          // Add IEs to message
          n4_ser->pfcp_ies.set(far);

          send_n4 = true;

        } else {
          Logger::smf_app().info(
              "Update SM Context procedure, could not get FAR ID of QoS Flow "
              "ID %d",
              flow.qfi.qfi);
        }

        // for UL
        if (flow.far_id_ul.first) {
          pfcp::update_far far              = {};
          pfcp::far_id_t far_id             = {};
          far_id.far_id                     = flow.far_id_ul.second.far_id;
          pfcp::apply_action_t apply_action = {};
          apply_action.drop                 = 1;

          far.set(far_id);
          far.set(apply_action);
          // Add IEs to message
          n4_ser->pfcp_ies.set(far);
          send_n4 = true;
        }

        // update in the PDU Session
        flow.mark_as_released();
        sps->add_qos_flow(flow);
      }
    } break;

    default: {
      Logger::smf_app().error(
          "Update SM Context procedure: Unknown session management type %d",
          session_procedure_type);
    }
  }

  if (send_n4) {
    Logger::smf_app().info(
        "Sending ITTI message %s to task TASK_SMF_N4", n4_ser->get_msg_name());
    int ret = itti_inst->send_msg(n4_triggered);
    if (RETURNok != ret) {
      Logger::smf_app().error(
          "Could not send ITTI message %s to task TASK_SMF_N4",
          n4_ser->get_msg_name());
      return RETURNerror;
    }
  } else {
    Logger::smf_app().error(
        "Update SM Context procedure: There is no QoS flow to be modified");
    return RETURNerror;
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
void session_update_sm_context_procedure::handle_itti_msg(
    itti_n4_session_modification_response& resp,
    std::shared_ptr<smf::smf_context> sc) {
  std::string n1_sm_msg, n1_sm_msg_hex;
  std::string n2_sm_info, n2_sm_info_hex;

  Logger::smf_app().info(
      "Handle N4 Session Modification Response (PDU Session Id %d)",
      n11_trigger.get()->req.get_pdu_session_id());

  pfcp::cause_t cause = {};
  resp.pfcp_ies.get(cause);

  // list of accepted QFI(s) and AN Tunnel Info corresponding to the PDU Session
  std::vector<pfcp::qfi_t> list_of_qfis_to_be_modified = {};
  n11_trigger->req.get_qfis(list_of_qfis_to_be_modified);

  Logger::smf_app().debug(
      "Session procedure type: %s",
      session_management_procedures_type_e2str
          .at(static_cast<int>(session_procedure_type))
          .c_str());

  switch (session_procedure_type) {
    case session_management_procedures_type_e::
        PDU_SESSION_ESTABLISHMENT_UE_REQUESTED:
    case session_management_procedures_type_e::
        PDU_SESSION_MODIFICATION_SMF_REQUESTED:
    case session_management_procedures_type_e::
        PDU_SESSION_MODIFICATION_AN_REQUESTED:
    case session_management_procedures_type_e::
        SERVICE_REQUEST_UE_TRIGGERED_STEP2:
    case session_management_procedures_type_e::
        PDU_SESSION_MODIFICATION_UE_INITIATED_STEP2: {
      pfcp::fteid_t dl_fteid = {};
      n11_trigger->req.get_dl_fteid(dl_fteid);

      Logger::smf_app().debug(
          "AN F-TEID ID"
          "0x%" PRIx32 ", IP Addr %s",
          dl_fteid.teid, conv::toString(dl_fteid.ipv4_address).c_str());

      std::map<uint8_t, qos_flow_context_updated>
          qos_flow_context_to_be_updateds = {};
      n11_triggered_pending->res.get_all_qos_flow_context_updateds(
          qos_flow_context_to_be_updateds);
      n11_triggered_pending->res.remove_all_qos_flow_context_updateds();

      for (auto it : qos_flow_context_to_be_updateds)
        Logger::smf_app().debug(
            "QoS Flow context to be modified QFI %d", it.first);

      for (auto it_created_pdr : resp.pfcp_ies.created_pdrs) {
        pfcp::pdr_id_t pdr_id = {};
        if (it_created_pdr.get(pdr_id)) {
          smf_qos_flow flow = {};
          if (sps->get_qos_flow(pdr_id, flow)) {
            Logger::smf_app().debug("QoS Flow, QFI %d", flow.qfi.qfi);
            for (auto it : qos_flow_context_to_be_updateds) {
              flow.dl_fteid = dl_fteid;
              if (it_created_pdr.get(flow.ul_fteid)) {
                Logger::smf_app().debug(
                    "Got local_up_fteid from created_pdr %s",
                    flow.ul_fteid.toString().c_str());
              } else {
                // UPF doesn't include its fteid in the response
                Logger::smf_app().debug(
                    "Could not get local_up_fteid from created_pdr");
              }

              flow.released = false;
              sps->add_qos_flow(flow);

              qos_flow_context_updated qcu = {};
              qcu.set_cause(static_cast<uint8_t>(
                  cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
              qcu.set_qfi(pfcp::qfi_t(it.first));
              qcu.set_ul_fteid(flow.ul_fteid);
              qcu.set_dl_fteid(flow.dl_fteid);
              qcu.set_qos_profile(flow.qos_profile);
              n11_triggered_pending->res.add_qos_flow_context_updated(qcu);
              // TODO: remove this QFI from the list (as well as in
              // n11_trigger->req)
              break;
            }
          }
        } else {
          Logger::smf_app().error(
              "Could not get pdr_id for created_pdr in %s",
              resp.pfcp_ies.get_msg_name());
        }
      }

      if (cause.cause_value == CAUSE_VALUE_REQUEST_ACCEPTED) {
        // TODO failed rule id
        for (auto it_update_far : n4_triggered->pfcp_ies.update_fars) {
          pfcp::far_id_t far_id = {};
          if (it_update_far.get(far_id)) {
            smf_qos_flow flow = {};
            if (sps->get_qos_flow(far_id, flow)) {
              for (auto it : qos_flow_context_to_be_updateds) {
                if (it.first == flow.qfi.qfi) {
                  flow.dl_fteid = dl_fteid;
                  sps->add_qos_flow(flow);

                  qos_flow_context_updated qcu = {};
                  qcu.set_cause(static_cast<uint8_t>(
                      cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
                  qcu.set_qfi(pfcp::qfi_t(it.first));
                  qcu.set_ul_fteid(flow.ul_fteid);
                  qcu.set_dl_fteid(flow.dl_fteid);
                  qcu.set_qos_profile(flow.qos_profile);
                  n11_triggered_pending->res.add_qos_flow_context_updated(qcu);
                  break;
                }
              }
            } else {
              Logger::smf_app().error(
                  "Could not get QoS flow for far_id for update_far in %s",
                  resp.pfcp_ies.get_msg_name());
            }
          } else {
            Logger::smf_app().error(
                "Could not get far_id for update_far in %s",
                resp.pfcp_ies.get_msg_name());
          }
        }
      } else {
        Logger::smf_app().info(
            "PDU Session Update SM Context, rejected by UPF");
        // trigger to send reply to AMF
        smf_app_inst->trigger_update_context_error_response(
            http_status_code_e::HTTP_STATUS_CODE_403_FORBIDDEN,
            PDU_SESSION_APPLICATION_ERROR_NETWORK_FAILURE,
            n11_triggered_pending->pid);
        return;
      }
    } break;

    case session_management_procedures_type_e::
        PDU_SESSION_RELEASE_AMF_INITIATED:
    case session_management_procedures_type_e::
        PDU_SESSION_RELEASE_UE_REQUESTED_STEP1: {
      if (cause.cause_value == CAUSE_VALUE_REQUEST_ACCEPTED) {
        Logger::smf_app().info(
            "PDU Session Update SM Context (PDU Session Release) accepted by "
            "UPF");
        // clear the resources including addresses allocated to this Session and
        // associated QoS flows
        sps->deallocate_ressources(
            n11_trigger.get()->req.get_dnn());  // TODO: for IPv6 (only for Ipv4
                                                // for the moment)
      }
    }
  }

  n11_triggered_pending->res.set_cause(cause.cause_value);
  n11_triggered_pending->res.set_http_code(
      http_status_code_e::HTTP_STATUS_CODE_200_OK);

  // TODO: check we got all responses vs
  // n11_triggered_pending->res.flow_context_modified

  // TODO: Optional: send ITTI message to N10 to trigger UDM registration
  // (Nudm_UECM_Registration)  see TS29503_Nudm_UECM.yaml (
  // /{ueId}/registrations/smf-registrations/{pduSessionId}:)

  // Prepare response to send to AMF (N1N2MessageTransfer or
  // PDUSession_UpdateSMContextResponse)
  nlohmann::json sm_context_updated_data = {};
  sm_context_updated_data["n1MessageContainer"]["n1MessageClass"] =
      N1N2_MESSAGE_CLASS;
  sm_context_updated_data["n1MessageContainer"]["n1MessageContent"]
                         ["contentId"] = N1_SM_CONTENT_ID;
  sm_context_updated_data["n2InfoContainer"]["n2InformationClass"] =
      N1N2_MESSAGE_CLASS;
  sm_context_updated_data["n2InfoContainer"]["smInfo"]["PduSessionId"] =
      n11_triggered_pending->res.get_pdu_session_id();
  sm_context_updated_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]
                         ["ngapData"]["contentId"] = N2_SM_CONTENT_ID;

  switch (session_procedure_type) {
      // PDU Session Establishment UE-Requested
    case session_management_procedures_type_e::
        PDU_SESSION_ESTABLISHMENT_UE_REQUESTED: {
      // No need to create N1/N2 Container, just Cause
      Logger::smf_app().info(
          "PDU Session Establishment Request (UE-Initiated)");
      nlohmann::json json_data = {};
      json_data["cause"]       = n11_triggered_pending->res.get_cause();
      n11_triggered_pending->res.set_json_data(json_data);

      // Update PDU session status to ACTIVE
      sps->set_pdu_session_status(pdu_session_status_e::PDU_SESSION_ACTIVE);

      // set UpCnxState to DEACTIVATED
      sps->set_upCnx_state(upCnx_state_e::UPCNX_STATE_ACTIVATED);
    } break;

      // UE-Triggered Service Request Procedure (Step 1)
    case session_management_procedures_type_e::
        SERVICE_REQUEST_UE_TRIGGERED_STEP1: {
      // Create N2 SM Information: PDU Session Resource Setup Request Transfer
      // IE

      // N2 SM Information
      smf_n2::get_instance()
          .create_n2_pdu_session_resource_setup_request_transfer(
              n11_triggered_pending->res, n2_sm_info_type_e::PDU_RES_SETUP_REQ,
              n2_sm_info);

      smf_app_inst->convert_string_2_hex(n2_sm_info, n2_sm_info_hex);
      n11_triggered_pending->res.set_n2_sm_information(n2_sm_info_hex);

      // fill the content of SmContextUpdatedData
      nlohmann::json json_data                           = {};
      json_data["n2InfoContainer"]["n2InformationClass"] = N1N2_MESSAGE_CLASS;
      json_data["n2InfoContainer"]["smInfo"]["PduSessionId"] =
          n11_triggered_pending->res.get_pdu_session_id();
      json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapData"]
               ["contentId"] = N2_SM_CONTENT_ID;
      json_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapIeType"] =
          "PDU_RES_SETUP_REQ";  // NGAP message
      json_data["upCnxState"] = "ACTIVATING";
      n11_triggered_pending->res.set_json_data(json_data);
      // TODO: verify whether cause is needed (as in 23.502 but not in 3GPP
      // TS 29.502)

      // Update upCnxState to ACTIVATING
      sps->set_upCnx_state(upCnx_state_e::UPCNX_STATE_ACTIVATING);
    } break;

      // UE-triggered Service Request (Step 2)
    case session_management_procedures_type_e::
        SERVICE_REQUEST_UE_TRIGGERED_STEP2: {
      // No need to create N1/N2 Container, just Cause
      Logger::smf_app().info("UE Triggered Service Request (Step 2)");
      nlohmann::json json_data = {};
      json_data["cause"]       = n11_triggered_pending->res.get_cause();
      json_data["upCnxState"]  = "ACTIVATED";
      n11_triggered_pending->res.set_json_data(json_data);
      n11_triggered_pending->res.set_http_code(
          http_status_code_e::HTTP_STATUS_CODE_200_OK);
    } break;

      // PDU Session Modification UE-initiated (Step 2)
    case session_management_procedures_type_e::
        PDU_SESSION_MODIFICATION_UE_INITIATED_STEP2: {
      // No need to create N1/N2 Container
      Logger::smf_app().info("PDU Session Modification UE-initiated (Step 2)");
    } break;

      // PDU Session Modification UE-initiated (Step 3)
    case session_management_procedures_type_e::
        PDU_SESSION_MODIFICATION_UE_INITIATED_STEP3: {
      // No need to create N1/N2 Container
      Logger::smf_app().info("PDU Session Modification UE-initiated (Step 3)");
      // TODO: To be completed
    } break;

      // PDU Session Release UE-initiated (Step 1)
    case session_management_procedures_type_e::
        PDU_SESSION_RELEASE_UE_REQUESTED_STEP1: {
      // N1 SM: PDU Session Release Command​
      // N2 SM: PDU Session Resource Release Command Transfer
      Logger::smf_app().info("PDU Session Release UE-initiated (Step 1))");

      // N1 SM
      smf_n1::get_instance().create_n1_pdu_session_release_command(
          n11_triggered_pending->res, n1_sm_msg,
          cause_value_5gsm_e::CAUSE_26_INSUFFICIENT_RESOURCES);  // TODO: check
                                                                 // Cause
      smf_app_inst->convert_string_2_hex(n1_sm_msg, n1_sm_msg_hex);
      n11_triggered_pending->res.set_n1_sm_message(n1_sm_msg_hex);

      // include N2 SM Resource Release Request only when User Plane connection
      // is activated
      if (sps->get_upCnx_state() == upCnx_state_e::UPCNX_STATE_ACTIVATED) {
        // N2 SM Information
        smf_n2::get_instance()
            .create_n2_pdu_session_resource_release_command_transfer(
                n11_triggered_pending->res, n2_sm_info_type_e::PDU_RES_REL_CMD,
                n2_sm_info);
        smf_app_inst->convert_string_2_hex(n2_sm_info, n2_sm_info_hex);
        n11_triggered_pending->res.set_n2_sm_information(n2_sm_info_hex);

        // fill the content of SmContextUpdatedData
        sm_context_updated_data["n2SmInfoType"] =
            //["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapIeType"] =
            "PDU_RES_REL_CMD";  // NGAP message
        n11_triggered_pending->res.set_json_data(sm_context_updated_data);
      } else {
        // fill the content of SmContextUpdatedData
        nlohmann::json json_data                          = {};
        json_data["n1MessageContainer"]["n1MessageClass"] = N1N2_MESSAGE_CLASS;
        json_data["n1MessageContainer"]["n1MessageContent"]["contentId"] =
            N1_SM_CONTENT_ID;
        n11_triggered_pending->res.set_json_data(json_data);
      }

      // Update PDU session status to PDU_SESSION_INACTIVE_PENDING
      sps->set_pdu_session_status(
          pdu_session_status_e::PDU_SESSION_INACTIVE_PENDING);

      // set UpCnxState to DEACTIVATED
      sps->set_upCnx_state(upCnx_state_e::UPCNX_STATE_DEACTIVATED);

      // TODO: To be completed
      // TODO: start timer T3592 (see Section 6.3.3@3GPP TS 24.501)
      // get smf_pdu_session and set the corresponding timer
      sps->timer_T3592 = itti_inst->timer_setup(
          T3591_TIMER_VALUE_SEC, 0, TASK_SMF_APP, TASK_SMF_APP_TRIGGER_T3592,
          n11_trigger->req.get_pdu_session_id());

      // TODO: How SMF can retransmit the PDU SESSION RELEASE COMMAND message on
      // the expiry of the timer T3592

    } break;

      // PDU Session Release UE-initiated (Step 2)
    case session_management_procedures_type_e::
        PDU_SESSION_RELEASE_UE_REQUESTED_STEP2: {
      // No need to create N1/N2 Container
      Logger::smf_app().info("PDU Session Release UE-initiated (Step 2)");
      // TODO: To be completed
    } break;

      // PDU Session Release UE-initiated (Step 3)
    case session_management_procedures_type_e::
        PDU_SESSION_RELEASE_UE_REQUESTED_STEP3: {
      // No need to create N1/N2 Container
      Logger::smf_app().info("PDU Session Release UE-initiated (Step 3)");
      // TODO: To be completed
    } break;

    default: {
      Logger::smf_app().info(
          "Unknown session procedure type %d", session_procedure_type);
    }
  }

  // send ITTI message to SMF_APP interface to trigger
  // SessionUpdateSMContextResponse towards AMFs
  Logger::smf_app().info(
      "Sending ITTI message %s to task TASK_SMF_APP",
      n11_triggered_pending->get_msg_name());
  n11_triggered_pending->session_procedure_type = session_procedure_type;
  int ret = itti_inst->send_msg(n11_triggered_pending);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_APP",
        n11_triggered_pending->get_msg_name());
  }

  // The SMF may subscribe to the UE mobility event notification from the AMF
  // (e.g. location reporting, UE  moving into or out of Area Of Interest), by
  // invoking Namf_EventExposure_Subscribe service operation
  // For LADN, the SMF subscribes to the UE moving into or out of LADN service
  // area event notification by providing the LADN DNN as an indicator for the
  // Area Of Interest
  // see step 17@section 4.3.2.2.1@3GPP TS 23.502

  if (cause.cause_value != CAUSE_VALUE_REQUEST_ACCEPTED) {
    // TODO: Nsmf_PDUSession_SMContextStatusNotify
    /*  If the PDU Session establishment is not successful, the SMF informs the
     AMF by invoking Nsmf_PDUSession_SMContextStatusNotify (Release). The SMF
     also releases any N4 session(s) created, any PDU Session address if
     allocated (e.g. IP address) and releases the association with PCF, if any.
     see step 18, section 4.3.2.2.1@3GPP TS 23.502)
     */
  }
}

//------------------------------------------------------------------------------
int session_release_sm_context_procedure::run(
    std::shared_ptr<itti_n11_release_sm_context_request> sm_context_req,
    std::shared_ptr<itti_n11_release_sm_context_response> sm_context_res,
    std::shared_ptr<smf::smf_context> sc) {
  Logger::smf_app().info("Release SM Context Request");
  // TODO check if compatible with ongoing procedures if any
  pfcp::node_id_t up_node_id = {};
  // Get UPF node
  std::shared_ptr<smf_context_ref> scf = {};
  scid_t scid                          = {};
  try {
    scid = std::stoi(sm_context_req->scid);
  } catch (const std::exception& err) {
    Logger::smf_app().warn(
        "SM Context associated with this id %s does not exit!",
        sm_context_req->scid);
  }
  if (smf_app_inst->is_scid_2_smf_context(scid)) {
    scf        = smf_app_inst->scid_2_smf_context(scid);
    up_node_id = scf.get()->upf_node_id;
  } else {
    Logger::smf_app().warn(
        "SM Context associated with this id " SCID_FMT " does not exit!", scid);
    // TODO:
  }

  /*  if (not pfcp_associations::get_instance().select_up_node(
            up_node_id, NODE_SELECTION_CRITERIA_MIN_PFCP_SESSIONS)) {
      sm_context_res->res.set_cause(
          PDU_SESSION_APPLICATION_ERROR_PEER_NOT_RESPONDING);
      Logger::smf_app().info("REMOTE_PEER_NOT_RESPONDING");
      return RETURNerror;
    }
  */
  //-------------------
  n11_trigger           = sm_context_req;
  n11_triggered_pending = sm_context_res;
  uint64_t seid         = smf_app_inst->generate_seid();
  sps->set_seid(seid);
  itti_n4_session_deletion_request* n4_ser =
      new itti_n4_session_deletion_request(TASK_SMF_APP, TASK_SMF_N4);
  n4_ser->seid       = sps->up_fseid.seid;
  n4_ser->trxn_id    = this->trxn_id;
  n4_ser->r_endpoint = endpoint(up_node_id.u1.ipv4_address, pfcp::default_port);
  n4_triggered = std::shared_ptr<itti_n4_session_deletion_request>(n4_ser);

  Logger::smf_app().info(
      "Sending ITTI message %s to task TASK_SMF_N4", n4_ser->get_msg_name());
  int ret = itti_inst->send_msg(n4_triggered);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_SMF_N4",
        n4_ser->get_msg_name());
    return RETURNerror;
  }
  return RETURNok;
}

//------------------------------------------------------------------------------
void session_release_sm_context_procedure::handle_itti_msg(
    itti_n4_session_deletion_response& resp,
    std::shared_ptr<smf::smf_context> sc) {
  Logger::smf_app().info(
      "Handle itti_n4_session_deletion_response (Release SM Context Request): "
      "pdu-session-id %d",
      n11_trigger.get()->req.get_pdu_session_id());

  pfcp::cause_t cause = {};
  resp.pfcp_ies.get(cause);

  if (cause.cause_value == CAUSE_VALUE_REQUEST_ACCEPTED) {
    Logger::smf_app().info("PDU Session Release SM Context accepted by UPF");
    // clear the resources including addresses allocated to this Session and
    // associated QoS flows
    sps->deallocate_ressources(
        n11_trigger.get()
            ->req.get_dnn());  // TODO: for IPv6 (only for Ipv4 for the moment)
    // trigger to send reply to AMF
    smf_app_inst->trigger_http_response(
        http_status_code_e::HTTP_STATUS_CODE_204_NO_CONTENT,
        n11_triggered_pending->pid, N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE);
  } else {
    oai::smf_server::model::ProblemDetails problem_details = {};
    problem_details.setCause(pdu_session_application_error_e2str.at(
        PDU_SESSION_APPLICATION_ERROR_NETWORK_FAILURE));
    // trigger to send reply to AMF
    /*
    smf_app_inst->trigger_http_response(
    http_status_code_e::HTTP_STATUS_CODE_406_NOT_ACCEPTABLE,
    n11_triggered_pending->pid, N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE);
    */
  }

  // TODO:
  /* If it is the last PDU Session the SMF is handling for the UE for the
   associated (DNN, S- NSSAI), the SMF unsubscribes from Session Management
   Subscription data changes notification with the UDM by means of the
   Nudm_SDM_Unsubscribe service operation. The SMF invokes the
   Nudm_UECM_Deregistration service operation so that the UDM removes the
   association it had stored between the SMF identity and the associated DNN and
   PDU Session Id
   */
}
