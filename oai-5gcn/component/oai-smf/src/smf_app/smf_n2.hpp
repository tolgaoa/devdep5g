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

/*! \file smf_n2.hpp
 * \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: tien-thinh.nguyen@eurecom.fr
 */

#ifndef FILE_SMF_N2_HPP_SEEN
#define FILE_SMF_N2_HPP_SEEN

#include <string>

#include "smf.h"
#include "smf_app.hpp"
#include "smf_msg.hpp"

extern "C" {
#include "Ngap_PDUSessionResourceModifyResponseTransfer.h"
#include "Ngap_PDUSessionResourceReleaseResponseTransfer.h"
#include "Ngap_PDUSessionResourceSetupResponseTransfer.h"
#include "Ngap_PDUSessionResourceSetupUnsuccessfulTransfer.h"
}

namespace smf {

class smf_n2 {
 private:
 public:
  smf_n2(){};
  smf_n2(smf_n2 const&) = delete;
  void operator=(smf_n2 const&) = delete;

 public:
  static smf_n2& get_instance() {
    static smf_n2 instance;
    return instance;
  }

  /*
   * Create N2 SM Information: PDU Session Resource Setup Request Transfer
   * This IE is included in N1N2MessageTransfer Request (Accept, PDU Session
   * Establishment procedure - UE initiated)
   * @param [pdu_session_create_sm_context_response&] sm_context_res: include
   * necessary information for encoding NGAP msg
   * @param [n2_sm_info_type_e] ngap_info_type: NGAP info's type
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n2_pdu_session_resource_setup_request_transfer(
      pdu_session_create_sm_context_response& sm_context_res,
      n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str);

  /*
   * Create N2 SM Information: PDU Session Resource Setup Request Transfer
   * This IE is included in PDU Session Update SM Context Response​ (Service
   * Request, step 2)
   * @param [pdu_session_update_sm_context_response&] sm_context_res: include
   * necessary information for encoding NGAP msg
   * @param [n2_sm_info_type_e] ngap_info_type: NGAP info's type
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n2_pdu_session_resource_setup_request_transfer(
      pdu_session_update_sm_context_response& sm_context_res,
      n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str);

  /*
   * Create N2 SM Information: PDU Session Resource Setup Request Transfer
   * This IE is included in N1N2MessageTranfer (N4 Data Report)
   * @param [pdu_session_report_response] msg: include necessary information for
   * encoding NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n2_pdu_session_resource_setup_request_transfer(
      pdu_session_report_response& msg, n2_sm_info_type_e ngap_info_type,
      std::string& ngap_msg_str);

  /*
   * Create N2 SM Information: PDU Session Resource Modify Request Transfer IE
   * This IE is included in  PDU Session Update SM Context Response (PDU Session
   * Modification procedure, UE-initiated, step 1)
   * @param [pdu_session_update_sm_context_response] sm_context_res: include
   * necessary information for encoding NGAP msg
   * @param [n2_sm_info_type_e] ngap_info_type: NGAP info's type
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n2_pdu_session_resource_modify_request_transfer(
      pdu_session_update_sm_context_response& sm_context_res,
      n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str);

  /*
   * Create N2 SM Information: PDU Session Resource Modify Request Transfer IE
   * This IE is included in  N1N2MessageTransfer Request (PDU Session
   * Modification procedure, SMF-requested, step 1)
   * @param [pdu_session_update_sm_context_response] sm_context_res: include
   * necessary information for encoding NGAP msg
   * @param [n2_sm_info_type_e] ngap_info_type: NGAP info's type
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n2_pdu_session_resource_modify_request_transfer(
      pdu_session_modification_network_requested& msg,
      n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str);

  /*
   * Create N2 SM Information: PDU Session Resource Modify Response Transfer IE
   *
   * @param [pdu_session_update_sm_context_response] sm_context_res: include
   * necessary information for encoding NGAP msg
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @param [n2_sm_info_type_e] ngap_info_type: NGAP info's type
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n2_pdu_session_resource_modify_response_transfer(
      pdu_session_update_sm_context_response& sm_context_res,
      n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str);

  /*
   * Create N2 SM Information: PDU Session Resource Release Command Transfer IE
   * This IE is included in the following messages:
   * PDU Session Update SM Context Response (PDU Session Release UE-Initiated:
   * section 4.3.4@3GPP TS 23.502, step 1) N1N2MessageTransfer Request​ (PDU
   * Session Release SMF-Requested, step 1)
   * @param [pdu_session_update_sm_context_response] sm_context_res: include
   * necessary information for encoding NGAP msg
   * @param [n2_sm_info_type_e] ngap_info_type: NGAP info's type
   * @param [std::string&] ngap_msg_str store the created NGAP message in form
   * of string
   * @return boolean: True if the NGAP message has been created successfully,
   * otherwise return false
   *
   */
  bool create_n2_pdu_session_resource_release_command_transfer(
      pdu_session_update_sm_context_response& sm_context_res,
      n2_sm_info_type_e ngap_info_type, std::string& ngap_msg_str);

  /*
   * Decode N2 SM Information Ngap_PDUSessionResourceSetupResponseTransfer
   * @param [std::shared_ptr<Ngap_PDUSessionResourceSetupResponseTransfer_t>&]
   * ngap_IE Store decoded NGAP message
   * @param [const std::string&] n2_sm_info N2 SM Information
   * @return status of the decode process
   */
  int decode_n2_sm_information(
      std::shared_ptr<Ngap_PDUSessionResourceSetupResponseTransfer_t>& ngap_IE,
      const std::string& n2_sm_info);

  /*
   * Decode N2 SM Information Ngap_PDUSessionResourceModifyResponseTransfer_t
   * @param [std::shared_ptr<Ngap_PDUSessionResourceModifyResponseTransfer_t>&]
   * ngap_IE Store decoded NGAP message
   * @param [std::string&] n2_sm_info N2 SM Information
   * @return status of the decode process
   */
  int decode_n2_sm_information(
      std::shared_ptr<Ngap_PDUSessionResourceModifyResponseTransfer_t>& ngap_IE,
      const std::string& n2_sm_info);

  /*
   * Decode N2 SM Information Ngap_PDUSessionResourceReleaseResponseTransfer_t
   * @param [std::shared_ptr<Ngap_PDUSessionResourceReleaseResponseTransfer_t>&]
   * ngap_IE Store decoded NGAP message
   * @param [std::string&] n2_sm_info N2 SM Information
   * @return status of the decode process
   */
  int decode_n2_sm_information(
      std::shared_ptr<Ngap_PDUSessionResourceReleaseResponseTransfer_t>&
          ngap_IE,
      const std::string& n2_sm_info);

  /*
   * Decode N2 SM Information Ngap_PDUSessionResourceSetupUnsuccessfulTransfer
   * @param
   * [std::shared_ptr<Ngap_PDUSessionResourceSetupUnsuccessfulTransfer_t>&]
   * ngap_IE Store decoded NGAP message
   * @param [std::string&] n2_sm_info N2 SM Information
   * @return status of the decode process
   */
  int decode_n2_sm_information(
      std::shared_ptr<Ngap_PDUSessionResourceSetupUnsuccessfulTransfer_t>&
          ngap_IE,
      const std::string& n2_sm_info);
};

}  // namespace smf

#endif /* FILE_SMF_N2_HPP_SEEN */
