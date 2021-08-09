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

#ifndef FILE_3GPP_29_503_SMF_SEEN
#define FILE_3GPP_29_503_SMF_SEEN

#include "smf.h"
#include "3gpp_29.571.h"

enum ssc_mode_e {
  SSC_MODE_1 = 1,
  SSC_MODE_2 = 2,
  SSC_MODE_3 = 3,
};
static const std::vector<std::string> ssc_mode_e2str = {
    "Error", "SSC_MODE_1", "SSC_MODE_2", "SSC_MODE_3"};

typedef struct ssc_mode_s {
  uint8_t ssc_mode;
  ssc_mode_s() : ssc_mode(SSC_MODE_1) {}
  ssc_mode_s(ssc_mode_e mode) : ssc_mode(mode) {}
  ssc_mode_s(const struct ssc_mode_s& p) : ssc_mode(p.ssc_mode) {}
} ssc_mode_t;

typedef struct pdu_session_types_s {
  pdu_session_type_t default_session_type;
  std::vector<pdu_session_type_t> allowed_session_types;
} pdu_session_types_t;

typedef struct ssc_modes_s {
  ssc_mode_t default_ssc_mode;
  std::vector<ssc_mode_t> allowed_ssc_modes;
} ssc_modes_t;

typedef struct dnn_configuration_s {
  pdu_session_types_t pdu_session_types;
  ssc_modes_t ssc_modes;
  session_ambr_t session_ambr;
  subscribed_default_qos_t _5g_qos_profile;
  // staticIpAddresses
} dnn_configuration_t;

#endif
