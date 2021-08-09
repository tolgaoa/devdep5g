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

/*! \file smf_n2.cpp
 \brief
 \author  Tien-Thinh NGUYEN, Keliang DU
 \company Eurecom
 \date 2019
 \email:  tien-thinh.nguyen@eurecom.fr
 */

#include "smf_n2.hpp"

#include <arpa/inet.h>
#include <stdexcept>
#include "string.hpp"

#include "smf.h"

extern "C" {
#include "Ngap_AssociatedQosFlowItem.h"
#include "Ngap_Criticality.h"
#include "Ngap_Dynamic5QIDescriptor.h"
#include "Ngap_GTPTunnel.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_NonDynamic5QIDescriptor.h"
#include "Ngap_PDUSessionResourceModifyRequestTransfer.h"
#include "Ngap_PDUSessionResourceReleaseCommandTransfer.h"
#include "Ngap_PDUSessionResourceReleaseResponseTransfer.h"
#include "Ngap_PDUSessionResourceSetupRequestTransfer.h"
#include "Ngap_PDUSessionResourceSetupResponseTransfer.h"
#include "Ngap_ProcedureCode.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_QosFlowAddOrModifyRequestItem.h"
#include "Ngap_QosFlowAddOrModifyResponseItem.h"
#include "Ngap_QosFlowAddOrModifyResponseList.h"
#include "Ngap_QosFlowSetupRequestItem.h"
#include "Ngap_UL-NGU-UP-TNLModifyItem.h"
#include "dynamic_memory_check.h"
}

using namespace smf;
extern smf_app* smf_app_inst;

