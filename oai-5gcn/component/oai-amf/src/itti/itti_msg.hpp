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
/*! \file itti_msg.hpp
 \brief
 \author  Lionel GAUTHIER
 \date 2018
 \email: lionel.gauthier@eurecom.fr
 */
#ifndef SRC_ITTI_ITTI_MSG_HPP_INCLUDED_
#define SRC_ITTI_ITTI_MSG_HPP_INCLUDED_

#include <stdint.h>

#include <utility>

typedef enum {
  TASK_FIRST      = 0,
  TASK_ITTI_TIMER = TASK_FIRST,
  TASK_ASYNC_SHELL_CMD,
  TASK_NGAP,
  TASK_AMF_N2,
  TASK_AMF_N1,
  TASK_AMF_N11,
  TASK_AMF_APP,
  AMF_SERVER,
  TASK_MAX,
  TASK_NONE,
  TASK_ALL = 255
} task_id_t;

typedef enum message_priorities_e {
  MESSAGE_PRIORITY_MAX       = 100,
  MESSAGE_PRIORITY_MAX_LEAST = 85,
  MESSAGE_PRIORITY_MED_PLUS  = 70,
  MESSAGE_PRIORITY_MED       = 55,
  MESSAGE_PRIORITY_MED_LEAST = 40,
  MESSAGE_PRIORITY_MIN_PLUS  = 25,
  MESSAGE_PRIORITY_MIN       = 10,
} message_priorities_t;

typedef enum {
  ITTI_MSG_TYPE_NONE  = -1,
  ITTI_MSG_TYPE_FIRST = 0,
  ASYNC_SHELL_CMD     = ITTI_MSG_TYPE_FIRST,
  NEW_SCTP_ASSOCIATION,
  NG_SETUP_REQ,
  NG_RESET,
  NG_SHUTDOWN,
  INITIAL_UE_MSG,
  ITTI_UL_NAS_TRANSPORT,
  ITTI_DL_NAS_TRANSPORT,
  INITIAL_CONTEXT_SETUP_REQUEST,
  PDU_SESSION_RESOURCE_SETUP_REQUEST,
  PDU_SESSION_RESOURCE_RELEASE_COMMAND,
  UE_CONTEXT_RELEASE_REQUEST,
  UE_CONTEXT_RELEASE_COMPLETE,
  UE_RADIO_CAP_IND,
  UL_NAS_DATA_IND,  // task amf_n1 message id
  DOWNLINK_NAS_TRANSFER,
  NAS_SIG_ESTAB_REQ,  // task amf_app
  N1N2_MESSAGE_TRANSFER_REQ,
  SMF_SERVICES_CONSUMER,
  NSMF_PDU_SESSION_UPDATE_SM_CTX,
  N11_REGISTER_NF_INSTANCE_REQUEST,
  N11_REGISTER_NF_INSTANCE_RESPONSE,
  N11_UPDATE_NF_INSTANCE_REQUEST,
  N11_UPDATE_NF_INSTANCE_RESPONSE,
  N11_DEREGISTER_NF_INSTANCE,
  UE_CONTEXT_RELEASE_COMMAND,
  NSMF_PDU_SESS_RELEASE_SMCTX,
  HANDOVER_REQUIRED,
  HANDOVER_REQUEST_ACK,
  HANDOVER_NOTIFY,
  UPLINKRANSTATUSTRANSFER,
  PDU_SESS_RES_SET_RESP,
  TIME_OUT,
  HEALTH_PING,
  TERMINATE,
  ITTI_MSG_TYPE_MAX
} itti_msg_type_t;

typedef unsigned long message_number_t;

class itti_msg {
 public:
  itti_msg();
  itti_msg(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination);
  itti_msg(const itti_msg& i);

  itti_msg& operator=(itti_msg other) {
    std::swap(msg_num, other.msg_num);
    std::swap(origin, other.origin);
    std::swap(destination, other.destination);
    std::swap(msg_type, other.msg_type);
    return *this;
  }

  virtual ~itti_msg() = default;
  static const char* get_msg_name();

  message_number_t msg_num;
  task_id_t origin;
  task_id_t destination;
  itti_msg_type_t msg_type;
};

class itti_msg_timeout : public itti_msg {
 public:
  itti_msg_timeout(
      const task_id_t origin, const task_id_t destination, uint32_t timer_id,
      uint64_t arg1_user, uint64_t arg2_user)
      : itti_msg(TIME_OUT, origin, destination),
        timer_id(timer_id),
        arg1_user(arg1_user),
        arg2_user(arg2_user) {}
  itti_msg_timeout(const itti_msg_timeout& i)
      : itti_msg(i),
        timer_id(i.timer_id),
        arg1_user(i.arg1_user),
        arg2_user(i.arg2_user) {}
  static const char* get_msg_name() { return "TIME_OUT"; };
  uint32_t timer_id;
  uint64_t arg1_user;
  uint64_t arg2_user;
};

class itti_msg_ping : public itti_msg {
 public:
  itti_msg_ping(
      const task_id_t origin, const task_id_t destination, uint32_t seq)
      : itti_msg(HEALTH_PING, origin, destination), seq(seq) {}
  itti_msg_ping(const itti_msg_ping& i) : itti_msg(i), seq(i.seq) {}
  static const char* get_msg_name() { return "HEALTH_PING"; };
  uint32_t seq;
};

class itti_msg_terminate : public itti_msg {
 public:
  itti_msg_terminate(const task_id_t origin, const task_id_t destination)
      : itti_msg(TERMINATE, origin, destination) {}
  itti_msg_terminate(const itti_msg_terminate& i) : itti_msg(i) {}
  static const char* get_msg_name() { return "TERMINATE"; };
};

#endif /* SRC_ITTI_ITTI_MSG_HPP_INCLUDED_ */
