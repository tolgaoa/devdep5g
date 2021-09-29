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

/*! \file spgwu_nrf.cpp
 \brief
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN
 \company Eurecom
 \date 2021
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr
 */

#include "spgwu_nrf.hpp"

#include <stdexcept>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "itti.hpp"
#include "logger.hpp"
#include "3gpp_29.510.h"
#include "spgwu_config.hpp"

using namespace spgwu;
using json = nlohmann::json;

extern itti_mw* itti_inst;
extern spgwu_nrf* spgwu_nrf_inst;
extern spgwu_config spgwu_cfg;
void spgwu_nrf_task(void*);

// To read content of the response from NF
static std::size_t callback(
    const char* in, std::size_t size, std::size_t num, std::string* out) {
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

//------------------------------------------------------------------------------
void spgwu_nrf_task(void* args_p) {
  const task_id_t task_id = TASK_SPGWU_NRF;
  itti_inst->notify_task_ready(task_id);

  do {
    std::shared_ptr<itti_msg> shared_msg = itti_inst->receive_msg(task_id);
    auto* msg                            = shared_msg.get();
    switch (msg->msg_type) {
      case TIME_OUT:
        if (itti_msg_timeout* to = dynamic_cast<itti_msg_timeout*>(msg)) {
          Logger::spgwu_app().info("TIME-OUT event timer id %d", to->timer_id);
          switch (to->arg1_user) {
            case TASK_SPGWU_NRF_TIMEOUT_NRF_HEARTBEAT:
              spgwu_nrf_inst->timer_nrf_heartbeat_timeout(
                  to->timer_id, to->arg2_user);
              break;
            case TASK_SPGWU_NRF_TIMEOUT_NRF_DEREGISTRATION:
              spgwu_nrf_inst->timer_nrf_deregistration(
                  to->timer_id, to->arg2_user);
              break;
            default:;
          }
        }
        break;

      case TERMINATE:
        if (itti_msg_terminate* terminate =
                dynamic_cast<itti_msg_terminate*>(msg)) {
          Logger::spgwu_app().info("Received terminate message");
          return;
        }
        break;

      default:
        Logger::spgwu_app().info("no handler for msg type %d", msg->msg_type);
    }

  } while (true);
}

//------------------------------------------------------------------------------
spgwu_nrf::spgwu_nrf() {
  Logger::spgwu_app().startup("Starting...");
  if (itti_inst->create_task(TASK_SPGWU_NRF, spgwu_nrf_task, nullptr)) {
    Logger::spgwu_app().error("Cannot create task TASK_SPGWU_NRF");
    throw std::runtime_error("Cannot create task TASK_SPGWU_NRF");
  }
  // Register to NRF
  register_to_nrf();
  Logger::spgwu_app().startup("Started");
}

//-----------------------------------------------------------------------------------------------------
void spgwu_nrf::send_register_nf_instance(std::string& url) {
  Logger::spgwu_app().debug("Send NF Instance Registration to NRF");

  nlohmann::json json_data = {};
  upf_profile.to_json(json_data);

  Logger::spgwu_app().debug(
      "Send NF Instance Registration to NRF (NRF URL %s)", url.c_str());

  std::string body = json_data.dump();
  Logger::spgwu_app().debug(
      "Send NF Instance Registration to NRF, msg body: \n %s", body.c_str());

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
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NRF_CURL_TIMEOUT_MS);

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    Logger::spgwu_app().debug("Response from NRF, Http Code: %d", httpCode);

    if (httpCode == 201) {
      json response_data = {};
      try {
        response_data = json::parse(*httpData.get());
      } catch (json::exception& e) {
        Logger::spgwu_app().warn("Could not parse json from the NRF response");
      }
      Logger::spgwu_app().debug(
          "Response from NRF, Json data: \n %s", response_data.dump().c_str());

      // Update NF profile
      upf_profile.from_json(response_data);
      Logger::spgwu_app().debug("Updated UPF profile");
      upf_profile.display();

      // set Heartbeat timer timeout
      timer_nrf_heartbeat = itti_inst->timer_setup(
          upf_profile.get_nf_heartBeat_timer(), 0, TASK_SPGWU_NRF,
          TASK_SPGWU_NRF_TIMEOUT_NRF_HEARTBEAT,
          0);  // TODO arg2_user

    } else {
      Logger::spgwu_app().warn("Could not get response from NRF");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}

//-----------------------------------------------------------------------------------------------------
void spgwu_nrf::send_update_nf_instance(
    std::string& url, nlohmann::json& data) {
  Logger::spgwu_app().debug("Send NF Update to NRF");

  std::string body = data.dump();
  Logger::spgwu_app().debug(
      "Send NF Update to NRF (Msg body %s)", body.c_str());

  Logger::spgwu_app().debug("Send NF Update to NRF (NRF URL %s)", url.c_str());

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
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NRF_CURL_TIMEOUT_MS);

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    Logger::spgwu_app().debug("Response from NRF, Http Code: %d", httpCode);

    if ((httpCode == 200) or (httpCode == 204)) {
      Logger::spgwu_app().debug("Got successful response from NRF");
    } else {
      Logger::spgwu_app().warn("Could not get response from NRF");
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}

//-----------------------------------------------------------------------------------------------------
void spgwu_nrf::send_deregister_nf_instance(std::string& url) {
  Logger::spgwu_app().debug("Send NF De-register to NRF");

  Logger::spgwu_app().debug(
      "Send NF De-register to NRF (NRF URL %s)", url.c_str());

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
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NRF_CURL_TIMEOUT_MS);

    // Response information.
    long httpCode = {0};
    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    Logger::spgwu_app().debug("Response from NRF, Http Code: %d", httpCode);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}