//------------------------------------------------------------------------------
bool smf_n2::create_n2_pdu_session_resource_setup_request_transfer(
    pdu_session_create_sm_context_response& sm_context_res,
    n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str) {
  Logger::smf_n2().info(
      "Create N2 SM Information, PDU Session Resource Setup Request Transfer");

  bool result                                             = false;
  Ngap_PDUSessionResourceSetupRequestTransfer_t* ngap_IEs = nullptr;
  ngap_IEs = (Ngap_PDUSessionResourceSetupRequestTransfer_t*) calloc(
      1, sizeof(Ngap_PDUSessionResourceSetupRequestTransfer_t));
  qos_flow_context_updated qos_flow = {};

  // get default QoS value
  qos_flow = sm_context_res.get_qos_flow_context();

  Logger::smf_n2().debug(
      "UL F-TEID, TEID "
      "0x%" PRIx32 ", IP Address %s",
      qos_flow.ul_fteid.teid,
      conv::toString(qos_flow.ul_fteid.ipv4_address).c_str());
  Logger::smf_n2().info(
      "QoS parameters: QFI %d, Priority level %d, ARP priority level %d",
      qos_flow.qfi.qfi, qos_flow.qos_profile.priority_level,
      qos_flow.qos_profile.arp.priority_level);

  // check the QoS Flow
  if ((qos_flow.qfi.qfi < QOS_FLOW_IDENTIFIER_FIRST) or
      (qos_flow.qfi.qfi > QOS_FLOW_IDENTIFIER_LAST)) {
    // error
    Logger::smf_n2().error("Incorrect QFI %d", qos_flow.qfi.qfi);
    free_wrapper((void**) &ngap_IEs);
    return false;
  }

  // PDUSessionAggregateMaximumBitRate
  Ngap_PDUSessionResourceSetupRequestTransferIEs_t*
      pduSessionAggregateMaximumBitRate = nullptr;
  pduSessionAggregateMaximumBitRate =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  pduSessionAggregateMaximumBitRate->id =
      Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate;
  pduSessionAggregateMaximumBitRate->criticality = Ngap_Criticality_reject;
  pduSessionAggregateMaximumBitRate->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate;
  asn_set_empty(&ngap_IEs->protocolIEs.list);

  // SessionAMBR
  supi_t supi                     = sm_context_res.get_supi();
  supi64_t supi64                 = smf_supi_to_u64(supi);
  std::shared_ptr<smf_context> sc = {};
  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n2().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
    sc.get()->get_session_ambr(
        pduSessionAggregateMaximumBitRate->value.choice
            .PDUSessionAggregateMaximumBitRate,
        sm_context_res.get_snssai(), sm_context_res.get_dnn());
  } else {
    Logger::smf_n2().warn(
        "SMF context with SUPI " SUPI_64_FMT " does not exist!", supi64);
    free_wrapper((void**) &pduSessionAggregateMaximumBitRate);
    return false;
  }

  ASN_SEQUENCE_ADD(
      &ngap_IEs->protocolIEs.list, pduSessionAggregateMaximumBitRate);

  // UPTransportLayerInformation
  pfcp::fteid_t ul_fteid = {};
  ul_fteid.v4            = qos_flow.ul_fteid.v4;

  ul_fteid.teid         = htonl(qos_flow.ul_fteid.teid);
  ul_fteid.ipv4_address = qos_flow.ul_fteid.ipv4_address;

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t*
      upTransportLayerInformation = nullptr;
  upTransportLayerInformation =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  upTransportLayerInformation->id =
      Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation;
  upTransportLayerInformation->criticality = Ngap_Criticality_reject;
  upTransportLayerInformation->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation;
  upTransportLayerInformation->value.choice.UPTransportLayerInformation
      .present = Ngap_UPTransportLayerInformation_PR_gTPTunnel;

  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel = (Ngap_GTPTunnel_t*) calloc(1, sizeof(Ngap_GTPTunnel_t));
  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->transportLayerAddress.size = sizeof(struct in_addr);
  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->transportLayerAddress.buf =
      (uint8_t*) calloc(sizeof(struct in_addr), sizeof(uint8_t));
  memcpy(
      upTransportLayerInformation->value.choice.UPTransportLayerInformation
          .choice.gTPTunnel->transportLayerAddress.buf,
      &ul_fteid.ipv4_address, sizeof(struct in_addr));
  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->transportLayerAddress.bits_unused = 0;

  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->gTP_TEID.size = TEID_GRE_KEY_LENGTH;
  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->gTP_TEID.buf =
      (uint8_t*) calloc(TEID_GRE_KEY_LENGTH, sizeof(uint8_t));
  memcpy(
      upTransportLayerInformation->value.choice.UPTransportLayerInformation
          .choice.gTPTunnel->gTP_TEID.buf,
      &ul_fteid.teid, TEID_GRE_KEY_LENGTH);

  ASN_SEQUENCE_ADD(&ngap_IEs->protocolIEs.list, upTransportLayerInformation);

  // TODO: DataForwardingNotPossible

  // PDUSessionType
  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* pduSessionType = nullptr;
  pduSessionType = (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
      1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  pduSessionType->id          = Ngap_ProtocolIE_ID_id_PDUSessionType;
  pduSessionType->criticality = Ngap_Criticality_reject;
  pduSessionType->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType;
  pduSessionType->value.choice.PDUSessionType =
      sm_context_res.get_pdu_session_type() -
      1;  // TODO: dirty code, difference between Ngap_PDUSessionType_ipv4 vs
          // pdu_session_type_e::PDU_SESSION_TYPE_E_IPV4 (TS 38.413 vs
          // TS 24.501)
  ASN_SEQUENCE_ADD(&ngap_IEs->protocolIEs.list, pduSessionType);
  Logger::smf_n2().debug(
      "PDU Session Type: %d ", sm_context_res.get_pdu_session_type());

  // SecurityIndication
  // TODO: should get from UDM
  //    Ngap_PDUSessionResourceSetupRequestTransferIEs_t  *securityIndication =
  //    nullptr;
  //   securityIndication = (Ngap_PDUSessionResourceSetupRequestTransferIEs_t *)
  //   calloc(1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  //   securityIndication->value.choice.SecurityIndication.integrityProtectionIndication
  //   = Ngap_IntegrityProtectionIndication_not_needed;
  //   securityIndication->value.choice.SecurityIndication.confidentialityProtectionIndication
  //   = Ngap_ConfidentialityProtectionIndication_not_needed;

  // TODO: NetworkInstance

  // QosFlowSetupRequestList
  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* qosFlowSetupRequestList =
      nullptr;
  qosFlowSetupRequestList =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  qosFlowSetupRequestList->id = Ngap_ProtocolIE_ID_id_QosFlowSetupRequestList;
  qosFlowSetupRequestList->criticality = Ngap_Criticality_reject;
  qosFlowSetupRequestList->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList;

  Ngap_QosFlowSetupRequestItem_t* ngap_QosFlowSetupRequestItem = nullptr;
  ngap_QosFlowSetupRequestItem = (Ngap_QosFlowSetupRequestItem_t*) calloc(
      1, sizeof(Ngap_QosFlowSetupRequestItem_t));
  ngap_QosFlowSetupRequestItem->qosFlowIdentifier = (uint8_t) qos_flow.qfi.qfi;
  ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters.qosCharacteristics
      .present = Ngap_QosCharacteristics_PR_nonDynamic5QI;
  ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters.qosCharacteristics
      .choice.nonDynamic5QI = (Ngap_NonDynamic5QIDescriptor_t*) (calloc(
      1, sizeof(Ngap_NonDynamic5QIDescriptor_t)));
  ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters.qosCharacteristics
      .choice.nonDynamic5QI->fiveQI = (uint8_t) qos_flow.qfi.qfi;
  ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
      .allocationAndRetentionPriority.priorityLevelARP =
      qos_flow.qos_profile.arp.priority_level;
  if (qos_flow.qos_profile.arp.preempt_cap.compare("NOT_PREEMPT") == 0) {
    ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
        .allocationAndRetentionPriority.pre_emptionCapability =
        Ngap_Pre_emptionCapability_shall_not_trigger_pre_emption;
  } else {
    ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
        .allocationAndRetentionPriority.pre_emptionCapability =
        Ngap_Pre_emptionCapability_may_trigger_pre_emption;
  }
  if (qos_flow.qos_profile.arp.preempt_vuln.compare("NOT_PREEMPTABLE") == 0) {
    ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
        .allocationAndRetentionPriority.pre_emptionVulnerability =
        Ngap_Pre_emptionVulnerability_not_pre_emptable;
  } else {
    ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
        .allocationAndRetentionPriority.pre_emptionVulnerability =
        Ngap_Pre_emptionVulnerability_pre_emptable;
  }

  asn_set_empty(
      &qosFlowSetupRequestList->value.choice.QosFlowSetupRequestList.list);
  ASN_SEQUENCE_ADD(
      &qosFlowSetupRequestList->value.choice.QosFlowSetupRequestList.list,
      ngap_QosFlowSetupRequestItem);
  ASN_SEQUENCE_ADD(&ngap_IEs->protocolIEs.list, qosFlowSetupRequestList);

  Logger::smf_n2().info(
      "QoS parameters: QFI %d, ARP priority level %d, "
      "qos_flow.qos_profile.arp.preempt_cap %s, "
      "qos_flow.qos_profile.arp.preempt_vuln %s",
      qos_flow.qfi.qfi, qos_flow.qos_profile.arp.priority_level,
      qos_flow.qos_profile.arp.preempt_cap.c_str(),
      qos_flow.qos_profile.arp.preempt_vuln.c_str());

  // encode
  size_t buffer_size = BUF_LEN;
  char* buffer       = (char*) calloc(1, buffer_size);

  ssize_t encoded_size = aper_encode_to_new_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer, nullptr, ngap_IEs,
      (void**) &buffer);
  if (encoded_size < 0) {
    Logger::smf_n2().warn(
        "NGAP PDU Session Resource Setup Request Transfer encode failed "
        "(encode size %d)",
        encoded_size);
    result = false;
  } else {
#if DEBUG_IS_ON
    Logger::smf_n2().debug("N2 SM buffer data: ");
    for (int i = 0; i < encoded_size; i++) printf("%02x ", (char) buffer[i]);
    printf(" (%d bytes)\n", (int) encoded_size);
#endif

    std::string ngap_message((char*) buffer, encoded_size);
    ngap_msg_str = ngap_message;
    result       = true;
  }
  // free memory
  free_wrapper((void**) &pduSessionAggregateMaximumBitRate);
  free_wrapper((void**) &upTransportLayerInformation->value.choice
                   .UPTransportLayerInformation.choice.gTPTunnel
                   ->transportLayerAddress.buf);
  free_wrapper((void**) &upTransportLayerInformation->value.choice
                   .UPTransportLayerInformation.choice.gTPTunnel->gTP_TEID.buf);
  free_wrapper((void**) &upTransportLayerInformation->value.choice
                   .UPTransportLayerInformation.choice.gTPTunnel);
  free_wrapper((void**) &upTransportLayerInformation);
  free_wrapper((void**) &pduSessionType);
  free_wrapper((void**) &qosFlowSetupRequestList);
  free_wrapper((void**) &ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
                   .qosCharacteristics.choice.nonDynamic5QI);
  free_wrapper((void**) &ngap_QosFlowSetupRequestItem);
  free_wrapper((void**) &ngap_IEs);
  free_wrapper((void**) &buffer);

  return result;
}

