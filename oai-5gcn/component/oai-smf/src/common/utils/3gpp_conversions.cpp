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

/*! \file 3gpp_conversions.cpp
 * \brief
 * \author Lionel Gauthier
 * \company Eurecom
 * \email: lionel.gauthier@eurecom.fr
 */
#include "3gpp_conversions.hpp"

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>
#include "SmContextCreateData.h"
#include "SmContextUpdateData.h"
#include "SmContextReleaseData.h"
#include "3gpp_29.500.h"
#include "3gpp_24.501.h"
#include "conversions.hpp"

//------------------------------------------------------------------------------
void xgpp_conv::paa_to_pfcp_ue_ip_address(
    const paa_t& paa, pfcp::ue_ip_address_t& ue_ip_address) {
  switch (paa.pdu_session_type.pdu_session_type) {
    case PDU_SESSION_TYPE_E_IPV4:
      ue_ip_address.v4           = 1;
      ue_ip_address.ipv4_address = paa.ipv4_address;
      break;
    case PDU_SESSION_TYPE_E_IPV6:
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv6_address = paa.ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_IPV4V6:
      ue_ip_address.v4           = 1;
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv4_address = paa.ipv4_address;
      ue_ip_address.ipv6_address = paa.ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_UNSTRUCTURED:
    case PDU_SESSION_TYPE_E_ETHERNET:
    case PDU_SESSION_TYPE_E_RESERVED:
    default:;
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::pdn_ip_to_pfcp_ue_ip_address(
    const pdu_session_type_t& pdu_session_type,
    const struct in_addr& ipv4_address, const struct in6_addr ipv6_address,
    pfcp::ue_ip_address_t& ue_ip_address) {
  switch (pdu_session_type.pdu_session_type) {
    case PDU_SESSION_TYPE_E_IPV4:
      ue_ip_address.v4           = 1;
      ue_ip_address.ipv4_address = ipv4_address;
      break;
    case PDU_SESSION_TYPE_E_IPV6:
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv6_address = ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_IPV4V6:
      ue_ip_address.v4           = 1;
      ue_ip_address.v6           = 1;
      ue_ip_address.ipv4_address = ipv4_address;
      ue_ip_address.ipv6_address = ipv6_address;
      break;
    case PDU_SESSION_TYPE_E_UNSTRUCTURED:
    case PDU_SESSION_TYPE_E_ETHERNET:
    case PDU_SESSION_TYPE_E_RESERVED:
    default:;
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::pco_nas_to_core(
    const protocol_configuration_options_nas_t& pco_nas,
    protocol_configuration_options_t& pco) {
  pco.ext                          = pco_nas.ext;
  pco.spare                        = pco_nas.spare;
  pco.configuration_protocol       = pco_nas.configuration_protocol;
  pco.num_protocol_or_container_id = pco_nas.num_protocol_or_container_id;

  for (int i = 0; i < pco.num_protocol_or_container_id; i++) {
    pco_protocol_or_container_id_t pco_item = {};

    pco_item.length_of_protocol_id_contents =
        pco_nas.protocol_or_container_ids[i].length;
    pco_item.protocol_id = pco_nas.protocol_or_container_ids[i].id;

    // pco.protocol_or_container_ids[i].length_of_protocol_id_contents =
    // pco_nas.protocol_or_container_ids[i].length;
    // pco.protocol_or_container_ids[i].protocol_id =
    // pco_nas.protocol_or_container_ids[i].id;
    if (pco_nas.protocol_or_container_ids[i].contents != nullptr) {
      unsigned char data[512] = {'\0'};
      memcpy(
          (void*) &data,
          (void*) pco_nas.protocol_or_container_ids[i].contents->data,
          pco_nas.protocol_or_container_ids[i].contents->slen);
      std::string msg_bstr(
          (char*) data, pco_nas.protocol_or_container_ids[i].contents->slen);
      // pco.protocol_or_container_ids[i].protocol_id_contents  = msg_bstr;
      pco_item.protocol_id_contents = msg_bstr;
    }

    pco.protocol_or_container_ids.push_back(pco_item);
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::pco_core_to_nas(
    const protocol_configuration_options_t& pco,
    protocol_configuration_options_nas_t& pco_nas) {
  pco_nas.ext                          = pco.ext;
  pco_nas.spare                        = pco.spare;
  pco_nas.configuration_protocol       = pco.configuration_protocol;
  pco_nas.num_protocol_or_container_id = pco.num_protocol_or_container_id;

  for (int i = 0; i < pco.num_protocol_or_container_id; i++) {
    pco_nas.protocol_or_container_ids[i].length =
        pco.protocol_or_container_ids[i].length_of_protocol_id_contents;
    pco_nas.protocol_or_container_ids[i].id =
        pco.protocol_or_container_ids[i].protocol_id;

    pco_nas.protocol_or_container_ids[i].contents = bfromcstralloc(
        pco.protocol_or_container_ids[i].protocol_id_contents.length(), "\0");
    pco_nas.protocol_or_container_ids[i].contents->slen =
        pco.protocol_or_container_ids[i].protocol_id_contents.length();
    memcpy(
        (void*) pco_nas.protocol_or_container_ids[i].contents->data,
        (void*) pco.protocol_or_container_ids[i].protocol_id_contents.c_str(),
        pco.protocol_or_container_ids[i].protocol_id_contents.length());
  }
}

//------------------------------------------------------------------------------
void xgpp_conv::sm_context_create_from_openapi(
    const oai::smf_server::model::SmContextMessage& scd,
    smf::pdu_session_create_sm_context_request& pcr) {
  Logger::smf_app().debug(
      "Convert SmContextMessage (OpenAPI) to "
      "PDUSession_CreateSMContext");

  oai::smf_server::model::SmContextCreateData context_data = {};
  if (scd.jsonDataIsSet()) {
    context_data = scd.getJsonData();
  } else {
    Logger::smf_app().warn("No Json data available");
  }

  if (scd.binaryDataN1SmMessageIsSet()) {
    std::string n1_sm_msg = scd.getBinaryDataN1SmMessage();
    // N1 SM Message
    pcr.set_n1_sm_message(n1_sm_msg);
    Logger::smf_app().debug("N1 SM message: %s", n1_sm_msg.c_str());
  } else {
    Logger::smf_app().warn("No N1 SM Message available");
  }

  if (context_data.supiIsSet()) {
    // supi
    supi_t supi             = {.length = 0};
    std::size_t pos         = context_data.getSupi().find("-");
    std::string supi_str    = context_data.getSupi().substr(pos + 1);
    std::string supi_prefix = context_data.getSupi().substr(0, pos);
    smf_string_to_supi(&supi, supi_str.c_str());
    pcr.set_supi(supi);
    pcr.set_supi_prefix(supi_prefix);
    Logger::smf_app().debug(
        "SUPI %s, SUPI Prefix %s, IMSI %s", context_data.getSupi().c_str(),
        supi_prefix.c_str(), supi_str.c_str());
  } else {
    Logger::smf_app().warn("No SUPI available");
  }

  // TODO: unauthenticatedSupi
  // DNN
  if (context_data.dnnIsSet()) {
    Logger::smf_app().debug("DNN %s", context_data.getDnn().c_str());
    pcr.set_dnn(context_data.getDnn().c_str());
  } else {
    Logger::smf_app().warn("No DNN available");
  }

  // S-Nssai
  if (context_data.sNssaiIsSet()) {
    Logger::smf_app().debug(
        "S-NSSAI SST %d, SD %s", context_data.getSNssai().getSst(),
        context_data.getSNssai().getSd().c_str());
    snssai_t snssai(
        context_data.getSNssai().getSst(), context_data.getSNssai().getSd());
    pcr.set_snssai(snssai);
  } else {
    Logger::smf_app().warn("No SNSSAI available");
  }

  // PDU session ID
  if (context_data.pduSessionIdIsSet()) {
    Logger::smf_app().debug(
        "PDU Session ID %d", context_data.getPduSessionId());
    pcr.set_pdu_session_id(context_data.getPduSessionId());
  } else {
	  Logger::smf_app().warn("No PDU Session ID available");
  }

  // AMF ID (ServingNFId/NfInstanceId)
  Logger::smf_app().debug(
      "ServingNfId %s", context_data.getServingNfId().c_str());
  pcr.set_serving_nf_id(context_data.getServingNfId()
                            .c_str());  // TODO: should be verified that AMF ID
                                        // is stored in GUAMI or ServingNfId

  // Request Type
  if (context_data.requestTypeIsSet()) {
    Logger::smf_app().debug(
        "RequestType %s", context_data.getRequestType().c_str());
    pcr.set_request_type(context_data.getRequestType());
  } else {
    Logger::smf_app().warn("No Request Type available");
  }

  // SMContextStatusUri
  pcr.set_sm_context_status_uri(context_data.getSmContextStatusUri());
  Logger::smf_app().debug(
      "SMContextStatusUri %s", context_data.getSmContextStatusUri().c_str());

  // DNN Selection Mode
  if (context_data.selModeIsSet()) {
    Logger::smf_app().debug("SelMode %s", context_data.getSelMode().c_str());
    pcr.set_dnn_selection_mode(context_data.getSelMode());
  } else {
    Logger::smf_app().warn("No SelMode available");
  }

  // ServingNetwork (PlmnId)
  Logger::smf_app().debug(
      "Serving Network (MCC %s, MNC %s)",
      context_data.getServingNetwork().getMcc().c_str(),
      context_data.getServingNetwork().getMnc().c_str());
  plmn_t p = {};
  if (conv::plmnFromString(
          p, context_data.getServingNetwork().getMcc(),
          context_data.getServingNetwork().getMnc())) {
    pcr.set_plmn(p);
  } else {
    Logger::smf_app().warn("Error while converting MCC, MNC to PLMN");
  }

  // anType (AccessType)
  Logger::smf_app().debug("AN Type %s", context_data.getAnType().c_str());
  pcr.set_an_type(context_data.getAnType());

  // Guami
  if (context_data.guamiIsSet()) {
    // Logger::smf_app().debug("GUAMI %s", context_data.getGuami().c_str());
    guami_5g_t guami = {};
    guami.amf_id     = context_data.getGuami().getAmfId();
    if (!conv::plmnFromString(
            guami.plmn, context_data.getGuami().getPlmnId().getMcc(),
            context_data.getGuami().getPlmnId().getMnc())) {
      Logger::smf_app().warn("Error while converting MCC, MNC to PLMN");
    }
    pcr.set_guami(guami);
  }

  // TODO:
  // PCF ID
  // Priority Access
  // User Location Information
  // Access Type
  // PEI
  // GPSI
  // UE presence in LADN service area
  // UETimeZone
  // PCFId
}

//------------------------------------------------------------------------------
void xgpp_conv::sm_context_update_from_openapi(
    const oai::smf_server::model::SmContextUpdateMessage& scu,
    smf::pdu_session_update_sm_context_request& pur) {
  Logger::smf_app().debug(
      "Convert SmContextUpdateMessage (OpenAPI) to "
      "PDUSession_UpdateSMContext");

  oai::smf_server::model::SmContextUpdateData context_data = scu.getJsonData();

  if (context_data.n2SmInfoIsSet()) {
    // N2 SM (for Session establishment)
    std::string n2_sm_information = scu.getBinaryDataN2SmInformation();
    Logger::smf_app().debug("N2 SM Information %s", n2_sm_information.c_str());
    pur.set_n2_sm_information(n2_sm_information);
    pur.set_n2_sm_info_type(context_data.getN2SmInfoType());
  }
  if (context_data.n1SmMsgIsSet()) {
    // N1 SM (for session modification)
    std::string n1_sm_message = scu.getBinaryDataN1SmMessage();
    Logger::smf_app().debug("N1 SM message %s", n1_sm_message.c_str());
    pur.set_n1_sm_message(n1_sm_message);
  }

  /* UE-initiated Service Request Operation, section 4.2.3.2@3GPP TS 23.502 */
  // PDU Session IDs, Operation Type, UE location Info, Access Type, RAT
  // Type, UE presence in LADN service area, Indication of Access Type can be
  // changed PDU Session IDs UpCnxState, for activation of user plane
  // (see 5.2.2.3.2.2@3GPP TS 29.502, step 1)

  if (context_data.upCnxStateIsSet())
    pur.set_upCnx_state(context_data.getUpCnxState());

  // Access Type (step 1 and 2)
  if (context_data.anTypeIsSet()) pur.set_an_type(context_data.getAnType());

  // RAT Type (step 1 and 2)
  if (context_data.ratTypeIsSet()) pur.set_rat_type(context_data.getRatType());

  // TODO:
  // UE presence in LADN service area
  // UE location information
  // Indication of Access Type can be changed
  // if (context_data.anTypeCanBeChangedIsSet())
  // pur.set_access_type_can_be_changed(context_data.isAnTypeCanBeChanged());
  // Step 15: N2 SM Info (AN Tunnel Info, List of accepted QoS Flow, List of
  // rejected Qos Flows, PDU Session ID), RAT Type, Access Type

  /* UE-initiated PDU Session Establishment Operation - section 4.3.2.2.1@3GPP
   * TS 23.502 */
  // TODO: Existing PDU session, step 3, SUPI, DNN, S-NSSAIs, SM Context ID, AMF
  // ID, Request Type, N1 SM Container (PDU Session Establishment Request), User
  // location, Access Type, RAT Type, PEI step 15. (SM Context ID -> SCID, N2
  // SM, Request Type)(Initial Request)
  // TODO: verify why Request Type is not define in context_data

  /* AMF-initiated with a release indication to request the release of the PDU
   * Session  (step 3.d, section 4.3.4.2@3GPP TS 23.502)*/
  if (context_data.releaseIsSet()) {
    pur.set_release(context_data.isRelease());
  }

  /* PDU Session Modification (SM Context ID -> SCID, N1/N2),
   * section 4.3.3.2@3GPP TS 23.502: */
  // step 1.a,UE-initiated: SM Context ID + N1 (PDU Session Modification
  // Request) step 1.e (AN initiated modification): SM Context ID, N2 SM
  // information (QFI, User location Information and an indication that the QoS
  // Flow is released) step 7a, SM Context ID, N2 SM information, UE location
  // information Step 11, SM Context ID, N1 SM (PDU Session Modification Command
  // ACK), User location
}

//------------------------------------------------------------------------------
void xgpp_conv::sm_context_release_from_openapi(
    const oai::smf_server::model::SmContextReleaseMessage& srm,
    smf::pdu_session_release_sm_context_request& prr) {
  Logger::smf_app().debug(
      "Convert SmContextReleaseMessage (OpenAPI) to "
      "PDUSession_ReleaseSMContext");

  oai::smf_server::model::SmContextReleaseData context_data = srm.getJsonData();

  if (context_data.n2SmInfoIsSet()) {
    // N2 SM (for Session establishment)
    std::string n2_sm_information = srm.getBinaryDataN2SmInformation();
    Logger::smf_app().debug("N2 SM Information %s", n2_sm_information.c_str());

    std::string n2_sm_info_type = context_data.getN2SmInfoType();
    prr.set_n2_sm_information(n2_sm_information);
    prr.set_n2_sm_info_type(n2_sm_info_type);
  }

  // TODO: Initialize necessary values for sm context req from context_data
  // cause:
  // ngApCause:
  // 5gMmCauseValue:
  // ueLocation:
  // ueTimeZone:
  // addUeLocation:
  // vsmfReleaseOnly:
  // ismfReleaseOnly:
}

//------------------------------------------------------------------------------
void xgpp_conv::data_notification_from_openapi(
    const oai::smf_server::model::NotificationData& nd,
    smf::data_notification_msg& dn_msg) {
  Logger::smf_app().debug(
      "Convert NotificationData (OpenAPI) to "
      "Data Notification Msg");

  dn_msg.set_notification_event_type(nd.getEvent());
  dn_msg.set_nf_instance_uri(nd.getNfInstanceUri());

  std::shared_ptr<smf::nf_profile> p = {};

  // Only support UPF for now
  if (nd.getNfProfile().getNfType() == "UPF")
    p = std::make_shared<smf::upf_profile>();

  nlohmann::json pj = {};
  to_json(pj, nd.getNfProfile());
  p.get()->from_json(pj);
  dn_msg.set_profile(p);
}

//------------------------------------------------------------------------------
void xgpp_conv::smf_event_exposure_notification_from_openapi(
    const oai::smf_server::model::NsmfEventExposure& nee,
    smf::event_exposure_msg& eem) {
  Logger::smf_app().debug(
      "Convert NsmfEventExposure (OpenAPI) to "
      "Event Exposure Msg");

  // Supi
  if (nee.supiIsSet()) {
    supi_t supi             = {.length = 0};
    std::size_t pos         = nee.getSupi().find("-");
    std::string supi_str    = nee.getSupi().substr(pos + 1);
    std::string supi_prefix = nee.getSupi().substr(0, pos);
    smf_string_to_supi(&supi, supi_str.c_str());

    eem.set_supi(supi);
    eem.set_supi_prefix(supi_prefix);
    Logger::smf_api_server().debug(
        "SUPI %s, SUPI Prefix %s, IMSI %s", nee.getSupi().c_str(),
        supi_prefix.c_str(), supi_str.c_str());
  }

  // PDU session ID
  if (nee.pduSeIdIsSet()) {
    Logger::smf_api_server().debug("PDU Session ID %d", nee.getPduSeId());
    eem.set_pdu_session_id(nee.getPduSeId());
  }

  eem.set_notif_id(nee.getNotifId());    // NotifId
  eem.set_notif_uri(nee.getNotifUri());  // NotifUri

  // EventSubscription: TODO
  event_subscription_t event_subscription = {};
  event_subscription.smf_event            = smf_event_t::SMF_EVENT_PDU_SES_REL;
  std::vector<event_subscription_t> event_subscriptions = {};
  event_subscriptions.push_back(event_subscription);
  eem.set_event_subs(event_subscriptions);

  // std::vector<EventSubscription> eventSubscriptions;
  // for (auto it: nee.getEventSubs()){
  // event_subscription.smf_event = it.getEvent();
  // getDnaiChgType
  // event_subscriptions.push_back(event_subscription);
  //}
}

//------------------------------------------------------------------------------
void xgpp_conv::sm_context_request_from_nas(
    const nas_message_t& nas_msg,
    smf::pdu_session_create_sm_context_request& pcr) {
  pdu_session_type_t pdu_session_type = {.pdu_session_type =
                                             PDU_SESSION_TYPE_E_IPV4};
  // Extended Protocol Discriminator
  pcr.set_epd(nas_msg.header.extended_protocol_discriminator);
  // Message Type
  pcr.set_message_type(nas_msg.plain.sm.header.message_type);
  // TODO: Integrity protection maximum data rate (Mandatory)

  // PDU session type (Optional)
  if (nas_msg.plain.sm.header.message_type ==
      PDU_SESSION_ESTABLISHMENT_REQUEST) {
    Logger::smf_app().debug(
        "PDU Session Type %d",
        nas_msg.plain.sm.pdu_session_establishment_request._pdusessiontype
            .pdu_session_type_value);
    pdu_session_type.pdu_session_type =
        nas_msg.plain.sm.pdu_session_establishment_request._pdusessiontype
            .pdu_session_type_value;
  }
  pcr.set_pdu_session_type(pdu_session_type.pdu_session_type);

  // TODO: SSCMode
  // TODO: store UE 5GSM Capability
  // TODO: MaximumNumberOfSupportedPacketFilters
  // TODO: AlwaysonPDUSessionRequested
  // TODO: SMPDUDNRequestContainer

  // ExtendedProtocolConfigurationOptions
  protocol_configuration_options_t pco = {};
  pco_nas_to_core(
      nas_msg.plain.sm.pdu_session_establishment_request
          .extendedprotocolconfigurationoptions,
      pco);
  pcr.set_epco(pco);

  // PTI
  procedure_transaction_id_t pti = {
      .procedure_transaction_id =
          nas_msg.plain.sm.header.procedure_transaction_identity};

  pcr.set_pti(pti);
}

//------------------------------------------------------------------------------
void xgpp_conv::create_sm_context_response_from_ct_request(
    const std::shared_ptr<itti_n11_create_sm_context_request>& ctx_request,
    std::shared_ptr<itti_n11_create_sm_context_response>& ctx_response) {
  ctx_response->http_version = ctx_request->http_version;
  ctx_response->res.set_http_code(http_status_code_e::HTTP_STATUS_CODE_200_OK);
  ctx_response->res.set_supi(ctx_request->req.get_supi());
  ctx_response->res.set_supi_prefix(ctx_request->req.get_supi_prefix());
  ctx_response->res.set_cause(
      static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
  ctx_response->res.set_pdu_session_id(ctx_request->req.get_pdu_session_id());
  ctx_response->res.set_snssai(ctx_request->req.get_snssai());
  ctx_response->res.set_dnn(ctx_request->req.get_dnn());
  ctx_response->res.set_pdu_session_type(
      ctx_request->req.get_pdu_session_type());
  ctx_response->res.set_pti(ctx_request->req.get_pti());
  ctx_response->set_scid(ctx_request->scid);
}

//------------------------------------------------------------------------------
void xgpp_conv::update_sm_context_response_from_ct_request(
    const std::shared_ptr<itti_n11_update_sm_context_request>& ct_request,
    std::shared_ptr<itti_n11_update_sm_context_response>& ct_response) {
  ct_response->res.set_http_code(
      http_status_code_e::HTTP_STATUS_CODE_200_OK);  // default status code
  ct_response->res.set_supi(ct_request->req.get_supi());
  ct_response->res.set_supi_prefix(ct_request->req.get_supi_prefix());
  ct_response->res.set_cause(
      static_cast<uint8_t>(cause_value_5gsm_e::CAUSE_255_REQUEST_ACCEPTED));
  ct_response->res.set_pdu_session_id(ct_request->req.get_pdu_session_id());
  ct_response->res.set_snssai(ct_request->req.get_snssai());
  ct_response->res.set_dnn(ct_request->req.get_dnn());
}
