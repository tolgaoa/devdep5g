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

#include <stdlib.h>
#include <iostream>

#include "epc.h"
#include "conversions.h"

using namespace EPC;

std::string Utility::home_network(const char* mnc, const char* mcc) {
  std::string s;

  // '^mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.APPEND_MNC(mnc).APPEND_MCC(mcc).APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::home_network(const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return home_network(mnc, mcc);
}

std::string Utility::home_network_gprs(const char* mnc, const char* mcc) {
  std::string s;

  // '^mnc(\d{3})\.mcc(\d{3})\.gprs'
  s.APPEND_MNC(mnc).APPEND_MCC(mcc).append("gprs");

  return s;
}

std::string Utility::home_network_gprs(const plmn_t& plmn) {
  // '.mnc(\d{3})\.mcc(\d{3})\.gprs'
  std::string s;

  uint16_t mcc     = 0;
  uint16_t mnc     = 0;
  uint16_t mnc_len = 0;

  mcc     = plmn.mcc_digit1 * 100 + plmn.mcc_digit2 * 10 + plmn.mcc_digit3;
  mnc_len = (plmn.mnc_digit3 == 0x0 ? 2 : 3);
  mnc     = plmn.mnc_digit1 * 10 + plmn.mnc_digit2;
  mnc     = (mnc_len == 2 ? mnc : mnc * 10 + plmn.mnc_digit3);

  s.append(".mnc");
  if (mnc_len == 2) s.append("0");
  s.append(std::to_string(mnc));
  s.append(".mcc");
  s.append(std::to_string(mcc));
  s.append(".gprs");

  return s;
}

std::string Utility::home_network_gprs(const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return home_network_gprs(mnc, mcc);
}

