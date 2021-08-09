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
 *  itti_msg_s1u.hpp
 *
 *  Created on: Feb 05, 2019
 *  Author: lionel.gauthier@eurecom.fr
 */

#ifndef ITTI_MSG_S1U_HPP_INCLUDED_
#define ITTI_MSG_S1U_HPP_INCLUDED_

#include "itti_msg.hpp"
#include "3gpp_29.281.hpp"
#include "msg_gtpv1u.hpp"

#include <sys/socket.h>

class itti_s1u_msg : public itti_msg {
 public:
  itti_s1u_msg(
      const itti_msg_type_t msg_type, const task_id_t orig,
      const task_id_t dest)
      : itti_msg(msg_type, orig, dest) {}

  itti_s1u_msg& operator=(itti_s1u_msg other) {
    this->itti_msg::operator=(other);
    return *this;
  }

  itti_s1u_msg(const itti_s1u_msg& i) : itti_msg(i) {}

  itti_s1u_msg(
      const itti_s1u_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_s1u_msg(i) {
    origin      = orig;
    destination = dest;
  }
};
//------------------------------------------------------------------------------
class itti_s1u_echo_request : public itti_s1u_msg {
 public:
  itti_s1u_echo_request(const task_id_t orig, const task_id_t dest)
      : itti_s1u_msg(S1U_ECHO_REQUEST, orig, dest) {}
  explicit itti_s1u_echo_request(const itti_s1u_echo_request& i)
      : itti_s1u_msg(i) {
    gtp_ies = i.gtp_ies;
  }
  itti_s1u_echo_request& operator=(itti_s1u_echo_request other) {
    this->itti_s1u_msg::operator=(other);
    std::swap(gtp_ies, other.gtp_ies);
    return *this;
  }
  itti_s1u_echo_request(
      const itti_s1u_echo_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s1u_msg(i, orig, dest) {
    gtp_ies = i.gtp_ies;
  }
  const char* get_msg_name() { return "S1U_ECHO_REQUEST"; };

  gtpv1u::gtpv1u_echo_request gtp_ies;
};
//------------------------------------------------------------------------------
class itti_s1u_echo_response : public itti_s1u_msg {
 public:
  itti_s1u_echo_response(const task_id_t orig, const task_id_t dest)
      : itti_s1u_msg(S1U_ECHO_RESPONSE, orig, dest) {}
  explicit itti_s1u_echo_response(const itti_s1u_echo_response& i)
      : itti_s1u_msg(i) {
    gtp_ies = i.gtp_ies;
  }
  itti_s1u_echo_response& operator=(itti_s1u_echo_response other) {
    this->itti_s1u_msg::operator=(other);
    std::swap(gtp_ies, other.gtp_ies);
    return *this;
  }
  itti_s1u_echo_response(
      const itti_s1u_echo_response& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s1u_msg(i, orig, dest) {
    gtp_ies = i.gtp_ies;
  }
  const char* get_msg_name() { return "S1U_ECHO_RESPONSE"; };

  gtpv1u::gtpv1u_echo_response gtp_ies;
};
//------------------------------------------------------------------------------
class itti_s1u_error_indication : public itti_s1u_msg {
 public:
  itti_s1u_error_indication(const task_id_t orig, const task_id_t dest)
      : itti_s1u_msg(S1U_ERROR_INDICATION, orig, dest) {}
  explicit itti_s1u_error_indication(const itti_s1u_error_indication& i)
      : itti_s1u_msg(i) {
    gtp_ies = i.gtp_ies;
  }
  itti_s1u_error_indication& operator=(itti_s1u_error_indication other) {
    this->itti_s1u_msg::operator=(other);
    std::swap(gtp_ies, other.gtp_ies);
    return *this;
  }
  itti_s1u_error_indication(
      const itti_s1u_error_indication& i, const task_id_t orig,
      const task_id_t dest)
      : itti_s1u_msg(i, orig, dest) {
    gtp_ies = i.gtp_ies;
  }
  const char* get_msg_name() { return "S1U_ERROR_INDICATION"; };

  gtpv1u::gtpv1u_error_indication gtp_ies;
};
//------------------------------------------------------------------------------
class itti_s1u_supported_extension_headers_notification : public itti_s1u_msg {
 public:
  itti_s1u_supported_extension_headers_notification(
      const task_id_t orig, const task_id_t dest)
      : itti_s1u_msg(S1U_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION, orig, dest) {
  }
  explicit itti_s1u_supported_extension_headers_notification(
      const itti_s1u_supported_extension_headers_notification& i)
      : itti_s1u_msg(i) {
    gtp_ies = i.gtp_ies;
  }
  itti_s1u_supported_extension_headers_notification& operator=(
      itti_s1u_supported_extension_headers_notification other) {
    this->itti_s1u_msg::operator=(other);
    std::swap(gtp_ies, other.gtp_ies);
    return *this;
  }
  itti_s1u_supported_extension_headers_notification(
      const itti_s1u_supported_extension_headers_notification& i,
      const task_id_t orig, const task_id_t dest)
      : itti_s1u_msg(i, orig, dest) {
    gtp_ies = i.gtp_ies;
  }
  const char* get_msg_name() {
    return "S1U_SUPPORTED_EXTENSION_HEADERS_NOTIFICATION";
  };

  gtpv1u::gtpv1u_supported_extension_headers_notification gtp_ies;
};
//------------------------------------------------------------------------------
class itti_s1u_end_marker : public itti_s1u_msg {
 public:
  itti_s1u_end_marker(const task_id_t orig, const task_id_t dest)
      : itti_s1u_msg(S1U_END_MARKER, orig, dest) {}
  explicit itti_s1u_end_marker(const itti_s1u_end_marker& i) : itti_s1u_msg(i) {
    gtp_ies = i.gtp_ies;
  }

  itti_s1u_end_marker& operator=(itti_s1u_end_marker other) {
    this->itti_s1u_msg::operator=(other);
    std::swap(gtp_ies, other.gtp_ies);
    return *this;
  }

  itti_s1u_end_marker(
      const itti_s1u_end_marker& i, const task_id_t orig, const task_id_t dest)
      : itti_s1u_msg(i, orig, dest) {
    gtp_ies = i.gtp_ies;
  }
  const char* get_msg_name() { return "S1U_END_MARKER"; };

  gtpv1u::gtpv1u_end_marker gtp_ies;
};

#endif /* ITTI_MSG_S1U_HPP_INCLUDED_ */
