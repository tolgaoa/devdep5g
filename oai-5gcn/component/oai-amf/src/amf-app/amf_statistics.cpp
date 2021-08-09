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

/*! \file amf_statistics.cpp
 \brief
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#include "amf_statistics.hpp"

#include "logger.hpp"

//------------------------------------------------------------------------------
void statistics::display() {
  Logger::amf_app().info("");

  Logger::amf_app().info(
      "|-----------------------------------------------------------------------"
      "-----------------------------------------|");
  Logger::amf_app().info(
      "|----------------------------------------------------gNBs' "
      "information-------------------------------------------|");
  Logger::amf_app().info(
      "|    Index    |      Status      |       Global ID       |       gNB "
      "Name       |               PLMN             |");
  if (gnbs.size() == 0) {
    Logger::amf_app().info(
        "|      -      |          -       |           -           |           "
        "-          |               -                |");
  }

  int i = 1;
  for (auto const& gnb : gnbs) {
    Logger::amf_app().info(
        "|      %d      |    Connected     |         0x%x       |         %s   "
        "     |            %s, %s             | ",
        i, gnb.second.gnb_id, gnb.second.gnb_name.c_str(),
        gnb.second.mcc.c_str(), gnb.second.mnc.c_str());
    // Comment out to show the supported TA list
    // Logger::amf_app().info(
    //    "| Supported TA list: %s|", gnb.second.plmn_to_string().c_str());
    i++;
  }

  Logger::amf_app().info(
      "|-----------------------------------------------------------------------"
      "-----------------------------------------|");
  Logger::amf_app().info("");

  Logger::amf_app().info(
      "|-----------------------------------------------------------------------"
      "-----------------------------------------|");
  Logger::amf_app().info(
      "|----------------------------------------------------UEs' "
      "information--------------------------------------------|");
  Logger::amf_app().info(
      "| Index |      5GMM state      |      IMSI        |     GUTI      | RAN "
      "UE NGAP ID | AMF UE ID |  PLMN   |Cell ID|");

  i = 0;
  for (auto const& ue : ue_infos) {
    Logger::amf_app().info(
        "|%7d|%22s|%18s|%15s|%16d|%11d| %3s,%3s |%7d|", i + 1,
        ue.second.registerStatus.c_str(), ue.second.imsi.c_str(),
        ue.second.guti.c_str(), ue.second.ranid, ue.second.amfid,
        ue.second.mcc.c_str(), ue.second.mnc.c_str(), ue.second.cellId);
    i++;
  }
  Logger::amf_app().info(
      "|-----------------------------------------------------------------------"
      "-----------------------------------------|");
  Logger::amf_app().info("");
}

//------------------------------------------------------------------------------
statistics::statistics() {
  gNB_connected = 0;
  UE_connected  = 0;
  UE_registred  = 0;
}

//------------------------------------------------------------------------------
void statistics::update_ue_info(const ue_info_t& ue_info) {
  if (!(ue_info.imsi.size() > 0)) {
    Logger::amf_app().warn("Update UE Info with invalid IMSI");
  }

  if (ue_infos.count(ue_info.imsi) > 0) {
    ue_infos.erase(ue_info.imsi);
    ue_infos.insert(std::pair<std::string, ue_info_t>(ue_info.imsi, ue_info));
    Logger::amf_app().debug(
        "Update UE Info (IMSI %s) success", ue_info.imsi.c_str());
  } else {
    ue_infos.insert(std::pair<std::string, ue_info_t>(ue_info.imsi, ue_info));
    Logger::amf_app().debug(
        "Add UE Info (IMSI %s) success", ue_info.imsi.c_str());
  }
}

//------------------------------------------------------------------------------
void statistics::update_5gmm_state(
    const std::string& imsi, const std::string& state) {
  if (ue_infos.count(imsi) > 0) {
    ue_info_t ue_info      = ue_infos.at(imsi);
    ue_info.registerStatus = state;
    ue_infos.erase(ue_info.imsi);
    ue_infos.insert(std::pair<std::string, ue_info_t>(imsi, ue_info));
    Logger::amf_app().debug(
        "Update UE State (IMSI %s, State %s) success", imsi.c_str(),
        state.c_str());
  } else {
    Logger::amf_app().warn(
        "Update UE State (IMSI %s), UE does not exist!", imsi.c_str());
  }
}

void statistics::remove_gnb(const uint32_t gnb_id) {}
//------------------------------------------------------------------------------
statistics::~statistics() {}
