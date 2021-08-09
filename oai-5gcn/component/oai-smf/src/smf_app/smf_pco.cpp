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

/*! \file smf_pco.cpp
 \brief
 \author Lionel Gauthier
 \company Eurecom
 \email: lionel.gauthier@eurecom.fr
 */

#include "smf_pco.hpp"

#include "3gpp_24.008.h"
#include "common_defs.h"
#include "logger.hpp"
#include "rfc_1332.h"
#include "rfc_1877.h"
#include "smf_app.hpp"
#include "smf_config.hpp"
#include "string.hpp"

using namespace smf;

extern smf_config smf_cfg;

//------------------------------------------------------------------------------
int smf_app::pco_push_protocol_or_container_id(
    protocol_configuration_options_t& pco,
    pco_protocol_or_container_id_t* const poc_id) {
  if (PCO_UNSPEC_MAXIMUM_PROTOCOL_ID_OR_CONTAINER_ID <=
      pco.num_protocol_or_container_id) {
    return RETURNerror;
  }
  pco_protocol_or_container_id_t pco_item = {};
  pco_item.protocol_id                    = poc_id->protocol_id;
  pco_item.length_of_protocol_id_contents =
      poc_id->length_of_protocol_id_contents;
  pco_item.protocol_id_contents = poc_id->protocol_id_contents;
  //  assert(pco_item.length_of_protocol_id_contents ==
  //    				pco_item.protocol_id_contents.size());
  //    pco_item.protocol_id_contents = nullptr;

  pco.protocol_or_container_ids.push_back(pco_item);
  pco.num_protocol_or_container_id += 1;
  return RETURNok;
}

