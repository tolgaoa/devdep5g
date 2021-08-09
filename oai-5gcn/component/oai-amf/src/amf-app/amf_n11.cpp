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

/*! \file amf_n11.cpp
 \brief
 \author Keliang DU (BUPT), Tien-Thinh NGUYEN (EURECOM)
 \date 2020
 \email: contact@openairinterface.org
 */

#include "amf_n11.hpp"

#include <curl/curl.h>

#include <nlohmann/json.hpp>

#include "3gpp_ts24501.hpp"
#include "amf.hpp"
#include "amf_app.hpp"
#include "amf_config.hpp"
#include "amf_n1.hpp"
#include "itti.hpp"
#include "itti_msg_amf_app.hpp"
#include "nas_context.hpp"
// For smf_client
#include "ApiClient.h"
#include "ApiConfiguration.h"
#include "SMContextsCollectionApi.h"
#include "SmContextCreateData.h"
#include "mime_parser.hpp"
#include "ue_context.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

using namespace oai::smf::model;
using namespace oai::smf::api;
using namespace web;
using namespace web::http;
// Common features like URIs.
using namespace web::http::client;
using namespace config;
using namespace amf_application;
extern itti_mw* itti_inst;
extern amf_config amf_cfg;
extern amf_n11* amf_n11_inst;
extern amf_n1* amf_n1_inst;
extern amf_app* amf_app_inst;
extern statistics stacs;

extern void msg_str_2_msg_hex(std::string msg, bstring& b);
extern void convert_string_2_hex(std::string& input, std::string& output);
extern void print_buffer(
    const std::string app, const std::string commit, uint8_t* buf, int len);
extern bool multipart_parser(
    std::string input, std::string& jsonData, std::string& n1sm,
    std::string& n2sm);
extern unsigned char* format_string_as_hex(std::string str);
extern char* bstring2charString(bstring b);

//------------------------------------------------------------------------------
std::size_t callback(
    const char* in, std::size_t size, std::size_t num, std::string* out) {
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

//------------------------------------------------------------------------------
void octet_stream_2_hex_stream(uint8_t* buf, int len, std::string& out) {
  out       = "";
  char* tmp = (char*) calloc(1, 2 * len * sizeof(uint8_t) + 1);
  for (int i = 0; i < len; i++) {
    sprintf(tmp + 2 * i, "%02x", buf[i]);
  }
  tmp[2 * len] = '\0';
  out          = tmp;
  printf("n1sm buffer: %s\n", out.c_str());
}

/****************************************************/
/**  used to run NF(s) consumer, like smf_client ****/
/***************************************************/

void amf_n11_task(void*);
//------------------------------------------------------------------------------
void amf_n11_task(void*) {
  const task_id_t task_id = TASK_AMF_N11;
  itti_inst->notify_task_ready(task_id);
  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case SMF_SERVICES_CONSUMER: {
        Logger::amf_n11().info("Running SMF_SERVICES_CONSUMER");
        itti_smf_services_consumer* m =
            dynamic_cast<itti_smf_services_consumer*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m));
      } break;
      case NSMF_PDU_SESSION_UPDATE_SM_CTX: {
        Logger::amf_n11().info(
            "Receive Nsmf_PDUSessionUpdateSMContext, handling ...");
        itti_nsmf_pdusession_update_sm_context* m =
            dynamic_cast<itti_nsmf_pdusession_update_sm_context*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m));
      } break;
      case PDU_SESS_RES_SET_RESP: {
        Logger::amf_n11().info(
            "Receive PDU Session Resource Setup Response, handling ...");
        itti_pdu_session_resource_setup_response* m =
            dynamic_cast<itti_pdu_session_resource_setup_response*>(msg);
        amf_n11_inst->handle_itti_message(ref(*m));
      } break;
      default: {
        Logger::amf_n11().info(
            "Receive unknown message type %d", msg->msg_type);
      }
    }
  } while (true);
}

//------------------------------------------------------------------------------
amf_n11::amf_n11() {
  if (itti_inst->create_task(TASK_AMF_N11, amf_n11_task, nullptr)) {
    Logger::amf_n11().error("Cannot create task TASK_AMF_N11");
    throw std::runtime_error("Cannot create task TASK_AMF_N11");
  }
  Logger::amf_n11().startup("amf_n11 started");
}

//------------------------------------------------------------------------------
amf_n11::~amf_n11() {}

//------------------------------------------------------------------------------
void amf_n11::handle_itti_message(
    itti_pdu_session_resource_setup_response& itti_msg) {}

