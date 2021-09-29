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
 *  itti_msg_n11.hpp
 *
 *  Created on:
 *  Author:
 */

#ifndef ITTI_MSG_N11_HPP_INCLUDED_
#define ITTI_MSG_N11_HPP_INCLUDED_

#include <nlohmann/json.hpp>
#include "itti_msg.hpp"
#include "smf_msg.hpp"
#include "pistache/http.h"
#include "smf_profile.hpp"
#include "PatchItem.h"

class itti_n11_msg : public itti_msg {
 public:
  itti_n11_msg(
      const itti_msg_type_t msg_type, const task_id_t orig,
      const task_id_t dest)
      : itti_msg(msg_type, orig, dest) {}
  itti_n11_msg(const itti_n11_msg& i) : itti_msg(i) {}
  itti_n11_msg(
      const itti_n11_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(i) {
    origin      = orig;
    destination = dest;
  }
};

//-----------------------------------------------------------------------------
class itti_n11_create_sm_context_request : public itti_n11_msg {
 public:
  itti_n11_create_sm_context_request(
      const task_id_t orig, const task_id_t dest, uint32_t promise_id)
      : itti_n11_msg(N11_SESSION_CREATE_SM_CONTEXT_REQUEST, orig, dest),
        pid(promise_id),
        req(),
        scid(),
        http_version(1) {}
  itti_n11_create_sm_context_request(
      const itti_n11_create_sm_context_request& i)
      : itti_n11_msg(i), pid(), req(i.req), scid(), http_version(1) {}
  itti_n11_create_sm_context_request(
      const itti_n11_create_sm_context_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest),
        pid(i.pid),
        req(i.req),
        scid(i.scid),
        http_version(i.http_version) {}
  const char* get_msg_name() {
    return "N11_SESSION_CREATE_SM_CONTEXT_REQUEST";
  };
  void set_scid(scid_t id) { scid = id; };
  smf::pdu_session_create_sm_context_request req;
  scid_t scid;   // SM Context ID
  uint32_t pid;  // Promise Id
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_n11_create_sm_context_response : public itti_n11_msg {
 public:
  itti_n11_create_sm_context_response(
      const task_id_t orig, const task_id_t dest, uint32_t promise_id)
      : itti_n11_msg(N11_SESSION_CREATE_SM_CONTEXT_RESPONSE, orig, dest),
        pid(promise_id),
        scid(0),
        http_version() {}
  itti_n11_create_sm_context_response(
      const itti_n11_create_sm_context_response& i)
      : itti_n11_msg(i),
        pid(i.pid),
        res(i.res),
        scid(i.scid),
        http_version(i.http_version) {}
  itti_n11_create_sm_context_response(
      const itti_n11_create_sm_context_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest),
        pid(i.pid),
        res(i.res),
        scid(i.scid),
        http_version(i.http_version) {}
  const char* get_msg_name() {
    return "N11_SESSION_CREATE_SM_CONTEXT_RESPONSE";
  };
  void set_scid(scid_t id) { scid = id; };

  smf::pdu_session_create_sm_context_response res;
  scid_t scid;           // SM Context ID
  uint32_t pid;          // Promise Id
  uint8_t http_version;  // HTTP version
};

//-----------------------------------------------------------------------------
class itti_n11_update_sm_context_request : public itti_n11_msg {
 public:
  itti_n11_update_sm_context_request(
      const task_id_t orig, const task_id_t dest, uint32_t promise_id)
      : itti_n11_msg(N11_SESSION_UPDATE_SM_CONTEXT_REQUEST, orig, dest),
        pid(promise_id),
        scid(),
        req(),
        http_version() {}
  itti_n11_update_sm_context_request(
      const task_id_t orig, const task_id_t dest, int32_t promise_id,
      const std::string id)
      : itti_n11_msg(N11_SESSION_UPDATE_SM_CONTEXT_REQUEST, orig, dest),
        req(),
        pid(promise_id),
        scid(id),
        http_version() {}
  itti_n11_update_sm_context_request(
      const itti_n11_update_sm_context_request& i)
      : itti_n11_msg(i),
        req(i.req),
        pid(i.pid),
        scid(i.scid),
        http_version(i.http_version) {}