//------------------------------------------------------------------------------
int smf_app::process_pco_request_ipcp(
    protocol_configuration_options_t& pco_resp,
    const pco_protocol_or_container_id_t* const poc_id) {
  in_addr_t ipcp_dns_prim_ipv4_addr          = INADDR_NONE;
  in_addr_t ipcp_dns_sec_ipv4_addr           = INADDR_NONE;
  in_addr_t ipcp_out_dns_prim_ipv4_addr      = INADDR_NONE;
  in_addr_t ipcp_out_dns_sec_ipv4_addr       = INADDR_NONE;
  pco_protocol_or_container_id_t poc_id_resp = {0};
  size_t ipcp_req_remaining_length = poc_id->length_of_protocol_id_contents;
  size_t pco_in_index              = 0;

  uint8_t ipcp_req_code          = 0;
  uint8_t ipcp_req_identifier    = 0;
  uint16_t ipcp_req_length       = 0;
  uint8_t ipcp_req_option        = 0;
  uint8_t ipcp_req_option_length = 0;

  uint8_t ipcp_out_code    = 0;
  uint16_t ipcp_out_length = 0;

  Logger::smf_app().debug(
      "PCO: Protocol identifier IPCP length %u\n",
      poc_id->length_of_protocol_id_contents);

  ipcp_req_code = poc_id->protocol_id_contents.at(pco_in_index++);
  UNUSED(ipcp_req_code);
  ipcp_req_identifier = poc_id->protocol_id_contents.at(pco_in_index++);
  ipcp_req_length =
      (((uint16_t) poc_id->protocol_id_contents.at(pco_in_index)) << 8) |
      ((uint16_t) poc_id->protocol_id_contents.at(pco_in_index + 1));
  UNUSED(ipcp_req_length);
  Logger::smf_app().trace(
      "PCO: Protocol identifier IPCP (0x%x) code 0x%x identifier 0x%x length "
      "%u",
      poc_id->protocol_id, ipcp_req_code, ipcp_req_identifier, ipcp_req_length);
  pco_in_index += 2;
  ipcp_req_remaining_length = ipcp_req_remaining_length - 1 - 1 - 2;
  ipcp_out_length           = 1 + 1 + 2;

  poc_id_resp.protocol_id = poc_id->protocol_id;
  poc_id_resp.length_of_protocol_id_contents =
      0;  // fill value after parsing req
  // 4 = length(code, identifier, length)
  poc_id_resp.protocol_id_contents.resize(
      4,
      0);  // fill values after parsing req

  ipcp_out_code = IPCP_CODE_CONFIGURE_ACK;

  while (ipcp_req_remaining_length >= 2) {
    ipcp_req_option        = poc_id->protocol_id_contents.at(pco_in_index);
    ipcp_req_option_length = poc_id->protocol_id_contents.at(pco_in_index + 1);
    ipcp_req_remaining_length =
        ipcp_req_remaining_length - ipcp_req_option_length;
    Logger::smf_app().trace(
        "PCO: Protocol identifier IPCP ipcp_option %u ipcp_option_length %u "
        "ipcp_remaining_length %lu pco_in_index %lu",
        ipcp_req_option, ipcp_req_option_length, ipcp_req_remaining_length,
        pco_in_index);

    switch (ipcp_req_option) {
      case IPCP_OPTION_PRIMARY_DNS_SERVER_IP_ADDRESS: {
        /* RFC 1877
         * This Configuration Option defines a method for negotiating with
         * the remote peer the address of the primary DNS server to be used
         * on the local end of the link. If local peer requests an invalid
         * server address (which it will typically do intentionally) the
         * remote peer specifies the address by NAKing this option, and
         * returning the IP address of a valid DNS server.
         * By default, no primary DNS address is provided.
         */
        Logger::smf_app().trace(
            "PCO: Protocol identifier IPCP option "
            "PRIMARY_DNS_SERVER_IP_ADDRESS length %u",
            ipcp_req_option_length);
        if (ipcp_req_option_length >= 6) {
          ipcp_dns_prim_ipv4_addr = htonl(
              (((uint32_t) poc_id->protocol_id_contents.at(pco_in_index + 2))
               << 24) |
              (((uint32_t) poc_id->protocol_id_contents.at(pco_in_index + 3))
               << 16) |
              (((uint32_t) poc_id->protocol_id_contents.at(pco_in_index + 4))
               << 8) |
              (((uint32_t) poc_id->protocol_id_contents.at(pco_in_index + 5))));
          Logger::smf_app().debug(
              "PCO: Protocol identifier IPCP option "
              "SECONDARY_DNS_SERVER_IP_ADDRESS ipcp_dns_prim_ipv4_addr 0x%x",
              ipcp_dns_prim_ipv4_addr);

          if (ipcp_dns_prim_ipv4_addr == INADDR_ANY) {
            ipcp_out_dns_prim_ipv4_addr = smf_cfg.default_dnsv4.s_addr;
            /* RFC 1877:
             * Primary-DNS-Address
             *  The four octet Primary-DNS-Address is the address of the primary
             *  DNS server to be used by the local peer. If all four octets are
             *  set to zero, it indicates an explicit request that the peer
             *  provide the address information in a Config-Nak packet. */
            ipcp_out_code = IPCP_CODE_CONFIGURE_NACK;
          } else if (smf_cfg.default_dnsv4.s_addr != ipcp_dns_prim_ipv4_addr) {
            ipcp_out_code               = IPCP_CODE_CONFIGURE_NACK;
            ipcp_out_dns_prim_ipv4_addr = smf_cfg.default_dnsv4.s_addr;
          } else {
            ipcp_out_dns_prim_ipv4_addr = ipcp_dns_prim_ipv4_addr;
          }

          Logger::smf_app().debug(
              "PCO: Protocol identifier IPCP option "
              "SECONDARY_DNS_SERVER_IP_ADDRESS ipcp_out_dns_prim_ipv4_addr "
              "0x%x",
              ipcp_out_dns_prim_ipv4_addr);
        }
        uint8_t idp[6] = {0};
        idp[0]         = IPCP_OPTION_PRIMARY_DNS_SERVER_IP_ADDRESS;
        idp[1]         = 6;
        idp[2]         = (uint8_t)(ipcp_out_dns_prim_ipv4_addr & 0x000000FF);
        idp[3] = (uint8_t)((ipcp_out_dns_prim_ipv4_addr >> 8) & 0x000000FF);
        idp[4] = (uint8_t)((ipcp_out_dns_prim_ipv4_addr >> 16) & 0x000000FF);
        idp[5] = (uint8_t)((ipcp_out_dns_prim_ipv4_addr >> 24) & 0x000000FF);
        ipcp_out_length += 6;
        std::string tmp_s((const char*) &idp[0], 6);
        poc_id_resp.protocol_id_contents.append(tmp_s);
      } break;

      case IPCP_OPTION_SECONDARY_DNS_SERVER_IP_ADDRESS: {
        /* RFC 1877
         * This Configuration Option defines a method for negotiating with
         * the remote peer the address of the secondary DNS server to be used
         * on the local end of the link. If local peer requests an invalid
         * server address (which it will typically do intentionally) the
         * remote peer specifies the address by NAKing this option, and
         * returning the IP address of a valid DNS server.
         * By default, no secondary DNS address is provided.
         */
        Logger::smf_app().debug(
            "PCO: Protocol identifier IPCP option "
            "SECONDARY_DNS_SERVER_IP_ADDRESS length %u",
            ipcp_req_option_length);

        if (ipcp_req_option_length >= 6) {
          ipcp_dns_sec_ipv4_addr = htonl(
              (((uint32_t) poc_id->protocol_id_contents.at(pco_in_index + 2))
               << 24) |
              (((uint32_t) poc_id->protocol_id_contents.at(pco_in_index + 3))
               << 16) |
              (((uint32_t) poc_id->protocol_id_contents.at(pco_in_index + 4))
               << 8) |
              (((uint32_t) poc_id->protocol_id_contents.at(pco_in_index + 5))));
          Logger::smf_app().debug(
              "PCO: Protocol identifier IPCP option "
              "SECONDARY_DNS_SERVER_IP_ADDRESS ipcp_dns_sec_ipv4_addr 0x%x",
              ipcp_dns_sec_ipv4_addr);

          if (ipcp_dns_sec_ipv4_addr == INADDR_ANY) {
            ipcp_out_dns_sec_ipv4_addr = smf_cfg.default_dns_secv4.s_addr;
            ipcp_out_code              = IPCP_CODE_CONFIGURE_NACK;
          } else if (
              smf_cfg.default_dns_secv4.s_addr != ipcp_dns_sec_ipv4_addr) {
            ipcp_out_code              = IPCP_CODE_CONFIGURE_NACK;
            ipcp_out_dns_sec_ipv4_addr = smf_cfg.default_dns_secv4.s_addr;
          } else {
            ipcp_out_dns_sec_ipv4_addr = ipcp_dns_sec_ipv4_addr;
          }

          Logger::smf_app().debug(
              "PCO: Protocol identifier IPCP option "
              "SECONDARY_DNS_SERVER_IP_ADDRESS ipcp_out_dns_sec_ipv4_addr 0x%x",
              ipcp_out_dns_sec_ipv4_addr);
        }
        uint8_t ids[6] = {0};
        ids[0]         = IPCP_OPTION_SECONDARY_DNS_SERVER_IP_ADDRESS;
        ids[1]         = 6;
        ids[2]         = (uint8_t)(ipcp_out_dns_sec_ipv4_addr & 0x000000FF);
        ids[3] = (uint8_t)((ipcp_out_dns_sec_ipv4_addr >> 8) & 0x000000FF);
        ids[4] = (uint8_t)((ipcp_out_dns_sec_ipv4_addr >> 16) & 0x000000FF);
        ids[5] = (uint8_t)((ipcp_out_dns_sec_ipv4_addr >> 24) & 0x000000FF);
        ipcp_out_length += 6;
        std::string tmp_s((const char*) &ids[0], 6);
        poc_id_resp.protocol_id_contents.append(tmp_s);
      } break;

      default:
        Logger::smf_app().debug(
            "PCO: Protocol identifier IPCP option 0x%04X unknown\n",
            ipcp_req_option);
    }
    pco_in_index += ipcp_req_option_length;
  }

  // finally we can fill code, length
  poc_id_resp.length_of_protocol_id_contents =
      ipcp_out_length;  // fill value after parsing req
  poc_id_resp.protocol_id_contents.at(0) = ipcp_out_code;
  poc_id_resp.protocol_id_contents.at(1) = ipcp_req_identifier;
  poc_id_resp.protocol_id_contents.at(2) = (uint8_t)(ipcp_out_length >> 8);
  poc_id_resp.protocol_id_contents.at(3) = (uint8_t)(ipcp_out_length & 0x00FF);

  return pco_push_protocol_or_container_id(pco_resp, &poc_id_resp);
}

