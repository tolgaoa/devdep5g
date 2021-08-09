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

/*! \file smf_n1.cpp
 \brief
 \author  Tien-Thinh NGUYEN, Keliang DU
 \company Eurecom
 \date 2019
 \email:  tien-thinh.nguyen@eurecom.fr
 */

#include "smf_n1.hpp"

#include <arpa/inet.h>
#include <stdexcept>
#include "string.hpp"

#include "smf.h"
#include "smf_app.hpp"
#include "3gpp_conversions.hpp"
#include "epc.h"

extern "C" {
#include "dynamic_memory_check.h"
#include "nas_message.h"
}

using namespace smf;
extern smf_app* smf_app_inst;

//-----------------------------------------------------------------------------------------------------
bool smf_n1::create_n1_pdu_session_establishment_accept(
    pdu_session_create_sm_context_response& sm_context_res,
    std::string& nas_msg_str, cause_value_5gsm_e sm_cause) {
  Logger::smf_n1().info(
      "Create N1 SM Container, PDU Session Establishment Accept");
  int bytes                   = {0};
  unsigned char data[BUF_LEN] = {'\0'};
  nas_message_t nas_msg       = {};
  bool result                 = false;

  nas_msg.header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_NOT_PROTECTED;
  // nas_msg.header.sequence_number = 0xfe;
  // nas_msg.header.message_authentication_code = 0xffee;

  SM_msg* sm_msg = &nas_msg.plain.sm;

  // Fill the content of SM header
  sm_msg->header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  // PDU Session Identity
  sm_msg->header.pdu_session_identity = sm_context_res.get_pdu_session_id();

  // get default QoS value
  qos_flow_context_updated qos_flow = {};
  qos_flow                          = sm_context_res.get_qos_flow_context();
  // check the QoS Flow
  if ((qos_flow.qfi.qfi < QOS_FLOW_IDENTIFIER_FIRST) or
      (qos_flow.qfi.qfi > QOS_FLOW_IDENTIFIER_LAST)) {
    Logger::smf_n1().error("Incorrect QFI %d", qos_flow.qfi.qfi);
    return false;
  }

  Logger::smf_n1().info("PDU_SESSION_ESTABLISHMENT_ACCEPT, encode starting...");

  // Fill the rest of SM header
  sm_msg->header.procedure_transaction_identity =
      sm_context_res.get_pti().procedure_transaction_id;
  sm_msg->header.message_type = PDU_SESSION_ESTABLISHMENT_ACCEPT;

  Logger::smf_n1().debug(
      "SM header, Extended Protocol Discriminator 0x%x, PDU Session Identity "
      "%d, Procedure Transaction Identity: %d, Message Type: %d",
      sm_msg->header.extended_protocol_discriminator,
      sm_msg->header.pdu_session_identity,
      sm_msg->header.procedure_transaction_identity,
      sm_msg->header.message_type);

  // Fill the content of PDU Session Establishment Accept message
  sm_msg->pdu_session_establishment_accept._pdusessiontype
      .pdu_session_type_value = sm_context_res.get_pdu_session_type();
  Logger::smf_n1().debug(
      "PDU Session Type: %d", sm_msg->pdu_session_establishment_accept
                                  ._pdusessiontype.pdu_session_type_value);

  // sm_msg->pdu_session_establishment_accept.sscmode.ssc_mode_value =
  //    SSC_MODE_1;  // TODO: get from sm_context_res
  // Logger::smf_n1().debug(
  //    "SSC Mode: %d",
  //    sm_msg->pdu_session_establishment_accept.sscmode.ssc_mode_value);

  // authorized QoS rules of the PDU session: QOSRules (Section 6.2.5@3GPP
  // TS 24.501) (Section 6.4.1.3@3GPP TS 24.501 V16.1.0) Make sure that the
  // number of the packet filters used in the authorized QoS rules of the PDU
  // Session does not exceed the maximum number of packet filters supported by
  // the UE for the PDU session
  if (qos_flow.qos_rules.size() > 0) {
    sm_msg->pdu_session_establishment_accept.qosrules.lengthofqosrulesie =
        qos_flow.qos_rules.size();
    sm_msg->pdu_session_establishment_accept.qosrules.qosrulesie =
        (QOSRulesIE*) calloc(qos_flow.qos_rules.size(), sizeof(QOSRulesIE));

    int i = 0;
    for (auto rule : qos_flow.qos_rules) {
      sm_msg->pdu_session_establishment_accept.qosrules.qosrulesie[i]
          .qosruleidentifer = rule.second.qosruleidentifer;
      memcpy(
          &sm_msg->pdu_session_establishment_accept.qosrules.qosrulesie[i],
          &rule.second, sizeof(QOSRulesIE));
      i++;
    }
  }

  // SessionAMBR
  Logger::smf_n1().debug("Get default values for Session-AMBR");
  supi_t supi                     = sm_context_res.get_supi();
  supi64_t supi64                 = smf_supi_to_u64(supi);
  std::shared_ptr<smf_context> sc = {};
  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n1().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
    sc.get()->get_session_ambr(
        sm_msg->pdu_session_establishment_accept.sessionambr,
        sm_context_res.get_snssai(), sm_context_res.get_dnn());
  } else {
    Logger::smf_n1().warn(
        "SMF context with SUPI " SUPI_64_FMT " does not exist!", supi64);
    // free memory
    if (qos_flow.qos_rules.size() > 0) {
      free_wrapper((void**) &sm_msg->pdu_session_establishment_accept.qosrules
                       .qosrulesie);
    }
    return false;
  }

  sm_msg->pdu_session_establishment_accept.presence = 0x038a;
  if (static_cast<uint8_t>(sm_cause) > 0) {
    sm_msg->pdu_session_establishment_accept.presence = 0x039b;
    sm_msg->pdu_session_establishment_accept._5gsmcause =
        static_cast<uint8_t>(sm_cause);
    Logger::smf_n1().debug(
        "5GSM Cause: %d", sm_msg->pdu_session_establishment_accept._5gsmcause);
  }

  // PDUAddress
  paa_t paa = sm_context_res.get_paa();
  Logger::smf_n1().debug(
      "PDU Session Type %s", paa.pdu_session_type.toString().c_str());
  sm_msg->pdu_session_establishment_accept.pduaddress.pdu_session_type_value =
      static_cast<uint8_t>(paa.pdu_session_type.pdu_session_type);
  if (paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV4) {
    sm_msg->pdu_session_establishment_accept.pduaddress
        .pdu_address_information = bfromcstralloc(4, "\0");
    util::ipv4_to_bstring(
        paa.ipv4_address, sm_msg->pdu_session_establishment_accept.pduaddress
                              .pdu_address_information);
    Logger::smf_n1().debug(
        "UE IPv4 Address %s", conv::toString(paa.ipv4_address).c_str());
  } else if (
      paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV4V6) {
    sm_msg->pdu_session_establishment_accept.pduaddress
        .pdu_address_information = bfromcstralloc(12, "\0");
    util::ipv4v6_to_pdu_address_information(
        paa.ipv4_address, paa.ipv6_address,
        sm_msg->pdu_session_establishment_accept.pduaddress
            .pdu_address_information);
    Logger::smf_n1().debug(
        "UE IPv4 Address %s", conv::toString(paa.ipv4_address).c_str());
    char str_addr6[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET6, &paa.ipv6_address, str_addr6, sizeof(str_addr6))) {
      Logger::smf_n1().debug("UE IPv6 Address: %s", str_addr6);
    }
  } else if (paa.pdu_session_type.pdu_session_type == PDU_SESSION_TYPE_E_IPV6) {
    // TODO:
    Logger::smf_n1().debug("IPv6 is not fully supported yet!");
  }

  // TODO: GPRSTimer
  // sm_msg->pdu_session_establishment_accept.gprstimer.unit =
  // GPRSTIMER_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_2_SECONDS;
  // sm_msg->pdu_session_establishment_accept.gprstimer.timeValue = 0;

  // SNSSAI
  sm_msg->pdu_session_establishment_accept.snssai.len = SST_AND_SD_LENGTH;
  sm_msg->pdu_session_establishment_accept.snssai.sst =
      sm_context_res.get_snssai().sST;

  try {
    sm_msg->pdu_session_establishment_accept.snssai.sd =
        std::stoul(sm_context_res.get_snssai().sD, nullptr, 10);
  } catch (const std::exception& e) {
    Logger::smf_n1().warn(
        "Error when converting from string to int for snssai.SD, error: %s",
        e.what());
    //"no SD value associated with the SST"
    sm_msg->pdu_session_establishment_accept.snssai.sd = 0xFFFFFF;
  }

  Logger::smf_n1().debug(
      "SNSSAI SST %d, SD %#0x",
      sm_msg->pdu_session_establishment_accept.snssai.sst,
      sm_msg->pdu_session_establishment_accept.snssai.sd);

  // TODO: AlwaysonPDUSessionIndication
  // sm_msg->pdu_session_establishment_accept.alwaysonpdusessionindication.apsi_indication
  // = ALWAYSON_PDU_SESSION_REQUIRED;

  // TODO: MappedEPSBearerContexts
  // TODO: EAPMessage

  // authorized QoS flow descriptions IE: QoSFlowDescritions
  // TODO: we may not need this IE (see section 6.4.1.3 @3GPP TS 24.501)
  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n1().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
    sm_msg->pdu_session_establishment_accept.qosflowdescriptions
        .qosflowdescriptionsnumber = 1;
    sm_msg->pdu_session_establishment_accept.qosflowdescriptions
        .qosflowdescriptionscontents = (QOSFlowDescriptionsContents*) calloc(
        1, sizeof(QOSFlowDescriptionsContents));
    sc.get()->get_default_qos_flow_description(
        sm_msg->pdu_session_establishment_accept.qosflowdescriptions
            .qosflowdescriptionscontents[0],
        sm_context_res.get_pdu_session_type(), qos_flow.qfi);
  }

  // TODO: ExtendedProtocolConfigurationOptions
  protocol_configuration_options_t pco_res = {};
  sm_context_res.get_epco(pco_res);
  // sm_msg->pdu_session_establishment_accept.
  xgpp_conv::pco_core_to_nas(
      pco_res, sm_msg->pdu_session_establishment_accept
                   .extendedprotocolconfigurationoptions);

  // DNN
  plmn_t plmn = {};
  sc.get()->get_plmn(plmn);
  std::string gprs = EPC::Utility::home_network_gprs(plmn);
  std::string full_dnn =
      sm_context_res.get_dnn() + gprs;  //".mnc011.mcc110.gprs";
  std::string dotted;
  util::string_to_dotted(full_dnn, dotted);
  Logger::smf_n1().debug(
      "Full DNN %s, dotted DNN %s", full_dnn.c_str(), dotted.c_str());
  sm_msg->pdu_session_establishment_accept.dnn =
      bfromcstralloc(dotted.length() + 1, "\0");
  util::string_to_dnn(dotted, sm_msg->pdu_session_establishment_accept.dnn);

  Logger::smf_n1().info("Encode PDU Session Establishment Accept");
  // Encode NAS message
  bytes = nas_message_encode(
      data, &nas_msg, sizeof(data) /*don't know the size*/, nullptr);

