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

/*
 *  This file contains NAS header bits format
 *  Refer TS24.007 TS24.501
 *  Author: Puzyu Dukl (BUPT), Tien-Thinh NGUYEN (EURECOM)
 *  Time:
 *  Email:
 */

#ifndef FILE_3GPP_24_501_H_SEEN
#define FILE_3GPP_24_501_H_SEEN
#ifdef __cplusplus
extern "C" {
#endif

// 9.3.1 Security header type
#define SECURITY_HEADER_TYPE_NOT_PROTECTED 0b0000
#define SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED 0b0001
#define SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED 0b0010
#define SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_NEW 0b0011
#define SECURITY_HEADER_TYPE_INTEGRITY_PROTECTED_CYPHERED_NEW 0b0100

/*Extended Protocol Discriminator (EPD)*/

// 8 bits
// big endian
#define _5GSSessionManagementMessages 0b00101110
#define _5GSMobilityManagementMessages 0b01111110

/* Security Header Type*/
// 4 bits
#define Plain5GSNASMessage 0b0000
#define IntegrityProtected 0b0001
#define IntegrityProtectedAndCiphered 0b0010
#define IntegrityProtectedWithNew5GNASSecurityContext 0b0011
#define IntegrityProtectedAndCipheredWithNew5GNASSecurityContext 0b0100

#define SpareHalfOctet 0b0000

/* Message Type for Mobility Management */

/* 5GS Mobility Management Messages */
#define REGISTRATION_REQUEST 0b01000001
#define REGISTRATION_ACCEPT 0b01000010
#define REGISTRATION_COMPLETE 0b01000011
#define REGISTRATION_REJECT 0b01000100
#define DEREGISTRATION_REQUEST_UE_ORIGINATING 0b01000101
#define DEREGISTRATION_ACCEPT_UE_ORIGINATING 0b01000110
#define DEREGISTRATION_REQUEST_UE_TERMINATED 0b01000111
#define DEREGISTRATION_ACCEPT_UE_TERMINATED 0b01001000

#define SERVICE_REQUEST 0b01001100
#define SERVICE_REJECT 0b01001101
#define SERVICE_ACCEPT 0b01001110

#define CONFIGURATION_UPDATE_COMMAND 0b01010100
#define CONFIGURATION_UPDATE_COMPLETE 0b01010101
#define AUTHENTICATION_REQUEST 0b01010110
#define AUTHENTICATION_RESPONSE 0b01010111
#define AUTHENTICATION_REJECT 0b01011000
#define AUTHENTICATION_FAILURE 0b01011001
#define AUTHENTICATION_RESULT 0b01011010
#define IDENTITY_REQUEST 0b01011011
#define IDENTITY_RESPONSE 0b01011100
#define SECURITY_MODE_COMMAND 0b01011101
#define SECURITY_MODE_COMPLETE 0b01011110
#define SECURITY_MODE_REJECT 0b01011111

#define _5GMM_STATUS 0b01100100
#define NOTIFICATION 0b01100101
#define NOTIFICATION_RESPONSE 0b01100110
#define ULNAS_TRANSPORT 0b01100111
#define DLNAS_TRANSPORT 0b01101000

/* Message Type for Session Management */

// 5GS Session Management Messages
// TODO should be updated with enum
#define PDU_SESSION_MESSAGE_TYPE_UNKNOWN 0b00000000
#define PDU_SESSION_ESTABLISHMENT_REQUEST 0b11000001
#define PDU_SESSION_ESTABLISHMENT_ACCEPT 0b11000010
#define PDU_SESSION_ESTABLISHMENT_REJECT 0b11000011

#define PDU_SESSION_AUTHENTICATION_COMMAND 0b11000101
#define PDU_SESSION_AUTHENTICATION_COMPLETE 0b11000110
#define PDU_SESSION_AUTHENTICATION_RESULT 0b11000111

#define PDU_SESSION_MODIFICATION_REQUEST 0b11001001
#define PDU_SESSION_MODIFICATION_REJECT 0b11001010
#define PDU_SESSION_MODIFICATION_COMMAND 0b11001011
#define PDU_SESSION_MODIFICATION_COMPLETE 0b11001100
#define PDU_SESSION_MODIFICATION_COMMAND_REJECT 0b11001101

#define PDU_SESSION_RELEASE_REQUEST 0b11010001
#define PDU_SESSION_RELEASE_REJECT 0b11010010
#define PDU_SESSION_RELEASE_COMMAND 0b11010011
#define PDU_SESSION_RELEASE_COMPLETE 0b11010100

#define _5GSM_STATUS 0b11010110

// 9.11.3.47 Request type
// typedef std::string request_type_t;

enum request_type_e {
  INITIAL_REQUEST                = 1,
  EXISTING_PDU_SESSION           = 2,
  INITIAL_EMERGENCY_REQUEST      = 3,
  EXISTING_EMERGENCY_PDU_SESSION = 4,
  MODIFICATION_REQUEST           = 5,
  MA_PDU_REQUEST                 = 6,
  REQUEST_TYPE_RESERVED          = 7
};

/*
 * Message Authentication Code
 * The message authentication code (MAC) information element contains
 * the integrity protection information for the message.
 */

/*---------------------------------------------------------------------------------------------*/

/*
 * Plain 5GS NAS Message
 *
 * This IE includes a complete plain 5GS NAS message as specified
 * in subclauses 8.2 and 8.3. The SECURITY PROTECTED 5GS NAS MESSAGE message
 * (see subclause 8.2.28) is not plain 5GS NAS messages and shall not be
 * included in this IE.
 */

/*---------------------------------------------------------------------------------------------*/

/*
 *
 * Sequence Number
 *
 * This IE includes the NAS message sequence number (SN)
 * which consists of the eight least significant bits of
 * the NAS COUNT for a SECURITY PROTECTED 5GS NAS MESSAGE message.
 * The usage of SN is specified in subclause 4.4.3.
 *
 */

/*---------------------------------------------------------------------------------------------*/

/*
 * Other information elements
 *
 */

/*---------------------------------------------------------------------------------------------*/

/*
 * 5GMM cause types
 */

#define IIEGAL_UE 0b00000011
#define PEI_NOT_ACCEPTED 0b00000101
#define IIEGAL_ME 0b00000110
#define FGS_SERVICES_NOT_ALLOWED 0b00000111
#define IMPLICITLY_DE_REGISTERED 0b00001010
#define PLMN_NOT_ALLOWED 0b00001011
#define TRACKING_AREA_NOT_ALLOWED 0b00001100
#define ROAMING_NOT_ALLOWED_IN_THIS_TA 0b00001101

/*********************************************************************************************/
/*
 *  5GS mobile identity information element
 *  Type of Identity*/
#define NO_IDENTITY 0b000
#define SUCI 0b001
//#define _5G_GUTI    0b110
#define IMEI 0b011
#define _5G_S_TMSI 0b100
#define IMEISVI 0b101

#define EVEN_IENTITY 0
#define ODD_IDENTITY 1

#ifdef __cplusplus
}

