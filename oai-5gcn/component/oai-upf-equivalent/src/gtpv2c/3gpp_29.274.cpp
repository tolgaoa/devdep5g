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

/*! \file 3gpp_29.274.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "3gpp_29.274.hpp"

#include <string>
#include <string.h>

using namespace gtpv2c;

const char* interface_type2char[40] = {"S1_U_ENODEB_GTP_U",
                                       "S1_U_SGW_GTP_U",
                                       "S12_RNC_GTP_U",
                                       "S12_SGW_GTP_U",
                                       "S5_S8_SGW_GTP_U",
                                       "S5_S8_PGW_GTP_U",
                                       "S5_S8_SGW_GTP_C",
                                       "S5_S8_PGW_GTP_C",
                                       "S5_S8_SGW_PMIPv6",
                                       "S5_S8_PGW_PMIPv6",
                                       "S11_MME_GTP_C",
                                       "S11_S4_SGW_GTP_C",
                                       "S10_MME_GTP_C",
                                       "S3_MME_GTP_C",
                                       "S3_SGSN_GTP_C",
                                       "S4_SGSN_GTP_U",
                                       "S4_SGW_GTP_U",
                                       "S4_SGSN_GTP_C",
                                       "S16_SGSN_GTP_C",
                                       "ENODEB_GTP_U_DL_DATA_FORWARDING",
                                       "ENODEB_GTP_U_UL_DATA_FORWARDING",
                                       "RNC_GTP_U_DATA_FORWARDING",
                                       "SGSN_GTP_U_DATA_FORWARDING",
                                       "SGW_UPF_GTP_U_DL_DATA_FORWARDING",
                                       "SM_MBMS_GW_GTP_C",
                                       "SN_MBMS_GW_GTP_C",
                                       "SM_MME_GTP_C",
                                       "SN_SGSN_GTP_C",
                                       "SGW_GTP_U_UL_DATA_FORWARDING",
                                       "SN_SGSN_GTP_U",
                                       "S2B_EPDG_GTP_C",
                                       "S2B_U_EPDG_GTP_U",
                                       "S2B_PGW_GTP_C",
                                       "S2B_U_PGW_GTP_U",
                                       "S2A_TWAN_GTP_U",
                                       "S2A_TWAN_GTP_C",
                                       "S2A_PGW_GTP_C",
                                       "S2A_PGW_GTP_U",
                                       "S11_MME_GTP_U",
                                       "S11_SGW_GTP_U"};

//------------------------------------------------------------------------------
gtpv2c_ie* gtpv2c_ie::new_gtpv2c_ie_from_stream(std::istream& is) {
  gtpv2c_tlv tlv;
  tlv.load_from(is);
  if (tlv.length) {
    switch (tlv.type) {
      case GTP_IE_IMSI: {
        gtpv2c_imsi_ie* ie = new gtpv2c_imsi_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_CAUSE: {
        gtpv2c_cause_ie* ie = new gtpv2c_cause_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_RECOVERY_RESTART_COUNTER: {
        gtpv2c_recovery_ie* ie = new gtpv2c_recovery_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_ACCESS_POINT_NAME: {
        gtpv2c_access_point_name_ie* ie = new gtpv2c_access_point_name_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_AGGREGATE_MAXIMUM_BIT_RATE: {
        gtpv2c_aggregate_maximum_bit_rate_ie* ie =
            new gtpv2c_aggregate_maximum_bit_rate_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_EPS_BEARER_ID: {
        gtpv2c_eps_bearer_id_ie* ie = new gtpv2c_eps_bearer_id_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_IP_ADDRESS: {
        gtpv2c_ip_address_ie* ie = new gtpv2c_ip_address_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_MOBILE_EQUIPMENT_IDENTITY: {
        gtpv2c_mei_ie* ie = new gtpv2c_mei_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_MSISDN: {
        gtpv2c_msisdn_ie* ie = new gtpv2c_msisdn_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_INDICATION: {
        gtpv2c_indication_ie* ie = new gtpv2c_indication_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_PROTOCOL_CONFIGURATION_OPTIONS: {
        gtpv2c_pco_ie* ie = new gtpv2c_pco_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_PDN_ADDRESS_ALLOCATION: {
        gtpv2c_paa_ie* ie = new gtpv2c_paa_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_BEARER_QUALITY_OF_SERVICE: {
        gtpv2c_bearer_qos_ie* ie = new gtpv2c_bearer_qos_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_FLOW_QUALITY_OF_SERVICE: {
        gtpv2c_flow_qos_ie* ie = new gtpv2c_flow_qos_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_RAT_TYPE: {
        gtpv2c_rat_type_ie* ie = new gtpv2c_rat_type_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_SERVING_NETWORK: {
        gtpv2c_serving_network_ie* ie = new gtpv2c_serving_network_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_EPS_BEARER_LEVEL_TRAFFIC_FLOW_TEMPLATE: {
        gtpv2c_bearer_tft_ie* ie = new gtpv2c_bearer_tft_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_TRAFFIC_AGGREGATE_DESCRIPTION:
      case GTP_IE_USER_LOCATION_INFORMATION: {
        gtpv2c_user_location_information_ie* ie =
            new gtpv2c_user_location_information_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_FULLY_QUALIFIED_TUNNEL_ENDPOINT_IDENTIFIER: {
        gtpv2c_fully_qualified_teid_ie* ie =
            new gtpv2c_fully_qualified_teid_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_TMSI:
      // case GTP_IE_GLOBAL_CN_ID:
      // case GTP_IE_S103_PDN_DATA_FORWARDING_INFO:
      // case GTP_IE_S1U_DATA_FORWARDING_INFO:
      case GTP_IE_DELAY_VALUE: {
        gtpv2c_delay_value_ie* ie = new gtpv2c_delay_value_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_BEARER_CONTEXT: {
        gtpv2c_bearer_context_ie* ie = new gtpv2c_bearer_context_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_CHARGING_ID: {
        gtpv2c_charging_id_ie* ie = new gtpv2c_charging_id_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_CHARGING_CHARACTERISTICS:
      // case GTP_IE_TRACE_INFORMATION:
      case GTP_IE_BEARER_FLAGS: {
        gtpv2c_bearer_flags_ie* ie = new gtpv2c_bearer_flags_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTP_IE_PDN_TYPE: {
        gtpv2c_pdn_type_ie* ie = new gtpv2c_pdn_type_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_PROCEDURE_TRANSACTION_ID:
      // case GTP_IE_MM_CONTEXT_GSM_KEY_AND_TRIPLETS:
      // case GTP_IE_MM_CONTEXT_UMTS_KEY_USED_CIPHER_AND_QUINTUPLETS:
      // case GTP_IE_MM_CONTEXT_GSM_KEY_USED_CIPHER_AND_QUINTUPLETS:
      // case GTP_IE_MM_CONTEXT_UMTS_KEY_AND_QUINTUPLETS:
      // case GTP_IE_MM_CONTEXT_EPS_SECURITY_CONTEXT_AND_QUADRUPLETS:
      // case GTP_IE_MM_CONTEXT_UMTS_KEY_QUADRUPLETS_AND_QUINTUPLETS:
      // case GTP_IE_PDN_CONNECTION:
      // case GTP_IE_PDU_NUMBERS:
      // case GTP_IE_PACKET_TMSI:
      // case GTP_IE_P_TMSI_SIGNATURE:
      // case GTP_IE_HOP_COUNTER:
      case GTP_IE_UE_TIME_ZONE: {
        gtpv2c_ue_time_zone_ie* ie = new gtpv2c_ue_time_zone_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_TRACE_REFERENCE:
      // case GTP_IE_COMPLETE_REQUEST_MESSAGE:
      // case GTP_IE_GUTI:
      // case GTP_IE_FULLY_QUALIFIED_CONTAINER:
      // case GTP_IE_FULLY_QUALIFIED_CAUSE:
      // case GTP_IE_PLMN_ID:
      // case GTP_IE_TARGET_IDENTIFICATION:
      // case GTP_IE_PACKET_FLOW_ID:
      // case GTP_IE_RAB_CONTEXT:
      // case GTP_IE_SOURCE_RNC_PDCP_CONTEXT_INFO:
      // case GTP_IE_PORT_NUMBER:
      case GTP_IE_APN_RESTRICTION: {
        gtpv2c_apn_restriction_ie* ie = new gtpv2c_apn_restriction_ie(tlv);
        ie->load_from(is);
        return ie;
      }

      case GTP_IE_SELECTION_MODE: {
        gtpv2c_selection_mode_ie* ie = new gtpv2c_selection_mode_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_SOURCE_IDENTIFICATION:
      // case GTP_IE_CHANGE_REPORTING_ACTION:
      case GTP_IE_FQ_CSID: {
        gtpv2c_fq_csid_ie* ie = new gtpv2c_fq_csid_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_CHANNEL_NEEDED:
      // case GTP_IE_EMLPP_PRIORITY:
      case GTP_IE_NODE_TYPE: {
        gtpv2c_node_type_ie* ie = new gtpv2c_node_type_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_FULLY_QUALIFIED_DOMAIN_NAME:
      // case GTP_IE_TRANSACTION_IDENTIFIER:
      // case GTP_IE_MBMS_SESSION_DURATION:
      // case GTP_IE_MBMS_SERVICE_AREA:
      // case GTP_IE_MBMS_SESSION_IDENTIFIER:
      // case GTP_IE_MBMS_FLOW_IDENTIFIER:
      // case GTP_IE_MBMS_IP_MULTICAST_DISTRIBUTION:
      // case GTP_IE_MBMS_DISTRIBUTION_ACKNOWLEDGE:
      // case GTP_IE_RFSP_INDEX:
      // case GTP_IE_USER_CSG_INFORMATION:
      // case GTP_IE_CSG_REPORTING_ACTION:
      // case GTP_IE_CSG_ID:
      // case GTP_IE_CSG_MEMBERSHIP_INDICATION:
      // case GTP_IE_SERVICE_INDICATOR:
      // case GTP_IE_DETACH_TYPE:
      // case GTP_IE_LOCAL_DISTINGUISHED_NAME:
      case GTP_IE_NODE_FEATURES: {
        gtpv2c_node_features_ie* ie = new gtpv2c_node_features_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_MBMS_TIME_TO_DATA_TRANSFER:
      // case GTP_IE_THROTTLING:
      // case GTP_IE_ALLOCATION_RETENTION_PRIORITY:
      // case GTP_IE_EPC_TIMER:
      // case GTP_IE_SIGNALLING_PRIORITY_INDICATION:
      // case GTP_IE_TMGI:
      // case GTP_IE_ADDITIONAL_MM_CONTEXT_FOR_SRVCC:
      // case GTP_IE_ADDITIONAL_FLAGS_FOR_SRVCC:
      // case GTP_IE_MDT_CONFIGURATION:
      // case GTP_IE_ADDITIONAL_PROTOCOL_CONFIGURATION_OPTIONS:
      // case GTP_IE_ABSOLUTE_TIME_OF_MBMS_DATA_TRANSFER:
      // case GTP_IE_H_E_NB_INFORMATION_REPORTING:
      // case GTP_IE_IPV4_CONFIGURATION_PARAMETERS:
      // case GTP_IE_CHANGE_TO_REPORT_FLAGS:
      // case GTP_IE_ACTION_INDICATION:
      // case GTP_IE_TWAN_IDENTIFIER:
      // case GTP_IE_ULI_TIMESTAMP:
      // case GTP_IE_MBMS_FLAGS:
      // case GTP_IE_RAN_NAS_CAUSE:
      // case GTP_IE_CN_OPERATOR_SELECTION_ENTITY:
      // case GTP_IE_TRUSTED_WLAN_MODE_INDICATION:
      // case GTP_IE_NODE_NUMBER:
      // case GTP_IE_NODE_IDENTIFIER:
      // case GTP_IE_PRESENCE_REPORTING_AREA_ACTION:
      // case GTP_IE_PRESENCE_REPORTING_AREA_INFORMATION:
      // case GTP_IE_TWAN_IDENTIFIER_TIMESTAMP:
      // case GTP_IE_OVERLOAD_CONTROL_INFORMATION:
      // case GTP_IE_LOAD_CONTROL_INFORMATION:
      // case GTP_IE_METRIC:
      // case GTP_IE_SEQUENCE_NUMBER:
      // case GTP_IE_APN_AND_RELATIVE_CAPACITY:
      // case GTP_IE_PAGING_AND_SERVICE_INFORMATION:
      // case GTP_IE_INTEGER_NUMBER:
      // case GTP_IE_MILLISECOND_TIME_STAMP:
      // case GTP_IE_MONITORING_EVENT_INFORMATION:
      // case GTP_IE_ECGI_LIST:
      // case GTP_IE_REMOTE_UE_CONTEXT:
      // case GTP_IE_REMOTE_USER_ID:
      // case GTP_IE_REMOTE_UE_IP_INFORMATION:
      case GTP_IE_CIOT_OPTIMIZATIONS_SUPPORT_INDICATION: {
        gtpv2c_ciot_optimizations_support_indication_ie* ie =
            new gtpv2c_ciot_optimizations_support_indication_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      // case GTP_IE_SCEF_PDN_CONNECTION:
      // case GTP_IE_HEADER_COMPRESSION_CONFIGURATION:
      // case GTP_IE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS:
      // case GTP_IE_SERVING_PLMN_RATE_CONTROL:
      // case GTP_IE_COUNTER:
      // case GTP_IE_MAPPED_UE_USAGE_TYPE:
      // case GTP_IE_SECONDARY_RAT_USAGE_DATA_REPORT:
      // case GTP_IE_UP_FUNCTION_SELECTION_INDICATION_FLAGS:
      // case GTP_IE_PRIVATE_EXTENSION:
      // case GTP_IE_WLAN_OFFLOADABILITY_INDICATION:
      // case GTP_IE_MAXIMUM_PACKET_LOSS:
      default:
        Logger::gtpv2_c().error(
            "Unknown GTP IE type %d (length %d)", tlv.get_type(),
            tlv.get_length());
        return nullptr;
    }
  } else {
    Logger::gtpv2_c().error(
        "GTP IE type %d length %d", tlv.get_type(), tlv.get_length());
    return nullptr;
  }
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_create_session_request& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_CREATE_SESSION_REQUEST);

  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_IMSI) {
    std::shared_ptr<gtpv2c_imsi_ie> sie(new gtpv2c_imsi_ie(gtp_ies.imsi));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MSISDN) {
    std::shared_ptr<gtpv2c_msisdn_ie> sie(new gtpv2c_msisdn_ie(gtp_ies.msisdn));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MEI) {
    std::shared_ptr<gtpv2c_mei_ie> sie(new gtpv2c_mei_ie(gtp_ies.mei));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_ULI) {
    std::shared_ptr<gtpv2c_user_location_information_ie> sie(
        new gtpv2c_user_location_information_ie(gtp_ies.uli));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SERVING_NETWORK) {
    std::shared_ptr<gtpv2c_serving_network_ie> sie(
        new gtpv2c_serving_network_ie(gtp_ies.serving_network));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_RAT_TYPE) {
    std::shared_ptr<gtpv2c_rat_type_ie> sie(
        new gtpv2c_rat_type_ie(gtp_ies.rat_type));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_INDICATION_FLAGS) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE) {
    std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
        new gtpv2c_fully_qualified_teid_ie(gtp_ies.sender_fteid_for_cp));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PGW_S5S8_ADDRESS_FOR_CONTROL_PLANE) {
    std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
        new gtpv2c_fully_qualified_teid_ie(gtp_ies.pgw_s5s8_address_for_cp));
    sie.get()->tlv.set_instance(1);
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN) {
    std::shared_ptr<gtpv2c_access_point_name_ie> sie(
        new gtpv2c_access_point_name_ie(gtp_ies.apn));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SELECTION_MODE) {
    std::shared_ptr<gtpv2c_selection_mode_ie> sie(
        new gtpv2c_selection_mode_ie(gtp_ies.selection_mode));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PDN_TYPE) {
    std::shared_ptr<gtpv2c_pdn_type_ie> sie(
        new gtpv2c_pdn_type_ie(gtp_ies.pdn_type));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PAA) {
    std::shared_ptr<gtpv2c_paa_ie> sie(new gtpv2c_paa_ie(gtp_ies.paa));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_RESTRICTION) {
    std::shared_ptr<gtpv2c_apn_restriction_ie> sie(
        new gtpv2c_apn_restriction_ie(gtp_ies.apn_restriction));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APN_AMBR) {
    std::shared_ptr<gtpv2c_aggregate_maximum_bit_rate_ie> sie(
        new gtpv2c_aggregate_maximum_bit_rate_ie(gtp_ies.ambr));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_LINKED_EPS_BEARER_ID) {
    std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
        new gtpv2c_eps_bearer_id_ie(gtp_ies.linked_eps_bearer_id));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TRUSTED_WLAN_MODE_INDICATION) {}
  if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PCO) {
    std::shared_ptr<gtpv2c_pco_ie> sie(new gtpv2c_pco_ie(gtp_ies.pco));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_CREATED) {
    if (gtp_ies.bearer_contexts_to_be_created.size() > 0) {
      for (auto i : gtp_ies.bearer_contexts_to_be_created) {
        gtpv2c_grouped_ie* gie = new gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT);
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID) {
          std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
              new gtpv2c_eps_bearer_id_ie(i.eps_bearer_id));
          gie->add_ie(sie);
        }
        // TODO if (i.ie_presence_mask &
        // GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_TFT)
        // {std::shared_ptr<gtpv2c_traffic_flow_template_ie> sie(new
        // gtpv2c_traffic_flow_template_ie(i.ambr)); gie->add_ie(sie);}
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S1_U_ENB_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s1_u_enb_fteid));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s4_u_sgsn_fteid));
          sie.get()->tlv.set_instance(1);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_SGW_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s5_s8_u_sgw_fteid));
          sie.get()->tlv.set_instance(2);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S5_S8_U_PGW_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s5_s8_u_pgw_fteid));
          sie.get()->tlv.set_instance(3);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S12_RNC_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s12_rnc_fteid));
          sie.get()->tlv.set_instance(4);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2B_U_EPDG_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s2b_u_epdg_fteid));
          sie.get()->tlv.set_instance(5);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S2A_U_TWAN_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s2a_u_twan_fteid));
          sie.get()->tlv.set_instance(6);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_BEARER_LEVEL_QOS) {
          std::shared_ptr<gtpv2c_bearer_qos_ie> sie(
              new gtpv2c_bearer_qos_ie(i.bearer_level_qos));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_CREATED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S11_U_MME_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s11_u_mme_fteid));
          sie.get()->tlv.set_instance(7);
          gie->add_ie(sie);
        }
        std::shared_ptr<gtpv2c_grouped_ie> sgie(gie);
        add_ie(sgie);
      }
    }
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_REMOVED) {
    if (gtp_ies.bearer_contexts_to_be_removed.size() > 0) {
      for (auto i : gtp_ies.bearer_contexts_to_be_removed) {
        gtpv2c_grouped_ie* gie = new gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT);
        gie->tlv.set_instance(1);
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_EPS_BEARER_ID) {
          std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
              new gtpv2c_eps_bearer_id_ie(i.eps_bearer_id));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_CREATE_SESSION_REQUEST_PR_IE_S4_U_SGSN_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s4_u_sgsn_fteid));
          gie->add_ie(sie);
        }
        std::shared_ptr<gtpv2c_grouped_ie> sie(gie);
        add_ie(sie);
      }
    }
  }

  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TRACE_INFORMATION) {} if
  // (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_RECOVERY)
  // {}
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MME_FQ_CSID) {
    std::shared_ptr<gtpv2c_fq_csid_ie> sie(
        new gtpv2c_fq_csid_ie(gtp_ies.mme_fq_csid));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_FQ_CSID) {
    std::shared_ptr<gtpv2c_fq_csid_ie> sie(
        new gtpv2c_fq_csid_ie(gtp_ies.sgw_fq_csid));
    sie.get()->tlv.set_instance(1);
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPDG_FQ_CSID)
  // {std::shared_ptr<gtpv2c_fq_csid_ie> sie(new gtpv2c_fq_csid_ie(gtp_ies.));
  // add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_FQ_CSID)
  // {std::shared_ptr<gtpv2c_fq_csid_ie> sie(new gtpv2c_fq_csid_ie(gtp_ies.));
  // add_ie(sie);}
  if (gtp_ies.ie_presence_mask &
      GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_TIME_ZONE) {
    std::shared_ptr<gtpv2c_ue_time_zone_ie> sie(
        new gtpv2c_ue_time_zone_ie(gtp_ies.ue_time_zone));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UCI)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_CHARGING_CHARACTERISTICS) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MME_S4_SGSN_LDN) {} if
  // (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_LDN) {}
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPDG_LDN) {} if
  // (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_LDN)
  // {} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SIGNALLING_PRIORITY_INDICATION) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_LOCAL_IP_ADDRESS) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_UDP_PORT)
  // {} if (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_APCO)
  // {} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_HENB_LOCAL_IP_ADDRESS) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_HENB_UDP_PORT) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MME_S4_SGSN_IDENTIFIER) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_IDENTIFIER) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPDG_IP_ADDRESS) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_CN_OPERATOR_SELECTION_ENTITY) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PRESENCE_REPORTING_AREA_INFORMATION) {}
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MMES4_SGSN_OVERLOAD_CONTROL_INFORMATION)
  // {} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_TWAN_EPDG_OVERLOAD_CONTROL_INFORMATION)
  // {} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_ORIGINATION_TIME_STAMP) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MAXIMUM_WAIT_TIME) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_WLAN_LOCATION_INFORMATION) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_WLAN_LOCATION_TIMESTAMP) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_NBIFOM_CONTAINER) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_REMOTE_UE_CONTEXT_CONNECTED) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_3GPP_AAA_SERVER_IDENTIFIER) {} if
  // (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_REQUEST_PR_IE_EPCO)
  // {}{std::shared_ptr<gtpv2c_epco_ie> sie(new gtpv2c_epco_ie(gtp_ies.));
  // add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SERVING_PLMN_RATE_CONTROL) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MO_EXCEPTION_DATA_COUNTER) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_UE_TCP_PORT)
  // {} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_MAPPED_UE_USAGE_TYPE) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_ULI_FOR_SGW)
  // {} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_SGW_U_NODE_NAME) {} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_REQUEST_PR_IE_PRIVATE_EXTENSION) {}
}

//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_create_session_response& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_CREATE_SESSION_RESPONSE);

  if (gtp_ies.cause.first) {
    std::shared_ptr<gtpv2c_cause_ie> sie(
        new gtpv2c_cause_ie(gtp_ies.cause.second));
    add_ie(sie);
  } else
    throw gtpc_missing_ie_exception(
        "GTP_CREATE_SESSION_RESPONSE", "GTP_IE_CAUSE");
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_CHANGE_REPORTING_ACTION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_CSG_INFORMATION_REPORTING_ACTION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_HENB_INFORMATION_REPORTING)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.sender_fteid_for_cp.first) {
    std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
        new gtpv2c_fully_qualified_teid_ie(gtp_ies.sender_fteid_for_cp.second));
    add_ie(sie);
  }
  if (gtp_ies.s5_s8_pgw_fteid.first) {
    std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
        new gtpv2c_fully_qualified_teid_ie(gtp_ies.s5_s8_pgw_fteid.second));
    sie.get()->tlv.set_instance(1);
    add_ie(sie);
  }
  if (gtp_ies.paa.first) {
    std::shared_ptr<gtpv2c_paa_ie> sie(new gtpv2c_paa_ie(gtp_ies.paa.second));
    add_ie(sie);
  }
  if (gtp_ies.apn_restriction.first) {
    std::shared_ptr<gtpv2c_apn_restriction_ie> sie(
        new gtpv2c_apn_restriction_ie(gtp_ies.apn_restriction.second));
    add_ie(sie);
  }
  if (gtp_ies.apn_ambr.first) {
    std::shared_ptr<gtpv2c_aggregate_maximum_bit_rate_ie> sie(
        new gtpv2c_aggregate_maximum_bit_rate_ie(gtp_ies.apn_ambr.second));
    add_ie(sie);
  }
  if (gtp_ies.linked_eps_bearer_id.first) {
    std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
        new gtpv2c_eps_bearer_id_ie(gtp_ies.linked_eps_bearer_id.second));
    add_ie(sie);
  }
  if (gtp_ies.pco.first) {
    std::shared_ptr<gtpv2c_pco_ie> sie(new gtpv2c_pco_ie(gtp_ies.pco.second));
    add_ie(sie);
  }
  if (gtp_ies.bearer_contexts_created.first) {
    if (gtp_ies.bearer_contexts_created.second.size() > 0) {
      for (auto i : gtp_ies.bearer_contexts_created.second) {
        gtpv2c_grouped_ie* gie = new gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT);
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID) {
          std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
              new gtpv2c_eps_bearer_id_ie(i.eps_bearer_id));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE) {
          std::shared_ptr<gtpv2c_cause_ie> sie(new gtpv2c_cause_ie(i.cause));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S1_U_SGW_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s1_u_sgw_fteid));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S4_U_SGW_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s4_u_sgw_fteid));
          sie.get()->tlv.set_instance(1);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_U_PGW_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s5_s8_u_pgw_fteid));
          sie.get()->tlv.set_instance(2);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S12_SGW_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s12_sgw_fteid));
          sie.get()->tlv.set_instance(3);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S5_S8_BEARER_LEVEL_QOS) {
          std::shared_ptr<gtpv2c_bearer_qos_ie> sie(
              new gtpv2c_bearer_qos_ie(i.bearer_level_qos));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CHARGING_ID) {
          std::shared_ptr<gtpv2c_charging_id_ie> sie(
              new gtpv2c_charging_id_ie(i.charging_id));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_BEARER_FLAGS) {
          std::shared_ptr<gtpv2c_bearer_flags_ie> sie(
              new gtpv2c_bearer_flags_ie(i.bearer_flags));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_CREATED_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_S11_U_SGW_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s11_u_sgw_fteid));
          sie.get()->tlv.set_instance(6);
          gie->add_ie(sie);
        }
        std::shared_ptr<gtpv2c_grouped_ie> sie(gie);
        add_ie(sie);
      }
    }
  }
  if (gtp_ies.bearer_contexts_marked_for_removal.first) {
    if (gtp_ies.bearer_contexts_marked_for_removal.second.size() > 0) {
      for (auto i : gtp_ies.bearer_contexts_marked_for_removal.second) {
        gtpv2c_grouped_ie* gie = new gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT);
        gie->tlv.set_instance(1);
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_EPS_BEARER_ID) {
          std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
              new gtpv2c_eps_bearer_id_ie(i.eps_bearer_id));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_MARKED_FOR_REMOVAL_WITHIN_CREATE_SESSION_RESPONSE_PR_IE_CAUSE) {
          std::shared_ptr<gtpv2c_cause_ie> sie(new gtpv2c_cause_ie(i.cause));
          gie->add_ie(sie);
        }
        std::shared_ptr<gtpv2c_grouped_ie> sie(gie);
        add_ie(sie);
      }
    }
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_RECOVERY) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_CHARGING_GATEWAY_NAME)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_CHARGING_GATEWAY_ADDRESS)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.pgw_fq_csid.first) {
    std::shared_ptr<gtpv2c_fq_csid_ie> sie(
        new gtpv2c_fq_csid_ie(gtp_ies.pgw_fq_csid.second));
    add_ie(sie);
  }
  if (gtp_ies.sgw_fq_csid.first) {
    std::shared_ptr<gtpv2c_fq_csid_ie> sie(
        new gtpv2c_fq_csid_ie(gtp_ies.sgw_fq_csid.second));
    sie.get()->tlv.set_instance(1);
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_SGW_LDN) {std::shared_ptr<xxx> sie(new
  // xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_PGW_LDN) {std::shared_ptr<xxx> sie(new
  // xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_PGW_BACK_OFF_TIME)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.pgw_back_off_time));
  // add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_APCO) {std::shared_ptr<xxx> sie(new
  // xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_TRUSTED_WLAN_IPV4_PARAMETERS)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.indication_flags.first) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags.second));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_PRESENCE_REPORTING_AREA_ACTION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_PGW_NODE_LEVEL_LOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_PGW_APN_LEVEL_LOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_SGW_NODE_LEVEL_LOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_PGW_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_NBIFOM_CONTAINER)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_PDN_CONNECTION_CHARGING_ID)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask & GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_EPCO)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_CREATE_SESSION_RESPONSE_PR_IE_PRIVATE_EXTENSION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
}

//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_delete_session_request& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_DELETE_SESSION_REQUEST);
  if (gtp_ies.ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_CAUSE) {
    std::shared_ptr<gtpv2c_cause_ie> sie(new gtpv2c_cause_ie(gtp_ies.cause));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_DELETE_SESSION_REQUEST_PR_IE_LINKED_EPS_BEARER_ID) {
    std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
        new gtpv2c_eps_bearer_id_ie(gtp_ies.linked_eps_bearer_id));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ULI) {
    std::shared_ptr<gtpv2c_user_location_information_ie> sie(
        new gtpv2c_user_location_information_ie(gtp_ies.uli));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_DELETE_SESSION_REQUEST_PR_IE_INDICATION_FLAGS) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_PCO) {
    std::shared_ptr<gtpv2c_pco_ie> sie(new gtpv2c_pco_ie(gtp_ies.pco));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ORIGINATING_NODE) {
    std::shared_ptr<gtpv2c_node_type_ie> sie(
        new gtpv2c_node_type_ie(gtp_ies.originating_node));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_DELETE_SESSION_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE) {
    std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
        new gtpv2c_fully_qualified_teid_ie(gtp_ies.sender_fteid_for_cp));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_TIME_ZONE) {
    std::shared_ptr<gtpv2c_ue_time_zone_ie> sie(
        new gtpv2c_ue_time_zone_ie(gtp_ies.ue_time_zone));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_ULI_TIMESTAMP) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.ie_presence_mask &
      GTPV2C_DELETE_SESSION_REQUEST_PR_IE_RAN_NAS_RELEASE_CAUSE) {
    std::shared_ptr<gtpv2c_ran_nas_cause_ie> sie(
        new gtpv2c_ran_nas_cause_ie(gtp_ies.ran_nas_release_cause));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_TWAN_IDENTIFIER) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_TWAN_IDENTIFIER_TIMESTAMP)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_MMES4_SGSN_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_TWAN_EPDG_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_WLAN_LOCATION_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_WLAN_LOCATION_TIMESTAMP)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_LOCAL_IP_ADDRESS)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_UDP_PORT) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.ie_presence_mask & GTPV2C_DELETE_SESSION_REQUEST_PR_IE_EPCO) {
    std::shared_ptr<gtpv2c_epco_ie> sie(new gtpv2c_epco_ie(gtp_ies.epco));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_UE_TCP_PORT) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_REQUEST_PR_IE_PRIVATE_EXTENSION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_delete_session_response& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_DELETE_SESSION_RESPONSE);
  if (gtp_ies.ie_presence_mask & GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_CAUSE) {
    std::shared_ptr<gtpv2c_cause_ie> sie(new gtpv2c_cause_ie(gtp_ies.cause));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_RECOVERY) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.ie_presence_mask & GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PCO) {
    std::shared_ptr<gtpv2c_pco_ie> sie(new gtpv2c_pco_ie(gtp_ies.pco));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_INDICATION_FLAGS) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PGW_NODE_LEVEL_LOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PGW_APN_LEVEL_LOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_SGW_NODE_LEVEL_LOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PGW_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.ie_presence_mask & GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_EPCO) {
    std::shared_ptr<gtpv2c_epco_ie> sie(new gtpv2c_epco_ie(gtp_ies.epco));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_DELETE_SESSION_RESPONSE_PR_IE_PRIVATE_EXTENSION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_modify_bearer_request& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_MODIFY_BEARER_REQUEST);
  if (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MEI) {
    std::shared_ptr<gtpv2c_mei_ie> sie(new gtpv2c_mei_ie(gtp_ies.mei));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_ULI) {
    std::shared_ptr<gtpv2c_user_location_information_ie> sie(
        new gtpv2c_user_location_information_ie(gtp_ies.uli));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SERVING_NETWORK) {
    std::shared_ptr<gtpv2c_serving_network_ie> sie(
        new gtpv2c_serving_network_ie(gtp_ies.serving_network));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_RAT_TYPE) {
    std::shared_ptr<gtpv2c_rat_type_ie> sie(
        new gtpv2c_rat_type_ie(gtp_ies.rat_type));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_INDICATION_FLAGS) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SENDER_FTEID_FOR_CONTROL_PLANE) {
    std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
        new gtpv2c_fully_qualified_teid_ie(gtp_ies.sender_fteid_for_cp));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_APN_AMBR) {
    std::shared_ptr<gtpv2c_aggregate_maximum_bit_rate_ie> sie(
        new gtpv2c_aggregate_maximum_bit_rate_ie(gtp_ies.apn_ambr));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_DELAY_DOWNLINK_PACKET_NOTIFICATION_REQUEST) {
    std::shared_ptr<gtpv2c_delay_value_ie> sie(
        new gtpv2c_delay_value_ie(gtp_ies.delay_dl_packet_notif_req));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_MODIFIED) {
    if (gtp_ies.bearer_contexts_to_be_modified.size() > 0) {
      for (auto i : gtp_ies.bearer_contexts_to_be_modified) {
        gtpv2c_grouped_ie* gie = new gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT);

        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID) {
          std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
              new gtpv2c_eps_bearer_id_ie(i.eps_bearer_id));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S1_U_ENB_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s1_u_enb_fteid));
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S5_S8_U_SGW_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s5_s8_u_sgw_fteid));
          sie.get()->tlv.set_instance(1);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S12_RNC_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s12_rnc_fteid));
          sie.get()->tlv.set_instance(2);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S4_U_SGSN_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s4_u_sgsn_fteid));
          sie.get()->tlv.set_instance(3);
          gie->add_ie(sie);
        }
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_MODIFIED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_S11_U_MME_FTEID) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s11_u_mme_fteid));
          sie.get()->tlv.set_instance(4);
          gie->add_ie(sie);
        }
        std::shared_ptr<gtpv2c_grouped_ie> sie(gie);
        add_ie(sie);
      }
    }
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_BEARER_CONTEXTS_TO_BE_REMOVED) {
    if (gtp_ies.bearer_contexts_to_be_removed.size() > 0) {
      for (auto i : gtp_ies.bearer_contexts_to_be_removed) {
        gtpv2c_grouped_ie* gie = new gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT);
        gie->tlv.set_instance(1);
        if (i.ie_presence_mask &
            GTPV2C_BEARER_CONTEXT_TO_BE_REMOVED_WITHIN_MODIFY_BEARER_REQUEST_PR_IE_EPS_BEARER_ID) {
          std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
              new gtpv2c_eps_bearer_id_ie(i.eps_bearer_id));
          gie->add_ie(sie);
        }
        std::shared_ptr<gtpv2c_grouped_ie> sie(gie);
        add_ie(sie);
      }
    }
  }
  // if (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_RECOVERY)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UE_TIME_ZONE) {
    std::shared_ptr<gtpv2c_ue_time_zone_ie> sie(
        new gtpv2c_ue_time_zone_ie(gtp_ies.ue_time_zone));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MME_FQ_CSID) {
    std::shared_ptr<gtpv2c_fq_csid_ie> sie(
        new gtpv2c_fq_csid_ie(gtp_ies.mme_fq_csid));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SGW_FQ_CSID) {
    std::shared_ptr<gtpv2c_fq_csid_ie> sie(
        new gtpv2c_fq_csid_ie(gtp_ies.sgw_fq_csid));
    sie.get()->tlv.set_instance(1);
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UCI)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.uci)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UE_LOCAL_IP_ADDRESS)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_UE_UDP_PORT)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MME_S4_SGSN_LDN) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SGW_LDN) {std::shared_ptr<xxx> sie(new
  // xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_HENB_LOCAL_IP_ADDRESS)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_HENB_UDP_PORT) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MME_S4_SGSN_IDENTIFIER)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_CN_OPERATOR_SELECTION_ENTITY)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_PRESENCE_REPORTING_AREA_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MMES4_SGSN_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SGW_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_EPDG_OVERLOAD_CONTROL_INFORMATION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_SERVING_PLMN_RATE_CONTROL)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if
  // (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_MO_EXCEPTION_DATA_COUNTER)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
  if (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_IMSI) {
    std::shared_ptr<gtpv2c_imsi_ie> sie(new gtpv2c_imsi_ie(gtp_ies.imsi));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_ULI_FOR_SGW) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);} if (gtp_ies.ie_presence_mask &
  // GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_PRIVATE_EXTENSION) {std::shared_ptr<xxx>
  // sie(new xxx(gtp_ies.xxx)); add_ie(sie);}
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_modify_bearer_response& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_MODIFY_BEARER_RESPONSE);
  /*
      cause = {};
      msisdn = {};
      linked_eps_bearer_id = {};
      apn_restriction = {};
      pco = {};
      bearer_contexts_modified = {};
      bearer_contexts_marked_for_removal = {};
      pgw_fq_csid = {};
      sgw_fq_csid = {};
      indication_flags = {};
      pdn_connection_charging_id = {};
   */

  if (gtp_ies.cause.first) {
    std::shared_ptr<gtpv2c_cause_ie> sie(
        new gtpv2c_cause_ie(gtp_ies.cause.second));
    add_ie(sie);
  }
  if (gtp_ies.msisdn.first) {
    std::shared_ptr<gtpv2c_msisdn_ie> sie(
        new gtpv2c_msisdn_ie(gtp_ies.msisdn.second));
    add_ie(sie);
  }
  if (gtp_ies.linked_eps_bearer_id.first) {
    std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
        new gtpv2c_eps_bearer_id_ie(gtp_ies.linked_eps_bearer_id.second));
    add_ie(sie);
  }
  if (gtp_ies.apn_restriction.first) {
    std::shared_ptr<gtpv2c_apn_restriction_ie> sie(
        new gtpv2c_apn_restriction_ie(gtp_ies.apn_restriction.second));
    add_ie(sie);
  }
  if (gtp_ies.pco.first) {
    std::shared_ptr<gtpv2c_pco_ie> sie(new gtpv2c_pco_ie(gtp_ies.pco.second));
    add_ie(sie);
  }
  if (gtp_ies.pgw_fq_csid.first) {
    std::shared_ptr<gtpv2c_fq_csid_ie> sie(
        new gtpv2c_fq_csid_ie(gtp_ies.pgw_fq_csid.second));
    add_ie(sie);
  }
  if (gtp_ies.sgw_fq_csid.first) {
    std::shared_ptr<gtpv2c_fq_csid_ie> sie(
        new gtpv2c_fq_csid_ie(gtp_ies.sgw_fq_csid.second));
    sie.get()->tlv.set_instance(1);
    add_ie(sie);
  }
  if (gtp_ies.indication_flags.first) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags.second));
    add_ie(sie);
  }
  if (gtp_ies.pdn_connection_charging_id.first) {
    std::shared_ptr<gtpv2c_charging_id_ie> sie(
        new gtpv2c_charging_id_ie(gtp_ies.pdn_connection_charging_id.second));
    add_ie(sie);
  }

  if (gtp_ies.bearer_contexts_modified.first) {
    if (gtp_ies.bearer_contexts_modified.second.size() > 0) {
      for (auto i : gtp_ies.bearer_contexts_modified.second) {
        gtpv2c_grouped_ie* gie = new gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT);
        if (i.eps_bearer_id.first) {
          std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
              new gtpv2c_eps_bearer_id_ie(i.eps_bearer_id.second));
          gie->add_ie(sie);
        }
        if (i.cause.first) {
          std::shared_ptr<gtpv2c_cause_ie> sie(
              new gtpv2c_cause_ie(i.cause.second));
          gie->add_ie(sie);
        }
        if (i.s1_u_sgw_fteid.first) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s1_u_sgw_fteid.second));
          gie->add_ie(sie);
        }
        if (i.s12_sgw_fteid.first) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s12_sgw_fteid.second));
          sie.get()->tlv.set_instance(1);
          gie->add_ie(sie);
        }
        if (i.s4_u_sgw_fteid.first) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s4_u_sgw_fteid.second));
          sie.get()->tlv.set_instance(2);
          gie->add_ie(sie);
        }
        if (i.s11_u_sgw_fteid.first) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s11_u_sgw_fteid.second));
          sie.get()->tlv.set_instance(3);
          gie->add_ie(sie);
        }
        if (i.charging_id.first) {
          std::shared_ptr<gtpv2c_charging_id_ie> sie(
              new gtpv2c_charging_id_ie(i.charging_id.second));
          gie->add_ie(sie);
        }
        if (i.bearer_flags.first) {
          std::shared_ptr<gtpv2c_bearer_flags_ie> sie(
              new gtpv2c_bearer_flags_ie(i.bearer_flags.second));
          gie->add_ie(sie);
        }
        if (i.s11_u_sgw_fteid.first) {
          std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
              new gtpv2c_fully_qualified_teid_ie(i.s11_u_sgw_fteid.second));
          sie.get()->tlv.set_instance(6);
          gie->add_ie(sie);
        }
        std::shared_ptr<gtpv2c_grouped_ie> sie(gie);
        add_ie(sie);
      }
    }
  }
  if (gtp_ies.bearer_contexts_marked_for_removal.first) {
    if (gtp_ies.bearer_contexts_marked_for_removal.second.size() > 0) {
      for (auto i : gtp_ies.bearer_contexts_marked_for_removal.second) {
        gtpv2c_grouped_ie* gie = new gtpv2c_grouped_ie(GTP_IE_BEARER_CONTEXT);
        gie->tlv.set_instance(1);
        if (i.eps_bearer_id.first) {
          std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
              new gtpv2c_eps_bearer_id_ie(i.eps_bearer_id.second));
          gie->add_ie(sie);
        }
        if (i.cause.first) {
          std::shared_ptr<gtpv2c_cause_ie> sie(
              new gtpv2c_cause_ie(i.cause.second));
          gie->add_ie(sie);
        }
        std::shared_ptr<gtpv2c_grouped_ie> sie(gie);
        add_ie(sie);
      }
    }
  }
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_release_access_bearers_request& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_RELEASE_ACCESS_BEARERS_REQUEST);
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_RELEASE_ACCESS_BEARERS_REQUEST_PR_IE_LIST_OF_RABS)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.uci)); add_ie(sie);}
  if (gtp_ies.originating_node.first) {
    std::shared_ptr<gtpv2c_node_type_ie> sie(
        new gtpv2c_node_type_ie(gtp_ies.originating_node.second));
    add_ie(sie);
  }
  if (gtp_ies.indication_flags.first) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags.second));
    add_ie(sie);
  }
  // if (gtp_ies.ie_presence_mask &
  // GTPV2C_RELEASE_ACCESS_BEARERS_REQUEST_PR_IE_PRIVATE_EXTENSION)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.uci)); add_ie(sie);}
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_release_access_bearers_response& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_RELEASE_ACCESS_BEARERS_RESPONSE);
  if (gtp_ies.cause.first) {
    std::shared_ptr<gtpv2c_cause_ie> sie(
        new gtpv2c_cause_ie(gtp_ies.cause.second));
    add_ie(sie);
  }
  // if (gtp_ies..first) {std::shared_ptr<xxx> sie(new xxx(gtp_ies.uci.second));
  // add_ie(sie);}
  if (gtp_ies.indication_flags.first) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags.second));
    add_ie(sie);
  }
  // if (gtp_ies..first) {std::shared_ptr<xxx> sie(new xxx(gtp_ies.uci.second));
  // add_ie(sie);} if (gtp_ies.first) {std::shared_ptr<xxx> sie(new
  // xxx(gtp_ies.uci.second)); add_ie(sie);} if (gtp_ies.first)
  // {std::shared_ptr<xxx> sie(new xxx(gtp_ies.uci.second)); add_ie(sie);}
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_downlink_data_notification& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_DOWNLINK_DATA_NOTIFICATION);

  if (gtp_ies.ie_presence_mask & DOWNLINK_DATA_NOTIFICATION_PR_IE_CAUSE) {
    std::shared_ptr<gtpv2c_cause_ie> sie(new gtpv2c_cause_ie(gtp_ies.cause));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      DOWNLINK_DATA_NOTIFICATION_PR_IE_EPS_BEARER_ID) {
    std::shared_ptr<gtpv2c_eps_bearer_id_ie> sie(
        new gtpv2c_eps_bearer_id_ie(gtp_ies.eps_bearer_id));
    add_ie(sie);
  }
  // TODO if (gtp_ies.ie_presence_mask & DOWNLINK_DATA_NOTIFICATION_PR_IE_ARP)
  // {std::shared_ptr<gtpv2c_arp_ie> sie(new gtpv2c_arp_ie(gtp_ies.arp));
  // add_ie(sie);}
  if (gtp_ies.ie_presence_mask & GTPV2C_MODIFY_BEARER_REQUEST_PR_IE_IMSI) {
    std::shared_ptr<gtpv2c_imsi_ie> sie(new gtpv2c_imsi_ie(gtp_ies.imsi));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      DOWNLINK_DATA_NOTIFICATION_PR_IE_SENDER_FTEID_FOR_CP) {
    std::shared_ptr<gtpv2c_fully_qualified_teid_ie> sie(
        new gtpv2c_fully_qualified_teid_ie(gtp_ies.sender_fteid_for_cp));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask &
      DOWNLINK_DATA_NOTIFICATION_PR_IE_INDICATION_FLAGS) {
    std::shared_ptr<gtpv2c_indication_ie> sie(
        new gtpv2c_indication_ie(gtp_ies.indication_flags));
    add_ie(sie);
  }
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(
    const gtpv2c_downlink_data_notification_acknowledge& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE);

  if (gtp_ies.ie_presence_mask & DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_CAUSE) {
    std::shared_ptr<gtpv2c_cause_ie> sie(new gtpv2c_cause_ie(gtp_ies.cause));
    add_ie(sie);
  }
  if (gtp_ies.ie_presence_mask & DOWNLINK_DATA_NOTIFICATION_ACK_PR_IE_IMSI) {
    std::shared_ptr<gtpv2c_imsi_ie> sie(new gtpv2c_imsi_ie(gtp_ies.imsi));
    add_ie(sie);
  }
}