#if DEBUG_IS_ON
  Logger::smf_n1().debug("Buffer Data: ");
  for (int i = 0; i < bytes; i++) printf("%02x ", data[i]);
  printf(" (bytes %d)\n", bytes);
#endif

  if (bytes > 0) {
    std::string n1Message((char*) data, bytes);
    nas_msg_str = n1Message;
    result      = true;
  } else {
    result = false;
  }

  // free memory
  if (qos_flow.qos_rules.size() > 0) {
    free_wrapper(
        (void**) &sm_msg->pdu_session_establishment_accept.qosrules.qosrulesie);
  }
  free_wrapper((void**) &sm_msg->pdu_session_establishment_accept
                   .qosflowdescriptions.qosflowdescriptionscontents);

  return result;
}

//-----------------------------------------------------------------------------------------------------
bool smf_n1::create_n1_pdu_session_establishment_reject(
    pdu_session_msg& msg, std::string& nas_msg_str,
    cause_value_5gsm_e sm_cause) {
  Logger::smf_n1().info(
      "Create N1 SM Container, PDU Session Establishment Reject");
  int bytes                   = {0};
  unsigned char data[BUF_LEN] = {'\0'};
  nas_message_t nas_msg       = {};
  bool result                 = false;

  nas_msg.header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_NOT_PROTECTED;

  SM_msg* sm_msg = &nas_msg.plain.sm;

  // Fill the content of SM header
  sm_msg->header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  sm_msg->header.pdu_session_identity = msg.get_pdu_session_id();

  Logger::smf_n1().info("PDU_SESSION_ESTABLISHMENT_REJECT, encode starting...");

  // Fill the content of PDU Session Establishment Reject message
  sm_msg->header.pdu_session_identity = msg.get_pdu_session_id();
  sm_msg->header.procedure_transaction_identity =
      msg.get_pti().procedure_transaction_id;
  sm_msg->header.message_type = PDU_SESSION_ESTABLISHMENT_REJECT;
  Logger::smf_n1().debug(
      "NAS header, Extended Protocol Discriminator 0x%x, Security Header "
      "Type: 0x%x",
      nas_msg.header.extended_protocol_discriminator,
      nas_msg.header.security_header_type);
  Logger::smf_n1().debug(
      "SM header, PDU Session Identity 0x%x, Procedure Transaction Identity "
      "0x%x, Message Type 0x%x",
      sm_msg->header.pdu_session_identity,
      sm_msg->header.procedure_transaction_identity,
      sm_msg->header.message_type);

  sm_msg->pdu_session_establishment_reject._5gsmcause =
      static_cast<uint8_t>(sm_cause);
  // Presence, should be updated according to the following IEs
  sm_msg->pdu_session_establishment_reject.presence =
      PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENCE;
  /*
   //TODO: GPRSTimer3
   sm_msg->pdu_session_establishment_reject.gprstimer3.unit =
   GPRSTIMER3_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_HOUR;
   sm_msg->pdu_session_establishment_reject.gprstimer3.timeValue = 0;
   */
  // AllowedSSCMode
  sm_msg->pdu_session_establishment_reject.allowedsscmode.is_ssc1_allowed =
      SSC_MODE1_ALLOWED;
  sm_msg->pdu_session_establishment_reject.allowedsscmode.is_ssc2_allowed =
      SSC_MODE2_NOT_ALLOWED;
  sm_msg->pdu_session_establishment_reject.allowedsscmode.is_ssc3_allowed =
      SSC_MODE3_NOT_ALLOWED;

  Logger::smf_n1().debug(
      "SM MSG, 5GSM Cause: 0x%x",
      sm_msg->pdu_session_establishment_reject._5gsmcause);
  Logger::smf_n1().debug(
      "SM MSG, Allowed SSC Mode, SSC1 allowed 0x%x, SSC2 allowed 0x%x, SSC3 "
      "allowed 0x%x",
      sm_msg->pdu_session_establishment_reject.allowedsscmode.is_ssc1_allowed,
      sm_msg->pdu_session_establishment_reject.allowedsscmode.is_ssc2_allowed,
      sm_msg->pdu_session_establishment_reject.allowedsscmode.is_ssc3_allowed);

  // Encode NAS message
  bytes = nas_message_encode(
      data, &nas_msg, sizeof(data) /*don't know the size*/, nullptr);

#if DEBUG_IS_ON
  Logger::smf_n1().debug("Buffer Data: ");
  for (int i = 0; i < bytes; i++) printf("%02x ", data[i]);
  printf(" (bytes %d)\n", bytes);
#endif

  if (bytes > 0) {
    std::string n1Message((char*) data, bytes);
    nas_msg_str = n1Message;
    result      = true;
  } else {
    result = false;
  }

  return result;
}

