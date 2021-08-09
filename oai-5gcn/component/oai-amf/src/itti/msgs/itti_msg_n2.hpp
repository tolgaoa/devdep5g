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

#ifndef _ITTI_MSG_N2_H_
#define _ITTI_MSG_N2_H_

#include "HandoverNotifyMsg.hpp"
#include "HandoverRequestAck.hpp"
#include "HandoverRequiredMsg.hpp"
#include "InitialUEMessage.hpp"
#include "NGReset.hpp"
#include "NGSetupRequest.hpp"
#include "UEContextReleaseRequest.hpp"
#include "UEContextReleaseComplete.hpp"
#include "UERadioCapabilityInfoIndication.hpp"
#include "UplinkNASTransport.hpp"
#include "UplinkRANStatusTransfer.hpp"
#include "itti_msg.hpp"
#include "sctp_server.hpp"
using namespace ngap;
using namespace sctp;

class itti_msg_n2 : public itti_msg {
 public:
  itti_msg_n2(
      const itti_msg_type_t msg_type, const task_id_t origin,
      const task_id_t destination)
      : itti_msg(msg_type, origin, destination) {}
  itti_msg_n2(const itti_msg_n2& i) : itti_msg(i) {
    assoc_id = i.assoc_id;
    stream   = i.stream;
  }
  sctp_assoc_id_t assoc_id;
  sctp_stream_id_t stream;
};

class itti_new_sctp_association : public itti_msg_n2 {
 public:
  itti_new_sctp_association(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(NEW_SCTP_ASSOCIATION, origin, destination) {}
};

class itti_ng_setup_request : public itti_msg_n2 {
 public:
  itti_ng_setup_request(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(NG_SETUP_REQ, origin, destination) {}
  itti_ng_setup_request(const itti_ng_setup_request& i) : itti_msg_n2(i) {}

 public:
  NGSetupRequestMsg* ngSetupReq;
};

class itti_ng_reset : public itti_msg_n2 {
 public:
  itti_ng_reset(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(NG_RESET, origin, destination) {}
  itti_ng_reset(const itti_ng_reset& i) : itti_msg_n2(i) {}

 public:
  NGResetMsg* ngReset;
};

class itti_ng_shutdown : public itti_msg_n2 {
 public:
  itti_ng_shutdown(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(NG_SHUTDOWN, origin, destination) {}
  itti_ng_shutdown(const itti_ng_shutdown& i) : itti_msg_n2(i) {}
};

class itti_initial_ue_message : public itti_msg_n2 {
 public:
  itti_initial_ue_message(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(INITIAL_UE_MSG, origin, destination) {}
  itti_initial_ue_message(const itti_initial_ue_message& i) : itti_msg_n2(i) {}

  InitialUEMessageMsg* initUeMsg;
};

class itti_ul_nas_transport : public itti_msg_n2 {
 public:
  itti_ul_nas_transport(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(ITTI_UL_NAS_TRANSPORT, origin, destination) {}
  itti_ul_nas_transport(const itti_ul_nas_transport& i) : itti_msg_n2(i) {}

  UplinkNASTransportMsg* ulNas;
};

class itti_dl_nas_transport : public itti_msg_n2 {
 public:
  itti_dl_nas_transport(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(ITTI_DL_NAS_TRANSPORT, origin, destination) {}
  itti_dl_nas_transport(const itti_dl_nas_transport& i) : itti_msg_n2(i) {}

 public:
  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  bstring nas;
};

class itti_initial_context_setup_request : public itti_msg_n2 {
 public:
  itti_initial_context_setup_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(INITIAL_CONTEXT_SETUP_REQUEST, origin, destination) {}
  itti_initial_context_setup_request(
      const itti_initial_context_setup_request& i)
      : itti_msg_n2(i) {
    is_pdu_exist     = false;
    isn2sm_avaliable = false;
  }
  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  bstring kgnb;
  bstring nas;
  bool is_sr;
  bstring n2sm;
  uint8_t pdu_session_id;
  bool is_pdu_exist;  // true is no pdu context
  bool isn2sm_avaliable;
};

class itti_pdu_session_resource_setup_request : public itti_msg_n2 {
 public:
  itti_pdu_session_resource_setup_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(PDU_SESSION_RESOURCE_SETUP_REQUEST, origin, destination) {}
  itti_pdu_session_resource_setup_request(
      const itti_pdu_session_resource_setup_request& i)
      : itti_msg_n2(i) {}
  bstring nas;
  bstring n2sm;
  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  uint8_t pdu_session_id;
};

class itti_pdu_session_resource_release_command : public itti_msg_n2 {
 public:
  itti_pdu_session_resource_release_command(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(PDU_SESSION_RESOURCE_RELEASE_COMMAND, origin, destination) {
  }
  itti_pdu_session_resource_release_command(
      const itti_pdu_session_resource_release_command& i)
      : itti_msg_n2(i) {}
  bstring nas;
  bstring n2sm;
  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  uint8_t pdu_session_id;
};

class itti_ue_context_release_request : public itti_msg_n2 {
 public:
  itti_ue_context_release_request(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UE_CONTEXT_RELEASE_REQUEST, origin, destination) {}
  itti_ue_context_release_request(const itti_ue_context_release_request& i)
      : itti_msg_n2(i) {}
  UEContextReleaseRequestMsg* ueCtxRel;
};

class itti_ue_context_release_command : public itti_msg_n2 {
 public:
  itti_ue_context_release_command(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UE_CONTEXT_RELEASE_COMMAND, origin, destination) {}
  itti_ue_context_release_command(const itti_dl_nas_transport& i)
      : itti_msg_n2(i) {}