//------------------------------------------------------------------------------
void amf_n11::handle_itti_message(
    itti_nsmf_pdusession_update_sm_context& itti_msg) {
  string ue_context_key = "app_ue_ranid_" + to_string(itti_msg.ran_ue_ngap_id) +
                          ":amfid_" + to_string(itti_msg.amf_ue_ngap_id);
  std::shared_ptr<ue_context> uc;

  uc = amf_app_inst->ran_amf_id_2_ue_context(ue_context_key);
  std::string supi;
  if (uc.get() != nullptr) {
    supi = uc->supi;
  }
  Logger::amf_n11().debug(
      "Send PDU Session Update SM Context Request to SMF (SUPI %s, PDU Session "
      "ID %d)",
      supi.c_str(), itti_msg.pdu_session_id);

  std::shared_ptr<pdu_session_context> psc = {};
  if (!uc.get()->find_pdu_session_context(itti_msg.pdu_session_id, psc)) {
    Logger::amf_n11().error(
        "Could not find psu_session_context with SUPI %s, Failed",
        supi.c_str());
    return;
  }

  std::string smf_addr;
  std::string smf_api_version;

  if (!psc.get()->smf_available) {
    Logger::amf_n11().error("No SMF is available for this PDU session");
  } else {
    smf_addr        = psc->smf_addr;
    smf_api_version = psc->smf_api_version;
  }

  std::string smf_ip_addr, remote_uri;

  // remove http port from the URI if existed
  std::size_t found_port = smf_addr.find(":");
  if (found_port != std::string::npos)
    smf_ip_addr = smf_addr.substr(0, found_port - 1);
  else
    smf_ip_addr = smf_addr;

  std::size_t found = psc.get()->smf_context_location.find(smf_ip_addr);
  if (found != std::string::npos)
    remote_uri = psc.get()->smf_context_location + "/modify";
  else
    remote_uri = smf_addr + psc.get()->smf_context_location + "/modify";

  Logger::amf_n11().debug("SMF URI: %s", remote_uri.c_str());

  nlohmann::json pdu_session_update_request = {};
  // if (itti_msg.is_n2sm_set){
  pdu_session_update_request["n2SmInfoType"]          = itti_msg.n2sm_info_type;
  pdu_session_update_request["n2SmInfo"]["contentId"] = "n2msg";
  std::string json_part = pdu_session_update_request.dump();
  std::string n2SmMsg;
  octet_stream_2_hex_stream(
      (uint8_t*) bdata(itti_msg.n2sm), blength(itti_msg.n2sm), n2SmMsg);
  curl_http_client(
      remote_uri, json_part, "", n2SmMsg, supi, itti_msg.pdu_session_id);

  stacs.display();
}

