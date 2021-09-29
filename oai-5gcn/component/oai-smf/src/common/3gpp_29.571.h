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

#ifndef FILE_3GPP_29_571_SEEN
#define FILE_3GPP_29_571_SEEN

typedef struct session_ambr_s {
  std::string uplink;
  std::string downlink;
} session_ambr_t;

enum preemtion_capability_e { NOT_PREEMPT = 1, MAY_PREEMPT = 2 };

static const std::vector<std::string> preemtion_capability_e2str = {
    "Error", "NOT_PREEMPT", "MAY_PREEMPT"};

// see section 5.5.4.1@TS 29.571
typedef struct arp_5gc_s {
  uint8_t priority_level;  // (integer 1-15)
  std::string preempt_cap;
  std::string preempt_vuln;  // NOT_PREEMPTABLE, PREEMPTABLE
} arp_5gc_t;

// see section 5.4.4.1@TS 29.571
typedef struct subscribed_default_qos_s {
  uint8_t _5qi;
  arp_5gc_t arp;
  uint8_t priority_level;  // 1-127
} subscribed_default_qos_t;

enum reflective_qos_attribute_e { RQOS = 1, NO_RQOS = 2 };

static const std::vector<std::string> reflective_qos_attribute_e2str = {
    "ERROR", "RQOS", "NO_RQOS"};

#endif