 public:
  uint32_t ran_ue_ngap_id;
  long amf_ue_ngap_id;
  Cause cause;
};

class itti_ue_context_release_complete : public itti_msg_n2 {
 public:
  itti_ue_context_release_complete(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UE_CONTEXT_RELEASE_COMPLETE, origin, destination) {}
  itti_ue_context_release_complete(const itti_ue_context_release_complete& i)
      : itti_msg_n2(i) {}
  UEContextReleaseCompleteMsg* ueCtxRelCmpl;
};

class itti_ue_radio_capability_indication : public itti_msg_n2 {
 public:
  itti_ue_radio_capability_indication(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UE_RADIO_CAP_IND, origin, destination) {}
  itti_ue_radio_capability_indication(
      const itti_ue_radio_capability_indication& i)
      : itti_msg_n2(i) {}
  UeRadioCapabilityInfoIndicationMsg* ueRadioCap;
};

class itti_handover_required : public itti_msg_n2 {
 public:
  itti_handover_required(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(HANDOVER_REQUIRED, origin, destination) {}
  itti_handover_required(const itti_handover_required& i) : itti_msg_n2(i) {}
  HandoverRequiredMsg* handvoerRequ;
};

class itti_handover_request_Ack : public itti_msg_n2 {
 public:
  itti_handover_request_Ack(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(HANDOVER_REQUEST_ACK, origin, destination) {}
  itti_handover_request_Ack(const itti_handover_request_Ack& i)
      : itti_msg_n2(i) {}
  HandoverRequestAck* handoverrequestAck;
};

class itti_handover_notify : public itti_msg_n2 {
 public:
  itti_handover_notify(const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(HANDOVER_NOTIFY, origin, destination) {}
  itti_handover_notify(const itti_handover_notify& i) : itti_msg_n2(i) {}
  HandoverNotifyMsg* handovernotify;
};

class itti_uplinkranstatsutransfer : public itti_msg_n2 {
 public:
  itti_uplinkranstatsutransfer(
      const task_id_t origin, const task_id_t destination)
      : itti_msg_n2(UPLINKRANSTATUSTRANSFER, origin, destination) {}
  itti_uplinkranstatsutransfer(const itti_uplinkranstatsutransfer& i)
      : itti_msg_n2(i) {}
  UplinkRANStatusTransfer* uplinkrantransfer;
};

#endif
