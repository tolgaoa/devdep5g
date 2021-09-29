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

#ifndef _REGISTRATION_REQUEST_H_
#define _REGISTRATION_REQUEST_H_

#include <bstrlib.h>
#include <stdint.h>

#include <iostream>
#include <string>
#include <vector>

#include "nas_ie_header.hpp"
using namespace std;
namespace nas {

class RegistrationRequest {
 public:
  RegistrationRequest();
  ~RegistrationRequest();
  int encode2buffer(uint8_t* buf, int len);
  int decodefrombuffer(NasMmPlainHeader* header, uint8_t* buf, int len);
  void setHeader(uint8_t security_header_type);
  void set5gsRegistrationType(bool is_for, uint8_t type);
  void setngKSI(uint8_t tsc, uint8_t key_set_id);
  /*** belongs to _5GSMobilityIdentity**/
  void setSUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, const string msin);
  void setSUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, const string routingInd,
      uint8_t protection_sch_id, uint8_t hnpki, const string msin);
  void set5G_GUTI();
  void setIMEI_IMEISV();
  void set5G_S_TMSI();
  void setAdditional_GUTI_SUCI_SUPI_format_IMSI(
      const string mcc, const string mnc, uint8_t amf_region_id,
      uint8_t amf_set_id, uint8_t amf_pointer, const string _5g_tmsi);
  // for Additional_GUTI
  /*** belongs to _5GSMobilityIdentity**/
  void setNon_current_native_nas_ksi(uint8_t tsc, uint8_t key_set_id);
  void set5G_MM_capability(uint8_t value);
  void setUE_Security_Capability(uint8_t g_EASel, uint8_t g_IASel);
  void setUE_Security_Capability(
      uint8_t g_EASel, uint8_t g_IASel, uint8_t EEASel, uint8_t EIASel);
  void setRequested_NSSAI(std::vector<struct SNSSAI_s> nssai);
  void setUENetworkCapability(uint8_t g_EEASel, uint8_t g_EIASel);

  void setUplink_data_status(uint16_t value);
  void setLast_Visited_Registered_TAI(
      uint8_t MNC_MCC1, uint8_t MNC_MCC2, uint8_t MNC_MCC3, uint32_t TAC);
  void setPDU_session_status(uint16_t value);
  void setMICO_Indication(bool sprti, bool raai);
  void setUE_Status(bool n1, bool s1);
  void setAllowed_PDU_Session_Status(uint16_t value);
  void setUES_Usage_Setting(bool ues_usage_setting);
  void set_5GS_DRX_arameters(uint8_t value);
  void get5gsRegistrationType(bool& is_for, uint8_t& type);
  void setEPS_NAS_Message_Container(bstring value);
  void setLADN_Indication(std::vector<bstring> ladnValue);
  void setPayload_Container_Type(uint8_t value);
  void setPayload_Container(std::vector<PayloadContainerEntry> content);
  void setNetwork_Slicing_Indication(bool dcni, bool nssci);
  void set_5GS_Update_Type(
      uint8_t eps_pnb_ciot, uint8_t _5gs_pnb_ciot, bool ng_ran, bool sms);
  void setNAS_Message_Container(bstring value);
  void setEPS_Bearer_Context_Status(uint16_t value);

  bool get5GSRegistrationType(bool& is_for, uint8_t& reg_type /*3bits*/);
  uint8_t getngKSI();
  uint8_t getMobilityIdentityType();
  std::string get_5g_guti();  //"error" for missing IE GUTI
  bool getSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi);
  uint8_t getNonCurrentNativeNasKSI();
  uint8_t get5GMMCapability();
  bool getUeSecurityCapability(uint8_t& ea, uint8_t& ia);
  bool getUeSecurityCapability(
      uint8_t& ea, uint8_t& ia, uint8_t& eea, uint8_t& eia);
  bool getRequestedNssai(std::vector<struct SNSSAI_s>& nssai);
  bool getS1UeNetworkCapability(uint8_t& eea, uint8_t& eia);
  uint16_t getUplinkDataStatus();
  uint16_t getPduSessionStatus();
  bool getMicoIndication(uint8_t& sprti, uint8_t& raai);
  bool getUeStatus(uint8_t& n1ModeReg, uint8_t& s1ModeReg);
  bool getAdditionalGuti(nas::_5G_GUTI_t& guti);
  uint16_t getAllowedPduSessionStatus();
  uint8_t getUEsUsageSetting();
  uint8_t get5GSDrxParameters();
  bool getEpsNasMessageContainer(bstring& epsNas);
  uint8_t getPayloadContainerType();
  bool getNetworkSlicingIndication(uint8_t& dcni, uint8_t& nssci);
  bool get5GSUpdateType(
      uint8_t& eps_pnb_ciot, uint8_t& _5gs_pnb_ciot, bool& ng_ran_rcu,
      bool& sms_requested);
  bool getNasMessageContainer(bstring& nas);
  uint16_t getEpsBearerContextStatus();
  bool getLadnIndication(std::vector<bstring>& ladnValue);
  bool getPayloadContainer(std::vector<PayloadContainerEntry>& content);

 public:
  NasMmPlainHeader* plain_header;
  _5GSRegistrationType* ie_5gsregistrationtype;
  NasKeySetIdentifier* ie_ngKSI;
  _5GSMobilityIdentity* ie_5gs_mobility_id;

  NasKeySetIdentifier* ie_non_current_native_nas_ksi;
  _5GMMCapability* ie_5g_mm_capability;
  UESecurityCapability* ie_ue_security_capability;
  NSSAI* ie_requested_NSSAI;
  UENetworkCapability* ie_s1_ue_network_capability;
  UplinkDataStatus* ie_uplink_data_status;
  _5GS_Tracking_Area_Identity* ie_last_visited_registered_TAI;
  PDU_Session_Status* ie_PDU_session_status;
  MICO_Indication* ie_MICO_indicationl;
  UE_Status* ie_ue_status;
  _5GSMobilityIdentity* ie_additional_guti;
  Allowed_PDU_Session_Status* ie_allowed_PDU_session_status;
  UES_Usage_Setting* ie_ues_usage_setting;
  _5GS_DRX_arameters* ie_5gs_drx_parameters;
  EPS_NAS_Message_Container* ie_eps_nas_message_container;
  LADN_Indication* ie_ladn_indication;
  Payload_Container_Type* ie_payload_container_type;
  Payload_Container* ie_payload_container;
  Network_Slicing_Indication* ie_network_slicing_indication;
  _5GS_Update_Type* ie_5gs_update_type;
  NAS_Message_Container* ie_nas_message_container;
  EPS_Bearer_Context_Status* ie_eps_bearer_context_status;
};

}  // namespace nas

#endif