//------------------------------------------------------------------------------
void amf_n11::handle_itti_message(itti_smf_services_consumer& smf) {
  Logger::amf_n11().debug("Handle ITTI_SMF_SERVICES_CONSUMER");
  std::shared_ptr<nas_context> nc;
  nc               = amf_n1_inst->amf_ue_id_2_nas_context(smf.amf_ue_ngap_id);
  std::string supi = "imsi-" + nc.get()->imsi;

  string ue_context_key = "app_ue_ranid_" +
                          to_string(nc.get()->ran_ue_ngap_id) + ":amfid_" +
                          to_string(nc.get()->amf_ue_ngap_id);
  std::shared_ptr<ue_context> uc;
  Logger::amf_n11().info(
      "Find ue_context in amf_app using UE Context Key: %s",
      ue_context_key.c_str());
  uc = amf_app_inst->ran_amf_id_2_ue_context(ue_context_key);
  std::shared_ptr<pdu_session_context> psc;
  if (!uc.get()->find_pdu_session_context(smf.pdu_sess_id, psc)) {
    psc = std::shared_ptr<pdu_session_context>(new pdu_session_context());
    uc.get()->add_pdu_session_context(smf.pdu_sess_id, psc);
    Logger::amf_n11().debug("Create a PDU Session Context");
  }

  psc.get()->amf_ue_ngap_id = nc.get()->amf_ue_ngap_id;
  psc.get()->ran_ue_ngap_id = nc.get()->ran_ue_ngap_id;
  psc.get()->req_type       = smf.req_type;
  psc.get()->pdu_session_id = smf.pdu_sess_id;
  psc.get()->snssai.sST     = smf.snssai.sST;
  psc.get()->snssai.sD      = smf.snssai.sD;
  psc.get()->plmn.mcc       = smf.plmn.mcc;
  psc.get()->plmn.mnc       = smf.plmn.mnc;

  // parse binary dnn and store
  std::string dnn = "default";
  if ((smf.dnn != nullptr) && (blength(smf.dnn) > 0)) {
    char* tmp = bstring2charString(smf.dnn);
    dnn       = tmp;
    free_wrapper((void**) &tmp);
  }

  Logger::amf_n11().debug("Requested DNN: %s", dnn.c_str());
  psc.get()->dnn = dnn;

  std::string smf_addr;
  std::string smf_api_version;
  if (!psc.get()->smf_available) {
    if (amf_cfg.enable_smf_selection) {
      // use NRF to find suitable SMF based on snssai, plmn and dnn
      if (!discover_smf(
              smf_addr, smf_api_version, psc.get()->snssai, psc.get()->plmn,
              psc.get()->dnn)) {
        Logger::amf_n11().error("SMF Selection, no SMF candidate is available");
        return;
      }
    } else if (!smf_selection_from_configuration(smf_addr, smf_api_version)) {
      Logger::amf_n11().error(
          "No SMF candidate is available (from configuration file)");
      return;
    }
    // store smf info to be used with this PDU session
    psc.get()->smf_available = true;
    psc->smf_addr            = smf_addr;
    psc->smf_api_version     = smf_api_version;
  } else {
    smf_addr        = psc->smf_addr;
    smf_api_version = psc->smf_api_version;
  }

  switch (smf.req_type & 0x07) {
    case PDU_SESSION_INITIAL_REQUEST: {
      // get pti
      uint8_t* sm_msg = (uint8_t*) bdata(smf.sm_msg);
      uint8_t pti     = sm_msg[2];
      Logger::amf_n11().debug(
          "Decoded PTI for PDUSessionEstablishmentRequest(0x%x)", pti);
      psc.get()->isn2sm_avaliable = false;
      handle_pdu_session_initial_request(
          supi, psc, smf_addr, smf_api_version, smf.sm_msg, dnn);
      /*
      if (psc.get()->isn1sm_avaliable && psc.get()->isn2sm_avaliable) {
        // TODO: should be removed
        itti_n1n2_message_transfer_request* itti_msg =
            new itti_n1n2_message_transfer_request(TASK_AMF_N11, TASK_AMF_APP);
        itti_msg->supi = supi;

        uint8_t accept_len = blength(psc.get()->n1sm);
        uint8_t* accept    = (uint8_t*) calloc(1, accept_len);
        memcpy(accept, (uint8_t*) bdata(psc.get()->n1sm), accept_len);
        accept[2]      = pti;
        itti_msg->n1sm = blk2bstr(accept, accept_len);
        free(accept);
        itti_msg->is_n1sm_set    = true;
        itti_msg->n2sm           = psc.get()->n2sm;
        itti_msg->is_n2sm_set    = true;
        itti_msg->pdu_session_id = psc.get()->pdu_session_id;
        std::shared_ptr<itti_n1n2_message_transfer_request> i =
            std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (0 != ret) {
          Logger::amf_n11().error(
              "Could not send ITTI message %s to task TASK_AMF_APP",
              i->get_msg_name());
        }
      } else {
        psc.get()->isn2sm_avaliable = false;
        handle_pdu_session_initial_request(
            supi, psc, smf_addr, smf_api_version, smf.sm_msg, dnn);
      }
      */
    } break;
    case EXISTING_PDU_SESSION: {
      // TODO:
    } break;
    case PDU_SESSION_TYPE_MODIFICATION_REQUEST: {
      // TODO:
    } break;
    default: {
      // send Nsmf_PDUSession_UpdateSM_Context to SMF e.g., for PDU Session
      // release request
      send_pdu_session_update_sm_context_request(
          supi, psc, smf_addr, smf.sm_msg, dnn);
    }
  }
}