//-----------------------------------------------------------------------------------------------------
bool smf_n1::create_n1_pdu_session_modification_command(
    pdu_session_update_sm_context_response& sm_context_res,
    std::string& nas_msg_str, cause_value_5gsm_e sm_cause) {
  Logger::smf_n1().info(
      "Create N1 SM Container, PDU Session Modification Command "
      "(pdu_session_update_sm_context_response)");

  int bytes                   = {0};
  unsigned char data[BUF_LEN] = {'\0'};
  nas_message_t nas_msg       = {};
  bool result                 = false;

  nas_msg.header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_NOT_PROTECTED;

  SM_msg* sm_msg = &nas_msg.plain.sm;

  // Fill the content of SM header
  sm_msg->header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  sm_msg->header.pdu_session_identity = sm_context_res.get_pdu_session_id();

  Logger::smf_n1().debug("PDU Session Modification Command");
  Logger::smf_n1().info("PDU_SESSION_MODIFICATION_COMMAND, encode starting...");

  // Get the SMF_PDU_Session
  std::shared_ptr<smf_context> sc     = {};
  std::shared_ptr<dnn_context> sd     = {};
  std::shared_ptr<smf_pdu_session> sp = {};
  supi_t supi                         = sm_context_res.get_supi();
  supi64_t supi64                     = smf_supi_to_u64(supi);

  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n1().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
  } else {
    Logger::smf_n1().warn(
        "SMF context with SUPI " SUPI_64_FMT " does not exist!", supi64);
    return false;
  }

  bool find_dnn = sc.get()->find_dnn_context(
      sm_context_res.get_snssai(), sm_context_res.get_dnn(), sd);
  bool find_pdu = false;
  if (find_dnn) {
    find_pdu =
        sd.get()->find_pdu_session(sm_context_res.get_pdu_session_id(), sp);
  }
  if (!find_dnn or !find_pdu) {
    // error
    Logger::smf_n1().warn("DNN or PDU session context does not exist!");
    return false;
  }

  sm_msg->header.procedure_transaction_identity =
      sm_context_res.get_pti().procedure_transaction_id;
  sm_msg->header.message_type = PDU_SESSION_MODIFICATION_COMMAND;
  sm_msg->pdu_session_modification_command.presence =
      0xff;  // TODO: to be updated
  sm_msg->pdu_session_modification_command._5gsmcause =
      static_cast<uint8_t>(sm_cause);
  // SessionAMBR (default)
  sc.get()->get_session_ambr(
      sm_msg->pdu_session_modification_command.sessionambr,
      sm_context_res.get_snssai(), sm_context_res.get_dnn());

  // TODO: GPRSTimer
  // TODO: AlwaysonPDUSessionIndication

  // QOSRules
  // Get the authorized QoS Rules
  std::vector<QOSRulesIE> qos_rules;
  sp.get()->get_qos_rules_to_be_synchronised(qos_rules);

  if (qos_rules.size() == 0) {
    return false;
  }

  sm_msg->pdu_session_modification_command.qosrules.lengthofqosrulesie =
      qos_rules.size();

  sm_msg->pdu_session_modification_command.qosrules.qosrulesie =
      (QOSRulesIE*) calloc(qos_rules.size(), sizeof(QOSRulesIE));
  for (int i = 0; i < qos_rules.size(); i++) {
    Logger::smf_n1().debug(
        "QoS Rule to be updated (Id %d)", qos_rules[i].qosruleidentifer);
    memcpy(
        &sm_msg->pdu_session_modification_command.qosrules.qosrulesie[i],
        &qos_rules[i], sizeof(QOSRulesIE));
  }

  // TODO: MappedEPSBearerContexts

  // QOSFlowDescriptions
  // TODO: get authorized QoS flow descriptions IE
  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n1().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
    sm_msg->pdu_session_modification_command.qosflowdescriptions
        .qosflowdescriptionsnumber = 1;
    sm_msg->pdu_session_modification_command.qosflowdescriptions
        .qosflowdescriptionscontents = (QOSFlowDescriptionsContents*) calloc(
        1, sizeof(QOSFlowDescriptionsContents));
    sc.get()->get_default_qos_flow_description(
        sm_msg->pdu_session_modification_command.qosflowdescriptions
            .qosflowdescriptionscontents[0],
        sm_context_res.get_pdu_session_type(), qos_rules[0].qosflowidentifer);
  }

  // Encode NAS message
  bytes = nas_message_encode(
      data, &nas_msg, sizeof(data) /*don't know the size*/, nullptr);

