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

#include "RegistrationRequest.hpp"

#include "3gpp_ts24501.hpp"
#include "String2Value.hpp"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
RegistrationRequest::RegistrationRequest() {
  plain_header                   = NULL;
  ie_5gsregistrationtype         = NULL;
  ie_ngKSI                       = NULL;
  ie_5gs_mobility_id             = NULL;
  ie_non_current_native_nas_ksi  = NULL;
  ie_5g_mm_capability            = NULL;
  ie_ue_security_capability      = NULL;
  ie_requested_NSSAI             = NULL;
  ie_s1_ue_network_capability    = NULL;
  ie_uplink_data_status          = NULL;
  ie_last_visited_registered_TAI = NULL;
  ie_PDU_session_status          = NULL;
  ie_MICO_indicationl            = NULL;
  ie_ue_status                   = NULL;
  ie_additional_guti             = NULL;
  ie_allowed_PDU_session_status  = NULL;
  ie_ues_usage_setting           = NULL;
  ie_5gs_drx_parameters          = NULL;
  ie_eps_nas_message_container   = NULL;
  ie_ladn_indication             = NULL;
  ie_payload_container_type      = NULL;
  ie_payload_container           = NULL;
  ie_network_slicing_indication  = NULL;
  ie_5gs_update_type             = NULL;
  ie_nas_message_container       = NULL;
  ie_eps_bearer_context_status   = NULL;
}

//------------------------------------------------------------------------------
RegistrationRequest::~RegistrationRequest() {}

//------------------------------------------------------------------------------
void RegistrationRequest::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(
      EPD_5GS_MM_MSG, security_header_type, REGISTRATION_REQUEST);
}

