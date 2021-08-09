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

/*! \file
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _RegistrationAccept_H_
#define _RegistrationAccept_H_

#include "nas_ie_header.hpp"

namespace nas {

class RegistrationAccept {
 public:
  RegistrationAccept();
  ~RegistrationAccept();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(NasMmPlainHeader* header, uint8_t* buf, int len);
  void setHeader(uint8_t security_header_type);
  void setSOR_Transparent_Container(uint8_t header, uint8_t* value);
  void set_5GS_Registration_Result(
      bool emergency, bool nssaa, bool sms, uint8_t value);
  /*** belongs to _5GSMobilityIdentity**/
  void setSUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, const string msin);
  void setSUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, uint8_t hnpki, const string msin);
  void set5G_GUTI(
      const string mcc, const string mnc, const string amfRegionId,
      const string amfSetId, const string amfPointer, const uint32_t tmsi);
  void setIMEI_IMEISV();
  void set5G_S_TMSI();
  /*** belongs to _5GSMobilityIdentity**/
  void setEquivalent_PLMNs(
      uint8_t MNC_MCC1, uint8_t MNC_MCC2, uint8_t MNC_MCC3);
  void setALLOWED_NSSAI(std::vector<struct SNSSAI_s> nssai);
  void setRejected_NSSAI(uint8_t cause, uint8_t value);
  void setCONFIGURED_NSSAI(std::vector<struct SNSSAI_s> nssai);
  void set_5GS_Network_Feature_Support(uint8_t value, uint8_t value2);
  void setPDU_session_status(uint16_t value);
  void setPDU_session_reactivation_result(uint16_t value);
  void setPDU_session_reactivation_result_error_cause(
      uint8_t session_id, uint8_t value);
  void setMICO_Indication(bool sprti, bool raai);
  void setNetwork_Slicing_Indication(bool dcni, bool nssci);
  void setT3512_Value(uint8_t unit, uint8_t value);
  void setNon_3GPP_de_registration_timer_value(uint8_t value);
  void setT3502_value(uint8_t value);
  void setEAP_Message(bstring eap);
  void setNSSAI_Inclusion_Mode(uint8_t value);
  void set_5GS_DRX_arameters(uint8_t value);
  void setNon_3GPP_NW_Provided_Policies(uint8_t value);
  void setEPS_Bearer_Context_Status(uint16_t value);
  void setExtended_DRX_Parameters(uint8_t paging_time, uint8_t value);
  void setT3447_Value(uint8_t unit, uint8_t value);
  void setT3448_Value(uint8_t unit, uint8_t value);
  void setT3324_Value(uint8_t unit, uint8_t value);
  void setUE_Radio_Capability_ID(uint8_t value);
  void setPending_NSSAI(std::vector<struct SNSSAI_s> nssai);
  void setTaiList(std::vector<p_tai_t> tai_list);

 public:
  NasMmPlainHeader* plain_header;
  _5GS_Registration_Result* ie_5gs_registration_result;
  _5GSMobilityIdentity* ie_5g_guti;
  PLMN_List* ie_equivalent_plmns;
  NSSAI* ie_allowed_nssai;
  Rejected_NSSAI* ie_rejected_nssai;
  NSSAI* ie_configured_nssai;
  _5GS_Network_Feature_Support* ie_5gs_network_feature_support;
  PDU_Session_Status* ie_PDU_session_status;
  PDU_Session_Reactivation_Result* ie_pdu_session_reactivation_result;
  PDU_Session_Reactivation_Result_Error_Cause*
      ie_pdu_session_reactivation_result_error_cause;
  MICO_Indication* ie_MICO_indicationl;
  Network_Slicing_Indication* ie_network_slicing_indication;
  GPRS_Timer_3* ie_T3512_value;
  GPRS_Timer_2* ie_Non_3GPP_de_registration_timer_value;
  GPRS_Timer_2* ie_T3502_value;
  SOR_Transparent_Container* ie_sor_transparent_container;
  EAP_Message* ie_eap_message;
  NSSAI_Inclusion_Mode* ie_nssai_inclusion_mode;
  _5GS_DRX_arameters* ie_negotiated_drx_parameters;
  Non_3GPP_NW_Provided_Policies* ie_non_3gpp_nw_policies;
  EPS_Bearer_Context_Status* ie_eps_bearer_context_status;
  Extended_DRX_Parameters* ie_extended_drx_parameters;
  GPRS_Timer_3* ie_T3447_value;
  GPRS_Timer_3* ie_T3448_value;
  GPRS_Timer_3* ie_T3324_value;
  UE_Radio_Capability_ID* ie_ue_radio_capability_id;
  NSSAI* ie_pending_nssai;
  _5GSTrackingAreaIdList* ie_tai_list;
};

}  // namespace nas

#endif