#if DEBUG_IS_ON
  Logger::smf_n1().debug("Buffer Data: ");
  for (int i = 0; i < bytes; i++) printf("%02x ", data[i]);
  printf(" (bytes %d)\n", bytes);
#endif

  if (bytes > 0) {
    std::string n1Message((char*) data, bytes);
    nas_msg_str = n1Message;
    result      = true;
  } else {
    result = false;
  }

  // free memory
  free_wrapper(
      (void**) &sm_msg->pdu_session_modification_command.qosrules.qosrulesie);
  free_wrapper((void**) &sm_msg->pdu_session_modification_command
                   .qosflowdescriptions.qosflowdescriptionscontents);

  return result;
}

//-----------------------------------------------------------------------------------------------------
bool smf_n1::create_n1_pdu_session_modification_command(
    pdu_session_modification_network_requested& msg, std::string& nas_msg_str,
    cause_value_5gsm_e sm_cause) {
  Logger::smf_n1().info(
      "Create N1 SM Container, PDU Session Modification Command "
      "(pdu_session_modification_network_requested)");

  int bytes                   = {0};
  unsigned char data[BUF_LEN] = {'\0'};
  nas_message_t nas_msg       = {};
  bool result                 = false;

  nas_msg.header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_NOT_PROTECTED;

  SM_msg* sm_msg = &nas_msg.plain.sm;

  // Fill the content of SM header
  sm_msg->header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  sm_msg->header.pdu_session_identity = msg.get_pdu_session_id();

  Logger::smf_n1().debug("PDU Session Modification Command");
  Logger::smf_n1().info("PDU_SESSION_MODIFICATION_COMMAND, encode starting...");

  // Get the SMF_PDU_Session
  std::shared_ptr<smf_context> sc     = {};
  std::shared_ptr<dnn_context> sd     = {};
  std::shared_ptr<smf_pdu_session> sp = {};
  supi_t supi                         = msg.get_supi();
  supi64_t supi64                     = smf_supi_to_u64(supi);

  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n1().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
  } else {
    Logger::smf_n1().warn(
        "SMF context with SUPI " SUPI_64_FMT " does not exist!", supi64);
    return false;
  }

  bool find_dnn =
      sc.get()->find_dnn_context(msg.get_snssai(), msg.get_dnn(), sd);
  bool find_pdu = false;
  if (find_dnn) {
    find_pdu = sd.get()->find_pdu_session(msg.get_pdu_session_id(), sp);
  }
  if (!find_dnn or !find_pdu) {
    // error
    Logger::smf_n1().warn("DNN or PDU session context does not exist!");
    return false;
  }

  sm_msg->header.procedure_transaction_identity =
      msg.get_pti().procedure_transaction_id;
  sm_msg->header.message_type = PDU_SESSION_MODIFICATION_COMMAND;
  sm_msg->pdu_session_modification_command.presence =
      0xff;  // TODO: to be updated
  sm_msg->pdu_session_modification_command._5gsmcause =
      static_cast<uint8_t>(sm_cause);
  // SessionAMBR (default)
  sc.get()->get_session_ambr(
      sm_msg->pdu_session_modification_command.sessionambr, msg.get_snssai(),
      msg.get_dnn());

  // TODO: GPRSTimer
  // TODO: AlwaysonPDUSessionIndication

  // QOSRules
  // Get the authorized QoS Rules
  std::vector<QOSRulesIE> qos_rules;
  sp.get()->get_qos_rules_to_be_synchronised(qos_rules);

  if (qos_rules.size() == 0) {
    return false;
  }

  sm_msg->pdu_session_modification_command.qosrules.lengthofqosrulesie =
      qos_rules.size();

  sm_msg->pdu_session_modification_command.qosrules.qosrulesie =
      (QOSRulesIE*) calloc(qos_rules.size(), sizeof(QOSRulesIE));
  for (int i = 0; i < qos_rules.size(); i++) {
    Logger::smf_n1().debug(
        "QoS Rule to be updated (Id %d)", qos_rules[i].qosruleidentifer);
    memcpy(
        &sm_msg->pdu_session_modification_command.qosrules.qosrulesie[i],
        &qos_rules[i], sizeof(QOSRulesIE));
  }

  // TODO: MappedEPSBearerContexts

  // QOSFlowDescriptions, TODO: get authorized QoS flow descriptions IE
  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n1().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
    sm_msg->pdu_session_modification_command.qosflowdescriptions
        .qosflowdescriptionsnumber = 1;
    sm_msg->pdu_session_modification_command.qosflowdescriptions
        .qosflowdescriptionscontents = (QOSFlowDescriptionsContents*) calloc(
        1, sizeof(QOSFlowDescriptionsContents));
    sc.get()->get_default_qos_flow_description(
        sm_msg->pdu_session_modification_command.qosflowdescriptions
            .qosflowdescriptionscontents[0],
        msg.get_pdu_session_type(), qos_rules[0].qosflowidentifer);
  }

  // Encode NAS message
  bytes = nas_message_encode(
      data, &nas_msg, sizeof(data) /*don't know the size*/, nullptr);