//---------------------------------------------------------------------------------------------
void spgwu_nrf::generate_upf_profile() {
  // generate UUID
  generate_uuid();
  // TODO: get hardcoded values from configuration file
  upf_profile.set_nf_instance_id(upf_instance_id);
  upf_profile.set_nf_instance_name("OAI-UPF");
  upf_profile.set_nf_type("UPF");
  upf_profile.set_nf_status("REGISTERED");
  upf_profile.set_nf_heartBeat_timer(50);
  upf_profile.set_nf_priority(1);
  upf_profile.set_nf_capacity(100);
  upf_profile.add_nf_ipv4_addresses(spgwu_cfg.s1_up.addr4);
  // TODO: get SNSSAIS from Configuration file
  snssai_t snssai = {};
  snssai.sD       = "123";
  snssai.sST      = 222;
  upf_profile.add_snssai(snssai);
  // TODO: get UPF info from configuration file
  dnn_upf_info_item_t dnn_item         = {.dnn = "default"};
  snssai_upf_info_item_t upf_info_item = {};
  upf_info_item.dnn_upf_info_list.push_back(dnn_item);
  upf_info_item.snssai.sD  = "123";
  upf_info_item.snssai.sST = 222;
  upf_profile.add_upf_info_item(upf_info_item);

  // Display the profile
  upf_profile.display();
}

//---------------------------------------------------------------------------------------------
void spgwu_nrf::register_to_nrf() {
  // create a NF profile to this instance
  generate_upf_profile();
  // register to NRF
  std::string url =
      std::string(
          inet_ntoa(*((struct in_addr*) &spgwu_cfg.nrf_addr.ipv4_addr))) +
      ":" + std::to_string(spgwu_cfg.nrf_addr.port) + NNRF_NFM_BASE +
      spgwu_cfg.nrf_addr.api_version + NNRF_NF_REGISTER_URL + upf_instance_id;
  send_register_nf_instance(url);
}

//------------------------------------------------------------------------------
void spgwu_nrf::generate_uuid() {
  upf_instance_id = to_string(boost::uuids::random_generator()());
}

//---------------------------------------------------------------------------------------------
void spgwu_nrf::timer_nrf_heartbeat_timeout(
    timer_id_t timer_id, uint64_t arg2_user) {
  Logger::spgwu_app().debug("Send Heartbeat to NRF");

  patch_item_t patch_item = {};
  //{"op":"replace","path":"/nfStatus", "value": "REGISTERED"}
  patch_item.op    = "replace";
  patch_item.path  = "/nfStatus";
  patch_item.value = "REGISTERED";

  nlohmann::json json_data = nlohmann::json::array();
  nlohmann::json item      = patch_item.to_json();
  json_data.push_back(item);

  std::string url =
      std::string(
          inet_ntoa(*((struct in_addr*) &spgwu_cfg.nrf_addr.ipv4_addr))) +
      ":" + std::to_string(spgwu_cfg.nrf_addr.port) + NNRF_NFM_BASE +
      spgwu_cfg.nrf_addr.api_version + NNRF_NF_REGISTER_URL + upf_instance_id;

  Logger::spgwu_app().debug(
      "Set a timer to the next Heart-beat (%d)",
      upf_profile.get_nf_heartBeat_timer());
  timer_nrf_heartbeat = itti_inst->timer_setup(
      upf_profile.get_nf_heartBeat_timer(), 0, TASK_SPGWU_NRF,
      TASK_SPGWU_NRF_TIMEOUT_NRF_HEARTBEAT,
      0);  // TODO arg2_user

  send_update_nf_instance(url, json_data);
}

//---------------------------------------------------------------------------------------------
void spgwu_nrf::timer_nrf_deregistration(
    timer_id_t timer_id, uint64_t arg2_user) {
  Logger::spgwu_app().debug("Send NF De-registration to NRF");
  std::string url =
      std::string(
          inet_ntoa(*((struct in_addr*) &spgwu_cfg.nrf_addr.ipv4_addr))) +
      ":" + std::to_string(spgwu_cfg.nrf_addr.port) + NNRF_NFM_BASE +
      spgwu_cfg.nrf_addr.api_version + NNRF_NF_REGISTER_URL + upf_instance_id;

  send_deregister_nf_instance(url);
}