//------------------------------------------------------------------------------
void amf_n11::send_pdu_session_update_sm_context_request(
    std::string supi, std::shared_ptr<pdu_session_context> psc,
    std::string smf_addr, bstring sm_msg, std::string dnn) {
  Logger::amf_n11().debug(
      "Send PDU Session Update SM Context Request to SMF (SUPI %s, PDU Session "
      "ID %d)",
      supi.c_str(), psc.get()->pdu_session_id);

  std::string smf_ip_addr, remote_uri;
  // remove http port from the URI if existed
  std::size_t found_port = smf_addr.find(":");
  if (found_port != std::string::npos)
    smf_ip_addr = smf_addr.substr(0, found_port - 1);
  else
    smf_ip_addr = smf_addr;

  std::size_t found = psc.get()->smf_context_location.find(smf_ip_addr);
  if (found != std::string::npos)
    remote_uri = psc.get()->smf_context_location + "/modify";
  else
    remote_uri = smf_addr + psc.get()->smf_context_location + "/modify";

  Logger::amf_n11().debug("SMF URI: %s", remote_uri.c_str());

  nlohmann::json pdu_session_update_request          = {};
  pdu_session_update_request["n1SmMsg"]["contentId"] = "n1SmMsg";
  std::string json_part = pdu_session_update_request.dump();

  std::string n1SmMsg;
  octet_stream_2_hex_stream((uint8_t*) bdata(sm_msg), blength(sm_msg), n1SmMsg);
  curl_http_client(
      remote_uri, json_part, n1SmMsg, "", supi, psc.get()->pdu_session_id);
}

//------------------------------------------------------------------------------
void amf_n11::handle_pdu_session_initial_request(
    std::string supi, std::shared_ptr<pdu_session_context> psc,
    std::string smf_addr, std::string smf_api_version, bstring sm_msg,
    std::string dnn) {
  Logger::amf_n11().debug(
      "Handle PDU Session Establishment Request (SUPI %s, PDU Session ID %d)",
      supi.c_str(), psc.get()->pdu_session_id);

  // TODO: Remove hardcoded values
  std::string remote_uri =
      smf_addr + "/nsmf-pdusession/" + smf_api_version + "/sm-contexts";
  nlohmann::json pdu_session_establishment_request;
  pdu_session_establishment_request["supi"]          = supi.c_str();
  pdu_session_establishment_request["pei"]           = "imei-200000000000001";
  pdu_session_establishment_request["gpsi"]          = "msisdn-200000000001";
  pdu_session_establishment_request["dnn"]           = dnn.c_str();
  pdu_session_establishment_request["sNssai"]["sst"] = psc.get()->snssai.sST;
  pdu_session_establishment_request["sNssai"]["sd"]  = psc.get()->snssai.sD;
  pdu_session_establishment_request["pduSessionId"] = psc.get()->pdu_session_id;
  pdu_session_establishment_request["requestType"] =
      "INITIAL_REQUEST";  // TODO: from SM_MSG
  pdu_session_establishment_request["servingNfId"] = "servingNfId";
  pdu_session_establishment_request["servingNetwork"]["mcc"] =
      psc.get()->plmn.mcc;
  pdu_session_establishment_request["servingNetwork"]["mnc"] =
      psc.get()->plmn.mnc;
  pdu_session_establishment_request["anType"] = "3GPP_ACCESS";  // TODO
  pdu_session_establishment_request["smContextStatusUri"] =
      "http://" +
      std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.n11.addr4))) +
      "/nsmf-pdusession/callback/" + supi + "/" +
      std::to_string(psc.get()->pdu_session_id);

  pdu_session_establishment_request["n1MessageContainer"]["n1MessageClass"] =
      "SM";
  pdu_session_establishment_request["n1MessageContainer"]["n1MessageContent"]
                                   ["contentId"] = "n1SmMsg";

  std::string json_part = pdu_session_establishment_request.dump();
  std::string n1SmMsg;
  octet_stream_2_hex_stream((uint8_t*) bdata(sm_msg), blength(sm_msg), n1SmMsg);
  curl_http_client(
      remote_uri, json_part, n1SmMsg, "", supi, psc.get()->pdu_session_id);
}

