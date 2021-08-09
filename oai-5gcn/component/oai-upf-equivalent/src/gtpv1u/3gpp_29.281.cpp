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

/*! \file 3gpp_29.281.cpp
  \brief
  \author Lionel Gauthier
  \company Eurecom
  \email: lionel.gauthier@eurecom.fr
*/

#include "3gpp_29.281.hpp"

#include <string>
#include <string.h>

using namespace gtpv1u;
//------------------------------------------------------------------------------
gtpv1u_ie* gtpv1u_ie::new_gtpv1u_ie_from_stream(std::istream& is) {
  gtpv1u_tlv tlv;
  tlv.load_from(is);
  if (tlv.length) {
    switch (tlv.type) {
      case GTPU_IE_RECOVERY: {
        gtpv1u_recovery_ie* ie = new gtpv1u_recovery_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTPU_IE_TUNNEL_ENDPOINT_IDENTIFIER_DATA_I: {
        gtpv1u_tunnel_endpoint_identifier_data_i_ie* ie =
            new gtpv1u_tunnel_endpoint_identifier_data_i_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTPU_IE_GTP_U_PEER_ADDRESS: {
        gtpv1u_gtp_u_peer_address_ie* ie =
            new gtpv1u_gtp_u_peer_address_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;
      case GTPU_IE_PRIVATE_EXTENSION: {
        gtpv1u_private_extension_ie* ie = new gtpv1u_private_extension_ie(tlv);
        ie->load_from(is);
        return ie;
      } break;

      default:
        Logger::gtpv1_u().error(
            "Unknown GTP IE type %d (length %d)", tlv.get_type(),
            tlv.get_length());
        return nullptr;
    }
  } else {
    Logger::gtpv1_u().error(
        "GTP IE type %d length %d", tlv.get_type(), tlv.get_length());
    return nullptr;
  }
}

//------------------------------------------------------------------------------
gtpv1u_msg::gtpv1u_msg(const gtpv1u_echo_request& gtp_ies)
    : gtpv1u_msg_header() {
  ies = {};
  set_message_type(GTPU_ECHO_REQUEST);
  if (gtp_ies.private_extension.first) {
    std::shared_ptr<gtpv1u_private_extension_ie> sie(
        new gtpv1u_private_extension_ie(gtp_ies.private_extension.second));
    add_ie(sie);
  }
}
//------------------------------------------------------------------------------
gtpv1u_msg::gtpv1u_msg(const gtpv1u_echo_response& gtp_ies)
    : gtpv1u_msg_header() {
  ies = {};
  set_message_type(GTPU_ECHO_RESPONSE);
  if (gtp_ies.recovery.first) {
    std::shared_ptr<gtpv1u_recovery_ie> sie(
        new gtpv1u_recovery_ie(gtp_ies.recovery.second));
    add_ie(sie);
  }
  if (gtp_ies.private_extension.first) {
    std::shared_ptr<gtpv1u_private_extension_ie> sie(
        new gtpv1u_private_extension_ie(gtp_ies.private_extension.second));
    add_ie(sie);
  }
}
//------------------------------------------------------------------------------
gtpv1u_msg::gtpv1u_msg(const gtpv1u_error_indication& gtp_ies)
    : gtpv1u_msg_header() {
  ies = {};
  set_message_type(GTPU_ERROR_INDICATION);
  if (gtp_ies.tunnel_endpoint_identifier_data_i.first) {
    std::shared_ptr<gtpv1u_tunnel_endpoint_identifier_data_i_ie> sie(
        new gtpv1u_tunnel_endpoint_identifier_data_i_ie(
            gtp_ies.tunnel_endpoint_identifier_data_i.second));
    add_ie(sie);
  }
  if (gtp_ies.gtp_u_peer_address.first) {
    std::shared_ptr<gtpv1u_gtp_u_peer_address_ie> sie(
        new gtpv1u_gtp_u_peer_address_ie(gtp_ies.gtp_u_peer_address.second));
    add_ie(sie);
  }
  if (gtp_ies.private_extension.first) {
    std::shared_ptr<gtpv1u_private_extension_ie> sie(
        new gtpv1u_private_extension_ie(gtp_ies.private_extension.second));
    add_ie(sie);
  }
}