//------------------------------------------------------------------------------
int smf_app::process_pco_dns_server_request(
    protocol_configuration_options_t& pco_resp,
    const pco_protocol_or_container_id_t* const poc_id) {
  in_addr_t ipcp_out_dns_prim_ipv4_addr      = smf_cfg.default_dnsv4.s_addr;
  pco_protocol_or_container_id_t poc_id_resp = {0};
  uint8_t dns_array[4];

  Logger::smf_app().debug(
      "PCO: Protocol identifier IPCP option DNS Server Request");
  poc_id_resp.protocol_id = PCO_CONTAINER_IDENTIFIER_DNS_SERVER_IPV4_ADDRESS;
  poc_id_resp.length_of_protocol_id_contents = 4;
  dns_array[0] = (uint8_t)(ipcp_out_dns_prim_ipv4_addr & 0x000000FF);
  dns_array[1] = (uint8_t)((ipcp_out_dns_prim_ipv4_addr >> 8) & 0x000000FF);
  dns_array[2] = (uint8_t)((ipcp_out_dns_prim_ipv4_addr >> 16) & 0x000000FF);
  dns_array[3] = (uint8_t)((ipcp_out_dns_prim_ipv4_addr >> 24) & 0x000000FF);
  std::string tmp_s((const char*) &dns_array[0], sizeof(dns_array));
  poc_id_resp.protocol_id_contents = tmp_s;

  return pco_push_protocol_or_container_id(pco_resp, &poc_id_resp);
}