//------------------------------------------------------------------------------
bool smf_n2::create_n2_pdu_session_resource_setup_request_transfer(
    pdu_session_update_sm_context_response& sm_context_res,
    n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str) {
  Logger::smf_n2().info(
      "Create N2 SM Information, PDU Session Resource Setup Request Transfer");

  bool result                                             = false;
  Ngap_PDUSessionResourceSetupRequestTransfer_t* ngap_IEs = nullptr;
  ngap_IEs = (Ngap_PDUSessionResourceSetupRequestTransfer_t*) calloc(
      1, sizeof(Ngap_PDUSessionResourceSetupRequestTransfer_t));
  qos_flow_context_updated qos_flow = {};

  // get default QoS value
  std::map<uint8_t, qos_flow_context_updated> qos_flows = {};
  sm_context_res.get_all_qos_flow_context_updateds(qos_flows);
  for (std::map<uint8_t, qos_flow_context_updated>::iterator it =
           qos_flows.begin();
       it != qos_flows.end(); ++it)
    Logger::smf_n2().debug("QoS Flow context to be updated QFI %d", it->first);

  if (qos_flows.empty()) {
    free_wrapper((void**) &ngap_IEs);
    return false;
  }
  // TODO: support only 1 qos flow
  qos_flow = qos_flows.begin()->second;

  Logger::smf_n2().debug(
      "UL F-TEID, TEID "
      "0x%" PRIx32 ", IP Address %s",
      qos_flow.ul_fteid.teid,
      conv::toString(qos_flow.ul_fteid.ipv4_address).c_str());
  Logger::smf_n2().info(
      "QoS parameters: QFI %d, Priority level %d, ARP priority level %d",
      qos_flow.qfi.qfi, qos_flow.qos_profile.priority_level,
      qos_flow.qos_profile.arp.priority_level);

  // check the QoS Flow
  if ((qos_flow.qfi.qfi < QOS_FLOW_IDENTIFIER_FIRST) or
      (qos_flow.qfi.qfi > QOS_FLOW_IDENTIFIER_LAST)) {
    // error
    Logger::smf_n2().error("Incorrect QFI %d", qos_flow.qfi.qfi);
    free_wrapper((void**) &ngap_IEs);
    return false;
  }

  // PDUSessionAggregateMaximumBitRate
  Ngap_PDUSessionResourceSetupRequestTransferIEs_t*
      pduSessionAggregateMaximumBitRate = nullptr;
  pduSessionAggregateMaximumBitRate =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  pduSessionAggregateMaximumBitRate->id =
      Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate;
  pduSessionAggregateMaximumBitRate->criticality = Ngap_Criticality_reject;
  pduSessionAggregateMaximumBitRate->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate;
  asn_set_empty(&ngap_IEs->protocolIEs.list);

  // SessionAMBR
  supi_t supi                     = sm_context_res.get_supi();
  supi64_t supi64                 = smf_supi_to_u64(supi);
  std::shared_ptr<smf_context> sc = {};
  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n2().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
    sc.get()->get_session_ambr(
        pduSessionAggregateMaximumBitRate->value.choice
            .PDUSessionAggregateMaximumBitRate,
        sm_context_res.get_snssai(), sm_context_res.get_dnn());
  } else {
    Logger::smf_n2().warn(
        "SMF context with SUPI " SUPI_64_FMT " does not exist!", supi64);
    free_wrapper((void**) &pduSessionAggregateMaximumBitRate);
    return false;
  }

  ASN_SEQUENCE_ADD(
      &ngap_IEs->protocolIEs.list, pduSessionAggregateMaximumBitRate);

  // UPTransportLayerInformation
  pfcp::fteid_t ul_fteid = {};
  ul_fteid.v4            = qos_flow.ul_fteid.v4;

  ul_fteid.teid         = htonl(qos_flow.ul_fteid.teid);
  ul_fteid.ipv4_address = qos_flow.ul_fteid.ipv4_address;

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t*
      upTransportLayerInformation = nullptr;
  upTransportLayerInformation =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  upTransportLayerInformation->id =
      Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation;
  upTransportLayerInformation->criticality = Ngap_Criticality_reject;
  upTransportLayerInformation->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation;
  upTransportLayerInformation->value.choice.UPTransportLayerInformation
      .present = Ngap_UPTransportLayerInformation_PR_gTPTunnel;

  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel = (Ngap_GTPTunnel_t*) calloc(1, sizeof(Ngap_GTPTunnel_t));
  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->transportLayerAddress.size = sizeof(struct in_addr);
  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->transportLayerAddress.buf =
      (uint8_t*) calloc(sizeof(struct in_addr), sizeof(uint8_t));
  memcpy(
      upTransportLayerInformation->value.choice.UPTransportLayerInformation
          .choice.gTPTunnel->transportLayerAddress.buf,
      &ul_fteid.ipv4_address, sizeof(struct in_addr));
  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->transportLayerAddress.bits_unused = 0;

  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->gTP_TEID.size = TEID_GRE_KEY_LENGTH;
  upTransportLayerInformation->value.choice.UPTransportLayerInformation.choice
      .gTPTunnel->gTP_TEID.buf =
      (uint8_t*) calloc(TEID_GRE_KEY_LENGTH, sizeof(uint8_t));
  memcpy(
      upTransportLayerInformation->value.choice.UPTransportLayerInformation
          .choice.gTPTunnel->gTP_TEID.buf,
      &ul_fteid.teid, TEID_GRE_KEY_LENGTH);

  ASN_SEQUENCE_ADD(&ngap_IEs->protocolIEs.list, upTransportLayerInformation);

  // TODO: DataForwardingNotPossible

  // PDUSessionType
  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* pduSessionType = nullptr;
  pduSessionType = (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
      1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  pduSessionType->id          = Ngap_ProtocolIE_ID_id_PDUSessionType;
  pduSessionType->criticality = Ngap_Criticality_reject;
  pduSessionType->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType;
  pduSessionType->value.choice.PDUSessionType =
      sm_context_res.get_pdu_session_type() -
      1;  // TODO: dirty code, difference between Ngap_PDUSessionType_ipv4 vs
          // pdu_session_type_e::PDU_SESSION_TYPE_E_IPV4 (TS 38.413 vs
          // TS 24.501)
  ASN_SEQUENCE_ADD(&ngap_IEs->protocolIEs.list, pduSessionType);
  Logger::smf_n2().debug(
      "PDU Session Type: %d ", sm_context_res.get_pdu_session_type());

  // SecurityIndication
  // TODO: should get from UDM
  //    Ngap_PDUSessionResourceSetupRequestTransferIEs_t  *securityIndication =
  //    nullptr;
  //   securityIndication = (Ngap_PDUSessionResourceSetupRequestTransferIEs_t *)
  //   calloc(1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  //   securityIndication->value.choice.SecurityIndication.integrityProtectionIndication
  //   = Ngap_IntegrityProtectionIndication_not_needed;
  //   securityIndication->value.choice.SecurityIndication.confidentialityProtectionIndication
  //   = Ngap_ConfidentialityProtectionIndication_not_needed;

  // TODO: NetworkInstance

  // QosFlowSetupRequestList
  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* qosFlowSetupRequestList =
      nullptr;
  qosFlowSetupRequestList =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  qosFlowSetupRequestList->id = Ngap_ProtocolIE_ID_id_QosFlowSetupRequestList;
  qosFlowSetupRequestList->criticality = Ngap_Criticality_reject;
  qosFlowSetupRequestList->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList;

  Ngap_QosFlowSetupRequestItem_t* ngap_QosFlowSetupRequestItem = nullptr;
  ngap_QosFlowSetupRequestItem = (Ngap_QosFlowSetupRequestItem_t*) calloc(
      1, sizeof(Ngap_QosFlowSetupRequestItem_t));
  ngap_QosFlowSetupRequestItem->qosFlowIdentifier = (uint8_t) qos_flow.qfi.qfi;
  ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters.qosCharacteristics
      .present = Ngap_QosCharacteristics_PR_nonDynamic5QI;
  ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters.qosCharacteristics
      .choice.nonDynamic5QI = (Ngap_NonDynamic5QIDescriptor_t*) (calloc(
      1, sizeof(Ngap_NonDynamic5QIDescriptor_t)));
  ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters.qosCharacteristics
      .choice.nonDynamic5QI->fiveQI = (uint8_t) qos_flow.qfi.qfi;
  ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
      .allocationAndRetentionPriority.priorityLevelARP =
      qos_flow.qos_profile.arp.priority_level;
  if (qos_flow.qos_profile.arp.preempt_cap.compare("NOT_PREEMPT") == 0) {
    ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
        .allocationAndRetentionPriority.pre_emptionCapability =
        Ngap_Pre_emptionCapability_shall_not_trigger_pre_emption;
  } else {
    ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
        .allocationAndRetentionPriority.pre_emptionCapability =
        Ngap_Pre_emptionCapability_may_trigger_pre_emption;
  }
  if (qos_flow.qos_profile.arp.preempt_vuln.compare("NOT_PREEMPTABLE") == 0) {
    ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
        .allocationAndRetentionPriority.pre_emptionVulnerability =
        Ngap_Pre_emptionVulnerability_not_pre_emptable;
  } else {
    ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
        .allocationAndRetentionPriority.pre_emptionVulnerability =
        Ngap_Pre_emptionVulnerability_pre_emptable;
  }

  asn_set_empty(
      &qosFlowSetupRequestList->value.choice.QosFlowSetupRequestList.list);
  ASN_SEQUENCE_ADD(
      &qosFlowSetupRequestList->value.choice.QosFlowSetupRequestList.list,
      ngap_QosFlowSetupRequestItem);
  ASN_SEQUENCE_ADD(&ngap_IEs->protocolIEs.list, qosFlowSetupRequestList);

  Logger::smf_n2().info(
      "QoS parameters: QFI %d, ARP priority level %d, "
      "qos_flow.qos_profile.arp.preempt_cap %s, "
      "qos_flow.qos_profile.arp.preempt_vuln %s",
      qos_flow.qfi.qfi, qos_flow.qos_profile.arp.priority_level,
      qos_flow.qos_profile.arp.preempt_cap.c_str(),
      qos_flow.qos_profile.arp.preempt_vuln.c_str());

  // encode
  size_t buffer_size = BUF_LEN;
  char* buffer       = (char*) calloc(1, buffer_size);

  ssize_t encoded_size = aper_encode_to_new_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer, nullptr, ngap_IEs,
      (void**) &buffer);
  if (encoded_size < 0) {
    Logger::smf_n2().warn(
        "NGAP PDU Session Resource Setup Request Transfer encode failed "
        "(encode size %d)",
        encoded_size);
    result = false;
  } else {
#if DEBUG_IS_ON
    Logger::smf_n2().debug("N2 SM buffer data: ");
    for (int i = 0; i < encoded_size; i++) printf("%02x ", (char) buffer[i]);
    printf(" (%d bytes)\n", (int) encoded_size);
#endif

    std::string ngap_message((char*) buffer, encoded_size);
    ngap_msg_str = ngap_message;
    result       = true;
  }

  // free memory
  free_wrapper((void**) &pduSessionAggregateMaximumBitRate);
  free_wrapper((void**) &upTransportLayerInformation->value.choice
                   .UPTransportLayerInformation.choice.gTPTunnel
                   ->transportLayerAddress.buf);
  free_wrapper((void**) &upTransportLayerInformation->value.choice
                   .UPTransportLayerInformation.choice.gTPTunnel->gTP_TEID.buf);
  free_wrapper((void**) &upTransportLayerInformation->value.choice
                   .UPTransportLayerInformation.choice.gTPTunnel);
  free_wrapper((void**) &upTransportLayerInformation);
  free_wrapper((void**) &pduSessionType);
  free_wrapper((void**) &qosFlowSetupRequestList);
  free_wrapper((void**) &ngap_QosFlowSetupRequestItem->qosFlowLevelQosParameters
                   .qosCharacteristics.choice.nonDynamic5QI);
  free_wrapper((void**) &ngap_QosFlowSetupRequestItem);
  free_wrapper((void**) &ngap_IEs);
  free_wrapper((void**) &buffer);

  return result;
}

