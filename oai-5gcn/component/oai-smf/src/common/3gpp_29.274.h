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

/*! \file 3gpp_29.274.h
 \brief
 \author Lionel Gauthier
 \company Eurecom
 \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_3GPP_29_274_SEEN
#define FILE_3GPP_29_274_SEEN
#include "common_root_types.h"
#include "3gpp_24.008.h"
#include "conversions.hpp"
#include "logger.hpp"  // for fmt::format in spdlog

#include <arpa/inet.h>
#include <stdint.h>
#include <string>

//------------------------------------------------------------------------------
// 8.3 International Mobile Subscriber Identity (IMSI)
struct imsi_s {
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
      uint8_t filler : 4;
    } digits;
#define IMSI_BCD8_SIZE 8
    uint8_t b[IMSI_BCD8_SIZE];
  } u1;
  uint num_digits;

  imsi_s() : num_digits(0) { memset(u1.b, 0, sizeof(u1.b)); }

  imsi_s(const imsi_s& i) : num_digits(i.num_digits) {
    memcpy(u1.b, i.u1.b, sizeof(u1.b));
  }

  std::string toString() const {
    std::string s = {};
    int l_i       = 0;
    int l_j       = 0;
    while (l_i < IMSI_BCD8_SIZE) {
      if ((u1.b[l_i] & 0xf) > 9) break;
      s.append(std::to_string(u1.b[l_i] & 0xf));
      l_j++;
      if (((u1.b[l_i] & 0xf0) >> 4) > 9) break;
      s.append(std::to_string((u1.b[l_i] & 0xf0) >> 4));
      l_j++;
      l_i++;
    }
    return s;
  }

  //------------------------------------------------------------------------------
  imsi64_t to_imsi64() const {
    imsi64_t imsi64 = 0;
    for (int i = 0; i < IMSI_BCD8_SIZE; i++) {
      uint8_t d1 = u1.b[i];
      uint8_t d2 = (d1 & 0xf0) >> 4;
      d1         = d1 & 0x0f;
      if (10 > d1) {
        imsi64 = imsi64 * 10 + d1;
        if (10 > d2) {
          imsi64 = imsi64 * 10 + d2;
        } else {
          break;
        }
      } else {
        break;
      }
    }
    return imsi64;
  }

  imsi_s& operator++()  // prefix ++
  {
    int l_i       = IMSI_BCD8_SIZE - 1;
    uint8_t carry = 1;
    while (l_i > 5) {
      uint8_t b  = u1.b[l_i];
      uint8_t d0 = b & 0x0f;
      uint8_t d1 = b & 0xf0;
      if (d0 <= 9) {
        d0 += carry;
        if (d0 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d0        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      if (d1 <= 9) {
        d1 += carry;
        if (d1 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d1        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      l_i++;
    }
    return (*this);
  }
};
typedef struct imsi_s imsi_t;

//-------------------------------------
// 8.9 IP Address
typedef struct ip_address_s {
  bool is_ipv4;  // if not ipv4, then it is ipv6
  union {
    struct in_addr ipv4_address;
    struct in6_addr ipv6_address;
  } address;
} ip_address_t;
//-------------------------------------
// 8.10 Mobile Equipment Identity (MEI)
// The ME Identity field contains either the IMEI or the IMEISV as defined in
// subclause 6.2 of 3GPP TS 23.003
struct mei_s {
#define MEI_MIN_LENGTH (15)
#define MEI_MAX_LENGTH (16)
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
      uint8_t filler : 4;
    } digits;
    uint8_t b[MEI_MAX_LENGTH / 2];
  } u1;
  uint num_digits;

  mei_s() : num_digits(0) { memset(u1.b, 0, sizeof(u1.b)); }

  mei_s(const mei_s& i) : num_digits(i.num_digits) {
    memcpy(u1.b, i.u1.b, sizeof(u1.b));
  }

  std::string toString() const {
    std::string s = {};
    int l_i       = 0;
    int l_j       = 0;
    while (l_i < MEI_MAX_LENGTH / 2) {
      if ((u1.b[l_i] & 0xf) > 9) break;
      s.append(std::to_string(u1.b[l_i] & 0xf));
      l_j++;
      if (((u1.b[l_i] & 0xf0) >> 4) > 9) break;
      s.append(std::to_string((u1.b[l_i] & 0xf0) >> 4));
      l_j++;
      l_i++;
    }
    return s;
  }

  mei_s& operator++()  // prefix ++
  {
    int l_i =
        MEI_MAX_LENGTH / 2 - 1 - 1;  // depends if imei or imei_sv -1 again
    uint8_t carry = 1;
    while (l_i) {
      uint8_t b  = u1.b[l_i];
      uint8_t d0 = b & 0x0f;
      uint8_t d1 = b & 0xf0;
      if (d0 <= 9) {
        d0 += carry;
        if (d0 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d0        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      if (d1 <= 9) {
        d1 += carry;
        if (d1 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d1        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      l_i++;
    }
    return (*this);
  }
};

typedef struct mei_s mei_t;
//-------------------------------------
// 8.11 MSISDN
struct msisdn_s {
#define MSISDN_MAX_LENGTH (15)
  union {
    struct {
      uint8_t digit1 : 4;
      uint8_t digit2 : 4;
      uint8_t digit3 : 4;
      uint8_t digit4 : 4;
      uint8_t digit5 : 4;
      uint8_t digit6 : 4;
      uint8_t digit7 : 4;
      uint8_t digit8 : 4;
      uint8_t digit9 : 4;
      uint8_t digit10 : 4;
      uint8_t digit11 : 4;
      uint8_t digit12 : 4;
      uint8_t digit13 : 4;
      uint8_t digit14 : 4;
      uint8_t digit15 : 4;
    } digits;
    uint8_t b[MSISDN_MAX_LENGTH / 2 + 1];
  } u1;
  uint num_digits;

  msisdn_s() : num_digits(0) { memset(u1.b, 0, sizeof(u1.b)); }

  msisdn_s(const msisdn_s& i) : num_digits(i.num_digits) {
    memcpy(u1.b, i.u1.b, sizeof(u1.b));
  }

  std::string toString() const {
    std::string s = {};
    int l_i       = 0;
    int l_j       = 0;
    while (l_i < sizeof(u1.b)) {
      if ((u1.b[l_i] & 0xf) > 9) break;
      s.append(std::to_string(u1.b[l_i] & 0xf));
      l_j++;
      if (((u1.b[l_i] & 0xf0) >> 4) > 9) break;
      s.append(std::to_string((u1.b[l_i] & 0xf0) >> 4));
      l_j++;
      l_i++;
    }
    return s;
  }

  // Should be refined see spec
  msisdn_s& operator++()  // prefix ++
  {
    int l_i       = sizeof(u1.b) - 1;
    uint8_t carry = 1;
    while (l_i > 5) {
      uint8_t b  = u1.b[l_i];
      uint8_t d0 = b & 0x0f;
      uint8_t d1 = b & 0xf0;
      if (d0 <= 9) {
        d0 += carry;
        if (d0 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d0        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      if (d1 <= 9) {
        d1 += carry;
        if (d1 <= 9) {
          u1.b[l_i] = d0 | d1;
          return (*this);
        } else {
          d1        = 0;
          u1.b[l_i] = d0 | d1;
        }
      }
      l_i++;
    }
    return (*this);
  }
};

typedef struct msisdn_s msisdn_t;

//-------------------------------------
// 8.35 Procedure Transaction ID (PTI)
typedef struct procedure_transaction_id_s {
  uint8_t procedure_transaction_id;
} procedure_transaction_id_t;

//-------------------------------------------------
// 8.50 PLMN ID
typedef struct plmn_id_s {
  union {
    struct {
      uint8_t mcc_digit_2 : 4;
      uint8_t mcc_digit_1 : 4;
      uint8_t mnc_digit_1 : 4;
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_3 : 4;
      uint8_t mnc_digit_2 : 4;
    } mnc3;
    struct {
      uint8_t mcc_digit_2 : 4;
      uint8_t mcc_digit_1 : 4;
      uint8_t filler1111 : 4;
      uint8_t mcc_digit_3 : 4;
      uint8_t mnc_digit_2 : 4;
      uint8_t mnc_digit_1 : 4;
    } mnc2;
    uint8_t b;
  } u;
} plmn_id_t;

//-------------------------------------
// 8.62 Fully qualified PDN Connection Set Identifier (FQ-CSID)

enum node_id_type_e {
  GLOBAL_UNICAST_IPv4 = 0,
  GLOBAL_UNICAST_IPv6 = 1,
  TYPE_EXOTIC = 2,  ///< (MCC * 1000 + MNC) << 12 + Integer value assigned to
                    ///< MME by operator
};

// Values of Number of CSID other than 1 are only employed in the Delete PDN
// Connection Set Request.
typedef struct fq_csid_s {
  struct fq_csid_ie_hdr_t {
    uint8_t node_id_type : 4;
    uint8_t number_of_csids : 4;
    union {
      struct in_addr unicast_ipv4;
      struct in6_addr unicast_ipv6;
      struct {
        uint16_t mcc;
        uint16_t mnc;
        uint16_t operator_specific_id;
      } exotic;
    } node_id;
  } fq_csid_ie_hdr;
#define PDN_CONNECTION_SET_IDENTIFIER_MAX 15
  uint16_t pdn_connection_set_identifier[PDN_CONNECTION_SET_IDENTIFIER_MAX];
} fq_csid_t;

//-------------------------------------
// 8.65 Node Type
enum node_type_e { NODE_TYPE_E_MME = 0, NODE_TYPE_E_SGSN = 1 };

typedef struct node_type_s {
  uint8_t node_type;
} node_type_t;
//-------------------------------------
// 8.66 Fully Qualified Domain Name (FQDN)
typedef struct fully_qualified_domain_name_s {
  std::string fqdn;
} fully_qualified_domain_name_t;

typedef fully_qualified_domain_name_t fqdn_t;

//-------------------------------------
// 8.67 Private Extension
typedef struct private_extension_s {
  uint16_t enterprise_id;
  std::string proprietary_value;
} private_extension_t;

//-------------------------------------
// 8.68 Transaction Identifier (TI)
typedef struct transaction_identifier_s {
  uint8_t transaction_identifier;
} transaction_identifier_t;

//-------------------------------------
// 8.128 Extended Protocol Configuration Options (ePCO)
typedef struct extended_protocol_configuration_options_s {
  std::string extended_protocol_configuration_options;
} extended_protocol_configuration_options_t;

#endif /* FILE_3GPP_29_274_SEEN */
