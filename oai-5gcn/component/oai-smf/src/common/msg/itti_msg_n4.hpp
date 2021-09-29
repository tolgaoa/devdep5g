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

/*! \file itti_msg_n4.hpp
 \author  Lionel GAUTHIER
 \date 2019
 \email: lionel.gauthier@eurecom.fr
 */

#ifndef ITTI_MSG_N4_HPP_INCLUDED_
#define ITTI_MSG_N4_HPP_INCLUDED_

#include "3gpp_29.244.hpp"
#include "endpoint.hpp"
#include "itti_msg.hpp"
#include "msg_pfcp.hpp"

class itti_n4_msg : public itti_msg {
 public:
  itti_n4_msg(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {
    l_endpoint = {};
    r_endpoint = {};
    seid       = UNASSIGNED_SEID;
    trxn_id    = 0;
  }
  itti_n4_msg(const itti_n4_msg& i) : itti_msg(i) {
    l_endpoint = i.l_endpoint;
    r_endpoint = i.r_endpoint;
    seid       = i.seid;
    trxn_id    = i.trxn_id;
  }
  itti_n4_msg(const itti_n4_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_n4_msg(i) {
    origin      = orig;
    destination = dest;
  }

  endpoint l_endpoint;
  endpoint r_endpoint;
  seid_t seid;
  uint64_t trxn_id;
};

//-----------------------------------------------------------------------------
class itti_n4_heartbeat_request : public itti_n4_msg {
 public:
  itti_n4_heartbeat_request(const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_HEARTBEAT_REQUEST, origin, destination) {}
  itti_n4_heartbeat_request(const itti_n4_heartbeat_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_heartbeat_request(
      const itti_n4_heartbeat_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }

  const char* get_msg_name() {
    return typeid(itti_n4_heartbeat_request).name();
  };

  pfcp::pfcp_heartbeat_request pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_heartbeat_response : public itti_n4_msg {
 public:
  itti_n4_heartbeat_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_HEARTBEAT_RESPONSE, origin, destination) {}
  itti_n4_heartbeat_response(const itti_n4_heartbeat_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_heartbeat_response(
      const itti_n4_heartbeat_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_heartbeat_response).name();
  };

  pfcp::pfcp_heartbeat_response pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_pfcp_pfd_management_request : public itti_n4_msg {
 public:
  itti_n4_pfcp_pfd_management_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_PFCP_PFD_MANAGEMENT_REQUEST, origin, destination) {}
  itti_n4_pfcp_pfd_management_request(
      const itti_n4_pfcp_pfd_management_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_pfcp_pfd_management_request(
      const itti_n4_pfcp_pfd_management_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_pfcp_pfd_management_request).name();
  };

  pfcp::pfcp_pfd_management_request pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_pfcp_pfd_management_response : public itti_n4_msg {
 public:
  itti_n4_pfcp_pfd_management_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_PFCP_PFD_MANAGEMENT_RESPONSE, origin, destination) {}
  itti_n4_pfcp_pfd_management_response(
      const itti_n4_pfcp_pfd_management_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_pfcp_pfd_management_response(
      const itti_n4_pfcp_pfd_management_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_pfcp_pfd_management_response).name();
  };

  pfcp::pfcp_pfd_management_response pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_association_setup_request : public itti_n4_msg {
 public:
  itti_n4_association_setup_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_ASSOCIATION_SETUP_REQUEST, origin, destination) {}
  itti_n4_association_setup_request(const itti_n4_association_setup_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_association_setup_request(
      const itti_n4_association_setup_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_association_setup_request).name();
  };

  pfcp::pfcp_association_setup_request pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_association_setup_response : public itti_n4_msg {
 public:
  itti_n4_association_setup_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_ASSOCIATION_SETUP_RESPONSE, origin, destination) {}
  itti_n4_association_setup_response(
      const itti_n4_association_setup_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_association_setup_response(
      const itti_n4_association_setup_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_association_setup_response).name();
  };

  pfcp::pfcp_association_setup_response pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_association_update_request : public itti_n4_msg {
 public:
  itti_n4_association_update_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_ASSOCIATION_UPDATE_REQUEST, origin, destination) {}
  itti_n4_association_update_request(
      const itti_n4_association_update_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_association_update_request(
      const itti_n4_association_update_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_association_update_request).name();
  };

  pfcp::pfcp_association_update_request pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_association_update_response : public itti_n4_msg {
 public:
  itti_n4_association_update_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_ASSOCIATION_UPDATE_RESPONSE, origin, destination) {}
  itti_n4_association_update_response(
      const itti_n4_association_update_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_association_update_response(
      const itti_n4_association_update_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_association_update_response).name();
  };

  pfcp::pfcp_association_update_response pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_association_release_request : public itti_n4_msg {
 public:
  itti_n4_association_release_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_ASSOCIATION_RELEASE_REQUEST, origin, destination) {}
  itti_n4_association_release_request(
      const itti_n4_association_release_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_association_release_request(
      const itti_n4_association_release_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_association_release_request).name();
  };

  pfcp::pfcp_association_release_request pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_association_release_response : public itti_n4_msg {
 public:
  itti_n4_association_release_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_ASSOCIATION_RELEASE_RESPONSE, origin, destination) {}
  itti_n4_association_release_response(
      const itti_n4_association_release_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_association_release_response(
      const itti_n4_association_release_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_association_release_response).name();
  };

  pfcp::pfcp_association_release_response pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_version_not_supported_response : public itti_n4_msg {
 public:
  itti_n4_version_not_supported_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_VERSION_NOT_SUPPORTED_RESPONSE, origin, destination) {}
  itti_n4_version_not_supported_response(
      const itti_n4_version_not_supported_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_version_not_supported_response(
      const itti_n4_version_not_supported_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_version_not_supported_response).name();
  };

  pfcp::pfcp_version_not_supported_response pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_node_report_request : public itti_n4_msg {
 public:
  itti_n4_node_report_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_NODE_REPORT_REQUEST, origin, destination) {}
  itti_n4_node_report_request(const itti_n4_node_report_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_node_report_request(
      const itti_n4_node_report_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_node_report_request).name();
  };

  pfcp::pfcp_node_report_request pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_node_report_response : public itti_n4_msg {
 public:
  itti_n4_node_report_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_NODE_REPORT_RESPONSE, origin, destination) {}
  itti_n4_node_report_response(const itti_n4_node_report_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_node_report_response(
      const itti_n4_node_report_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_node_report_response).name();
  };

  pfcp::pfcp_node_report_response pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_node_failure : public itti_n4_msg {
 public:
  itti_n4_node_failure(const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_NODE_FAILURE, origin, destination), node_id() {}
  itti_n4_node_failure(const itti_n4_node_failure& i) : itti_n4_msg(i) {
    node_id = i.node_id;
  }
  itti_n4_node_failure(
      const itti_n4_node_failure& i, const task_id_t orig, const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    node_id = i.node_id;
  }
  const char* get_msg_name() { return typeid(itti_n4_node_failure).name(); };

  pfcp::node_id_t node_id;
};

//-----------------------------------------------------------------------------
class itti_n4_session_set_deletion_request : public itti_n4_msg {
 public:
  itti_n4_session_set_deletion_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_SET_DELETION_REQUEST, origin, destination) {}
  itti_n4_session_set_deletion_request(
      const itti_n4_session_set_deletion_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_set_deletion_request(
      const itti_n4_session_set_deletion_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_set_deletion_request).name();
  };

  pfcp::pfcp_session_set_deletion_request pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_set_deletion_response : public itti_n4_msg {
 public:
  itti_n4_session_set_deletion_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_SET_DELETION_RESPONSE, origin, destination) {}
  itti_n4_session_set_deletion_response(
      const itti_n4_session_set_deletion_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_set_deletion_response(
      const itti_n4_session_set_deletion_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_set_deletion_response).name();
  };

  pfcp::pfcp_session_set_deletion_response pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_establishment_request : public itti_n4_msg {
 public:
  itti_n4_session_establishment_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_ESTABLISHMENT_REQUEST, origin, destination) {}
  itti_n4_session_establishment_request(
      const itti_n4_session_establishment_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_establishment_request(
      const itti_n4_session_establishment_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_establishment_request).name();
  };

  pfcp::pfcp_session_establishment_request pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_establishment_response : public itti_n4_msg {
 public:
  itti_n4_session_establishment_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_ESTABLISHMENT_RESPONSE, origin, destination) {}
  itti_n4_session_establishment_response(
      const itti_n4_session_establishment_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_establishment_response(
      const itti_n4_session_establishment_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_establishment_response).name();
  };

  pfcp::pfcp_session_establishment_response pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_modification_request : public itti_n4_msg {
 public:
  itti_n4_session_modification_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_MODIFICATION_REQUEST, origin, destination) {}
  itti_n4_session_modification_request(
      const itti_n4_session_modification_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_modification_request(
      const itti_n4_session_modification_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_modification_request).name();
  };

  pfcp::pfcp_session_modification_request pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_modification_response : public itti_n4_msg {
 public:
  itti_n4_session_modification_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_MODIFICATION_RESPONSE, origin, destination) {}
  itti_n4_session_modification_response(
      const itti_n4_session_modification_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_modification_response(
      const itti_n4_session_modification_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_modification_response).name();
  };

  pfcp::pfcp_session_modification_response pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_deletion_request : public itti_n4_msg {
 public:
  itti_n4_session_deletion_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_DELETION_REQUEST, origin, destination) {}
  itti_n4_session_deletion_request(const itti_n4_session_deletion_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_deletion_request(
      const itti_n4_session_deletion_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_deletion_request).name();
  };

  pfcp::pfcp_session_deletion_request pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_deletion_response : public itti_n4_msg {
 public:
  itti_n4_session_deletion_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_DELETION_RESPONSE, origin, destination) {}
  itti_n4_session_deletion_response(const itti_n4_session_deletion_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_deletion_response(
      const itti_n4_session_deletion_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_deletion_response).name();
  };

  pfcp::pfcp_session_deletion_response pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_report_request : public itti_n4_msg {
 public:
  itti_n4_session_report_request(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_REPORT_REQUEST, origin, destination) {}
  itti_n4_session_report_request(const itti_n4_session_report_request& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_report_request(
      const itti_n4_session_report_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_deletion_request).name();
  };

  pfcp::pfcp_session_report_request pfcp_ies;
};
//-----------------------------------------------------------------------------
class itti_n4_session_report_response : public itti_n4_msg {
 public:
  itti_n4_session_report_response(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_REPORT_RESPONSE, origin, destination) {}
  itti_n4_session_report_response(const itti_n4_session_report_response& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_report_response(
      const itti_n4_session_report_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_report_response).name();
  };

  pfcp::pfcp_session_report_response pfcp_ies;
};

//-----------------------------------------------------------------------------
class itti_n4_session_failure_indication : public itti_n4_msg {
 public:
  itti_n4_session_failure_indication(
      const task_id_t origin, const task_id_t destination)
      : itti_n4_msg(N4_SESSION_REPORT_RESPONSE, origin, destination) {}
  itti_n4_session_failure_indication(
      const itti_n4_session_failure_indication& i)
      : itti_n4_msg(i) {
    pfcp_ies = i.pfcp_ies;
  }
  itti_n4_session_failure_indication(
      const itti_n4_session_failure_indication& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n4_msg(i, orig, dest) {
    pfcp_ies = i.pfcp_ies;
  }
  const char* get_msg_name() {
    return typeid(itti_n4_session_failure_indication).name();
  };

  pfcp::pfcp_session_modification_request pfcp_ies;
};

#endif /* ITTI_MSG_N4_HPP_INCLUDED_ */