//------------------------------------------------------------------------------
bool smf_n2::create_n2_pdu_session_resource_modify_request_transfer(
    pdu_session_update_sm_context_response& sm_context_res,
    n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str) {
  Logger::smf_n2().debug(
      "Create N2 SM Information: NGAP PDU Session Resource Modify Request "
      "Transfer");

  bool result = false;
  // get default QoS info
  std::map<uint8_t, qos_flow_context_updated> qos_flows = {};
  sm_context_res.get_all_qos_flow_context_updateds(qos_flows);
  for (std::map<uint8_t, qos_flow_context_updated>::iterator it =
           qos_flows.begin();
       it != qos_flows.end(); ++it)
    Logger::smf_n2().debug(
        "QoS flow context to be updated with QFI %d", it->first);
  // TODO: support only 1 qos flow
  qos_flow_context_updated qos_flow = qos_flows.begin()->second;

  // check the QoS Flow
  if ((qos_flow.qfi.qfi < QOS_FLOW_IDENTIFIER_FIRST) or
      (qos_flow.qfi.qfi > QOS_FLOW_IDENTIFIER_LAST)) {
    // error
    Logger::smf_n2().error("Incorrect QFI %d", qos_flow.qfi.qfi);
    return false;
  }

  Logger::smf_n2().debug(
      "QoS Flow, UL F-TEID ID "
      "0x%" PRIx32 ", IP Address %s ",
      qos_flow.ul_fteid.teid,
      conv::toString(qos_flow.ul_fteid.ipv4_address).c_str());
  Logger::smf_n2().debug(
      "QoS Flow, DL F-TEID ID"
      "0x%" PRIx32 ", IP Address %s",
      qos_flow.dl_fteid.teid,
      conv::toString(qos_flow.dl_fteid.ipv4_address).c_str());

  Ngap_PDUSessionResourceModifyRequestTransfer_t* ngap_IEs = nullptr;
  ngap_IEs = (Ngap_PDUSessionResourceModifyRequestTransfer_t*) calloc(
      1, sizeof(Ngap_PDUSessionResourceModifyRequestTransfer_t));

  // PDUSessionAggregateMaximumBitRate
  Ngap_PDUSessionResourceModifyRequestTransferIEs_t*
      pduSessionAggregateMaximumBitRate = nullptr;
  pduSessionAggregateMaximumBitRate =
      (Ngap_PDUSessionResourceModifyRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestTransferIEs_t));
  pduSessionAggregateMaximumBitRate->id =
      Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate;
  pduSessionAggregateMaximumBitRate->criticality = Ngap_Criticality_reject;
  pduSessionAggregateMaximumBitRate->value.present =
      Ngap_PDUSessionResourceModifyRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate;

  supi_t supi                     = sm_context_res.get_supi();
  supi64_t supi64                 = smf_supi_to_u64(supi);
  std::shared_ptr<smf_context> sc = {};
  if (smf_app_inst->is_supi_2_smf_context(supi64)) {
    Logger::smf_n2().debug("Get SMF context with SUPI " SUPI_64_FMT "", supi64);
    sc = smf_app_inst->supi_2_smf_context(supi64);
    sc.get()->get_session_ambr(
        pduSessionAggregateMaximumBitRate->value.choice
            .PDUSessionAggregateMaximumBitRate,
        sm_context_res.get_snssai(), sm_context_res.get_dnn());
  } else {
    Logger::smf_n2().warn(
        "SMF context with SUPI " SUPI_64_FMT " does not exist!", supi64);
    // TODO:
  }
  ASN_SEQUENCE_ADD(
      &ngap_IEs->protocolIEs.list, pduSessionAggregateMaximumBitRate);

  // Ngap_UL_NGU_UP_TNLModifyList_t (included if the PDU Session modification
  // was requested by the UE for a  PDU Session that has no established User
  // Plane resources)
  pfcp::fteid_t ul_fteid = {};
  ul_fteid.v4            = qos_flow.ul_fteid.v4;
  ul_fteid.teid          = htonl(qos_flow.ul_fteid.teid);
  ul_fteid.ipv4_address  = qos_flow.ul_fteid.ipv4_address;

  pfcp::fteid_t dl_fteid = {};
  dl_fteid.v4            = qos_flow.dl_fteid.v4;
  dl_fteid.teid          = htonl(qos_flow.dl_fteid.teid);
  dl_fteid.ipv4_address  = qos_flow.dl_fteid.ipv4_address;

  Ngap_PDUSessionResourceModifyRequestTransferIEs_t* ul_NGU_UP_TNLModifyList =
      nullptr;
  ul_NGU_UP_TNLModifyList =
      (Ngap_PDUSessionResourceModifyRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestTransferIEs_t));
  ul_NGU_UP_TNLModifyList->id = Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLModifyList;
  ul_NGU_UP_TNLModifyList->criticality = Ngap_Criticality_reject;
  ul_NGU_UP_TNLModifyList->value.present =
      Ngap_PDUSessionResourceModifyRequestTransferIEs__value_PR_UL_NGU_UP_TNLModifyList;
  Ngap_UL_NGU_UP_TNLModifyItem_t* ngap_UL_NGU_UP_TNLModifyItem = nullptr;
  ngap_UL_NGU_UP_TNLModifyItem = (Ngap_UL_NGU_UP_TNLModifyItem_t*) calloc(
      1, sizeof(Ngap_UL_NGU_UP_TNLModifyItem_t));
  ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.present =
      Ngap_UPTransportLayerInformation_PR_gTPTunnel;
  ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.choice.gTPTunnel =
      (Ngap_GTPTunnel_t*) calloc(1, sizeof(Ngap_GTPTunnel_t));
  ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->transportLayerAddress.buf =
      (uint8_t*) calloc(sizeof(struct in_addr), sizeof(uint8_t));
  memcpy(
      ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.choice.gTPTunnel
          ->transportLayerAddress.buf,
      &ul_fteid.ipv4_address, sizeof(struct in_addr));
  ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->transportLayerAddress.size = sizeof(struct in_addr);
  ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->transportLayerAddress.bits_unused = 0;

  ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->gTP_TEID.size = TEID_GRE_KEY_LENGTH;
  ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->gTP_TEID.buf = (uint8_t*) calloc(TEID_GRE_KEY_LENGTH, sizeof(uint8_t));
  memcpy(
      ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation.choice.gTPTunnel
          ->gTP_TEID.buf,
      &ul_fteid.teid, TEID_GRE_KEY_LENGTH);

  ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.present =
      Ngap_UPTransportLayerInformation_PR_gTPTunnel;
  ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.choice.gTPTunnel =
      (Ngap_GTPTunnel_t*) calloc(1, sizeof(Ngap_GTPTunnel_t));
  ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->transportLayerAddress.buf =
      (uint8_t*) calloc(sizeof(struct in_addr), sizeof(uint8_t));
  memcpy(
      ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.choice.gTPTunnel
          ->transportLayerAddress.buf,
      &dl_fteid.ipv4_address, sizeof(struct in_addr));
  ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->transportLayerAddress.size = sizeof(struct in_addr);
  ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->transportLayerAddress.bits_unused = 0;

  ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->gTP_TEID.size = TEID_GRE_KEY_LENGTH;
  ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.choice.gTPTunnel
      ->gTP_TEID.buf = (uint8_t*) calloc(TEID_GRE_KEY_LENGTH, sizeof(uint8_t));
  memcpy(
      ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation.choice.gTPTunnel
          ->gTP_TEID.buf,
      &dl_fteid.teid, TEID_GRE_KEY_LENGTH);
  ASN_SEQUENCE_ADD(
      &ul_NGU_UP_TNLModifyList->value.choice.UL_NGU_UP_TNLModifyList.list,
      ngap_UL_NGU_UP_TNLModifyItem);
  ASN_SEQUENCE_ADD(&ngap_IEs->protocolIEs.list, ul_NGU_UP_TNLModifyList);

  // TODO: Ngap_NetworkInstance_t

  // Ngap_QosFlowAddOrModifyRequestList_t
  // TODO: to be completed
  Ngap_PDUSessionResourceModifyRequestTransferIEs_t*
      qosFlowAddOrModifyRequestList = nullptr;
  qosFlowAddOrModifyRequestList =
      (Ngap_PDUSessionResourceModifyRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyRequestTransferIEs_t));

  qosFlowAddOrModifyRequestList->id =
      Ngap_ProtocolIE_ID_id_QosFlowAddOrModifyRequestList;
  qosFlowAddOrModifyRequestList->criticality = Ngap_Criticality_reject;
  qosFlowAddOrModifyRequestList->value.present =
      Ngap_PDUSessionResourceModifyRequestTransferIEs__value_PR_QosFlowAddOrModifyRequestList;
  Ngap_QosFlowAddOrModifyRequestItem* ngap_QosFlowAddOrModifyRequestItem =
      nullptr;
  ngap_QosFlowAddOrModifyRequestItem =
      (Ngap_QosFlowAddOrModifyRequestItem*) calloc(
          1, sizeof(Ngap_QosFlowAddOrModifyRequestItem));
  ngap_QosFlowAddOrModifyRequestItem->qosFlowIdentifier = qos_flow.qfi.qfi;

  ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters =
      (struct Ngap_QosFlowLevelQosParameters*) calloc(
          1, sizeof(struct Ngap_QosFlowLevelQosParameters));
  ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
      ->qosCharacteristics.present = Ngap_QosCharacteristics_PR_nonDynamic5QI;
  ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
      ->qosCharacteristics.choice.nonDynamic5QI =
      (Ngap_NonDynamic5QIDescriptor_t*) (calloc(
          1, sizeof(Ngap_NonDynamic5QIDescriptor_t)));
  ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
      ->qosCharacteristics.choice.nonDynamic5QI->fiveQI = qos_flow.qfi.qfi;
  ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
      ->allocationAndRetentionPriority.priorityLevelARP =
      qos_flow.qos_profile.priority_level;
  if (qos_flow.qos_profile.arp.preempt_cap.compare("NOT_PREEMPT") == 0) {
    ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
        ->allocationAndRetentionPriority.pre_emptionCapability =
        Ngap_Pre_emptionCapability_shall_not_trigger_pre_emption;
  } else {
    ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
        ->allocationAndRetentionPriority.pre_emptionCapability =
        Ngap_Pre_emptionCapability_may_trigger_pre_emption;
  }
  if (qos_flow.qos_profile.arp.preempt_vuln.compare("NOT_PREEMPTABLE") == 0) {
    ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
        ->allocationAndRetentionPriority.pre_emptionVulnerability =
        Ngap_Pre_emptionVulnerability_not_pre_emptable;
  } else {
    ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
        ->allocationAndRetentionPriority.pre_emptionVulnerability =
        Ngap_Pre_emptionVulnerability_pre_emptable;
  }

  ASN_SEQUENCE_ADD(
      &qosFlowAddOrModifyRequestList->value.choice.QosFlowAddOrModifyRequestList
           .list,
      ngap_QosFlowAddOrModifyRequestItem);
  // Ngap_E_RAB_ID_t *e_RAB_ID;  //optional
  ASN_SEQUENCE_ADD(&ngap_IEs->protocolIEs.list, qosFlowAddOrModifyRequestList);

  // TODO: Ngap_QosFlowList_t - QoS to release list??
  // TODO: Ngap_UPTransportLayerInformation_t

  // encode
  size_t buffer_size = BUF_LEN;
  char* buffer       = (char*) calloc(1, buffer_size);

  ssize_t encoded_size = aper_encode_to_new_buffer(
      &asn_DEF_Ngap_PDUSessionResourceModifyRequestTransfer, nullptr, ngap_IEs,
      (void**) &buffer);
  if (encoded_size < 0) {
    Logger::smf_n2().warn(
        "NGAP PDU Session Resource Modify Request Transfer encode failed "
        "(encoded size %d)",
        encoded_size);
    result = false;
  } else {
#if DEBUG_IS_ON
    Logger::smf_n2().debug("N2 SM buffer data: ");
    for (int i = 0; i < encoded_size; i++) printf("%02x ", (char) buffer[i]);
    printf(" (%d bytes)\n", (int) encoded_size);
#endif

    std::string ngap_message((char*) buffer, encoded_size);
    ngap_msg_str = ngap_message;
    result       = true;
  }

  // free memory
  free_wrapper((void**) &pduSessionAggregateMaximumBitRate);
  free_wrapper((void**) &ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation
                   .choice.gTPTunnel->transportLayerAddress.buf);
  free_wrapper((void**) &ngap_UL_NGU_UP_TNLModifyItem->uL_NGU_UP_TNLInformation
                   .choice.gTPTunnel->gTP_TEID.buf);
  free_wrapper((void**) &ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation
                   .choice.gTPTunnel->transportLayerAddress.buf);
  free_wrapper((void**) &ngap_UL_NGU_UP_TNLModifyItem->dL_NGU_UP_TNLInformation
                   .choice.gTPTunnel->gTP_TEID.buf);
  free_wrapper((void**) &ngap_UL_NGU_UP_TNLModifyItem);
  free_wrapper((void**) &ul_NGU_UP_TNLModifyList);
  free_wrapper(
      (void**) &ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters
          ->qosCharacteristics.choice.nonDynamic5QI);
  free_wrapper(
      (void**) &ngap_QosFlowAddOrModifyRequestItem->qosFlowLevelQosParameters);
  free_wrapper((void**) &ngap_QosFlowAddOrModifyRequestItem);
  free_wrapper((void**) &qosFlowAddOrModifyRequestList);
  free_wrapper((void**) &ngap_IEs);
  free_wrapper((void**) &buffer);

  return result;
}

