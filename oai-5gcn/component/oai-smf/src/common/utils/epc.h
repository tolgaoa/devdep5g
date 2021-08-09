/*
 * Copyright (c) 2017 Sprint
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __EPC_H
#define __EPC_H

#include <stdio.h>

#include <algorithm>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include "3gpp_23.003.h"

/*
 MCC digit 1 - low order nibble octet 1
 MCC digit 2 - high order nibble octet 1
 MCC digit 3 - low order nibble octet 2

 MCC = [MCC digit 1][MCC digit 2][MCC digit 3]


 MNC digit 1 - low order nibble octet 3
 MNC digit 2 - high order nibble octet 3
 MNC digit 3 - high order nibble octet 2, nibble set to 1111 if MNC len is 2

 MNC = [MNC digit 1][MNC digit 2][MNC digit 3]
 */

#define LOW_NIBBLE(a) (((unsigned char) a) & 0x0f)
#define HIGH_NIBBLE(a) ((((unsigned char) a) & 0xf0) >> 4)

#define PARSE_MNC(a, b)                                                        \
  {                                                                            \
    if (HIGH_NIBBLE(b[1]) == 0x0f) {                                           \
      a[0] = '0';                                                              \
      a[1] = LOW_NIBBLE(b[2]) + '0';                                           \
      a[2] = HIGH_NIBBLE(b[2]) + '0';                                          \
      a[3] = '\0';                                                             \
    } else {                                                                   \
      a[0] = LOW_NIBBLE(b[2]) + '0';                                           \
      a[1] = HIGH_NIBBLE(b[2]) + '0';                                          \
      a[2] = HIGH_NIBBLE(b[1]) + '0';                                          \
      a[3] = '\0';                                                             \
    }                                                                          \
  }

#define PARSE_MCC(a, b)                                                        \
  {                                                                            \
    a[0] = LOW_NIBBLE(b[0]) + '0';                                             \
    a[1] = HIGH_NIBBLE(b[0]) + '0';                                            \
    a[2] = LOW_NIBBLE(b[1]) + '0';                                             \
    a[3] = '\0';                                                               \
  }

#define PARSE_PLMNID(a)                                                        \
  char mnc[4], mcc[4];                                                         \
  PARSE_MNC(mnc, a);                                                           \
  PARSE_MCC(mcc, a);

#define APPEND_MNC(a) append("mnc").append(a).append(".")
#define APPEND_MCC(a) append("mcc").append(a).append(".")
#define APPEND_3GPPNETWORK append("3gppnetwork.org")

namespace EPC {

class Utility {
 public:
  static std::string home_network(const char* mnc, const char* mcc);
  static std::string home_network(const unsigned char* plmnid);
  static std::string home_network_gprs(const char* mnc, const char* mcc);
  static std::string home_network_gprs(const plmn_t& plmn);
  static std::string home_network_gprs(const unsigned char* plmnid);
  static std::string tai_fqdn(
      const char* lb, const char* hb, const char* mnc, const char* mcc);
  static std::string tai_fqdn(
      const char* lb, const char* hb, const unsigned char* plmnid);
  static std::string mme_fqdn(
      const char* mmec, const char* mmegi, const char* mnc, const char* mcc);
  static std::string mme_fqdn(
      const char* mmec, const char* mmegi, const unsigned char* plmnid);
  static std::string mme_pool_fqdn(
      const char* mmegi, const char* mnc, const char* mcc);
  static std::string mme_pool_fqdn(
      const char* mmegi, const unsigned char* plmnid);
  static std::string rai_fqdn(
      const char* rac, const char* lac, const char* mnc, const char* mcc);
  static std::string rai_fqdn(
      const char* rac, const char* lac, const unsigned char* plmnid);
  static std::string rnc_fqdn(
      const char* rnc, const char* mnc, const char* mcc);
  static std::string rnc_fqdn(const char* rnc, const unsigned char* plmnid);
  static std::string sgsn_fqdn(
      const char* nri, const char* rac, const char* lac, const char* mnc,
      const char* mcc);
  static std::string sgsn_fqdn(
      const char* nri, const char* rac, const char* lac,
      const unsigned char* plmnid);
  static std::string epc_nodes_domain_fqdn(const char* mnc, const char* mcc);
  static std::string epc_nodes_domain_fqdn(const unsigned char* plmnid);
  static std::string epc_node_fqdn(
      const char* node, const char* mnc, const char* mcc);
  static std::string epc_node_fqdn(
      const char* node, const unsigned char* plmnid);
  static std::string nonemergency_epdg_oi_fqdn(
      const char* mnc, const char* mcc);
  static std::string nonemergency_epdg_oi_fqdn(const unsigned char* plmnid);
  static std::string nonemergency_epdg_tai_fqdn(
      const char* lb, const char* hb, const char* mnc, const char* mcc);
  static std::string nonemergency_epdg_tai_fqdn(
      const char* lb, const char* hb, const unsigned char* plmnid);
  static std::string nonemergency_epdg_lac_fqdn(
      const char* lac, const char* mnc, const char* mcc);
  static std::string nonemergency_epdg_lac_fqdn(
      const char* lac, const unsigned char* plmnid);
  static std::string nonemergency_epdg_visitedcountry_fqdn(const char* mcc);
  static std::string nonemergency_epdg_visitedcountry_fqdn(
      const unsigned char* plmnid);
  static std::string emergency_epdg_oi_fqdn(const char* mnc, const char* mcc);
  static std::string emergency_epdg_oi_fqdn(const unsigned char* plmnid);
  static std::string emergency_epdg_tai_fqdn(
      const char* lb, const char* hb, const char* mnc, const char* mcc);
  static std::string emergency_epdg_tai_fqdn(
      const char* lb, const char* hb, const unsigned char* plmnid);
  static std::string emergency_epdg_lac_fqdn(
      const char* lac, const char* mnc, const char* mcc);
  static std::string emergency_epdg_lac_fqdn(
      const char* lac, const unsigned char* plmnid);
  static std::string emergency_epdg_visitedcountry_fqdn(const char* mcc);
  static std::string emergency_epdg_visitedcountry_fqdn(
      const unsigned char* plmnid);
  static std::string global_enodeb_id_fqdn(const char* enb, const char* mcc);
  static std::string global_enodeb_id_fqdn(
      const char* enb, const unsigned char* plmnid);
  static std::string local_homenetwork_fqdn(const char* lhn, const char* mcc);
  static std::string local_homenetwork_fqdn(
      const char* lhn, const unsigned char* plmnid);
  static std::string epc(const char* mnc, const char* mcc);
  static std::string epc(const unsigned char* plmnid);
  static std::string dnn_fqdn(
      const char* apnoi, const char* mnc, const char* mcc);
  static std::string dnn_fqdn(const char* apnoi, const unsigned char* plmnid);
  static std::string dnn(const char* apnoi, const char* mnc, const char* mcc);
  static std::string dnn(const char* apnoi, const unsigned char* plmnid);
  static std::string dnn_label(const std::string& dnn);

 private:
  Utility() {}
};

}  // namespace EPC

#endif  // #ifndef __EPC_H