//------------------------------------------------------------------------------
int smf_app::process_pco_dns_server_v6_request(
    protocol_configuration_options_t& pco_resp,
    const pco_protocol_or_container_id_t* const poc_id) {
  in6_addr ipcp_out_dns_prim_ipv6_addr       = smf_cfg.default_dnsv6;
  pco_protocol_or_container_id_t poc_id_resp = {0};
  uint8_t dnsv6_array[16];

  Logger::smf_app().debug(
      "PCO: Protocol identifier IPCP option DNS Server v6 Request");
  poc_id_resp.protocol_id = PCO_CONTAINER_IDENTIFIER_DNS_SERVER_IPV6_ADDRESS;
  poc_id_resp.length_of_protocol_id_contents = 16;

  char str_addr6[INET6_ADDRSTRLEN];
  if (inet_ntop(
          AF_INET6, &ipcp_out_dns_prim_ipv6_addr, str_addr6,
          sizeof(str_addr6))) {
    std::string ipv6_addr_str((char*) str_addr6, INET6_ADDRSTRLEN);
    // Logger::smf_app().info(" Ipv6 address....: %s", ipv6_addr_str.c_str());
    unsigned char buf_in6_addr[sizeof(struct in6_addr)];
    if (inet_pton(AF_INET6, util::trim(ipv6_addr_str).c_str(), buf_in6_addr) ==
        1) {
      for (int i = 0; i <= 15; i++) dnsv6_array[i] = (uint8_t)(buf_in6_addr[i]);
    }
  }

  std::string tmp_s((const char*) &dnsv6_array[0], sizeof(dnsv6_array));
  poc_id_resp.protocol_id_contents = tmp_s;

  return pco_push_protocol_or_container_id(pco_resp, &poc_id_resp);
}
//------------------------------------------------------------------------------
int smf_app::process_pco_link_mtu_request(
    protocol_configuration_options_t& pco_resp,
    const pco_protocol_or_container_id_t* const poc_id) {
  pco_protocol_or_container_id_t poc_id_resp = {0};
  uint8_t mtu_array[2];

  Logger::smf_app().debug(
      "PCO: Protocol identifier IPCP option Link MTU Request");
  poc_id_resp.protocol_id = PCO_CONTAINER_IDENTIFIER_IPV4_LINK_MTU;
  poc_id_resp.length_of_protocol_id_contents = 2;
  mtu_array[0]                               = (uint8_t)(smf_cfg.ue_mtu >> 8);
  mtu_array[1]                               = (uint8_t)(smf_cfg.ue_mtu & 0xFF);
  std::string tmp_s((const char*) &mtu_array[0], 2);
  poc_id_resp.protocol_id_contents = tmp_s;

  return pco_push_protocol_or_container_id(pco_resp, &poc_id_resp);
}

