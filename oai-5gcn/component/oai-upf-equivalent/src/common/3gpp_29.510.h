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

#ifndef FILE_3GPP_29_510_NRF_SEEN
#define FILE_3GPP_29_510_NRF_SEEN

#include <vector>
#include <nlohmann/json.hpp>

typedef struct s_nssai  // section 28.4, TS23.003
{
  uint8_t sST;
  std::string sD;
  s_nssai(const uint8_t& sst, const std::string sd) : sST(sst), sD(sd) {}
  s_nssai() : sST(), sD() {}
  s_nssai(const s_nssai& p) : sST(p.sST), sD(p.sD) {}
  bool operator==(const struct s_nssai& s) const {
    if ((s.sST == this->sST) && (s.sD.compare(this->sD) == 0)) {
      return true;
    } else {
      return false;
    }
  }

} snssai_t;

typedef struct dnai_s {
} dnai_t;

typedef struct dnn_upf_info_item_s {
  std::string dnn;
  // std::vector<std::string> dnai_list
  // std::vector<std::string> pdu_session_types
} dnn_upf_info_item_t;

typedef struct snssai_upf_info_item_s {
  snssai_t snssai;
  std::vector<dnn_upf_info_item_t> dnn_upf_info_list;
} snssai_upf_info_item_t;

typedef struct upf_info_s {
  std::vector<snssai_upf_info_item_t> snssai_upf_info_list;
  void add_snssai(snssai_t snssai, std::string dnn) {
    bool found                   = false;
    dnn_upf_info_item_t dnn_item = {};
    dnn_item.dnn                 = dnn;
    for (int i = 0; i < snssai_upf_info_list.size(); i++) {
      if (snssai_upf_info_list[i].snssai == snssai) {
        for (int j = 0; j < snssai_upf_info_list[i].dnn_upf_info_list.size();
             j++) {
          if (snssai_upf_info_list[i].dnn_upf_info_list[j].dnn.compare(dnn) !=
              0) {
            snssai_upf_info_list[i].dnn_upf_info_list.push_back(dnn_item);
            break;
          }
        }
        found = true;
      }
    }
    if (!found) {
      snssai_upf_info_item_t snssai_item = {};
      snssai_item.snssai                 = snssai;
      snssai_item.dnn_upf_info_list.push_back(dnn_item);
      snssai_upf_info_list.push_back(snssai_item);
    }
  }
} upf_info_t;

typedef struct patch_item_s {
  std::string op;
  std::string path;
  // std::string from;
  std::string value;

  nlohmann::json to_json() const {
    nlohmann::json json_data = {};
    json_data["op"]          = op;
    json_data["path"]        = path;
    json_data["value"]       = value;
    return json_data;
  }
} patch_item_t;

#define NRF_CURL_TIMEOUT_MS 100L
#define NNRF_NFM_BASE "/nnrf-nfm/"
#define NNRF_NF_REGISTER_URL "/nf-instances/"

#endif