//------------------------------------------------------------------------------
bool smf_n2::create_n2_pdu_session_resource_release_command_transfer(
    pdu_session_update_sm_context_response& sm_context_res,
    n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str) {
  Logger::smf_n2().debug(
      "Create N2 SM Information: NGAP PDU Session Resource Release Command "
      "Transfer IE");
  bool result = false;
  Ngap_PDUSessionResourceReleaseCommandTransfer_t*
      ngap_resource_release_command_transfer = nullptr;
  ngap_resource_release_command_transfer =
      (Ngap_PDUSessionResourceReleaseCommandTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandTransfer_t));

  // TODO: To be completed, here's an example
  ngap_resource_release_command_transfer->cause.present =
      Ngap_Cause_PR_radioNetwork;
  ngap_resource_release_command_transfer->cause.choice.radioNetwork = 1;

  // encode
  size_t buffer_size = BUF_LEN;
  char* buffer       = (char*) calloc(1, buffer_size);

  ssize_t encoded_size = aper_encode_to_new_buffer(
      &asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer, nullptr,
      ngap_resource_release_command_transfer, (void**) &buffer);
  if (encoded_size < 0) {
    Logger::smf_n2().warn(
        "NGAP PDU Session Release Command encode failed (encoded size %d)",
        encoded_size);
    result = false;
  } else {
#if DEBUG_IS_ON
    Logger::smf_n2().debug("N2 SM buffer data: ");
    for (int i = 0; i < encoded_size; i++) printf("%02x ", (char) buffer[i]);
    Logger::smf_n2().debug(" (%d bytes) \n", encoded_size);
#endif

    std::string ngap_message((char*) buffer, encoded_size);
    ngap_msg_str = ngap_message;
    result       = true;
  }

  // free memory
  free_wrapper((void**) &ngap_resource_release_command_transfer);
  free_wrapper((void**) &buffer);
  return result;
}