//------------------------------------------------------------------------------
int smf_app::process_pco_request(
    const protocol_configuration_options_t& pco_req,
    protocol_configuration_options_t& pco_resp,
    protocol_configuration_options_ids_t& pco_ids) {
  switch (pco_req.configuration_protocol) {
    case PCO_CONFIGURATION_PROTOCOL_PPP_FOR_USE_WITH_IP_PDP_TYPE_OR_IP_PDN_TYPE:
      pco_resp.ext                          = 1;
      pco_resp.spare                        = 0;
      pco_resp.num_protocol_or_container_id = 0;
      pco_resp.configuration_protocol       = pco_req.configuration_protocol;
      break;

    default:
      Logger::smf_app().warn(
          "PCO: configuration protocol 0x%X not supported now",
          pco_req.configuration_protocol);
      break;
  }

  for (int id = 0; id < pco_req.num_protocol_or_container_id; id++) {
    switch (pco_req.protocol_or_container_ids[id].protocol_id) {
      case PCO_PROTOCOL_IDENTIFIER_IPCP:
        process_pco_request_ipcp(
            pco_resp, &pco_req.protocol_or_container_ids[id]);
        pco_ids.pi_ipcp = true;
        break;

      case PCO_CONTAINER_IDENTIFIER_DNS_SERVER_IPV4_ADDRESS_REQUEST:
        process_pco_dns_server_request(
            pco_resp, &pco_req.protocol_or_container_ids[id]);
        pco_ids.ci_dns_server_ipv4_address_request = true;
        break;
      case PCO_CONTAINER_IDENTIFIER_DNS_SERVER_IPV6_ADDRESS:
        process_pco_dns_server_v6_request(
            pco_resp, &pco_req.protocol_or_container_ids[id]);
        pco_ids.ci_dns_server_ipv6_address_request = true;
        break;
      case PCO_CONTAINER_IDENTIFIER_IP_ADDRESS_ALLOCATION_VIA_NAS_SIGNALLING:
        Logger::smf_app().debug("PCO: Allocation via NAS signaling requested");
        pco_ids.ci_ip_address_allocation_via_nas_signalling = true;
        break;

      case PCO_CONTAINER_IDENTIFIER_IPV4_LINK_MTU_REQUEST:
        process_pco_link_mtu_request(
            pco_resp, &pco_req.protocol_or_container_ids[id]);
        pco_ids.ci_ipv4_link_mtu_request = true;
        break;

      default:
        Logger::smf_app().warn(
            "PCO: Protocol/container identifier 0x%04X not supported now",
            pco_req.protocol_or_container_ids[id].protocol_id);
    }
  }

  if (smf_cfg.force_push_pco) {
    pco_ids.ci_ip_address_allocation_via_nas_signalling = true;
    if (!pco_ids.ci_dns_server_ipv4_address_request) {
      process_pco_dns_server_request(pco_resp, nullptr);
    }
    if (!pco_ids.ci_ipv4_link_mtu_request) {
      process_pco_link_mtu_request(pco_resp, nullptr);
    }
  }
  return RETURNok;
}