//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_echo_request& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_ECHO_REQUEST);
  if (gtp_ies.recovery_restart_counter.first) {
    std::shared_ptr<gtpv2c_recovery_ie> sie(
        new gtpv2c_recovery_ie(gtp_ies.recovery_restart_counter.second));
    add_ie(sie);
  }
  if (gtp_ies.sending_node_features.first) {
    std::shared_ptr<gtpv2c_node_features_ie> sie(
        new gtpv2c_node_features_ie(gtp_ies.sending_node_features.second));
    add_ie(sie);
  }
}
//------------------------------------------------------------------------------
gtpv2c_msg::gtpv2c_msg(const gtpv2c_echo_response& gtp_ies)
    : gtpv2c_msg_header() {
  ies = {};
  set_message_type(GTP_ECHO_RESPONSE);
  if (gtp_ies.recovery_restart_counter.first) {
    std::shared_ptr<gtpv2c_recovery_ie> sie(
        new gtpv2c_recovery_ie(gtp_ies.recovery_restart_counter.second));
    add_ie(sie);
  }
  if (gtp_ies.sending_node_features.first) {
    std::shared_ptr<gtpv2c_node_features_ie> sie(
        new gtpv2c_node_features_ie(gtp_ies.sending_node_features.second));
    add_ie(sie);
  }
}
