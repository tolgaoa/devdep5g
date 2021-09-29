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
 *  itti_msg_sbi.hpp
 *
 *  Created on:
 *  Author:
 */

#ifndef ITTI_MSG_SBI_HPP_INCLUDED_
#define ITTI_MSG_SBI_HPP_INCLUDED_

#include "itti_msg.hpp"
#include "pistache/http.h"
#include "smf_msg.hpp"

class itti_sbi_msg : public itti_msg {
 public:
  itti_sbi_msg(
      const itti_msg_type_t msg_type, const task_id_t orig,
      const task_id_t dest)
      : itti_msg(msg_type, orig, dest) {}
  itti_sbi_msg(const itti_sbi_msg& i) : itti_msg(i) {}
  itti_sbi_msg(
      const itti_sbi_msg& i, const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(i) {
    origin      = orig;
    destination = dest;
  }
};

//-----------------------------------------------------------------------------
class itti_sbi_event_exposure_request : public itti_sbi_msg {
 public:
  itti_sbi_event_exposure_request(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_EVENT_EXPOSURE_REQUEST, orig, dest),
        event_exposure(),
        http_version(1) {}
  itti_sbi_event_exposure_request(const itti_sbi_event_exposure_request& i)
      : itti_sbi_msg(i), event_exposure(i.event_exposure), http_version(1) {}
  itti_sbi_event_exposure_request(
      const itti_sbi_event_exposure_request& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        event_exposure(i.event_exposure),
        http_version(i.http_version) {}
  const char* get_msg_name() { return "SBI_EVENT_EXPOSURE_REQUEST"; };
  smf::event_exposure_msg event_exposure;
  uint8_t http_version;
};

//-----------------------------------------------------------------------------
class itti_sbi_notification_data : public itti_sbi_msg {
 public:
  itti_sbi_notification_data(const task_id_t orig, const task_id_t dest)
      : itti_sbi_msg(SBI_NOTIFICATION_DATA, orig, dest),
        notification_msg(),
        http_version(1) {}
  itti_sbi_notification_data(const itti_sbi_notification_data& i)
      : itti_sbi_msg(i),
        notification_msg(i.notification_msg),
        http_version(1) {}
  itti_sbi_notification_data(
      const itti_sbi_notification_data& i, const task_id_t orig,
      const task_id_t dest)
      : itti_sbi_msg(i, orig, dest),
        notification_msg(i.notification_msg),
        http_version(i.http_version) {}
  const char* get_msg_name() { return "SBI_NOTIFICATION_DATA"; };
  smf::data_notification_msg notification_msg;
  uint8_t http_version;
};

#endif /* ITTI_MSG_SBI_HPP_INCLUDED_ */