static const std::vector<std::string> request_type_e2str = {
    "ERROR",
    "INITIAL REQUEST",
    "EXISTING_PDU_SESSION",
    "INITIAL_EMERGENCY_REQUEST",
    "EXISTING_EMERGENCY_PDU_SESSION",
    "MODIFICATION_REQUEST",
    "MA_PDU_REQUEST",
    "RESERVED"};

enum class cause_value_5gsm_e {
  CAUSE_0_UNKNOWN                                            = 0,
  CAUSE_8_OPERATOR_DETERMINED_BARRING                        = 8,
  CAUSE_26_INSUFFICIENT_RESOURCES                            = 26,
  CAUSE_27_MISSING_OR_UNKNOWN_DNN                            = 27,
  CAUSE_28_UNKNOWN_PDU_SESSION_TYPE                          = 28,
  CAUSE_29_USER_AUTHENTICATION_OR_AUTHORIZATION_FAILED       = 29,
  CAUSE_31_REQUEST_REJECTED_UNSPECIFIED                      = 31,
  CAUSE_32_SERVICE_OPTION_NOT_SUPPORTED                      = 32,
  CAUSE_33_REQUESTED_SERVICE_OPTION_NOT_SUBSCRIBED           = 33,
  CAUSE_35_PTI_ALREADY_IN_USE                                = 35,
  CAUSE_36_REGULAR_DEACTIVATION                              = 36,
  CAUSE_38_NETWORK_FAILURE                                   = 38,
  CAUSE_39_REACTIVATION_REQUESTED                            = 39,
  CAUSE_41_SEMATIC_ERROR_IN_THE_TFT_OPERATION                = 41,
  CAUSE_42_SYNTACTICAL_ERROR_IN_THE_TFT_OPERATIOIN           = 42,
  CAUSE_43_INVALID_PDU_SESSION_IDENTITY                      = 43,
  CAUSE_44_SEMANTIC_ERRORS_IN_PACKET_FILTERS                 = 44,
  CAUSE_45_SYNTACTICAL_ERROR_IN_PACKET_FILTERS               = 45,
  CAUSE_46_OUT_OF_LAND_SERVICE_AREA                          = 46,
  CAUSE_47_PTI_MISMATCH                                      = 47,
  CAUSE_50_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED                = 50,
  CAUSE_51_PDU_SESSION_TYPE_IPV6_ONLY_ALLOWED                = 51,
  CAUSE_54_PDU_SESSION_DOES_NOT_EXIST                        = 54,
  CAUSE_67_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE_AND_DNN = 67,
  CAUSE_68_NOT_SUPPORTED_SSC_MODE                            = 68,
  CAUSE_69_INSUFFICIENT_RESOURCES_FOR_SPECIFIC_SLICE         = 69,
  CAUSE_70_MISSING_OR_UNKNOWN_DNN_IN_A_SLICE                 = 70,
  CAUSE_81_INVALID_PTI_VALUE                                 = 81,
  CAUSE_82_MAXIMUM_DATA_RATE_PER_UE_FOR_USER_PLANE_INTEGRITY_PROTECTION_IS_TOO_LOW =
      82,
  CAUSE_83_SEMANTIC_ERROR_IN_THE_QOS_OPERATION    = 83,
  CAUSE_84_SYNTACTICAL_ERROR_IN_THE_QOS_OPERATION = 84,
  CAUSE_85_INVALID_MAPPED_EPS_BEARER_IDENTITY     = 85,
  // Protocol errors
  CAUSE_95_SEMANTICALLY_INCORRECT_MESSAGE                      = 95,
  CAUSE_96_INVALID_MANDATORY_INFORMATION                       = 96,
  CAUSE_97_MESSAGE_TYPE_NON_EXISTENT_OR_NOTIMPLEMENTED         = 97,
  CAUSE_98_MESSAGE_TYPE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE     = 98,
  CAUSE_99_INFORMATION_ELEMENT_NON_EXISTENT_OR_NOT_IMPLEMENTED = 99,
  CAUSE_100_CONDITIONAL_IE_ERROR                               = 100,
  CAUSE_101_MESSAGE_NOT_COMPATIBLE_WITH_PROTOCOL_STATE         = 101,
  CAUSE_111_PROTOCOL_ERROR_UNSPECIFIED                         = 111,
  CAUSE_255_REQUEST_ACCEPTED                                   = 255
};