//------------------------------------------------------------------------------
bool smf_n2::create_n2_pdu_session_resource_setup_request_transfer(
    pdu_session_report_response& msg, n2_sm_info_type_e ngap_info_type,
    std::string& ngap_msg_str) {
  Logger::smf_n2().debug(
      "Create N2 SM Information: NGAP PDU Session Resource Setup Request "
      "Transfer IE");
  // TODO:
  return true;
}

//------------------------------------------------------------------------------
bool smf_n2::create_n2_pdu_session_resource_modify_request_transfer(
    pdu_session_modification_network_requested& msg,
    n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str) {
  Logger::smf_n2().debug(
      "Create N2 SM Information: NGAP PDU Session Resource Modify Request "
      "Transfer IE");
  // TODO:
  return true;
}

//------------------------------------------------------------------------------
bool smf_n2::create_n2_pdu_session_resource_modify_response_transfer(
    pdu_session_update_sm_context_response& sm_context_res,
    n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str) {
  // PDU Session Resource Modify Response Transfer IE for testing purpose
  Logger::smf_n2().debug(
      "Create N2 SM Information: NGAP PDU Session Resource Modify Response "
      "Transfer IE");
  bool result = false;

  Ngap_PDUSessionResourceModifyResponseTransfer_t*
      ngap_resource_response_transfer = nullptr;
  ngap_resource_response_transfer =
      (Ngap_PDUSessionResourceModifyResponseTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceModifyResponseTransfer_t));

  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation =
      (Ngap_UPTransportLayerInformation*) calloc(
          1, sizeof(Ngap_UPTransportLayerInformation));

  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->present =
      Ngap_UPTransportLayerInformation_PR_gTPTunnel;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel =
      (Ngap_GTPTunnel_t*) calloc(1, sizeof(Ngap_GTPTunnel_t));

  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->transportLayerAddress.size = 4;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->transportLayerAddress.buf = (uint8_t*) calloc(4, sizeof(uint8_t));
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->transportLayerAddress.buf[0] = 0xc0;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->transportLayerAddress.buf[1] = 0xa8;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->transportLayerAddress.buf[2] = 0xf8;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->transportLayerAddress.buf[3] = 0x9f;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->transportLayerAddress.bits_unused = 0;

  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->gTP_TEID.size = 4;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->gTP_TEID.buf = (uint8_t*) calloc(4, sizeof(uint8_t));
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->gTP_TEID.buf[0] = 0x00;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->gTP_TEID.buf[1] = 0x00;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->gTP_TEID.buf[2] = 0x00;
  ngap_resource_response_transfer->dL_NGU_UP_TNLInformation->choice.gTPTunnel
      ->gTP_TEID.buf[3] = 0x01;

  ngap_resource_response_transfer->qosFlowAddOrModifyResponseList =
      (Ngap_QosFlowAddOrModifyResponseList_t*) calloc(
          1, sizeof(Ngap_QosFlowAddOrModifyResponseList_t));
  Ngap_QosFlowAddOrModifyResponseItem* qosFlowAddOrModifyResponseItem = nullptr;
  qosFlowAddOrModifyResponseItem =
      (Ngap_QosFlowAddOrModifyResponseItem*) calloc(
          1, sizeof(Ngap_QosFlowAddOrModifyResponseItem));
  qosFlowAddOrModifyResponseItem->qosFlowIdentifier = 60;

  ASN_SEQUENCE_ADD(
      &ngap_resource_response_transfer->qosFlowAddOrModifyResponseList->list,
      qosFlowAddOrModifyResponseItem);

  // encode
  size_t buffer_size = BUF_LEN;
  char* buffer       = (char*) calloc(1, buffer_size);

  ssize_t encoded_size = aper_encode_to_new_buffer(
      &asn_DEF_Ngap_PDUSessionResourceModifyResponseTransfer, nullptr,
      ngap_resource_response_transfer, (void**) &buffer);
  if (encoded_size < 0) {
    Logger::smf_n2().warn(
        " NGAP PDU Session Resource Modify Response Transfer encode failed "
        "(encoded size %d)",
        encoded_size);
    result = false;
  } else {
#if DEBUG_IS_ON
    Logger::smf_n2().debug("N2 SM buffer data: ");
    for (int i = 0; i < encoded_size; i++) printf("%02x ", (char) buffer[i]);
    Logger::smf_n2().debug(" (%d bytes) \n", encoded_size);
#endif

    std::string ngap_message((char*) buffer, encoded_size);
    ngap_msg_str = ngap_message;
    result       = true;
  }

  // free memory
  free_wrapper(
      (void**) &ngap_resource_response_transfer->dL_NGU_UP_TNLInformation
          ->choice.gTPTunnel->transportLayerAddress.buf);
  free_wrapper((void**) &ngap_resource_response_transfer
                   ->dL_NGU_UP_TNLInformation->choice.gTPTunnel->gTP_TEID.buf);
  free_wrapper((void**) &ngap_resource_response_transfer
                   ->dL_NGU_UP_TNLInformation->choice.gTPTunnel);
  free_wrapper((void**) &qosFlowAddOrModifyResponseItem);
  free_wrapper((void**) &ngap_resource_response_transfer
                   ->qosFlowAddOrModifyResponseList);
  free_wrapper((void**) &ngap_resource_response_transfer);
  free_wrapper((void**) &buffer);

  return result;
}