//------------------------------------------------------------------------------
void amf_n11::handle_itti_message(
    itti_nsmf_pdusession_release_sm_context& itti_msg) {
  // TODO: Need PDU session ID
  uint8_t pdu_session_id                   = 1;  // Hardcoded
  std::shared_ptr<pdu_session_context> psc = {};
  if (!amf_app_inst->find_pdu_session_context(
          itti_msg.supi, pdu_session_id, psc)) {
    Logger::amf_n11().warn(
        "PDU Session context for SUPI %s doesn't exit!", itti_msg.supi.c_str());
    return;
  }

  string smf_addr, smf_api_version;

  if (!psc.get()->smf_available) {
    Logger::amf_n11().error("No SMF is available for this PDU session");
  } else {
    smf_addr        = psc->smf_addr;
    smf_api_version = psc->smf_api_version;
  }

  string remote_uri = psc.get()->location + "release";
  nlohmann::json pdu_session_release_request;
  pdu_session_release_request["supi"]          = itti_msg.supi.c_str();
  pdu_session_release_request["dnn"]           = psc.get()->dnn.c_str();
  pdu_session_release_request["sNssai"]["sst"] = 1;
  pdu_session_release_request["sNssai"]["sd"]  = "0";
  pdu_session_release_request["pduSessionId"]  = psc.get()->pdu_session_id;
  pdu_session_release_request["cause"]         = "REL_DUE_TO_REACTIVATION";
  pdu_session_release_request["ngApCause"]     = "radioNetwork";
  std::string json_part = pdu_session_release_request.dump();
  curl_http_client(
      remote_uri, json_part, "", "", itti_msg.supi, psc.get()->pdu_session_id);
}

// SMF selection
//------------------------------------------------------------------------------
bool amf_n11::smf_selection_from_configuration(
    std::string& smf_addr, std::string& smf_api_version) {
  for (int i = 0; i < amf_cfg.smf_pool.size(); i++) {
    if (amf_cfg.smf_pool[i].selected) {
      smf_addr = amf_cfg.smf_pool[i].ipv4 + ":" + amf_cfg.smf_pool[i].port;
      smf_api_version = amf_cfg.smf_pool[i].version;
      return true;
    }
  }
  return false;
}

//------------------------------------------------------------------------------
void amf_n11::handle_post_sm_context_response_error_400() {}