std::string Utility::tai_fqdn(
    const char* lb, const char* hb, const char* mnc, const char* mcc) {
  std::string s;

  // '^tac-lb([0-9a-fA-F]{2})\.tac-hb([0-9a-fA-F]{2})\.tac\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.append("tac-lb")
      .append(lb)
      .append(".tac-hb")
      .append(hb)
      .append(".tac.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::tai_fqdn(
    const char* lb, const char* hb, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return tai_fqdn(lb, hb, mnc, mcc);
}

std::string Utility::mme_fqdn(
    const char* mmec, const char* mmegi, const char* mnc, const char* mcc) {
  std::string s;

  // '^mmec([0-9a-fA-F]{2})\.mmegi([0-9a-fA-F]{4})\.mme.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.append("mmec")
      .append(mmec)
      .append(".mmegi")
      .append(mmegi)
      .append(".mme.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::mme_fqdn(
    const char* mmec, const char* mmegi, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return mme_fqdn(mmec, mmegi, mnc, mcc);
}

std::string Utility::mme_pool_fqdn(
    const char* mmegi, const char* mnc, const char* mcc) {
  std::string s;

  // '^mmegi([0-9a-fA-F]{4})\.mme\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.append("mmegi")
      .append(mmegi)
      .append(".mme.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::mme_pool_fqdn(
    const char* mmegi, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return mme_pool_fqdn(mmegi, mnc, mcc);
}

std::string Utility::rai_fqdn(
    const char* rac, const char* lac, const char* mnc, const char* mcc) {
  std::string s;

  // '^rac([0-9a-fA-F]{4})\.lac([0-9a-fA-F]{4})\.rac\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.append("rac")
      .append(rac)
      .append(".lac")
      .append(lac)
      .append(".rac.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::rai_fqdn(
    const char* rac, const char* lac, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return rai_fqdn(rac, lac, mnc, mcc);
}

std::string Utility::rnc_fqdn(
    const char* rnc, const char* mnc, const char* mcc) {
  std::string s;

  // '^rnc([0-9a-fA-F]{4})\.rnc\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.append("rnc")
      .append(rnc)
      .append(".rnc.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::rnc_fqdn(const char* rnc, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return rnc_fqdn(rnc, mnc, mcc);
}

std::string Utility::sgsn_fqdn(
    const char* nri, const char* rac, const char* lac, const char* mnc,
    const char* mcc) {
  std::string s;

  // '^nri([0-9a-fA-F]{4})\.rac([0-9a-fA-F]{4})\.lac([0-9a-fA-F]{4})\.rac\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.append("nri")
      .append(nri)
      .append(".rac")
      .append(rac)
      .append(".lac")
      .append(lac)
      .append(".rac.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::sgsn_fqdn(
    const char* nri, const char* rac, const char* lac,
    const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return sgsn_fqdn(nri, rac, lac, mnc, mcc);
}

std::string Utility::epc_nodes_domain_fqdn(const char* mnc, const char* mcc) {
  std::string s;

  // '^node\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.append("node.epc.").APPEND_MNC(mnc).APPEND_MCC(mcc).APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::epc_nodes_domain_fqdn(const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return epc_nodes_domain_fqdn(mnc, mcc);
}

std::string Utility::epc_node_fqdn(
    const char* node, const char* mnc, const char* mcc) {
  std::string s;

  // '^(.+)\.node\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org$'
  s.append(node)
      .append(".node.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::epc_node_fqdn(
    const char* node, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return epc_node_fqdn(node, mnc, mcc);
}

std::string Utility::nonemergency_epdg_oi_fqdn(
    const char* mnc, const char* mcc) {
  std::string s;

  // '^epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
  s.append("epdg.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .append("pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::nonemergency_epdg_oi_fqdn(const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return nonemergency_epdg_oi_fqdn(mnc, mcc);
}

std::string Utility::nonemergency_epdg_tai_fqdn(
    const char* lb, const char* hb, const char* mnc, const char* mcc) {
  std::string s;

  // '^tac-lb([0-9a-fA-F]{2})\.tac-hb([0-9a-fA-F]{2})\.tac\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
  s.append("tac-lb")
      .append(lb)
      .append(".tac-hb")
      .append(hb)
      .append(".tac.epdg.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::nonemergency_epdg_tai_fqdn(
    const char* lb, const char* hb, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return nonemergency_epdg_tai_fqdn(lb, hb, mnc, mcc);
}

std::string Utility::nonemergency_epdg_lac_fqdn(
    const char* lac, const char* mnc, const char* mcc) {
  std::string s;

  // '^lac([0-9a-fA-F]{4})\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
  s.append("lac")
      .append(lac)
      .append(".epdg.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .append("pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::nonemergency_epdg_lac_fqdn(
    const char* lac, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return nonemergency_epdg_lac_fqdn(lac, mnc, mcc);
}

std::string Utility::nonemergency_epdg_visitedcountry_fqdn(const char* mcc) {
  std::string s;

  // '^epdg\.epc\.mcc(\d{3})\.visited-country\.pub\.3gppnetworks\.org'
  s.append("epdg.epc.")
      .APPEND_MCC(mcc)
      .append("visited-country.pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::nonemergency_epdg_visitedcountry_fqdn(
    const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return nonemergency_epdg_visitedcountry_fqdn(mcc);
}

std::string Utility::emergency_epdg_oi_fqdn(const char* mnc, const char* mcc) {
  std::string s;

  // '^sos\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
  s.append("sos.epdg.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .append("pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::emergency_epdg_oi_fqdn(const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return emergency_epdg_oi_fqdn(mnc, mcc);
}

std::string Utility::emergency_epdg_tai_fqdn(
    const char* lb, const char* hb, const char* mnc, const char* mcc) {
  std::string s;

  // '^tac-lb([0-9a-fA-F]{2})\.tac-hb([0-9a-fA-F]{2})\.tac\.sos\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org',
  s.append("tac-lb")
      .append(lb)
      .append(".tac-hb")
      .append(hb)
      .append(".tac.sos.epdg.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .append("pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::emergency_epdg_tai_fqdn(
    const char* lb, const char* hb, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return emergency_epdg_tai_fqdn(lb, hb, mnc, mcc);
}

std::string Utility::emergency_epdg_lac_fqdn(
    const char* lac, const char* mnc, const char* mcc) {
  std::string s;

  // '^lac([0-9a-fA-F]{4})\.sos\.epdg\.epc\.mnc(\d{3})\.mcc(\d{3})\.pub\.3gppnetworks\.org'
  s.append("lac")
      .append(lac)
      .append(".sos.epdg.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .append("pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::emergency_epdg_lac_fqdn(
    const char* lac, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return emergency_epdg_lac_fqdn(lac, mnc, mcc);
}

std::string Utility::emergency_epdg_visitedcountry_fqdn(const char* mcc) {
  std::string s;

  // '^sos\.epdg\.epc\.mcc(\d{3})\.visited-country\.pub\.3gppnetworks\.org'
  s.append("sos.epdg.epc.")
      .APPEND_MCC(mcc)
      .append("visited-country.pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::emergency_epdg_visitedcountry_fqdn(
    const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return emergency_epdg_visitedcountry_fqdn(mcc);
}

std::string Utility::global_enodeb_id_fqdn(const char* enb, const char* mcc) {
  std::string s;

  // '^enb([0-9a-fA-F]{4})\.enb\.epc\.mcc(\d{3})\.visited-country\.pub\.3gppnetworks\.org'
  s.append("enb")
      .append(enb)
      .append(".enb.epc.")
      .APPEND_MCC(mcc)
      .append("visited-country.pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::global_enodeb_id_fqdn(
    const char* enb, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return global_enodeb_id_fqdn(enb, mcc);
}

std::string Utility::local_homenetwork_fqdn(const char* lhn, const char* mcc) {
  std::string s;

  // '^lhn(.+)\.lhn\.epc\.mcc(\d{3})\.visited-country\.pub\.3gppnetworks\.org$'
  s.append("lhn")
      .append(lhn)
      .append(".lhn.epc.")
      .APPEND_MCC(mcc)
      .append("visited-country.pub.")
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::local_homenetwork_fqdn(
    const char* lhn, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return local_homenetwork_fqdn(lhn, mcc);
}

std::string Utility::epc(const char* mnc, const char* mcc) {
  std::string s;

  // '^epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org'
  s.append("epc.").APPEND_MNC(mnc).APPEND_MCC(mcc).APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::epc(const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return epc(mnc, mcc);
}

std::string Utility::dnn_fqdn(
    const char* apnoi, const char* mnc, const char* mcc) {
  std::string s;

  // '(.+)\.dnn\.epc\.mnc(\d{3})\.mcc(\d{3})\.3gppnetworks\.org$'
  s.append(apnoi)
      .append(".dnn.epc.")
      .APPEND_MNC(mnc)
      .APPEND_MCC(mcc)
      .APPEND_3GPPNETWORK;

  return s;
}

std::string Utility::dnn_fqdn(const char* apnoi, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return dnn_fqdn(apnoi, mnc, mcc);
}

std::string Utility::dnn(const char* apnoi, const char* mnc, const char* mcc) {
  std::string s;

  // '(.+)\.dnn\.mnc(\d{3})\.mcc(\d{3})\.gprs$'}
  s.append(apnoi).append(".dnn.").APPEND_MNC(mnc).APPEND_MCC(mcc).append(
      "gprs");

  return s;
}

std::string Utility::dnn(const char* apnoi, const unsigned char* plmnid) {
  PARSE_PLMNID(plmnid);
  return dnn(apnoi, mnc, mcc);
}

std::string Utility::dnn_label(const std::string& dnn) {
  std::string dnn_label = {};
  bool to_count         = true;
  uint8_t counted       = 0;
  int index             = 0;

  dnn_label.push_back('?');

  for (int i = 0; i < dnn.length(); ++i) {
    if (isalnum(dnn[i]) || (dnn[i] == '-')) {
      dnn_label.push_back(dnn[i]);
      counted++;
    } else if (dnn[i] == '.') {
      dnn_label.push_back('?');
      if (to_count) {  // always true
        dnn_label[index] = counted;
      }
      to_count = true;
      counted  = 0;
      index    = dnn_label.length() - 1;
    }
  }
  if (to_count) {
    dnn_label[index] = counted;
  }
  return dnn_label;
}
