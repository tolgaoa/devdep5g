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

/*! \file ngap_app.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "ngap_app.hpp"

#include "amf_module_from_config.hpp"
#include "logger.hpp"
#include "ngap_message_callback.hpp"
#include "sctp_server.hpp"

extern "C" {
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
}

using namespace sctp;
using namespace config;
using namespace ngap;

//------------------------------------------------------------------------------
ngap_app::ngap_app(const std::string& address, const uint16_t port_num)
    : ppid_(60), sctp_s_38412(address.c_str(), port_num) {
  sctp_s_38412.start_receive(this);
  Logger::ngap().info(
      "Set N2 AMF IPv4 Addr, port: %s, %d", address.c_str(), port_num);
}

//------------------------------------------------------------------------------
ngap_app::~ngap_app() {}

//------------------------------------------------------------------------------
void ngap_app::handle_receive(
    bstring payload, sctp_assoc_id_t assoc_id, sctp_stream_id_t stream,
    sctp_stream_id_t instreams, sctp_stream_id_t outstreams) {
  Logger::ngap().debug(
      "Handling SCTP payload from SCTP Server on assoc_id (%d), stream_id "
      "(%d), instreams (%d), outstreams (%d)",
      assoc_id, stream, instreams, outstreams);
  Ngap_NGAP_PDU_t* ngap_msg_pdu =
      (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER, &asn_DEF_Ngap_NGAP_PDU,
      (void**) &ngap_msg_pdu, bdata(payload), blength(payload));
  Logger::ngap().debug(
      "Decoded NGAP message, procedure code %d, present %d",
      ngap_msg_pdu->choice.initiatingMessage->procedureCode,
      ngap_msg_pdu->present);
  printf("after decoding ...\n");
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, ngap_msg_pdu);
  printf("end decoding ...\n");
  // Handle the message
  (*messages_callback[ngap_msg_pdu->choice.initiatingMessage->procedureCode]
                     [ngap_msg_pdu->present - 1])(
      assoc_id, stream, ngap_msg_pdu);
}

//------------------------------------------------------------------------------
void ngap_app::handle_sctp_new_association(
    sctp_assoc_id_t assoc_id, sctp_stream_id_t instreams,
    sctp_stream_id_t outstreams) {
  Logger::ngap().debug(
      "Ready to handle new NGAP SCTP association (id: %d) request", assoc_id);
  std::shared_ptr<gnb_context> gc;
  if (!is_assoc_id_2_gnb_context(assoc_id)) {
    Logger::ngap().debug(
        "Create a new gNB context with assoc_id (%d)", assoc_id);
    gc = std::shared_ptr<gnb_context>(new gnb_context());
    set_assoc_id_2_gnb_context(assoc_id, gc);
  } else {
    gc = assoc_id_2_gnb_context(assoc_id);
    if (gc.get()->ng_state == NGAP_RESETING ||
        gc.get()->ng_state == NGAP_SHUTDOWN) {
      Logger::ngap().warn(
          "Received new association request on an association that is being "
          "%s, ignoring",
          ng_gnb_state_str[gc.get()->ng_state]);
    } else {
      Logger::ngap().debug("Update gNB context with assoc id (%d)", assoc_id);
    }
  }
  if (gc.get() == nullptr) {
    Logger::ngap().error(
        "Failed to create gNB context for assoc_id(%d)", assoc_id);
  } else {
    gc.get()->sctp_assoc_id    = assoc_id;
    gc.get()->instreams        = instreams;
    gc.get()->outstreams       = outstreams;
    gc.get()->next_sctp_stream = 1;
    gc.get()->ng_state         = NGAP_INIT;
  }
}

//------------------------------------------------------------------------------
void ngap_app::handle_sctp_shutdown(sctp_assoc_id_t assoc_id) {
  Logger::ngap().debug(
      "Handle a SCTP Shutdown event (association id: %d)", assoc_id);

  // Handle the message
  (*events_callback[0])(assoc_id);
}

//------------------------------------------------------------------------------
uint32_t ngap_app::getPpid() {
  return ppid_;
}

//------------------------------------------------------------------------------
bool ngap_app::is_assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id) const {
  std::shared_lock lock(m_assoc2gnbContext);
  return bool{assoc2gnbContext.count(assoc_id) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<gnb_context> ngap_app::assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id) const {
  std::shared_lock lock(m_assoc2gnbContext);
  return assoc2gnbContext.at(assoc_id);
}

//------------------------------------------------------------------------------
void ngap_app::set_assoc_id_2_gnb_context(
    const sctp_assoc_id_t& assoc_id, std::shared_ptr<gnb_context> gc) {
  std::shared_lock lock(m_assoc2gnbContext);
  assoc2gnbContext[assoc_id] = gc;
  return;
}

//------------------------------------------------------------------------------
bool ngap_app::is_gnb_id_2_gnb_context(const long& gnb_id) const {
  std::shared_lock lock(m_gnbid2gnbContext);
  return bool{gnbid2gnbContext.count(gnb_id) > 0};
}

//------------------------------------------------------------------------------
std::shared_ptr<gnb_context> ngap_app::gnb_id_2_gnb_context(
    const long& gnb_id) const {
  std::shared_lock lock(m_gnbid2gnbContext);
  return gnbid2gnbContext.at(gnb_id);
}

//------------------------------------------------------------------------------
void ngap_app::set_gnb_id_2_gnb_context(
    const long& gnb_id, std::shared_ptr<gnb_context> gc) {
  std::unique_lock lock(m_gnbid2gnbContext);
  gnbid2gnbContext[gnb_id] = gc;
  return;
}

//------------------------------------------------------------------------------
void ngap_app::remove_gnb_context(const long& gnb_id) {
  if (is_gnb_id_2_gnb_context(gnb_id)) {
    std::unique_lock lock(m_gnbid2gnbContext);
    gnbid2gnbContext.erase(gnb_id);
    return;
  }
}