//------------------------------------------------------------------------------
void RegistrationRequest::set5gsRegistrationType(bool is_for, uint8_t type) {
  ie_5gsregistrationtype = new _5GSRegistrationType(is_for, type);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::get5GSRegistrationType(
    bool& is_for, uint8_t& reg_type /*3bits*/) {
  is_for   = ie_5gsregistrationtype->isFollowOnReq();
  reg_type = ie_5gsregistrationtype->getRegType();
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setngKSI(uint8_t tsc, uint8_t key_set_id) {
  ie_ngKSI = new NasKeySetIdentifier(tsc, key_set_id);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::getngKSI() {
  if (ie_ngKSI) {
    uint8_t a = 0;
    a = (ie_ngKSI->getTypeOfSecurityContext()) | ie_ngKSI->getasKeyIdentifier();
    return a;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setSUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, const string routingInd,
    uint8_t protection_sch_id, const string msin) {
  if (protection_sch_id != NULL_SCHEME) {
    Logger::nas_mm().error(
        "encoding suci and supi format for imsi error, please choose right "
        "interface");
    return;
  } else {
    ie_5gs_mobility_id =
        new _5GSMobilityIdentity(mcc, mnc, routingInd, protection_sch_id, msin);
  }
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::getMobilityIdentityType() {
  if (ie_5gs_mobility_id) {
    return ie_5gs_mobility_id->gettypeOfIdentity();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getSuciSupiFormatImsi(nas::SUCI_imsi_t& imsi) {
  if (ie_5gs_mobility_id) {
    ie_5gs_mobility_id->getSuciWithSupiImsi(imsi);
    return true;
  } else {
    return false;
  }
}

//------------------------------------------------------------------------------
std::string RegistrationRequest::get_5g_guti() {
  if (ie_5gs_mobility_id) {
    nas::_5G_GUTI_t guti;
    ie_5gs_mobility_id->get5GGUTI(guti);
    return "1234567890";
  } else {
    return "error";
  }
}

//------------------------------------------------------------------------------
// Additional_GUTI
void RegistrationRequest::setAdditional_GUTI_SUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, uint8_t amf_region_id,
    uint8_t amf_set_id, uint8_t amf_pointer, const string _5g_tmsi) {
  /*if (amf_pointer&0x80) {
   Logger::nas_mm().error("encoding suci and supi format for imsi error, please
   choose right interface"); return;
   }
   else {*/
  ie_additional_guti = new _5GSMobilityIdentity();
  ie_additional_guti->setIEI(0x77);
  uint32_t tmsi = fromString<uint32_t>(_5g_tmsi);
  ie_additional_guti->set5GGUTI(
      mcc, mnc, amf_region_id, amf_set_id, amf_pointer, tmsi);
  //}
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getAdditionalGuti(nas::_5G_GUTI_t& guti) {
  if (ie_additional_guti) {
    ie_additional_guti->get5GGUTI(guti);
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setSUCI_SUPI_format_IMSI(
    const string mcc, const string mnc, const string routingInd,
    uint8_t protection_sch_id, uint8_t hnpki, const string msin) {}

//------------------------------------------------------------------------------
void RegistrationRequest::set5G_GUTI() {}

//------------------------------------------------------------------------------
void RegistrationRequest::setIMEI_IMEISV() {}

//------------------------------------------------------------------------------
void RegistrationRequest::set5G_S_TMSI() {}

//------------------------------------------------------------------------------
void RegistrationRequest::setNon_current_native_nas_ksi(
    uint8_t tsc, uint8_t key_set_id) {
  ie_non_current_native_nas_ksi = new NasKeySetIdentifier(0xC, tsc, key_set_id);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::getNonCurrentNativeNasKSI() {
  if (ie_non_current_native_nas_ksi) {
    uint8_t a = 0;
    a |= (ie_non_current_native_nas_ksi->getTypeOfSecurityContext()) |
         (ie_non_current_native_nas_ksi->getasKeyIdentifier());
    return a;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::set5G_MM_capability(uint8_t value) {
  ie_5g_mm_capability = new _5GMMCapability(0x10, value);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::get5GMMCapability() {
  if (ie_5g_mm_capability)
    return ie_5g_mm_capability->getValue();
  else
    return -1;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUE_Security_Capability(
    uint8_t g_EASel, uint8_t g_IASel) {
  ie_ue_security_capability = new UESecurityCapability(0x2E, g_EASel, g_IASel);
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUE_Security_Capability(
    uint8_t g_EASel, uint8_t g_IASel, uint8_t EEASel, uint8_t EIASel) {
  ie_ue_security_capability =
      new UESecurityCapability(0x2E, g_EASel, g_IASel, EEASel, EIASel);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getUeSecurityCapability(uint8_t& ea, uint8_t& ia) {
  if (ie_ue_security_capability) {
    ea = ie_ue_security_capability->getEASel();
    ia = ie_ue_security_capability->getIASel();
  } else {
    return -1;
  }
  return true;
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getUeSecurityCapability(
    uint8_t& ea, uint8_t& ia, uint8_t& eea, uint8_t& eia) {
  if (ie_ue_security_capability) {
    ea = ie_ue_security_capability->getEASel();
    ia = ie_ue_security_capability->getIASel();
    if (ie_ue_security_capability->getLength() >= 4) {
      eea = ie_ue_security_capability->getEEASel();
      eia = ie_ue_security_capability->getEIASel();
    }
  } else {
    return -1;
  }
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setRequested_NSSAI(
    std::vector<struct SNSSAI_s> nssai) {
  ie_requested_NSSAI = new NSSAI(0x2F, nssai);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getRequestedNssai(
    std::vector<struct SNSSAI_s>& nssai) {
  if (ie_requested_NSSAI) {
    ie_requested_NSSAI->getValue(nssai);
  } else {
    return -1;
  }
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setLast_Visited_Registered_TAI(
    uint8_t MNC_MCC1, uint8_t MNC_MCC2, uint8_t MNC_MCC3, uint32_t TAC) {
  ie_last_visited_registered_TAI =
      new _5GS_Tracking_Area_Identity(0x52, MNC_MCC1, MNC_MCC2, MNC_MCC3, TAC);
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUENetworkCapability(
    uint8_t g_EEASel, uint8_t g_EIASel) {
  ie_s1_ue_network_capability =
      new UENetworkCapability(0x17, g_EEASel, g_EIASel);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getS1UeNetworkCapability(uint8_t& eea, uint8_t& eia) {
  if (ie_s1_ue_network_capability) {
    eea = ie_s1_ue_network_capability->getEEASel();
    eia = ie_s1_ue_network_capability->getEIASel();
  } else {
    return -1;
  }
  return true;
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUplink_data_status(uint16_t value) {
  ie_uplink_data_status = new UplinkDataStatus(0x40, value);
}

//------------------------------------------------------------------------------
uint16_t RegistrationRequest::getUplinkDataStatus() {
  if (ie_uplink_data_status) {
    return ie_uplink_data_status->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setPDU_session_status(uint16_t value) {
  ie_PDU_session_status = new PDU_Session_Status(0x50, value);
}

//------------------------------------------------------------------------------
uint16_t RegistrationRequest::getPduSessionStatus() {
  if (ie_PDU_session_status) {
    return ie_PDU_session_status->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setMICO_Indication(bool sprti, bool raai) {
  ie_MICO_indicationl = new MICO_Indication(0x0B, sprti, raai);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getMicoIndication(uint8_t& sprti, uint8_t& raai) {
  if (ie_PDU_session_status) {
    sprti = ie_MICO_indicationl->getSPRTI();
    raai  = ie_MICO_indicationl->getRAAI();
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUE_Status(bool n1, bool s1) {
  ie_ue_status = new UE_Status(0x2B, n1, s1);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getUeStatus(uint8_t& n1ModeReg, uint8_t& s1ModeReg) {
  if (ie_ue_status) {
    n1ModeReg = ie_ue_status->getN1();
    s1ModeReg = ie_ue_status->getS1();
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setAllowed_PDU_Session_Status(uint16_t value) {
  ie_allowed_PDU_session_status = new Allowed_PDU_Session_Status(0x25, value);
}

//------------------------------------------------------------------------------
uint16_t RegistrationRequest::getAllowedPduSessionStatus() {
  if (ie_allowed_PDU_session_status) {
    return ie_allowed_PDU_session_status->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setUES_Usage_Setting(bool ues_usage_setting) {
  ie_ues_usage_setting = new UES_Usage_Setting(0x18, ues_usage_setting);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::getUEsUsageSetting() {
  if (ie_ues_usage_setting) {
    return ie_ues_usage_setting->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::set_5GS_DRX_arameters(uint8_t value) {
  ie_5gs_drx_parameters = new _5GS_DRX_arameters(0x51, value);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::get5GSDrxParameters() {
  if (ie_5gs_drx_parameters) {
    return ie_5gs_drx_parameters->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setEPS_NAS_Message_Container(bstring value) {
  ie_eps_nas_message_container = new EPS_NAS_Message_Container(0x70, value);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getEpsNasMessageContainer(bstring& epsNas) {
  if (ie_eps_nas_message_container) {
    ie_eps_nas_message_container->getValue(epsNas);
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setLADN_Indication(std::vector<bstring> ladnValue) {
  ie_ladn_indication = new LADN_Indication(0x74, ladnValue);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getLadnIndication(std::vector<bstring>& ladnValue) {
  if (ie_ladn_indication) {
    return ie_ladn_indication->getValue(ladnValue);
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setPayload_Container_Type(uint8_t value) {
  ie_payload_container_type = new Payload_Container_Type(0x08, value);
}

//------------------------------------------------------------------------------
uint8_t RegistrationRequest::getPayloadContainerType() {
  if (ie_payload_container_type) {
    return ie_payload_container_type->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setPayload_Container(
    std::vector<PayloadContainerEntry> content) {
  ie_payload_container = new Payload_Container(0x7B, content);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getPayloadContainer(
    std::vector<PayloadContainerEntry>& content) {
  if (ie_payload_container) {
    ie_payload_container->getValue(content);
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setNetwork_Slicing_Indication(bool dcni, bool nssci) {
  ie_network_slicing_indication =
      new Network_Slicing_Indication(0x09, dcni, nssci);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getNetworkSlicingIndication(
    uint8_t& dcni, uint8_t& nssci) {
  if (ie_network_slicing_indication) {
    dcni  = ie_network_slicing_indication->getDCNI();
    nssci = ie_network_slicing_indication->getNSSCI();
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::set_5GS_Update_Type(
    uint8_t eps_pnb_ciot, uint8_t _5gs_pnb_ciot, bool ng_ran, bool sms) {
  ie_5gs_update_type =
      new _5GS_Update_Type(0x53, eps_pnb_ciot, _5gs_pnb_ciot, ng_ran, sms);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::get5GSUpdateType(
    uint8_t& eps_pnb_ciot, uint8_t& _5gs_pnb_ciot, bool& ng_ran_rcu,
    bool& sms_requested) {
  if (ie_5gs_update_type) {
    eps_pnb_ciot  = ie_5gs_update_type->getEPS_PNB_CIoT();
    _5gs_pnb_ciot = ie_5gs_update_type->get_5GS_PNB_CIoT();
    ng_ran_rcu    = ie_5gs_update_type->getNG_RAN();
    sms_requested = ie_5gs_update_type->getSMS();
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setNAS_Message_Container(bstring value) {
  ie_nas_message_container = new NAS_Message_Container(0x71, value);
}

//------------------------------------------------------------------------------
bool RegistrationRequest::getNasMessageContainer(bstring& nas) {
  if (ie_nas_message_container) {
    ie_nas_message_container->getValue(nas);
    return 0;
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::setEPS_Bearer_Context_Status(uint16_t value) {
  ie_eps_bearer_context_status = new EPS_Bearer_Context_Status(0x60, value);
}

//------------------------------------------------------------------------------
uint16_t RegistrationRequest::getEpsBearerContextStatus() {
  if (ie_eps_bearer_context_status) {
    return ie_eps_bearer_context_status->getValue();
  } else {
    return -1;
  }
}

//------------------------------------------------------------------------------
void RegistrationRequest::get5gsRegistrationType(bool& is_for, uint8_t& type) {
  is_for = ie_5gsregistrationtype->isFollowOnReq();
  type   = ie_5gsregistrationtype->getRegType();
}

//------------------------------------------------------------------------------
int RegistrationRequest::encode2buffer(uint8_t* buf, int len) {
  Logger::nas_mm().debug("encoding RegistrationRequest message");
  int encoded_size = 0;
  if (!plain_header) {
    Logger::nas_mm().error("Mandatory IE missing Header");
    return 0;
  }
  if (!ie_5gsregistrationtype) {
    Logger::nas_mm().error("Mandatory IE missing ie_5gsregistrationtype");
    return 0;
  }
  if (!ie_ngKSI) {
    Logger::nas_mm().error("Mandatory IE missing ie_ngKSI");
    return 0;
  }
  if (!ie_5gs_mobility_id) {
    Logger::nas_mm().error("Mandatory IE missing ie_5gs_mobility_id");
    return 0;
  }
  if (!(plain_header->encode2buffer(buf, len))) return 0;
  encoded_size += 3;
  if (!(ie_5gsregistrationtype->encode2buffer(
          buf + encoded_size, len - encoded_size))) {
    if (!(ie_ngKSI->encode2buffer(buf + encoded_size, len - encoded_size))) {
      encoded_size += 1;
    } else {
      Logger::nas_mm().error("encoding ie ie_ngKSI error");
      return 0;
    }
  } else {
    Logger::nas_mm().error("encoding ie 5gsregistrationtype error");
    return 0;
  }
  if (int size = ie_5gs_mobility_id->encode2buffer(
          buf + encoded_size, len - encoded_size)) {
    encoded_size += size;
  } else {
    Logger::nas_mm().error("encoding ie ie_5gs_mobility_id  error");
    return 0;
  }
  if (!ie_non_current_native_nas_ksi) {
    Logger::nas_mm().warn("IE non_current_native_nas_ksi is not available");
  } else {
    if (ie_non_current_native_nas_ksi->encode2buffer(
            buf + encoded_size, len - encoded_size) == 1) {
      encoded_size++;
    } else {
      Logger::nas_mm().error("encoding ie_non_current_native_nas_ksi  error");
    }
  }
  if (!ie_5g_mm_capability) {
    Logger::nas_mm().warn("IE ie_5g_mm_capability is not available");
  } else {
    if (int size = ie_5g_mm_capability->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_5g_mm_capability  error");
      return 0;
    }
  }
  if (!ie_ue_security_capability) {
    Logger::nas_mm().warn("IE ie_ue_security_capability is not available");
  } else {
    if (int size = ie_ue_security_capability->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_ue_security_capability  error");
      return 0;
    }
  }
  if (!ie_requested_NSSAI) {
    Logger::nas_mm().warn("IE ie_requested_NSSAI is not available");
  } else {
    if (int size = ie_requested_NSSAI->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_requested_NSSAI  error");
      return 0;
    }
  }
  if (!ie_last_visited_registered_TAI) {
    Logger::nas_mm().warn("IE ie_Last_visited_registered_TAI is not available");
  } else {
    if (int size = ie_last_visited_registered_TAI->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_Last_visited_registered_TAI  error");
      return 0;
    }
  }
  if (!ie_s1_ue_network_capability) {
    Logger::nas_mm().warn("IE ie_s1_ue_network_capability is not available");
  } else {
    if (int size = ie_s1_ue_network_capability->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_s1_ue_network_capability  error");
      return 0;
    }
  }
  if (!ie_uplink_data_status) {
    Logger::nas_mm().warn("IE ie_uplink_data_status is not available");
  } else {
    if (int size = ie_uplink_data_status->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_uplink_data_status  error");
      return 0;
    }
  }
  if (!ie_PDU_session_status) {
    Logger::nas_mm().warn("IE ie_PDU_session_status is not available");
  } else {
    if (int size = ie_PDU_session_status->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_PDU_session_status  error");
      return 0;
    }
  }
  if (!ie_MICO_indicationl) {
    Logger::nas_mm().warn("IE ie_MICO_indicationl is not available");
  } else {
    if (int size = ie_MICO_indicationl->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_MICO_indicationl  error");
      return 0;
    }
  }
  if (!ie_ue_status) {
    Logger::nas_mm().warn("IE ie_ue_status is not available");
  } else {
    if (int size = ie_ue_status->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_ue_status  error");
      return 0;
    }
  }
  if (!ie_additional_guti) {
    Logger::nas_mm().warn("IE ie_additional_guti- is not available");
  } else {
    if (int size = ie_additional_guti->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie ie_additional_guti  error");
      return 0;
    }
  }
  if (!ie_allowed_PDU_session_status) {
    Logger::nas_mm().warn("IE ie_allowed_PDU_session_status is not available");
  } else {
    if (int size = ie_allowed_PDU_session_status->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_allowed_PDU_session_status  error");
      return 0;
    }
  }
  if (!ie_ues_usage_setting) {
    Logger::nas_mm().warn("IE ie_ues_usage_setting is not available");
  } else {
    if (int size = ie_ues_usage_setting->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_ues_usage_setting  error");
      return 0;
    }
  }
  if (!ie_5gs_drx_parameters) {
    Logger::nas_mm().warn("IE ie_5gs_drx_parameters is not available");
  } else {
    if (int size = ie_5gs_drx_parameters->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_5gs_drx_parameters  error");
      return 0;
    }
  }
  if (!ie_eps_nas_message_container) {
    Logger::nas_mm().warn("IE ie_eps_nas_message_container is not available");
  } else {
    if (int size = ie_eps_nas_message_container->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_eps_nas_message_container  error");
      return 0;
    }
  }
  if (!ie_ladn_indication) {
    Logger::nas_mm().warn("IE ie_ladn_indication is not available");
  } else {
    if (int size = ie_ladn_indication->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_ladn_indication  error");
      return 0;
    }
  }
  if (!ie_payload_container_type) {
    Logger::nas_mm().warn("IE ie_payload_container_type is not available");
  } else {
    if (int size = ie_payload_container_type->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_payload_container_type  error");
      return 0;
    }
  }
  if (!ie_payload_container) {
    Logger::nas_mm().warn("IE ie_payload_container is not available");
  } else {
    if (int size = ie_payload_container->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_payload_container  error");
      return 0;
    }
  }
  if (!ie_network_slicing_indication) {
    Logger::nas_mm().warn("IE ie_network_slicing_indication is not available");
  } else {
    if (int size = ie_network_slicing_indication->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_network_slicing_indication  error");
      return 0;
    }
  }
  if (!ie_5gs_update_type) {
    Logger::nas_mm().warn("IE ie_5gs_update_type is not available");
  } else {
    if (int size = ie_5gs_update_type->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_5gs_update_type  error");
      return 0;
    }
  }

  if (!ie_nas_message_container) {
    Logger::nas_mm().warn("IE ie_nas_message_container is not available");
  } else {
    if (int size = ie_nas_message_container->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_nas_message_container  error");
      return 0;
    }
  }
  if (!ie_eps_bearer_context_status) {
    Logger::nas_mm().warn("IE ie_eps_bearer_context_status is not available");
  } else {
    if (int size = ie_eps_bearer_context_status->encode2buffer(
            buf + encoded_size, len - encoded_size)) {
      encoded_size += size;
    } else {
      Logger::nas_mm().error("encoding ie_eps_bearer_context_status  error");
      return 0;
    }
  }
  Logger::nas_mm().debug(
      "encoded RegistrationRequest message len(%d)", encoded_size);
  return 1;
}

//------------------------------------------------------------------------------
int RegistrationRequest::decodefrombuffer(
    NasMmPlainHeader* header, uint8_t* buf, int len) {
  Logger::nas_mm().debug("Decoding RegistrationRequest message");
  int decoded_size       = 3;
  plain_header           = header;
  ie_5gsregistrationtype = new _5GSRegistrationType();
  decoded_size += ie_5gsregistrationtype->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  ie_ngKSI = new NasKeySetIdentifier();
  decoded_size += ie_ngKSI->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false, true);
  decoded_size++;
  ie_5gs_mobility_id = new _5GSMobilityIdentity();
  decoded_size += ie_5gs_mobility_id->decodefrombuffer(
      buf + decoded_size, len - decoded_size, false);
  uint8_t octet = *(buf + decoded_size);
  Logger::nas_mm().debug("First option IEI 0x%x", octet);
  while ((octet != 0x0)) {
    switch ((octet & 0xf0) >> 4) {
      case 0xC: {
        Logger::nas_mm().debug("Decoding IEI(0xC)");
        ie_non_current_native_nas_ksi = new NasKeySetIdentifier();
        decoded_size += ie_non_current_native_nas_ksi->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true, false);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0xB: {
        Logger::nas_mm().debug("Decoding IEI (0xB)");
        ie_MICO_indicationl = new MICO_Indication();
        decoded_size += ie_MICO_indicationl->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x08: {
        Logger::nas_mm().debug("Decoding IEI (0x8)");
        ie_payload_container_type = new Payload_Container_Type();
        decoded_size += ie_payload_container_type->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x9: {
        Logger::nas_mm().debug("Decoding IEI (0x9)");
        ie_network_slicing_indication = new Network_Slicing_Indication();
        decoded_size += ie_network_slicing_indication->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);

      } break;
    }
    switch (octet) {
      case 0x10: {
        Logger::nas_mm().debug("Decoding IEI (0x10)");
        ie_5g_mm_capability = new _5GMMCapability();
        decoded_size += ie_5g_mm_capability->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x2E: {
        Logger::nas_mm().debug("Decoding IEI (0x2E)");
        ie_ue_security_capability = new UESecurityCapability();
        decoded_size += ie_ue_security_capability->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x2F: {
        Logger::nas_mm().debug("Decoding IEI (0x2F)");
        ie_requested_NSSAI = new NSSAI();
        decoded_size += ie_requested_NSSAI->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x52: {
        Logger::nas_mm().debug("Decoding IEI(0x52)");
        ie_last_visited_registered_TAI = new _5GS_Tracking_Area_Identity();
        decoded_size += ie_last_visited_registered_TAI->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x17: {
        Logger::nas_mm().debug("Decoding IEI (0x17)");
        ie_s1_ue_network_capability = new UENetworkCapability();
        decoded_size += ie_s1_ue_network_capability->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x40: {
        Logger::nas_mm().debug("Decoding IEI(0x40)");
        ie_uplink_data_status = new UplinkDataStatus();
        decoded_size += ie_uplink_data_status->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);

      } break;
      case 0x50: {
        Logger::nas_mm().debug("Decoding IEI (0x50)");
        ie_PDU_session_status = new PDU_Session_Status();
        decoded_size += ie_PDU_session_status->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x2B: {
        Logger::nas_mm().debug("Decoding IEI (0x2B)");
        ie_ue_status = new UE_Status();
        decoded_size += ie_ue_status->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x77: {
        Logger::nas_mm().debug("Decoding IEI (0x77)");
        ie_additional_guti = new _5GSMobilityIdentity();
        decoded_size += ie_additional_guti->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);

      } break;
      case 0x25: {
        Logger::nas_mm().debug("Decoding IEI(0x25)");
        ie_allowed_PDU_session_status = new Allowed_PDU_Session_Status();
        decoded_size += ie_allowed_PDU_session_status->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);

      } break;
      case 0x18: {
        Logger::nas_mm().debug("Decoding IEI(0x18)");
        ie_ues_usage_setting = new UES_Usage_Setting();
        decoded_size += ie_ues_usage_setting->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x51: {
        Logger::nas_mm().debug("Decoding IEI(0x51)");
        ie_5gs_drx_parameters = new _5GS_DRX_arameters();
        decoded_size += ie_5gs_drx_parameters->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x70: {
        Logger::nas_mm().debug("Decoding IEI(0x70)");
        ie_eps_nas_message_container = new EPS_NAS_Message_Container();
        decoded_size += ie_eps_nas_message_container->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x74: {
        Logger::nas_mm().debug("Decoding IEI(0x74)");
        ie_ladn_indication = new LADN_Indication();
        decoded_size += ie_ladn_indication->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x7B: {
        Logger::nas_mm().debug("Decoding IEI(0x7B)");
        ie_payload_container = new Payload_Container();
        decoded_size += ie_payload_container->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x53: {
        Logger::nas_mm().debug("Decoding IEI(0x53)");
        ie_5gs_update_type = new _5GS_Update_Type();
        decoded_size += ie_5gs_update_type->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x71: {
        Logger::nas_mm().debug("Decoding IEI(0x71)");
        ie_nas_message_container = new NAS_Message_Container();
        decoded_size += ie_nas_message_container->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
      case 0x60: {
        Logger::nas_mm().debug("Decoding IEI(0x71)");
        ie_eps_bearer_context_status = new EPS_Bearer_Context_Status();
        decoded_size += ie_eps_bearer_context_status->decodefrombuffer(
            buf + decoded_size, len - decoded_size, true);
        octet = *(buf + decoded_size);
        Logger::nas_mm().debug("Next IEI 0x%x", octet);
      } break;
    }
  }
  Logger::nas_mm().debug(
      "Decoded RegistrationRequest message (len %d)", decoded_size);
  return 1;
}
