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
 *  itti_msg_s5s8.hpp
 *
 *  Created on: Oct 26, 2018
 *  Author: lionel.gauthier@eurecom.fr
 */

#ifndef ITTI_MSG_S5S8_HPP_INCLUDED_
#define ITTI_MSG_S5S8_HPP_INCLUDED_

#include "3gpp_29.274.hpp"
#include "endpoint.hpp"
#include "itti_msg.hpp"
#include "msg_gtpv2c.hpp"

class itti_s5s8_msg : public itti_msg {
 public:
  itti_s5s8_msg(
      const itti_msg_type_t msg_type, const task_id_t orig,
      const task_id_t dest)
      : itti_msg(msg_type, orig, dest) {
    l_endpoint = {};
    r_endpoint = {};
    teid       = UNASSIGNED_TEID;
    l_teid     = UNASSIGNED_TEID;
    gtpc_tx_id = 0;
  }

  itti_s5s8_msg(const itti_s5s8_msg& i)
      : itti_msg(i),
        l_endpoint(i.l_endpoint),
        r_endpoint(i.r_endpoint),
        teid(i.teid),
        l_teid(i.l_teid),
        gtpc_tx_id(i.gtpc_tx_id) {}

  itti_s5s8_msg(
      const itti_s5s8_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(i) {
    origin      = orig;
    destination = dest;
  }

  endpoint l_endpoint;
  endpoint r_endpoint;
  teid_t teid;    // remote teid
  teid_t l_teid;  // local teid
  uint64_t gtpc_tx_id;
};

class itti_s5s8_remote_peer_not_responding : public itti_s5s8_msg {
 public:
  itti_s5s8_remote_peer_not_responding(
      const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_REMOTE_PEER_NOT_RESPONDING, orig, dest) {}
  itti_s5s8_remote_peer_not_responding(
      const itti_s5s8_remote_peer_not_responding& i)
      : itti_s5s8_msg(i) {}
  itti_s5s8_remote_peer_not_responding(
      const itti_s5s8_remote_peer_not_responding& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest) {}
  const char* get_msg_name() { return "S5S8_REMOTE_PEER_NOT_RESPONDING"; };
};
//-----------------------------------------------------------------------------
class itti_s5s8_create_session_request : public itti_s5s8_msg {
 public:
  itti_s5s8_create_session_request(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_CREATE_SESSION_REQUEST, orig, dest) {}
  itti_s5s8_create_session_request(const itti_s5s8_create_session_request& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_create_session_request(
      const itti_s5s8_create_session_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_CREATE_SESSION_REQUEST"; };

  gtpv2c::gtpv2c_create_session_request gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_create_session_response_t
 *  @brief Create Session Response
 *
 * The Create Session Response will be sent on S5S8 interface as
 * part of these procedures:
 * - E-UTRAN Initial Attach
 * - UE requested PDN connectivity
 * - Tracking Area Update procedure with SGW change
 * - S1/X2-based handover with SGW change
 */
class itti_s5s8_create_session_response : public itti_s5s8_msg {
 public:
  itti_s5s8_create_session_response(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_CREATE_SESSION_RESPONSE, orig, dest) {}
  itti_s5s8_create_session_response(const itti_s5s8_create_session_response& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_create_session_response(
      const itti_s5s8_create_session_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_CREATE_SESSION_RESPONSE"; };

  gtpv2c::gtpv2c_create_session_response gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_create_bearer_request
 *  @brief Create Bearer Request
 *
 * The direction of this message shall be from PGW to SGW and from SGW to
 * MME/S4-SGSN, and from PGW to ePDG The Create Bearer Request message shall be
 * sent on the S5/S8 interface by the PGW to the SGW and on the S5S8 interface
 * by the SGW to the MME as part of the Dedicated Bearer Activation procedure.
 * The message shall also be sent on the S5/S8 interface by the PGW to the SGW
 * and on the S4 interface by the SGW to the SGSN as part of the Secondary PDP
 * Context Activation procedure or the Network Requested Secondary PDP Context
 * Activation procedure. The message shall also be sent on the S2b interface by
 * the PGW to the ePDG as part of the Dedicated S2b bearer activation with GTP
 * on S2b.
 */
class itti_s5s8_create_bearer_request : public itti_s5s8_msg {
 public:
  itti_s5s8_create_bearer_request(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_CREATE_BEARER_REQUEST, orig, dest) {}
  itti_s5s8_create_bearer_request(const itti_s5s8_create_bearer_request& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_create_bearer_request(
      const itti_s5s8_create_bearer_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_CREATE_BEARER_REQUEST"; };

  gtpv2c::gtpv2c_create_bearer_request gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_create_bearer_response
 *  @brief Create Bearer Response
 *
 * The Create Bearer Response message shall be sent on the S5/S8 interface by
 * the SGW to the PGW, and on the S5S8 interface by the MME to the SGW as part
 * of the Dedicated Bearer Activation procedure. The message shall also be sent
 * on the S5/S8 interface by the SGW to the PGW and on the S4 interface by the
 * SGSN to the SGW as part of Secondary PDP Context Activation procedure or the
 * Network Requested Secondary PDP Context Activation procedure. The message
 * shall also be sent on the S2b interface by the ePDG to the PGW as part of the
 * Dedicated S2b bearer activation with GTP on S2b. Possible Cause values are
 * specified in Table 8.4-1. Message specific cause values are:
 * - "Request accepted".
 * - "Request accepted partially".
 * - "Context not found".
 * - "Semantic error in the TFT operation".
 * - "Syntactic error in the TFT operation".
 * - "Semantic errors in packet filter(s)".
 * - "Syntactic errors in packet filter(s)".
 * - "Service not supported".
 * - "Unable to page UE".
 * - "UE not responding".
 * - "Unable to page UE due to Suspension".
 * - "UE refuses".
 * - "Denied in RAT".
 * - "UE context without TFT already activated".
 */
class itti_s5s8_create_bearer_response : public itti_s5s8_msg {
 public:
  itti_s5s8_create_bearer_response(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_CREATE_BEARER_RESPONSE, orig, dest) {}
  itti_s5s8_create_bearer_response(const itti_s5s8_create_bearer_response& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_create_bearer_response(
      const itti_s5s8_create_bearer_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_CREATE_BEARER_RESPONSE"; };

  gtpv2c::gtpv2c_create_bearer_response gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_modify_bearer_request
 *  @brief Modify Bearer Request
 *
 * The Modify Bearer Request will be sent on S5S8 interface as
 * part of these procedures:
 * - E-UTRAN Tracking Area Update without SGW Change
 * - UE triggered Service Request
 * - S1-based Handover
 * - E-UTRAN Initial Attach
 * - UE requested PDN connectivity
 * - X2-based handover without SGWrelocation
 */
class itti_s5s8_modify_bearer_request : public itti_s5s8_msg {
 public:
  itti_s5s8_modify_bearer_request(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_MODIFY_BEARER_REQUEST, orig, dest) {}
  itti_s5s8_modify_bearer_request(const itti_s5s8_modify_bearer_request& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_modify_bearer_request(
      const itti_s5s8_modify_bearer_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() {
    return typeid(itti_s5s8_modify_bearer_request).name();
  };

  gtpv2c::gtpv2c_modify_bearer_request gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_modify_bearer_response
 *  @brief Modify Bearer Response
 *
 * The Modify Bearer Response will be sent on S5S8 interface as
 * part of these procedures:
 * - E-UTRAN Tracking Area Update without SGW Change
 * - UE triggered Service Request
 * - S1-based Handover
 * - E-UTRAN Initial Attach
 * - UE requested PDN connectivity
 * - X2-based handover without SGWrelocation
 */
class itti_s5s8_modify_bearer_response : public itti_s5s8_msg {
 public:
  itti_s5s8_modify_bearer_response(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_MODIFY_BEARER_RESPONSE, orig, dest) {}
  itti_s5s8_modify_bearer_response(const itti_s5s8_modify_bearer_response& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_modify_bearer_response(
      const itti_s5s8_modify_bearer_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_MODIFY_BEARER_RESPONSE"; };

  gtpv2c::gtpv2c_modify_bearer_response gtp_ies;
};

//-----------------------------------------------------------------------------
class itti_s5s8_delete_session_request : public itti_s5s8_msg {
 public:
  itti_s5s8_delete_session_request(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_DELETE_SESSION_REQUEST, orig, dest) {
    noDelete = false;
  }
  itti_s5s8_delete_session_request(const itti_s5s8_delete_session_request& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies), noDelete(i.noDelete) {}
  itti_s5s8_delete_session_request(
      const itti_s5s8_delete_session_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest),
        gtp_ies(i.gtp_ies),
        noDelete(i.noDelete) {}

  const char* get_msg_name() { return "S5S8_DELETE_SESSION_REQUEST"; };

  gtpv2c::gtpv2c_delete_session_request gtp_ies;
  bool noDelete;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_delete_session_response
 *  @brief Delete Session Response
 *
 * The Delete Session Response will be sent on S5S8 interface as
 * part of these procedures:
 * - EUTRAN Initial Attach
 * - UE, HSS or MME Initiated Detach
 * - UE or MME Requested PDN Disconnection
 * - Tracking Area Update with SGW Change
 * - S1 Based Handover with SGW Change
 * - X2 Based Handover with SGW Relocation
 * - S1 Based handover cancel with SGW change
 */
class itti_s5s8_delete_session_response : public itti_s5s8_msg {
 public:
  itti_s5s8_delete_session_response(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_DELETE_SESSION_RESPONSE, orig, dest) {}
  itti_s5s8_delete_session_response(const itti_s5s8_delete_session_response& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_delete_session_response(
      const itti_s5s8_delete_session_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_DELETE_SESSION_RESPONSE"; };

  gtpv2c::gtpv2c_delete_session_response gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_release_access_bearers_request
 *  @brief Release AccessBearers Request
 *
 * The Release Access Bearers Request message shall sent on the S5S8 interface
 * by the MME to the SGW as part of the S1 release procedure. The message shall
 * also be sent on the S4 interface by the SGSN to the SGW as part of the
 * procedures:
 * -    RAB release using S4
 * -    Iu Release using S4
 * -    READY to STANDBY transition within the network
 */
class itti_s5s8_release_access_bearers_request : public itti_s5s8_msg {
 public:
  itti_s5s8_release_access_bearers_request(
      const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_RELEASE_ACCESS_BEARERS_REQUEST, orig, dest) {}
  itti_s5s8_release_access_bearers_request(
      const itti_s5s8_release_access_bearers_request& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_release_access_bearers_request(
      const itti_s5s8_release_access_bearers_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_RELEASE_ACCESS_BEARERS_REQUEST"; };

  gtpv2c::gtpv2c_release_access_bearers_request gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_release_access_bearers_response
 *  @brief Release AccessBearers Response
 *
 * The Release Access Bearers Response message is sent on the S5S8 interface by
 * the SGW to the MME as part of the S1 release procedure. The message shall
 * also be sent on the S4 interface by the SGW to the SGSN as part of the
 * procedures:
 * -  RAB release using S4
 * -  Iu Release using S4
 * -  READY to STANDBY transition within the network
 * Possible Cause values are specified in Table 8.4-1. Message specific cause
 * values are:
 * - "Request accepted".
 * - "Request accepted partially".
 * - "Context not found
 */
class itti_s5s8_release_access_bearers_response : public itti_s5s8_msg {
 public:
  itti_s5s8_release_access_bearers_response(
      const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_RELEASE_ACCESS_BEARERS_RESPONSE, orig, dest) {}
  itti_s5s8_release_access_bearers_response(
      const itti_s5s8_release_access_bearers_response& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_release_access_bearers_response(
      const itti_s5s8_release_access_bearers_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_RELEASE_ACCESS_BEARERS_RESPONSE"; };

  gtpv2c::gtpv2c_release_access_bearers_response gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_delete_bearer_command_t
 *  @brief Initiate Delete Bearer procedure
 *
 * A Delete Bearer Command message shall be sent on the S5S8 interface by the
 * MME to the SGW and on the S5/S8 interface by the SGW to the PGW as a part of
 * the eNodeB requested bearer release or MME-Initiated Dedicated Bearer
 * Deactivation procedure.
 * The message shall also be sent on the S4 interface by the SGSN to the SGW and
 * on the S5/S8 interface by the SGW to the PGW as part of the MS and SGSN
 * Initiated Bearer Deactivation procedure using S4.
 */
class itti_s5s8_delete_bearer_command : public itti_s5s8_msg {
 public:
  itti_s5s8_delete_bearer_command(const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(S5S8_DELETE_BEARER_COMMAND, orig, dest) {}
  itti_s5s8_delete_bearer_command(const itti_s5s8_delete_bearer_command& i)
      : itti_s5s8_msg(i), gtp_ies(i.gtp_ies) {}
  itti_s5s8_delete_bearer_command(
      const itti_s5s8_delete_bearer_command& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest), gtp_ies(i.gtp_ies) {}
  const char* get_msg_name() { return "S5S8_DELETE_BEARER_COMMAND"; };

  gtpv2c::gtpv2c_delete_bearer_command gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_downlink_data_notification
 *  @brief Downlink Data Notification, not in spec but necessary due to SGWU and
 * PGWU tied together.
 *
 * The Downlink Data Notification message is sent on the S5S8 interface by the
 * SGW to the MME as part of the S1 paging procedure.
 */
class itti_s5s8_downlink_data_notification : public itti_s5s8_msg {
 public:
  itti_s5s8_downlink_data_notification(
      const task_id_t origin, const task_id_t destination)
      : itti_s5s8_msg(S5S8_DOWNLINK_DATA_NOTIFICATION, origin, destination) {}
  itti_s5s8_downlink_data_notification(
      const itti_s5s8_downlink_data_notification& i)
      : itti_s5s8_msg(i) {
    gtp_ies = i.gtp_ies;
  }
  itti_s5s8_downlink_data_notification(
      const itti_s5s8_downlink_data_notification& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest) {
    gtp_ies = i.gtp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_s5s8_downlink_data_notification).name();
  };

  gtpv2c::gtpv2c_downlink_data_notification gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_downlink_data_notification_acknowledge
 *  @brief Downlink Data Notification Acknowledge, not in spec but necessary due
 * to SGWU and PGWU tied together.
 *
 * The Downlink Data Notification Acknowledge message is sent on the S5S8
 * interface by the MME to the SGW as part of the S1 paging procedure.
 */
class itti_s5s8_downlink_data_notification_acknowledge : public itti_s5s8_msg {
 public:
  itti_s5s8_downlink_data_notification_acknowledge(
      const task_id_t origin, const task_id_t destination)
      : itti_s5s8_msg(
            S5S8_DOWNLINK_DATA_NOTIFICATION_ACKNOWLEDGE, origin, destination) {}
  itti_s5s8_downlink_data_notification_acknowledge(
      const itti_s5s8_downlink_data_notification_acknowledge& i)
      : itti_s5s8_msg(i) {
    gtp_ies = i.gtp_ies;
  }
  itti_s5s8_downlink_data_notification_acknowledge(
      const itti_s5s8_downlink_data_notification_acknowledge& i,
      const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest) {
    gtp_ies = i.gtp_ies;
  }
  itti_s5s8_downlink_data_notification_acknowledge(
      const gtpv2c::gtpv2c_downlink_data_notification_acknowledge& ies,
      const task_id_t orig, const task_id_t dest)
      : itti_s5s8_downlink_data_notification_acknowledge(orig, dest) {
    gtp_ies = ies;
  }

  const char* get_msg_name() {
    return typeid(itti_s5s8_downlink_data_notification_acknowledge).name();
  };

  gtpv2c::gtpv2c_downlink_data_notification_acknowledge gtp_ies;
};

//-----------------------------------------------------------------------------
/** @class itti_s5s8_downlink_data_notification_failure_indication
 *  @brief Downlink Data Notification Failure Indication, not in spec but
 * necessary due to SGWU and PGWU tied together.
 *
 */
class itti_s5s8_downlink_data_notification_failure_indication
    : public itti_s5s8_msg {
 public:
  itti_s5s8_downlink_data_notification_failure_indication(
      const task_id_t origin, const task_id_t destination)
      : itti_s5s8_msg(
            S5S8_DOWNLINK_DATA_NOTIFICATION_FAILURE_INDICATION, origin,
            destination) {}
  itti_s5s8_downlink_data_notification_failure_indication(
      const itti_s5s8_downlink_data_notification_failure_indication& i)
      : itti_s5s8_msg(i) {
    gtp_ies = i.gtp_ies;
  }
  itti_s5s8_downlink_data_notification_failure_indication(
      const itti_s5s8_downlink_data_notification_failure_indication& i,
      const task_id_t orig, const task_id_t dest)
      : itti_s5s8_msg(i, orig, dest) {
    gtp_ies = i.gtp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_s5s8_downlink_data_notification_failure_indication)
        .name();
  };

  gtpv2c::gtpv2c_downlink_data_notification_failure_indication gtp_ies;
};
#endif /* ITTI_MSG_S5S8_HPP_INCLUDED_ */