// The 5GSM sublayer states for PDU session handling in the network
// Section 6.1.3.3 @3GPP TS 24.501 V16.1.0
enum class pdu_session_status_e {
  PDU_SESSION_INACTIVE              = 0,
  PDU_SESSION_INACTIVE_PENDING      = 1,
  PDU_SESSION_MODIFICATION_PENDING  = 2,
  PDU_SESSION_ESTABLISHMENT_PENDING = 3,
  PDU_SESSION_ACTIVE                = 4
};

static const std::vector<std::string> pdu_session_status_e2str = {
    "PDU_SESSION_INACTIVE", "PDU_SESSION_INACTIVE_PENDING",
    "PDU_SESSION_MODIFICATION_PENDING", "PDU_SESSION_ESTABLISHMENT_PENDING",
    "PDU_SESSION_ACTIVE"};

// see Table 9.11.4.12.1: QoS flow descriptions information element
typedef struct flow_bit_rate_type_s {
  uint8_t unit;
  uint16_t value;
} flow_bit_rate_type_t;

// Guaranteed Flow Bit Rate
typedef struct gfbr_s {
  flow_bit_rate_type_t uplink;
  flow_bit_rate_type_t donwlink;
} gfbr_t;

// Maximum Flow Bit Rate
typedef struct mfbr_s {
  flow_bit_rate_type_t uplink;
  flow_bit_rate_type_t donwlink;
} mfbr_t;