//---------------------------------------------------------------------------------------------
int smf_n2::decode_n2_sm_information(
    std::shared_ptr<Ngap_PDUSessionResourceSetupResponseTransfer_t>& ngap_IE,
    const std::string& n2_sm_info) {
  Logger::smf_n2().info(
      "Decode NGAP message (PDUSessionResourceSetupResponseTransfer) from N2 "
      "SM Information");
  unsigned int data_len = n2_sm_info.length();
  unsigned char* data   = (unsigned char*) malloc(data_len + 1);
  memset(data, 0, data_len + 1);
  memcpy((void*) data, (void*) n2_sm_info.c_str(), data_len);

#if DEBUG_IS_ON
  printf("Content: ");
  for (int i = 0; i < data_len; i++) printf(" %02x ", data[i]);
  printf("\n");
#endif

  // PDUSessionResourceSetupResponseTransfer
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer, (void**) &ngap_IE,
      (void*) data, data_len);

  // free memory
  free_wrapper((void**) &data);

  if (rc.code != RC_OK) {
    Logger::smf_n2().warn("asn_decode failed with code %d", rc.code);
    return RETURNerror;
  }
  return RETURNok;
}

//---------------------------------------------------------------------------------------------
int smf_n2::decode_n2_sm_information(
    std::shared_ptr<Ngap_PDUSessionResourceModifyResponseTransfer_t>& ngap_IE,
    const std::string& n2_sm_info) {
  Logger::smf_n2().info(
      "Decode NGAP message (Ngap_PDUSessionResourceModifyResponseTransfer) "
      "from N2 SM Information");

  unsigned int data_len = n2_sm_info.length();
  unsigned char* data   = (unsigned char*) malloc(data_len + 1);
  memset(data, 0, data_len + 1);
  memcpy((void*) data, (void*) n2_sm_info.c_str(), data_len);

  // Ngap_PDUSessionResourceModifyResponseTransfer
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceModifyResponseTransfer, (void**) &ngap_IE,
      (void*) data, data_len);

  // free memory
  free_wrapper((void**) &data);

  if (rc.code != RC_OK) {
    Logger::smf_n2().warn("asn_decode failed with code %d", rc.code);

    return RETURNerror;
  }
  return RETURNok;
}

