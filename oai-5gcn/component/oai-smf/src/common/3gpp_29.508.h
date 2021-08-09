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

#ifndef FILE_3GPP_29_508_SMF_SEEN
#define FILE_3GPP_29_508_SMF_SEEN

#include "smf.h"

typedef enum smf_event_e {
  SMF_EVENT_AC_TY_CH    = 1,
  SMF_EVENT_UP_PATH_CH  = 2,
  SMF_EVENT_PDU_SES_REL = 3,
  SMF_EVENT_PLMN_CH     = 4,
  SMF_EVENT_UE_IP_CH    = 5,
  SMF_EVENT_DDDS        = 6
} smf_event_t;

static const std::vector<std::string> smf_event_e2str = {
    "SMF_EVENT_UNKNOWN",
    "Access Type Change",
    "UP Path Change",
    "PDU Session Release",
    "PLMN Change",
    "UE IP address change",
    "Downlink data delivery status"};

enum class notification_method_e {
  PERIODIC           = 1,
  ONE_TIME           = 2,
  ON_EVENT_DETECTION = 3
};

static const std::vector<std::string> notification_method_e2str = {
    "NOTIFICATION_METHOD_UNKNOWN", "PERIODIC", "ONE_TIME",
    "ON_EVENT_DETECTION"};

enum class dnai_change_type_e { EARLY = 1, EARLY_LATE = 2, LATE = 3 };
enum class ddd_status_e { BUFFERED = 1, TRANSMITTED = 2, DISCARDED = 3 };

typedef struct event_subscription_s {
  smf_event_t smf_event;
  dnai_change_type_e dnai_change_type;
  // DddTrafficDescriptor
  std::vector<ddd_status_e> ddd_status;

} event_subscription_t;

#endif
