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

/*! \file smf_n1.hpp
 * \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: tien-thinh.nguyen@eurecom.fr
 */

#ifndef FILE_SMF_N1_HPP_SEEN
#define FILE_SMF_N1_HPP_SEEN

#include <string>

#include "smf.h"
#include "smf_context.hpp"
#include "smf_msg.hpp"

extern "C" {
#include "nas_message.h"
}

namespace smf {

class smf_n1 {
 private:
 public:
  smf_n1(){};
  smf_n1(smf_n1 const&) = delete;
  void operator=(smf_n1 const&) = delete;

 public:
  static smf_n1& get_instance() {
    static smf_n1 instance;
    return instance;
  }

  /*
   * Create N1 SM Container: PDU Session Establishment Accept
   * @param [pdu_session_create_sm_context_response&] sm_context_res: include
   * necessary information for encoding NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @param [cause_value_5gsm_e] sm_cause store NAS Cause
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n1_pdu_session_establishment_accept(
      pdu_session_create_sm_context_response& sm_context_res,
      std::string& nas_msg_str, cause_value_5gsm_e sm_cause);

  /*
   * Create N1 SM Container: PDU Session Establishment Reject
   * @param [pdu_session_msg&] msg: include necessary information for encoding
   * NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @param [cause_value_5gsm_e] sm_cause store NAS Cause
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n1_pdu_session_establishment_reject(
      pdu_session_msg& msg, std::string& nas_msg_str,
      cause_value_5gsm_e sm_cause);
  /*
   * Create N1 SM Container: PDU Session Modification Command
   * Included in PDU Session Update SM Context Response (PDU Session
   * Modification UE-Initiated procedure - step 1)
   * @param [pdu_session_update_sm_context_response&] msg: include necessary
   * information for encoding NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @param [cause_value_5gsm_e] sm_cause store NAS Cause
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n1_pdu_session_modification_command(
      pdu_session_update_sm_context_response& msg, std::string& nas_msg_str,
      cause_value_5gsm_e sm_cause);

  /*
   * Create N1 SM Container: PDU Session Modification Command
   * Included in N1N2MessageTransfer Request (PDU Session Modification
   * SMF-Requested, step 1 (from SMF to AMF))
   * @param [pdu_session_modification_network_requested&] msg: include necessary
   * information for encoding NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @param [cause_value_5gsm_e] sm_cause store NAS Cause
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n1_pdu_session_modification_command(
      pdu_session_modification_network_requested& msg, std::string& nas_msg_str,
      cause_value_5gsm_e sm_cause);

  /*
   * Create N1 SM Container: PDU Session Release Reject
   * Included in PDU Session Update SM Context Response (PDU Session Release
   * UE-Initiated, step 1)
   * @param [pdu_session_update_sm_context_request&] msg: include necessary
   * information for encoding NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @param [cause_value_5gsm_e] sm_cause store NAS Cause
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n1_pdu_session_release_reject(
      pdu_session_update_sm_context_request& sm_context_res,
      std::string& nas_msg_str, cause_value_5gsm_e sm_cause);

  /*
   * Create N1 SM Container: PDU Session Release Command
   * Included in PDU Session Update SM Context Response (PDU Session Release
   * UE-Initiated, step 1)
   * @param [pdu_session_update_sm_context_response&] msg: include necessary
   * information for encoding NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @param [cause_value_5gsm_e] sm_cause store NAS Cause
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n1_pdu_session_release_command(
      pdu_session_update_sm_context_response& msg, std::string& nas_msg_str,
      cause_value_5gsm_e sm_cause);

  /*
   * Create N1 SM Container: PDU Session Release Command
   * Included in N1N2MessageTransfer Request (PDU Session Release SMF-Requested,
   * step 1)
   * @param [pdu_session_modification_network_requested&] msg: include necessary
   * information for encoding NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @param [cause_value_5gsm_e] sm_cause store NAS Cause
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n1_pdu_session_release_command(
      pdu_session_modification_network_requested& msg, std::string& nas_msg_str,
      cause_value_5gsm_e sm_cause);

  /*
   * Decode N1 SM Container into the NAS mesasge (using NAS lib)
   * @param [nas_message_t&] nas_msg Store NAS message after decoded
   * @param [const std::string&] n1_sm_msg N1 SM Container
   * @return status of the decode process
   */
  int decode_n1_sm_container(
      nas_message_t& nas_msg, const std::string& n1_sm_msg);
};

}  // namespace smf

#endif /* FILE_SMF_N1_HPP_SEEN */