//---------------------------------------------------------------------------------------------
int smf_n2::decode_n2_sm_information(
    std::shared_ptr<Ngap_PDUSessionResourceReleaseResponseTransfer_t>& ngap_IE,
    const std::string& n2_sm_info) {
  Logger::smf_n2().info(
      "Decode NGAP message (Ngap_PDUSessionResourceReleaseResponseTransfer) "
      "from N2 SM Information");

  unsigned int data_len = n2_sm_info.length();
  unsigned char* data   = (unsigned char*) malloc(data_len + 1);
  memset(data, 0, data_len + 1);
  memcpy((void*) data, (void*) n2_sm_info.c_str(), data_len);

  // Ngap_PDUSessionResourceModifyResponseTransfer
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceReleaseResponseTransfer,
      (void**) &ngap_IE, (void*) data, data_len);

  // free memory
  free_wrapper((void**) &data);

  if (rc.code != RC_OK) {
    Logger::smf_n2().warn("asn_decode failed with code %d", rc.code);

    return RETURNerror;
  }
  return RETURNok;
}

//---------------------------------------------------------------------------------------------
int smf_n2::decode_n2_sm_information(
    std::shared_ptr<Ngap_PDUSessionResourceSetupUnsuccessfulTransfer_t>&
        ngap_IE,
    const std::string& n2_sm_info) {
  Logger::smf_n2().info(
      "Decode NGAP message (Ngap_PDUSessionResourceSetupUnsuccessfulTransfer) "
      "from N2 SM Information");

  unsigned int data_len = n2_sm_info.length();
  unsigned char* data   = (unsigned char*) malloc(data_len + 1);
  memset(data, 0, data_len + 1);
  memcpy((void*) data, (void*) n2_sm_info.c_str(), data_len);

  // Ngap_PDUSessionResourceSetupUnsuccessfulTransfer
  asn_dec_rval_t rc = asn_decode(
      nullptr, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceSetupUnsuccessfulTransfer,
      (void**) &ngap_IE, (void*) data, data_len);

  // free memory
  free_wrapper((void**) &data);

  if (rc.code != RC_OK) {
    Logger::smf_n2().warn("asn_decode failed with code %d", rc.code);

    return RETURNerror;
  }
  return RETURNok;
}
