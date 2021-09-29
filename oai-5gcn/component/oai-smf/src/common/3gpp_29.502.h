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

#ifndef FILE_3GPP_29_502_SMF_SEEN
#define FILE_3GPP_29_502_SMF_SEEN

enum pdu_session_application_error_e {
  PDU_SESSION_APPLICATION_ERROR_N1_SM_ERROR                            = 1,
  PDU_SESSION_APPLICATION_ERROR_SNSSAI_DENIED                          = 2,
  PDU_SESSION_APPLICATION_ERROR_DNN_DENIED                             = 3,
  PDU_SESSION_APPLICATION_ERROR_PDUTYPE_DENIED                         = 4,
  PDU_SESSION_APPLICATION_ERROR_SSC_DENIED                             = 5,
  PDU_SESSION_APPLICATION_ERROR_SUBSCRIPTION_DENIED                    = 6,
  PDU_SESSION_APPLICATION_ERROR_DNN_NOT_SUPPORTED                      = 7,
  PDU_SESSION_APPLICATION_ERROR_PDUTYPE_NOT_SUPPORTED                  = 8,
  PDU_SESSION_APPLICATION_ERROR_SSC_NOT_SUPPORTED                      = 9,
  PDU_SESSION_APPLICATION_ERROR_HOME_ROUTED_ROAMING_REQUIRED           = 10,
  PDU_SESSION_APPLICATION_ERROR_OUT_OF_LADN_SERVICE_AREA               = 11,
  PDU_SESSION_APPLICATION_ERROR_N2_SM_ERROR                            = 12,
  PDU_SESSION_APPLICATION_ERROR_PRIORITIZED_SERVICES_ONLY              = 13,
  PDU_SESSION_APPLICATION_ERROR_PDU_SESSION_ANCHOR_CHANGE              = 14,
  PDU_SESSION_APPLICATION_ERROR_TARGET_MME_CAPABILITY                  = 15,
  PDU_SESSION_APPLICATION_ERROR_NO_EPS_5GS_CONTINUITY                  = 16,
  PDU_SESSION_APPLICATION_ERROR_UNABLE_TO_PAGE_UE                      = 17,
  PDU_SESSION_APPLICATION_ERROR_UE_NOT_RESPONDING                      = 18,
  PDU_SESSION_APPLICATION_ERROR_REJECTED_BY_UE                         = 19,
  PDU_SESSION_APPLICATION_ERROR_REJECTED_DUE_VPLMN_POLICY              = 20,
  PDU_SESSION_APPLICATION_ERROR_HO_TAU_IN_PROGRESS                     = 21,
  PDU_SESSION_APPLICATION_ERROR_INTEGRITY_PROTECTED_MDR_NOT_ACCEPTABLE = 22,
  PDU_SESSION_APPLICATION_ERROR_EBI_EXHAUSTED                          = 23,
  PDU_SESSION_APPLICATION_ERROR_EBI_REJECTED_LOCAL_POLICY              = 24,
  PDU_SESSION_APPLICATION_ERROR_EBI_REJECTED_NO_N26                    = 25,
  PDU_SESSION_APPLICATION_ERROR_DEFAULT_EPS_BEARER_INACTIVE            = 26,
  PDU_SESSION_APPLICATION_ERROR_HANDOVER_RESOURCE_ALLOCATION_FAILURE   = 27,
  PDU_SESSION_APPLICATION_ERROR_CONTEXT_NOT_FOUND                      = 28,
  PDU_SESSION_APPLICATION_ERROR_INSUFFICIENT_RESOURCES_SLICE           = 29,
  PDU_SESSION_APPLICATION_ERROR_INSUFFICIENT_RESOURCES_SLICE_DNN       = 30,
  PDU_SESSION_APPLICATION_ERROR_DNN_CONGESTION                         = 31,
  PDU_SESSION_APPLICATION_ERROR_S_NSSAI_CONGESTION                     = 32,
  PDU_SESSION_APPLICATION_ERROR_PEER_NOT_RESPONDING                    = 33,
  PDU_SESSION_APPLICATION_ERROR_NETWORK_FAILURE                        = 34
};