//------------------------------------------------------------------------------
void amf_n11::handle_post_sm_context_response_error(
    long code, std::string cause, bstring n1sm, std::string supi,
    uint8_t pdu_session_id) {
  print_buffer("amf_n11", "n1 sm", (uint8_t*) bdata(n1sm), blength(n1sm));
  itti_n1n2_message_transfer_request* itti_msg =
      new itti_n1n2_message_transfer_request(TASK_AMF_N11, TASK_AMF_APP);
  itti_msg->n1sm           = n1sm;
  itti_msg->is_n2sm_set    = false;
  itti_msg->supi           = supi;
  itti_msg->pdu_session_id = pdu_session_id;
  std::shared_ptr<itti_n1n2_message_transfer_request> i =
      std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_n11().error(
        "Could not send ITTI message %s to task TASK_AMF_APP",
        i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void amf_n11::curl_http_client(
    std::string remoteUri, std::string jsonData, std::string n1SmMsg,
    std::string n2SmMsg, std::string supi, uint8_t pdu_session_id) {
  Logger::amf_n11().debug("Call SMF service: %s", remoteUri.c_str());

  uint8_t number_parts = 0;
  mime_parser parser   = {};
  std::string body;
  std::shared_ptr<pdu_session_context> psc;

  if (!amf_app_inst->find_pdu_session_context(supi, pdu_session_id, psc)) {
    Logger::amf_n11().warn(
        "PDU Session context for SUPI %s doesn't exit!", supi.c_str());
    // TODO:
  }

  if ((n1SmMsg.size() > 0) and (n2SmMsg.size() > 0)) {
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, jsonData, CURL_MIME_BOUNDARY, n1SmMsg, n2SmMsg);
  } else if (n1SmMsg.size() > 0) {  // only N1 content
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, jsonData, CURL_MIME_BOUNDARY, n1SmMsg,
        multipart_related_content_part_e::NAS);
  } else if (n2SmMsg.size() > 0) {  // only N2 content
    // prepare the body content for Curl
    parser.create_multipart_related_content(
        body, jsonData, CURL_MIME_BOUNDARY, n2SmMsg,
        multipart_related_content_part_e::NGAP);
  }

  Logger::amf_n11().debug(
      "Send HTTP message to SMF with body %s", body.c_str());

  uint32_t str_len = body.length();
  char* body_data  = (char*) malloc(str_len + 1);
  memset(body_data, 0, str_len + 1);
  memcpy((void*) body_data, (void*) body.c_str(), str_len);

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;

    std::string content_type = "content-type: multipart/related; boundary=" +
                               std::string(CURL_MIME_BOUNDARY);
    headers = curl_slist_append(headers, content_type.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, remoteUri.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, SMF_CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.n11.if_name.c_str());

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    std::unique_ptr<std::string> httpHeaderData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, httpHeaderData.get());

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_data);

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get cause from the response
    std::string response           = *httpData.get();
    std::string json_data_response = "";
    std::string n1sm               = "";
    std::string n2sm               = "";
    nlohmann::json response_data   = {};
    bstring n1sm_hex, n2sm_hex;

    Logger::amf_n11().debug("Get response with HTTP code (%d)", httpCode);
    Logger::amf_n11().debug("Response body %s", response.c_str());

    if (static_cast<http_response_codes_e>(httpCode) ==
        http_response_codes_e::HTTP_RESPONSE_CODE_0) {
      // TODO: should be removed
      Logger::amf_n11().error(
          "Cannot get response when calling %s", remoteUri.c_str());
      // free curl before returning
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
      // TODO: To be verified
      psc.get()->smf_context_location =
          "/nsmf-pdusession/v2/sm-contexts/1";  // try to fix bugs for
                                                // no-response from SMF when
                                                // requesting
                                                // /nsmf-pdusession/v2/sm-contexts
                                                // (first pdu session
                                                // establishment request)
      return;
    }

    if (response.size() > 0) {
      number_parts = multipart_parser(response, json_data_response, n1sm, n2sm);
    }

    if ((static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_200_OK) &&
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_201_CREATED) &&
        (static_cast<http_response_codes_e>(httpCode) !=
         http_response_codes_e::HTTP_RESPONSE_CODE_204_UPDATED)) {
      // ERROR
      if (response.size() < 1) {
        Logger::amf_n11().error("There's no content in the response");
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        // TODO: send context response error
        return;
      }

      // Transfer N1 to gNB/UE if available
      if (number_parts > 1) {
        try {
          response_data = nlohmann::json::parse(json_data_response);
        } catch (nlohmann::json::exception& e) {
          Logger::amf_n11().warn(
              "Could not get Json content from the response");
          // Set the default Cause
          response_data["error"]["cause"] = "504 Gateway Timeout";
        }

        Logger::amf_n11().debug(
            "Get response with jsonData: %s", json_data_response.c_str());
        msg_str_2_msg_hex(n1sm, n1sm_hex);
        print_buffer(
            "amf_n11", "Get response with n1sm:", (uint8_t*) bdata(n1sm_hex),
            blength(n1sm_hex));

        std::string cause = response_data["error"]["cause"];
        Logger::amf_n11().debug(
            "Call Network Function services failure (with cause %s)",
            cause.c_str());
        //         if (!cause.compare("DNN_DENIED"))
        handle_post_sm_context_response_error(
            httpCode, cause, n1sm_hex, supi, pdu_session_id);
      }

    } else {  // Response with success code
      // Store location of the created context in case of PDU Session
      // Establishment
      std::string header_response = *httpHeaderData.get();
      std::string CRLF            = "\r\n";
      std::size_t location_pos    = header_response.find("Location");

      if (location_pos != std::string::npos) {
        std::size_t crlf_pos = header_response.find(CRLF, location_pos);
        if (crlf_pos != std::string::npos) {
          std::string location = header_response.substr(
              location_pos + 10, crlf_pos - (location_pos + 10));
          Logger::amf_n11().info(
              "Location of the created SMF context: %s", location.c_str());
          psc.get()->smf_context_location = location;
        }
      }

      // Transfer N1/N2 to gNB/UE if available
      if (number_parts > 1) {
        try {
          response_data = nlohmann::json::parse(json_data_response);
        } catch (nlohmann::json::exception& e) {
          Logger::amf_n11().warn(
              "Could not get Json content from the response");
          // TODO:
        }

        itti_n1n2_message_transfer_request* itti_msg =
            new itti_n1n2_message_transfer_request(TASK_AMF_N11, TASK_AMF_APP);

        if (n1sm.size() > 0) {
          msg_str_2_msg_hex(n1sm, n1sm_hex);
          print_buffer(
              "amf_n11", "Get response n1sm:", (uint8_t*) bdata(n1sm_hex),
              blength(n1sm_hex));
          itti_msg->n1sm        = n1sm_hex;
          itti_msg->is_n1sm_set = true;
        }
        if (n2sm.size() > 0) {
          msg_str_2_msg_hex(n2sm, n2sm_hex);
          print_buffer(
              "amf_n11", "Get response n2sm:", (uint8_t*) bdata(n2sm_hex),
              blength(n2sm_hex));
          itti_msg->n2sm           = n2sm_hex;
          itti_msg->is_n2sm_set    = true;
          itti_msg->n2sm_info_type = response_data
              ["n2SmInfoType"];  // response_data["n2InfoContainer"]["smInfo"]["n2InfoContent"]["ngapIeType"];
        }

        itti_msg->supi           = supi;
        itti_msg->pdu_session_id = pdu_session_id;
        std::shared_ptr<itti_n1n2_message_transfer_request> i =
            std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
        int ret = itti_inst->send_msg(i);
        if (0 != ret) {
          Logger::amf_n11().error(
              "Could not send ITTI message %s to task TASK_AMF_APP",
              i->get_msg_name());
        }
      }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();
  free_wrapper((void**) &body_data);
}