#if DEBUG_IS_ON
  Logger::smf_n1().debug("Buffer Data: ");
  for (int i = 0; i < bytes; i++) printf("%02x ", data[i]);
  printf(" (bytes %d)\n", bytes);
#endif

  if (bytes > 0) {
    std::string n1Message((char*) data, bytes);
    nas_msg_str = n1Message;
    result      = true;
  } else {
    result = false;
  }
  // free memory
  free_wrapper(
      (void**) &sm_msg->pdu_session_modification_command.qosrules.qosrulesie);
  free_wrapper((void**) &sm_msg->pdu_session_modification_command
                   .qosflowdescriptions.qosflowdescriptionscontents);

  return result;
}

//-----------------------------------------------------------------------------------------------------
bool smf_n1::create_n1_pdu_session_release_reject(
    pdu_session_update_sm_context_request& sm_context_res,
    std::string& nas_msg_str, cause_value_5gsm_e sm_cause) {
  Logger::smf_n1().info("Create N1 SM Container, PDU Session Release Reject");

  int bytes                   = {0};
  unsigned char data[BUF_LEN] = {'\0'};
  nas_message_t nas_msg       = {};

  nas_msg.header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_NOT_PROTECTED;

  SM_msg* sm_msg = &nas_msg.plain.sm;

  // Fill the content of SM header
  sm_msg->header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  sm_msg->header.pdu_session_identity = sm_context_res.get_pdu_session_id();

  // Fill the content of PDU Session Release Reject
  sm_msg->header.pdu_session_identity = sm_context_res.get_pdu_session_id();
  sm_msg->header.procedure_transaction_identity =
      sm_context_res.get_pti().procedure_transaction_id;
  sm_msg->header.message_type = PDU_SESSION_RELEASE_REJECT;
  sm_msg->pdu_session_release_reject._5gsmcause =
      static_cast<uint8_t>(sm_cause);

  sm_msg->pdu_session_release_command.presence = 0x00;

  // Encode NAS message
  bytes = nas_message_encode(
      data, &nas_msg, sizeof(data) /*don't know the size*/, nullptr);

#if DEBUG_IS_ON
  Logger::smf_n1().debug("Buffer Data: ");
  for (int i = 0; i < bytes; i++) printf("%02x ", data[i]);
  printf(" (bytes %d)\n", bytes);
#endif

  if (bytes > 0) {
    std::string n1Message((char*) data, bytes);
    nas_msg_str = n1Message;
    return true;
  } else {
    return false;
  }
}