// Notification control
enum notification_control_e { REQUESTED = 1, NOT_REQUESTED = 2 };

static const std::vector<std::string> notification_control_e2str = {
    "ERROR", "REQUESTED", "NOT_REQUESTED"};

// PDU Session Type value
enum pdu_session_type_e {
  PDU_SESSION_TYPE_E_UNKNOWN      = 0,
  PDU_SESSION_TYPE_E_IPV4         = 1,
  PDU_SESSION_TYPE_E_IPV6         = 2,
  PDU_SESSION_TYPE_E_IPV4V6       = 3,
  PDU_SESSION_TYPE_E_UNSTRUCTURED = 4,
  PDU_SESSION_TYPE_E_ETHERNET     = 5,
  PDU_SESSION_TYPE_E_RESERVED     = 7,
};

static const std::vector<std::string> pdu_session_type_e2str = {
    "Error",        "IPV4",     "IPV6",   "IPV4V6",
    "UNSTRUCTURED", "ETHERNET", "IPV4V6", "RESERVED"};

typedef struct pdu_session_type_s {
  uint8_t pdu_session_type;
  pdu_session_type_s() : pdu_session_type(PDU_SESSION_TYPE_E_IPV4) {}
  pdu_session_type_s(const uint8_t& p) : pdu_session_type(p) {}
  pdu_session_type_s(const struct pdu_session_type_s& p)
      : pdu_session_type(p.pdu_session_type) {}
  bool operator==(const struct pdu_session_type_s& p) const {
    return (p.pdu_session_type == pdu_session_type);
  }
  //------------------------------------------------------------------------------
  bool operator==(const pdu_session_type_e& p) const {
    return (p == pdu_session_type);
  }
  //------------------------------------------------------------------------------
  const std::string& toString() const {
    return pdu_session_type_e2str.at(pdu_session_type);
  }
} pdu_session_type_t;

//-------------------------------------
// 8.14 PDU Session (UE IP) Address Allocation (PAA)
struct paa_s {
  pdu_session_type_t pdu_session_type;
  uint8_t ipv6_prefix_length;
  struct in6_addr ipv6_address;
  struct in_addr ipv4_address;
  //------------------------------------------------------------------------------
  bool is_ip_assigned() {
    switch (pdu_session_type.pdu_session_type) {
      case PDU_SESSION_TYPE_E_IPV4:
        if (ipv4_address.s_addr) return true;
        return false;
        break;
      case PDU_SESSION_TYPE_E_IPV6:
        if (ipv6_address.s6_addr32[0] | ipv6_address.s6_addr32[1] |
            ipv6_address.s6_addr32[2] | ipv6_address.s6_addr32[3])
          return true;
        return false;
        break;
      case PDU_SESSION_TYPE_E_IPV4V6:
        // TODO
        if (ipv4_address.s_addr) return true;
        if (ipv6_address.s6_addr32[0] | ipv6_address.s6_addr32[1] |
            ipv6_address.s6_addr32[2] | ipv6_address.s6_addr32[3])
          return true;
        return false;
        break;
      case PDU_SESSION_TYPE_E_UNSTRUCTURED:
      case PDU_SESSION_TYPE_E_ETHERNET:
      case PDU_SESSION_TYPE_E_RESERVED:
      default:
        return false;
    }
  }
};

typedef struct paa_s paa_t;
#endif

#endif