//-----------------------------------------------------------------------------------------------------
bool amf_n11::discover_smf(
    std::string& smf_addr, std::string& smf_api_version, const snssai_t snssai,
    const plmn_t plmn, const std::string dnn) {
  Logger::amf_n11().debug(
      "Send NFDiscovery to NRF to discover the available SMFs");
  bool result = true;

  nlohmann::json json_data = {};
  // TODO: remove hardcoded values
  std::string url =
      std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.nrf_addr.ipv4_addr))) +
      ":" + std::to_string(amf_cfg.nrf_addr.port) + "/nnrf-disc/" +
      amf_cfg.nrf_addr.api_version +
      "/nf-instances?target-nf-type=SMF&requester-nf-type=AMF";

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;
    // headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NRF_CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.n11.if_name.c_str());

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    Logger::amf_n11().debug(
        "NFDiscovery, response from NRF, HTTP Code: %d", httpCode);

    if (httpCode == 200) {
      Logger::amf_n11().debug("NFDiscovery, got successful response from NRF");

      nlohmann::json response_data = {};
      try {
        response_data = nlohmann::json::parse(*httpData.get());
      } catch (nlohmann::json::exception& e) {
        Logger::amf_n11().warn(
            "NFDiscovery, could not parse json from the NRF "
            "response");
      }
      Logger::amf_n11().debug(
          "NFDiscovery, response from NRF, json data: \n %s",
          response_data.dump().c_str());

      // Process data to obtain SMF info
      if (response_data.find("nfInstances") != response_data.end()) {
        for (auto& it : response_data["nfInstances"].items()) {
          nlohmann::json instance_json = it.value();
          // TODO: convert instance_json to SMF profile
          // TODO: add SMF to the list of available SMF
          // TODO: check with sNSSAI and DNN
          // TODO: PLMN (need to add plmnList into NRF profile, SMF profile)
          // for now, just IP addr of SMF of the first NF instance
          if (instance_json.find("ipv4Addresses") != instance_json.end()) {
            if (instance_json["ipv4Addresses"].size() > 0)
              smf_addr =
                  instance_json["ipv4Addresses"].at(0).get<std::string>();
            // break;
          }
          if (instance_json.find("nfServices") != instance_json.end()) {
            if (instance_json["nfServices"].size() > 0) {
              nlohmann::json nf_service = instance_json["nfServices"].at(0);
              if (nf_service.find("versions") != nf_service.end()) {
                nlohmann::json nf_version = nf_service["versions"].at(0);
                if (nf_version.find("apiVersionInUri") != nf_version.end()) {
                  smf_api_version =
                      nf_version["apiVersionInUri"].get<std::string>();
                }
              }
            }
            break;
          }
        }
      }
    } else {
      Logger::amf_n11().warn("NFDiscovery, could not get response from NRF");
      result = false;
    }

    Logger::amf_n11().debug(
        "NFDiscovery, SMF Addr: %s, SMF Api Version: %s", smf_addr.c_str(),
        smf_api_version.c_str());

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return result;
}