//-----------------------------------------------------------------------------------------------------
bool smf_n1::create_n1_pdu_session_release_command(
    pdu_session_update_sm_context_response& sm_context_res,
    std::string& nas_msg_str, cause_value_5gsm_e sm_cause) {
  Logger::smf_n1().info(
      "Create N1 SM Container, PDU Session Release Command "
      "(pdu_session_update_sm_context_response)");

  int bytes                   = {0};
  unsigned char data[BUF_LEN] = {'\0'};
  nas_message_t nas_msg       = {};

  nas_msg.header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  nas_msg.header.security_header_type = SECURITY_HEADER_TYPE_NOT_PROTECTED;

  SM_msg* sm_msg = &nas_msg.plain.sm;

  // Fill the content of SM header
  sm_msg->header.extended_protocol_discriminator =
      EPD_5GS_SESSION_MANAGEMENT_MESSAGES;
  sm_msg->header.pdu_session_identity = sm_context_res.get_pdu_session_id();

  Logger::smf_n1().info("PDU_SESSION_RELEASE_COMMAND, encode starting...");
  // Fill the content of PDU Session Release Command
  sm_msg->header.pdu_session_identity = sm_context_res.get_pdu_session_id();
  sm_msg->header.procedure_transaction_identity =
      sm_context_res.get_pti().procedure_transaction_id;
  sm_msg->header.message_type = PDU_SESSION_RELEASE_COMMAND;
  sm_msg->pdu_session_release_command._5gsmcause =
      static_cast<uint8_t>(sm_cause);
  // TODO: to be updated when adding the following IEs
  sm_msg->pdu_session_release_command.presence = 0x00;
  // GPRSTimer3
  // EAPMessage
  //_5GSMCongestionReattemptIndicator
  // ExtendedProtocolConfigurationOptions

  Logger::smf_n1().debug(
      "SM message, 5GSM Cause: 0x%x",
      sm_msg->pdu_session_release_command._5gsmcause);

  // Encode NAS message
  bytes = nas_message_encode(
      data, &nas_msg, sizeof(data) /*don't know the size*/, nullptr);

#if DEBUG_IS_ON
  Logger::smf_n1().debug("Buffer Data: ");
  for (int i = 0; i < bytes; i++) printf("%02x ", data[i]);
  printf(" (bytes %d)\n", bytes);
#endif

  if (bytes > 0) {
    std::string n1Message((char*) data, bytes);
    nas_msg_str = n1Message;
    return true;
  } else {
    return false;
  }
}

