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

/*! \file pfcp_far.cpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/

#include "pfcp_far.hpp"
#include "pfcp_switch.hpp"
#include "spgwu_config.hpp"
#include "spgwu_s1u.hpp"

using namespace pfcp;
using namespace spgwu;

extern pfcp_switch* pfcp_switch_inst;
extern spgwu_s1u* spgwu_s1u_inst;
extern spgwu_config spgwu_cfg;

//------------------------------------------------------------------------------
void pfcp_far::apply_forwarding_rules(
    struct iphdr* const iph, const std::size_t num_bytes, bool& nocp,
    bool& buff, uint8_t  qfi) {
  // TODO dupl
  // TODO nocp
  // TODO buff
  // Logger::pfcp_switch().info( "pfcp_far::apply_forwarding_rules FAR id %4x ",
  // far_id.far_id);
  if (apply_action.forw) {
    if (forwarding_parameters.first) {
      if (forwarding_parameters.second.destination_interface.first) {
        if (forwarding_parameters.second.destination_interface.second
                .interface_value == INTERFACE_VALUE_ACCESS) {
          if (forwarding_parameters.second.outer_header_creation.first) {
            switch (forwarding_parameters.second.outer_header_creation.second
                        .outer_header_creation_description) {
              case OUTER_HEADER_CREATION_GTPU_UDP_IPV4:
                spgwu_s1u_inst->send_g_pdu(
                    forwarding_parameters.second.outer_header_creation.second
                        .ipv4_address,
                    spgwu_cfg.s1_up.port,
                    forwarding_parameters.second.outer_header_creation.second
                        .teid,
                    reinterpret_cast<const char*>(iph), num_bytes, qfi);

                break;
              case OUTER_HEADER_CREATION_GTPU_UDP_IPV6:
                spgwu_s1u_inst->send_g_pdu(
                    forwarding_parameters.second.outer_header_creation.second
                        .ipv6_address,
                    spgwu_cfg.s1_up.port,
                    forwarding_parameters.second.outer_header_creation.second
                        .teid,
                    reinterpret_cast<const char*>(iph), num_bytes);
                break;
              case OUTER_HEADER_CREATION_UDP_IPV4:  // TODO
              case OUTER_HEADER_CREATION_UDP_IPV6:  // TODO
              default:;
            }
          }
        } else if (
            forwarding_parameters.second.destination_interface.second
                .interface_value == INTERFACE_VALUE_CORE) {
          pfcp_switch_inst->send_to_core(
              reinterpret_cast<char* const>(iph), num_bytes);
        } else {
        }
      } else {
        // Mandatory IE
      }
    } else {
      // Mandatory if FW set in apply action
    }
  } else if (apply_action.drop) {
    // DONE !
  } else if (apply_action.buff) {
    buff = true;
  }

  if (apply_action.nocp) {
    nocp = true;
  }
}

//------------------------------------------------------------------------------
bool pfcp_far::update(const pfcp::update_far& update, uint8_t& cause_value) {
  update.get(apply_action);
  if (update.update_forwarding_parameters.first) {
    forwarding_parameters.second.update(
        update.update_forwarding_parameters.second);
    if (update.update_forwarding_parameters.second.pfcpsmreq_flags.first) {
      // TODO
    }
  }
  if (update.update_duplicating_parameters.first) {
    duplicating_parameters.second.update(
        update.update_duplicating_parameters.second);
  }
  if (update.get(bar_id.second)) bar_id.first = true;
  return true;
}
