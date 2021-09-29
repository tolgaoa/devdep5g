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

/*! \file amf_n2.hpp
 \brief
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _AMF_N2_H_
#define _AMF_N2_H_

#include "DownlinkRANStatusTransfer.hpp"
#include "HandoverCommandMsg.hpp"
#include "HandoverRequest.hpp"
#include "PduSessionResourceReleaseCommand.hpp"
#include "amf.hpp"
#include "itti_msg_n2.hpp"
#include "ngap_app.hpp"
#include "ue_ngap_context.hpp"

namespace amf_application {

class amf_n2 : public ngap::ngap_app {
 public:
  amf_n2(const std::string& address, const uint16_t port_num);
  ~amf_n2();
  void handle_itti_message(itti_new_sctp_association& new_assoc);
  void handle_itti_message(itti_ng_setup_request& ngsetupreq);
  void handle_itti_message(itti_ng_reset&);
  void handle_itti_message(itti_ng_shutdown&);
  void handle_itti_message(itti_initial_ue_message& init_ue_msg);
  void handle_itti_message(itti_ul_nas_transport& ul_nas_transport);
  void handle_itti_message(itti_dl_nas_transport& dl_nas_transport);
  void handle_itti_message(itti_initial_context_setup_request& itti_msg);
  void handle_itti_message(itti_pdu_session_resource_setup_request& itti_msg);
  void handle_itti_message(itti_ue_context_release_request& itti_msg);
  void handle_itti_message(itti_ue_context_release_complete& itti_msg);
  void handle_itti_message(itti_ue_radio_capability_indication& itti_msg);
  void handle_itti_message(itti_ue_context_release_command& itti_msg);
  void handle_itti_message(itti_pdu_session_resource_release_command& itti_msg);
  void handle_itti_message(itti_handover_required& itti_msg);
  void handle_itti_message(itti_handover_request_Ack& itti_msg);
  void handle_itti_message(itti_handover_notify& itti_msg);
  void handle_itti_message(itti_uplinkranstatsutransfer& itti_msg);

  bool verifyPlmn(std::vector<SupportedItem_t> list);
  std::vector<SupportedItem_t> get_common_plmn(
      std::vector<SupportedItem_t> list);
  std::shared_ptr<ue_ngap_context> ran_ue_id_2_ue_ngap_context(
      const uint32_t& ran_ue_ngap_id) const;

  bool is_ran_ue_id_2_ue_ngap_context(const uint32_t& ran_ue_ngap_id) const;

  void set_ran_ue_ngap_id_2_ue_ngap_context(
      const uint32_t& ran_ue_ngap_id, std::shared_ptr<ue_ngap_context> unc);

 private:
  std::map<uint32_t, std::shared_ptr<ue_ngap_context>>
      ranid2uecontext;  // ran ue ngap id
  mutable std::shared_mutex m_ranid2uecontext;
};

}  // namespace amf_application

#endif