static const std::vector<std::string> pdu_session_application_error_e2str = {
    "UNKNOWN ERROR",
    "N1_SM_ERROR",
    "SNSSAI_DENIED",
    "DNN_DENIED",
    "PDUTYPE_DENIED",
    "SSC_DENIED",
    "SUBSCRIPTION_DENIED",
    "DNN_NOT_SUPPORTED",
    "PDUTYPE_NOT_SUPPORTED",
    "SSC_NOT_SUPPORTED",
    "HOME_ROUTED_ROAMING_REQUIRED",
    "OUT_OF_LADN_SERVICE_AREA",
    "N2_SM_ERROR",
    "PRIORITIZED_SERVICES_ONLY",
    "PDU_SESSION_ANCHOR_CHANGE",
    "TARGET_MME_CAPABILITY",
    "NO_EPS_5GS_CONTINUITY",
    "UNABLE_TO_PAGE_UE",
    "UE_NOT_RESPONDING",
    "REJECTED_BY_UE",
    "REJECTED_DUE_VPLMN_POLICY",
    "HO_TAU_IN_PROGRESS",
    "INTEGRITY_PROTECTED_MDR_NOT_ACCEPTABLE",
    "EBI_EXHAUSTED",
    "EBI_REJECTED_LOCAL_POLICY",
    "EBI_REJECTED_NO_N26",
    "DEFAULT_EPS_BEARER_INACTIVE",
    "HANDOVER_RESOURCE_ALLOCATION_FAILURE",
    "CONTEXT_NOT_FOUND",
    "INSUFFICIENT_RESOURCES_SLICE",
    "INSUFFICIENT_RESOURCES_SLICE_DNN",
    "DNN_CONGESTION",
    "S_NSSAI_CONGESTION",
    "PEER_NOT_RESPONDING",
    "NETWORK_FAILURE"};

// 6.1.6.3.12 Enumeration: N2SmInfoType @3GPP TS 29.502 V16.0.0
enum class n2_sm_info_type_e {
  UNKNOWN_TYPE       = 0,
  PDU_RES_SETUP_REQ  = 1,   // PDU Session Resource Setup Request Transfer
  PDU_RES_SETUP_RSP  = 2,   // PDU Session Resource Setup Response Transfer
  PDU_RES_SETUP_FAIL = 3,   // PDU Session Resource Setup Unsuccessful Transfer
  PDU_RES_REL_CMD    = 4,   // PDU Session Resource Release Command Transfer
  PDU_RES_REL_RSP    = 5,   // PDU Session Resource Release Response Transfer
  PDU_RES_MOD_REQ    = 6,   // PDU Session Resource Modify Request Transfer
  PDU_RES_MOD_RSP    = 7,   // PDU Session Resource Modify Response Transfer
  PDU_RES_MOD_FAIL   = 8,   // PDU Session Resource Modify Unsuccessful Transfer
  PDU_RES_NTY        = 9,   // PDU Session Resource Notify Transfer
  PDU_RES_NTY_REL    = 10,  // PDU Session Resource Notify Released Transfer
  PDU_RES_MOD_IND    = 11,  // PDU Session Resource Modify Indication Transfer
  PDU_RES_MOD_CFM    = 12,  // PDU Session Resource Modify Confirm Transfer
  PATH_SWITCH_REQ    = 13,  // Path Switch Request Transfer
  PATH_SWITCH_SETUP_FAIL = 14,  // Path Switch Request Setup Failed Transfer
  PATH_SWITCH_REQ_ACK    = 15,  // Path Switch Request Acknowledge Transfer
  PATH_SWITCH_REQ_FAIL   = 16,  // Path Switch Request Unsuccessful Transfer
  HANDOVER_REQUIRED      = 17,  // Handover Required Transfer
  HANDOVER_CMD           = 18,  // Handover Command Transfer
  HANDOVER_PREP_FAIL     = 19,  // Handover Preparation Unsuccessful Transfer
  HANDOVER_REQ_ACK       = 20,  // Handover Request Acknowledge Transfer
  HANDOVER_RES_ALLOC_FAIL =
      21,  // Handover Resource Allocation Unsuccessful Transfer
  SECONDARY_RAT_USAGE = 22  // Secondary RAT Data Usage Report Transfer
};

static const std::vector<std::string> n2_sm_info_type_e2str = {
    "UNKNOWN_TYPE",           "PDU_RES_SETUP_REQ",
    "PDU_RES_SETUP_RSP",      "PDU_RES_SETUP_FAIL",
    "PDU_RES_REL_CMD",        "PDU_RES_REL_RSP",
    "PDU_RES_MOD_REQ",        "PDU_RES_MOD_RSP",
    "PDU_RES_MOD_FAIL",       "PDU_RES_NTY",
    "PDU_RES_NTY_REL",        "PDU_RES_MOD_IND",
    "PDU_RES_MOD_CFM",        "PATH_SWITCH_REQ",
    "PATH_SWITCH_SETUP_FAIL", "PATH_SWITCH_REQ_ACK",
    "PATH_SWITCH_REQ_FAIL",   "HANDOVER_REQUIRED",
    "HANDOVER_CMD",           "HANDOVER_PREP_FAIL",
    "HANDOVER_REQ_ACK",       "HANDOVER_RES_ALLOC_FAIL",
    "SECONDARY_RAT_USAGE"};

enum class upCnx_state_e {
  UPCNX_STATE_ACTIVATED   = 0,
  UPCNX_STATE_DEACTIVATED = 1,
  UPCNX_STATE_ACTIVATING  = 2
};

static const std::vector<std::string> upCnx_state_e2str = {
    "UPCNX_STATE_ACTIVATED", "UPCNX_STATE_DEACTIVATED",
    "UPCNX_STATE_ACTIVATING"};

#endif
