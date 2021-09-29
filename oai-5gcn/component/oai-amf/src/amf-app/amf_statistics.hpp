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

/*! \file amf_statistics.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <string>
#include <vector>

#include "amf.hpp"
#include "ngap_app.hpp"

typedef struct {
  uint32_t gnb_id;
  // TODO: list of PLMNs
  std::vector<SupportedItem_t> plmn_list;
  std::string mcc;
  std::string mnc;
  std::string gnb_name;
  uint32_t tac;
  // long nrCellId;
  std::string plmn_to_string() const {
    std::string s = {};
    for (auto supported_item : plmn_list) {
      s.append("TAC " + std::to_string(supported_item.tac));
      for (auto plmn_slice : supported_item.b_plmn_list) {
        s.append("( MCC " + plmn_slice.mcc);
        s.append(", MNC " + plmn_slice.mnc);
        for (auto slice : plmn_slice.slice_list) {
          s.append("(SST " + slice.sst + ", SD " + slice.sd + "),");
        }
        s.append(")");
      }
      s.append("),");
    }
    return s;
  }
} gnb_infos;

typedef struct ue_info_s {
  std::string connStatus;
  std::string registerStatus;
  uint32_t ranid;
  long amfid;
  std::string imsi;
  std::string guti;
  std::string mcc;
  std::string mnc;
  uint32_t cellId;
} ue_info_t;

class statistics {
 public:
  void display();
  statistics();
  ~statistics();
  void update_ue_info(const ue_info_t& ue_info);
  void update_5gmm_state(const std::string& imsi, const std::string& state);
  void remove_gnb(const uint32_t gnb_id);

 public:
  uint32_t gNB_connected;
  uint32_t UE_connected;
  uint32_t UE_registred;
  // uint32_t        system_pdu_sessions;
  std::map<uint32_t, gnb_infos> gnbs;
  std::map<std::string, ue_info_t> ue_infos;
};

#endif
