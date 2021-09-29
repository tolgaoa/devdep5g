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

/*! \file ue_context.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _UE_CONTEXT_H_
#define _UE_CONTEXT_H_

#include <stdint.h>

#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>

#include "NgapIEsStruct.hpp"
#include "pdu_session_context.hpp"

extern "C" {
#include "Ngap_RRCEstablishmentCause.h"
}

using namespace ngap;

class ue_context {
 public:
  ue_context();
  virtual ~ue_context(){};
  bool find_pdu_session_context(
      const std::uint8_t& session_id,
      std::shared_ptr<pdu_session_context>& context) const;
  void add_pdu_session_context(
      const std::uint8_t& session_id,
      const std::shared_ptr<pdu_session_context>& context);
  void copy_pdu_sessions(std::shared_ptr<ue_context>& ue_ctx);
  bool get_pdu_sessions_context(
      std::vector<std::shared_ptr<pdu_session_context>>& sessions_ctx);

 public:
  uint32_t ran_ue_ngap_id;   // 32bits
  long amf_ue_ngap_id : 40;  // 40bits

  e_Ngap_RRCEstablishmentCause rrc_estb_cause;
  bool isUeContextRequest;
  NrCgi_t cgi;
  Tai_t tai;
  std::string supi;
  // pdu session id <-> pdu_session_contex: map stores all pdu sessions for this
  // UE
  std::map<std::uint8_t, std::shared_ptr<pdu_session_context>> pdu_sessions;
  mutable std::shared_mutex m_pdu_session;
};

#endif
