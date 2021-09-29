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

/*! \file pfcp_pdr.cpp
   \author  Lionel GAUTHIER
   \date 2019
   \email: lionel.gauthier@eurecom.fr
*/
#include "common_defs.h"
#include "endian.h"
#include "pfcp_pdr.hpp"
#include "spgwu_sx.hpp"

using namespace pfcp;

extern spgwu::spgwu_sx* spgwu_sx_inst;

//------------------------------------------------------------------------------
bool pfcp_pdr::look_up_pack_in_access(
    struct iphdr* const iph, const std::size_t num_bytes,
    const endpoint& r_endpoint, const uint32_t tunnel_id) {
  // implicit packet arrives from ACCESS interface
  if (outer_header_removal.first) {
    if (outer_header_removal.second.outer_header_removal_description !=
        OUTER_HEADER_REMOVAL_GTPU_UDP_IPV4) {
      return false;
    }
  } else {
    // IPV4/UDP/GTP header already removed
    return false;
  }
  if (pdi.first) {
    if (pdi.second.source_interface.first) {
      if (pdi.second.source_interface.second.interface_value !=
          INTERFACE_VALUE_ACCESS) {
        return false;
      }
    }
    // local_fteid should be fine since pdr created for this fteid
    if (pdi.second.local_fteid.first) {
      if (pdi.second.local_fteid.second.teid != tunnel_id) {
        return false;
      }
    }
    if (pdi.second.ue_ip_address.first) {
      if (!pdi.second.ue_ip_address.second.v4) {
        return false;
      }
      if (pdi.second.ue_ip_address.second.ipv4_address.s_addr != iph->saddr) {
        return false;
      }
    }
    // SDF filter
    if (pdi.second.sdf_filter.first) {
      // TODO (create ss_pdi_t with ss_sdf_filter_t with optimized flow
      // description matching )
      return true;
    }
    return true;  // No SDF filter actually
  } else {
    // Mandatory IE
    return false;
  }
}
//------------------------------------------------------------------------------
bool pfcp_pdr::look_up_pack_in_core(
    struct iphdr* const iph, const std::size_t num_bytes) {
  // implicit packet arrives from CORE interface
  if (outer_header_removal.first) {
    // TODO ... when necessary (split U)
    // Logger::pfcp_switch().info( "look_up_pack_in_core failed PDR id %4x,
    // cause outer_header_removal present ", pdr_id.rule_id);
    return false;
  }
  // if (pdi.second.source_interface.second.interface_value !=
  // INTERFACE_VALUE_CORE) {
  //  return false;
  //}
  if (pdi.second.ue_ip_address.first) {
    if (!pdi.second.ue_ip_address.second.v4) {
      // Logger::pfcp_switch().info( "look_up_pack_in_core failed PDR id %4x,
      // cause ue_ip_address not present ", pdr_id.rule_id);
      return false;
    }
    if (pdi.second.ue_ip_address.second.ipv4_address.s_addr != iph->daddr) {
      // Logger::pfcp_switch().info( "look_up_pack_in_core failed PDR id %4x,
      // cause PDR ue_ip_address %8X do not match IP dest %8X of packet ",
      //    pdr_id.rule_id, pdi.second.ue_ip_address.second.ipv4_address.s_addr,
      //    iph->daddr);
      return false;
    }
  }
  // SDF filters TODO vector
  // if (pdi.second.sdf_filter.first) {
  // TODO (create ss_pdi_t with ss_sdf_filter_t with optimized flow description
  // matching )
  return true;
  //}
  // return false;
}

//------------------------------------------------------------------------------
bool pfcp_pdr::update(const pfcp::update_pdr& update, uint8_t& cause_value) {
  if (update.get(outer_header_removal.second))
    outer_header_removal.first = true;
  if (update.get(precedence.second)) precedence.first = true;
  if (update.get(pdi.second)) pdi.first = true;
  if (update.get(far_id.second)) far_id.first = true;
  if (update.get(urr_id.second)) urr_id.first = true;
  if (update.get(qer_id.second)) qer_id.first = true;
  // TODO activate_predefined_rules
  // TODO deactivate_predefined_rules
  return true;
}

//------------------------------------------------------------------------------
void pfcp_pdr::buffering_requested(
    const char* buffer, const std::size_t num_bytes) {
  Logger::spgwu_sx().warn("TODO pfcp_pdr::buffering_requested()");
  /*
    // TODO find smarter solution
    char filename[] = "/tmp/buff_pdrzzzxxxyyy.XXXXXX";
    int fd = mkstemp(filename);

    if (fd == -1) return 1;
    write(fd, buffer, num_bytes);

    close(fd);
    unlink(filename);
    num_packets++
   */
}

//------------------------------------------------------------------------------
void pfcp_pdr::notify_cp_requested(
    std::shared_ptr<pfcp::pfcp_session> session) {
  if (not notified_cp) {
    Logger::spgwu_sx().trace("notify_cp_requested()");
    notified_cp = true;

    pfcp::pfcp_session_report_request h;

    pfcp::report_type_t report = {};
    report.dldr                = 1;

    pfcp::downlink_data_report dl_data_report;
    dl_data_report.set(pdr_id);

    h.set(report);
    h.set(dl_data_report);

    spgwu_sx_inst->send_sx_msg(session->cp_fseid, h);
  }
}