  const char* get_msg_name() {
    return "N11_SESSION_UPDATE_SM_CONTEXT_REQUEST";
  };
  smf::pdu_session_update_sm_context_request req;
  uint32_t pid;
  std::string scid;  // SM Context ID
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_n11_update_sm_context_response : public itti_n11_msg {
 public:
  itti_n11_update_sm_context_response(
      const task_id_t orig, const task_id_t dest, uint32_t promise_id)
      : itti_n11_msg(N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE, orig, dest),
        pid(promise_id),
        res(),
        session_procedure_type() {}
  itti_n11_update_sm_context_response(
      const itti_n11_update_sm_context_response& i)
      : itti_n11_msg(i),
        res(i.res),
        pid(i.pid),
        session_procedure_type(i.session_procedure_type) {}
  itti_n11_update_sm_context_response(
      const itti_n11_update_sm_context_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest),
        res(i.res),
        pid(i.pid),
        session_procedure_type(i.session_procedure_type) {}
  const char* get_msg_name() {
    return "N11_SESSION_UPDATE_SM_CONTEXT_RESPONSE";
  };
  smf::pdu_session_update_sm_context_response res;
  uint32_t pid;  // promise Id
  session_management_procedures_type_e session_procedure_type;
};

//-----------------------------------------------------------------------------
class itti_n11_update_pdu_session_status : public itti_n11_msg {
 public:
  itti_n11_update_pdu_session_status(const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_SESSION_UPDATE_PDU_SESSION_STATUS, orig, dest),
        scid(0),
        pdu_session_status(pdu_session_status_e::PDU_SESSION_INACTIVE) {}
  itti_n11_update_pdu_session_status(
      const itti_n11_update_pdu_session_status& i)
      : itti_n11_msg(i),
        scid(i.scid),
        pdu_session_status(i.pdu_session_status) {}
  itti_n11_update_pdu_session_status(
      const itti_n11_update_pdu_session_status& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest),
        scid(i.scid),
        pdu_session_status(i.pdu_session_status) {}
  const char* get_msg_name() {
    return "N11_SESSION_UPDATE_PDU_SESSION_STATUS";
  };
  void set_scid(scid_t id) { scid = id; };
  scid_t scid;  // SM Context ID
  pdu_session_status_e pdu_session_status;
  void set_pdu_session_status(pdu_session_status_e status) {
    pdu_session_status = status;
  };
};

//-----------------------------------------------------------------------------
class itti_n11_n1n2_message_transfer_response_status : public itti_n11_msg {
 public:
  itti_n11_n1n2_message_transfer_response_status(
      const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(
            N11_SESSION_N1N2_MESSAGE_TRANSFER_RESPONSE_STATUS, orig, dest),
        scid(0),
        response_code(0),
        msg_type(0),
        procedure_type(
            session_management_procedures_type_e::PROCEDURE_TYPE_UNKNOWN),
        seid(0),
        trxn_id(0) {}
  itti_n11_n1n2_message_transfer_response_status(
      const itti_n11_n1n2_message_transfer_response_status& i)
      : itti_n11_msg(i),
        scid(i.scid),
        response_code(i.response_code),
        msg_type(i.msg_type),
        procedure_type(i.procedure_type),
        seid(i.seid),
        trxn_id(i.trxn_id) {}
  itti_n11_n1n2_message_transfer_response_status(
      const itti_n11_n1n2_message_transfer_response_status& i,
      const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(i, orig, dest),
        scid(i.scid),
        response_code(i.response_code),
        msg_type(i.msg_type),
        procedure_type(i.procedure_type),
        seid(i.seid),
        trxn_id(i.trxn_id) {}
  const char* get_msg_name() {
    return "N11_SESSION_N1N2_MESSAGE_TRANSFER_RESPONSE_STATUS";
  };
  void set_scid(scid_t id) { scid = id; };
  void set_response_code(int16_t code) { response_code = code; };
  void set_cause(std::string c) { cause = c; };
  void set_msg_type(uint8_t type) { msg_type = type; };
  void set_procedure_type(session_management_procedures_type_e type) {
    procedure_type = type;
  };

  void set_seid(const seid_t& s) { seid = s; };

  void set_trxn_id(uint64_t t) { trxn_id = t; };

  scid_t scid;  // SM Context ID
  int16_t response_code;
  std::string cause;
  uint8_t msg_type;
  session_management_procedures_type_e procedure_type;
  seid_t seid;
  uint64_t trxn_id;
};

//-----------------------------------------------------------------------------
class itti_n11_release_sm_context_request : public itti_n11_msg {
 public:
  itti_n11_release_sm_context_request(
      const task_id_t orig, const task_id_t dest, uint32_t promise_id)
      : itti_n11_msg(N11_SESSION_RELEASE_SM_CONTEXT_REQUEST, orig, dest),
        pid(promise_id),
        http_version(1) {}
  itti_n11_release_sm_context_request(
      const task_id_t orig, const task_id_t dest, uint32_t promise_id,
      const std::string id)
      : itti_n11_msg(N11_SESSION_RELEASE_SM_CONTEXT_REQUEST, orig, dest),
        pid(promise_id),
        scid(id),
        http_version(1) {}
  itti_n11_release_sm_context_request(
      const itti_n11_release_sm_context_request& i)
      : itti_n11_msg(i),
        pid(i.pid),
        scid(i.scid),
        req(i.req),
        http_version(i.http_version) {}
  itti_n11_release_sm_context_request(
      const itti_n11_release_sm_context_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest),
        pid(i.pid),
        scid(i.scid),
        req(i.req),
        http_version(i.http_version) {}
  const char* get_msg_name() {
    return "N11_SESSION_RELEASE_SM_CONTEXT_REQUEST";
  };
  smf::pdu_session_release_sm_context_request req;
  std::string scid;      // SM Context ID
  uint32_t pid;          // Promise Id
  uint8_t http_version;  // HTTP version
};

//-----------------------------------------------------------------------------
class itti_n11_release_sm_context_response : public itti_n11_msg {
 public:
  itti_n11_release_sm_context_response(
      const task_id_t orig, const task_id_t dest, uint32_t promise_id)
      : itti_n11_msg(N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE, orig, dest),
        pid(promise_id),
        res() {}
  itti_n11_release_sm_context_response(
      const itti_n11_release_sm_context_response& i)
      : itti_n11_msg(i), res(i.res), pid(i.pid) {}
  itti_n11_release_sm_context_response(
      const itti_n11_release_sm_context_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest), res(i.res), pid(i.pid) {}
  const char* get_msg_name() {
    return "N11_SESSION_RELEASE_SM_CONTEXT_RESPONSE";
  };
  smf::pdu_session_release_sm_context_response res;
  uint32_t pid;  // Promise Id
};

//-----------------------------------------------------------------------------
class itti_n11_session_report_request : public itti_n11_msg {
 public:
  itti_n11_session_report_request(const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_SESSION_REPORT_RESPONSE, orig, dest),
        res(),
        http_version() {}
  itti_n11_session_report_request(const itti_n11_session_report_request& i)
      : itti_n11_msg(i), res(i.res), http_version(i.http_version) {}
  itti_n11_session_report_request(
      const itti_n11_session_report_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest), res(i.res), http_version(i.http_version) {}
  const char* get_msg_name() { return "N11_SESSION_REPORT_RESPONSE"; };
  smf::pdu_session_report_response res;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_n11_notify_sm_context_status : public itti_n11_msg {
 public:
  itti_n11_notify_sm_context_status(const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_SESSION_NOTIFY_SM_CONTEXT_STATUS, orig, dest),
        scid(0),
        sm_context_status(),
        amf_status_uri(),
        http_version() {}

  itti_n11_notify_sm_context_status(const itti_n11_notify_sm_context_status& i)
      : itti_n11_msg(i),
        scid(i.scid),
        sm_context_status(i.sm_context_status),
        amf_status_uri(i.amf_status_uri),
        http_version(i.http_version) {}
  itti_n11_notify_sm_context_status(
      const itti_n11_notify_sm_context_status& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest),
        scid(i.scid),
        sm_context_status(i.sm_context_status),
        amf_status_uri(i.amf_status_uri),
        http_version(i.http_version) {}
  const char* get_msg_name() { return "N11_SESSION_NOTIFY_SM_CONTEXT_STATUS"; };
  void set_scid(scid_t id) { scid = id; };
  void set_sm_context_status(std::string status) {
    sm_context_status = status;
  };
  scid_t scid;  // SM Context ID
  std::string sm_context_status;
  std::string amf_status_uri;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_n11_notify_subscribed_event : public itti_n11_msg {
 public:
  itti_n11_notify_subscribed_event(const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_NOTIFY_SUBSCRIBED_EVENT, orig, dest),
        notif_id(),
        http_version() {}

  itti_n11_notify_subscribed_event(const itti_n11_notify_subscribed_event& i)
      : itti_n11_msg(i), notif_id(i.notif_id), http_version(i.http_version) {}
  itti_n11_notify_subscribed_event(
      const itti_n11_notify_subscribed_event& i, const task_id_t orig,
      const task_id_t dest)
      : itti_n11_msg(i, orig, dest),
        notif_id(i.notif_id),
        http_version(i.http_version) {}
  const char* get_msg_name() { return "N11_NOTIFY_SUBSCRIBED_EVENT"; };

  std::string notif_id;
  std::vector<smf::event_notification> event_notifs;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_n11_register_nf_instance_request : public itti_n11_msg {
 public:
  itti_n11_register_nf_instance_request(
      const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_REGISTER_NF_INSTANCE_REQUEST, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "N11_REGISTER_NF_INSTANCE_REQUEST"; };

  smf::smf_profile profile;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_n11_register_nf_instance_response : public itti_n11_msg {
 public:
  itti_n11_register_nf_instance_response(
      const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_REGISTER_NF_INSTANCE_RESPONSE, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "N11_REGISTER_NF_INSTANCE_RESPONSE"; };

  smf::smf_profile profile;
  uint8_t http_version;
  uint8_t http_response_code;
};

//-----------------------------------------------------------------------------
class itti_n11_update_nf_instance_request : public itti_n11_msg {
 public:
  itti_n11_update_nf_instance_request(
      const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_UPDATE_NF_INSTANCE_REQUEST, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "N11_UPDATE_NF_INSTANCE_REQUEST"; };

  std::vector<oai::smf_server::model::PatchItem> patch_items;
  uint8_t http_version;
  std::string smf_instance_id;
};

//-----------------------------------------------------------------------------
class itti_n11_update_nf_instance_response : public itti_n11_msg {
 public:
  itti_n11_update_nf_instance_response(
      const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_UPDATE_NF_INSTANCE_RESPONSE, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "N11_UPDATE_NF_INSTANCE_RESPONSE"; };

  uint8_t http_version;
  std::string smf_instance_id;
  uint8_t http_response_code;
};

//-----------------------------------------------------------------------------
class itti_n11_deregister_nf_instance : public itti_n11_msg {
 public:
  itti_n11_deregister_nf_instance(const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_DEREGISTER_NF_INSTANCE, orig, dest), http_version(1) {}
  const char* get_msg_name() { return "N11_DEREGISTER_NF_INSTANCE"; };

  uint8_t http_version;
  std::string smf_instance_id;
};

//-----------------------------------------------------------------------------
class itti_n11_subscribe_upf_status_notify : public itti_n11_msg {
 public:
  itti_n11_subscribe_upf_status_notify(
      const task_id_t orig, const task_id_t dest)
      : itti_n11_msg(N11_SUBSCRIBE_UPF_STATUS_NOTIFY, orig, dest),
        http_version(1) {}
  const char* get_msg_name() { return "N11_SUBSCRIBE_UPF_STATUS_NOTIFY"; };

  std::string url;
  nlohmann::json json_data;
  uint8_t http_version;
};

#endif /* ITTI_MSG_N11_HPP_INCLUDED_ */