//-----------------------------------------------------------------------------------------------------
bool create_n1_pdu_session_release_command(
    pdu_session_modification_network_requested& msg, std::string& nas_msg_str,
    cause_value_5gsm_e sm_cause) {
  Logger::smf_n1().info(
      "Create N1 SM Container, PDU Session Release Command "
      "(pdu_session_modification_network_requested)");
  // TODO:
  return true;
}

//------------------------------------------------------------------------------
int smf_n1::decode_n1_sm_container(
    nas_message_t& nas_msg, const std::string& n1_sm_msg) {
  Logger::smf_n1().info("Decode NAS message from N1 SM Container.");

  // step 1. Decode NAS  message (for instance, ... only served as an example)
  nas_message_decode_status_t decode_status = {0};
  int decoder_rc                            = RETURNok;

  unsigned int data_len = n1_sm_msg.length();
  unsigned char* data   = (unsigned char*) malloc(data_len + 1);
  memset(data, 0, data_len + 1);
  memcpy((void*) data, (void*) n1_sm_msg.c_str(), data_len);

#if DEBUG_IS_ON
  printf("Content: ");
  for (int i = 0; i < data_len; i++) printf(" %02x ", data[i]);
  printf("\n");
#endif

  // decode the NAS message
  decoder_rc =
      nas_message_decode(data, &nas_msg, data_len, nullptr, &decode_status);
  Logger::smf_n1().debug(
      "NAS message, Extended Protocol Discriminator 0x%x, PDU Session Identity "
      "0x%x, Procedure Transaction Identity 0x%x, Message Type 0x%x",
      nas_msg.plain.sm.header.extended_protocol_discriminator,
      nas_msg.plain.sm.header.pdu_session_identity,
      nas_msg.plain.sm.header.procedure_transaction_identity,
      nas_msg.plain.sm.header.message_type);

  // free memory
  free_wrapper((void**) &data);

  return decoder_rc;
}