//-----------------------------------------------------------------------------------------------------
bool amf_n11::send_ue_authentication_request(
    oai::amf::model::AuthenticationInfo& auth_info,
    oai::amf::model::UEAuthenticationCtx& ue_auth_ctx, uint8_t http_version) {
  Logger::amf_n11().debug(
      "Send UE Authentication Request to AUSF (HTTP version %d)", http_version);

  nlohmann::json json_data = {};
  to_json(json_data, auth_info);
  std::string url =
      std::string(
          inet_ntoa(*((struct in_addr*) &amf_cfg.ausf_addr.ipv4_addr))) +
      ":" + std::to_string(amf_cfg.ausf_addr.port) + "/nausf-auth/" +
      amf_cfg.ausf_addr.api_version + "/ue-authentications";

  Logger::amf_n11().debug(
      "Send UE Authentication Request to AUSF, URL %s", url.c_str());

  std::string body = json_data.dump();
  Logger::amf_n11().debug(
      "Send UE Authentication Request to AUSF, msg body: \n %s", body.c_str());

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;
    // headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "content-type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, AUSF_CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.n11.if_name.c_str());

    if (http_version == 2) {
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      // we use a self-signed test server, skip verification during debugging
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
      curl_easy_setopt(
          curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
    }

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    Logger::amf_n11().debug(
        "UE Authentication, response from AUSF, HTTP Code: %d", httpCode);

    if ((httpCode == 200) or
        (httpCode == 201)) {  // TODO: remove hardcoded value
      try {
        std::string tmp = *httpData.get();
        nlohmann::json::parse(tmp.c_str()).get_to(ue_auth_ctx);
        Logger::amf_n11().debug(
            "UE Authentication, response from AUSF\n, %s ", tmp.c_str());
      } catch (nlohmann::json::exception& e) {
        Logger::amf_n11().warn(
            "UE Authentication, could not parse Json from the AUSF "
            "response");
        // TODO: error handling
        return false;
      }

    } else {
      Logger::amf_n11().warn(
          "UE Authentication, could not get response from AUSF");
      return false;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
  return true;
}

//-----------------------------------------------------------------------------------------------------
// From AMF_N1, need to be reworked
void amf_n11::curl_http_client(
    std::string remoteUri, std::string Method, std::string msgBody,
    std::string& Response) {
  Logger::amf_n1().info("Send HTTP message with body %s", msgBody.c_str());

  uint32_t str_len = msgBody.length();
  char* body_data  = (char*) malloc(str_len + 1);
  memset(body_data, 0, str_len + 1);
  memcpy((void*) body_data, (void*) msgBody.c_str(), str_len);

  curl_global_init(CURL_GLOBAL_ALL);
  CURL* curl = curl_easy_init();

  if (curl) {
    CURLcode res               = {};
    struct curl_slist* headers = nullptr;
    if (!Method.compare("POST") || !Method.compare("PATCH") ||
        !Method.compare("PUT")) {
      std::string content_type = "Content-Type: application/json";
      headers = curl_slist_append(headers, content_type.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }
    curl_easy_setopt(curl, CURLOPT_URL, remoteUri.c_str());
    if (!Method.compare("POST"))
      curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
    else if (!Method.compare("PATCH"))
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    else if (!Method.compare("PUT")) {
      curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    } else
      curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, CURL_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1);
    curl_easy_setopt(curl, CURLOPT_INTERFACE, amf_cfg.n11.if_name.c_str());
    //    Logger::amf_n1().info("[CURL] request sent by interface " +
    //    udm_cfg.nudr.if_name);

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    std::unique_ptr<std::string> httpHeaderData(new std::string());

    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, httpHeaderData.get());
    if (!Method.compare("POST") || !Method.compare("PATCH") ||
        !Method.compare("PUT")) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, msgBody.length());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body_data);
    }

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get the response
    std::string response           = *httpData.get();
    std::string json_data_response = "";
    std::string resMsg             = "";
    bool is_response_ok            = true;
    Logger::amf_n1().info("Get response with httpcode (%d)", httpCode);

    if (httpCode == 0) {
      Logger::amf_n1().info(
          "Cannot get response when calling %s", remoteUri.c_str());
      // free curl before returning
      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
      return;
    }

    nlohmann::json response_data = {};

    if (httpCode != 200 && httpCode != 201 && httpCode != 204) {
      is_response_ok = false;
      if (response.size() < 1) {
        Logger::amf_n1().info("There's no content in the response");
        // TODO: send context response error
        return;
      }
      Logger::amf_n1().info("Wrong response code");

      return;
    }

    else {
      Response = *httpData.get();
    }

    if (!is_response_ok) {
      try {
        response_data = nlohmann::json::parse(json_data_response);
      } catch (nlohmann::json::exception& e) {
        Logger::amf_n1().info("Could not get Json content from the response");
        // Set the default Cause
        response_data["error"]["cause"] = "504 Gateway Timeout";
      }

      Logger::amf_n1().info(
          "Get response with jsonData: %s", json_data_response.c_str());

      std::string cause = response_data["error"]["cause"];
      Logger::amf_n1().info("Call Network Function services failure");
      Logger::amf_n1().info("Cause value: %s", cause.c_str());
    }
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  if (body_data) {
    free(body_data);
    body_data = NULL;
  }
  fflush(stdout);
}
