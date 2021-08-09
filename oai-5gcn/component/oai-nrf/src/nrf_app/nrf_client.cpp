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

/*! \file nrf_client.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#include "nrf_client.hpp"

#include <curl/curl.h>
#include <pistache/http.h>
#include <pistache/mime.h>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "3gpp_29.500.h"
#include "logger.hpp"
#include "nrf.h"
#include "nrf_config.hpp"

using namespace Pistache::Http;
using namespace Pistache::Http::Mime;
using namespace oai::nrf::app;
using json = nlohmann::json;

extern nrf_client* nrf_client_inst;
extern nrf_config nrf_cfg;

//------------------------------------------------------------------------------
// To read content of the response from NF
static std::size_t callback(
    const char* in, std::size_t size, std::size_t num, std::string* out) {
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

//------------------------------------------------------------------------------
nrf_client::nrf_client(nrf_event& ev) : m_event_sub(ev) {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl_multi = curl_multi_init();
  handles    = {};
  headers    = NULL;
  headers    = curl_slist_append(headers, "Accept: application/json");
  headers    = curl_slist_append(headers, "Content-Type: application/json");
  headers    = curl_slist_append(headers, "charsets: utf-8");
  // subscribe_task_curl();
}

//------------------------------------------------------------------------------
nrf_client::~nrf_client() {
  Logger::nrf_app().debug("Delete NRF Client instance...");
  // Remove handle, free memory
  for (auto h : handles) {
    curl_multi_remove_handle(curl_multi, h);
    curl_easy_cleanup(h);
  }

  handles.clear();
  curl_multi_cleanup(curl_multi);
  curl_global_cleanup();
  curl_slist_free_all(headers);

  if (task_connection.connected()) task_connection.disconnect();
}

//------------------------------------------------------------------------------
CURL* nrf_client::curl_create_handle(
    const std::string& uri, const std::string& data,
    std::string& response_data) {
  // create handle for a curl request
  CURL* curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    // curl_easy_setopt(curl, CURLOPT_PRIVATE, str);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NF_CURL_TIMEOUT_MS);
    // Hook up data handling function.
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
  }
  return curl;
}

//------------------------------------------------------------------------------
void nrf_client::send_curl_multi(
    const std::string& uri, const std::string& data,
    std::string& response_data) {
  // create a new handle and add to the multi handle
  // the curl will actually be sent in perform_curl_multi
  CURL* tmp = curl_create_handle(uri, data, response_data);
  curl_multi_add_handle(curl_multi, tmp);
  handles.push_back(tmp);
}

//------------------------------------------------------------------------------
void nrf_client::perform_curl_multi(uint64_t ms) {
  _unused(ms);
  int still_running = 0, numfds = 0;

  CURLMcode code = curl_multi_perform(curl_multi, &still_running);

  do {
    code = curl_multi_wait(curl_multi, NULL, 0, 200000, &numfds);
    if (code != CURLM_OK) {
      Logger::nrf_app().debug("curl_multi_wait() returned %d!", code);
    }
    curl_multi_perform(curl_multi, &still_running);
  } while (still_running);

  curl_release_handles();
}

//------------------------------------------------------------------------------
void nrf_client::wait_curl_end() {
  // block until activity is detected on at least one of the handles or
  // MAX_WAIT_MSECS has passed.
  int still_running = 0, numfds = 0;
  do {
    CURLMcode code = curl_multi_perform(curl_multi, &still_running);
    if (code == CURLM_OK) {
      code = curl_multi_wait(curl_multi, NULL, 0, MAX_WAIT_MSECS, &numfds);
      if (code != CURLM_OK) break;
    } else {
      break;
    }
  } while (still_running);

  curl_release_handles();
}

//------------------------------------------------------------------------------
void nrf_client::curl_release_handles() {
  CURLMsg* curl_msg = nullptr;
  CURL* curl        = nullptr;
  CURLcode code     = {};
  int http_code     = 0;
  int msgs_left     = 0;

  while ((curl_msg = curl_multi_info_read(curl_multi, &msgs_left))) {
    if (curl_msg && curl_msg->msg == CURLMSG_DONE) {
      curl = curl_msg->easy_handle;
      code = curl_msg->data.result;
      // int res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &curl_url);
      if (code != CURLE_OK) {
        Logger::nrf_app().debug("CURL error code  %d!", curl_msg->data.result);
        continue;
      }
      // Get HTTP code
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
      Logger::nrf_app().debug("Got response with HTTP code  %d!", http_code);

      // TODO: remove handle from the multi session and end this handle now, or
      // later
      curl_multi_remove_handle(curl_multi, curl);
      curl_easy_cleanup(curl);

      std::vector<CURL*>::iterator it;
      it = find(handles.begin(), handles.end(), curl);
      if (it != handles.end()) {
        handles.erase(it);
      }

    } else if (curl_msg) {
      curl = curl_msg->easy_handle;
      Logger::nrf_app().debug("Error after curl_multi_info_read()");
      curl_multi_remove_handle(curl_multi, curl);
      curl_easy_cleanup(curl);

      std::vector<CURL*>::iterator it;
      it = find(handles.begin(), handles.end(), curl);
      if (it != handles.end()) {
        handles.erase(it);
      }
    } else {
      Logger::nrf_app().debug("curl_msg null");
    }
  }
}

//------------------------------------------------------------------------------
void nrf_client::notify_subscribed_event(
    const std::shared_ptr<nrf_profile>& profile, const uint8_t& event_type,
    const std::vector<std::string>& uris) {
  Logger::nrf_app().debug(
      "Send notification for the subscribed event to the subscriptions");

  std::map<std::string, std::string> responses = {};
  // Fill the json part
  nlohmann::json json_data = {};
  json_data["event"]       = notification_event_type_e2str[event_type];

  std::vector<struct in_addr> instance_addrs = {};
  profile.get()->get_nf_ipv4_addresses(instance_addrs);
  // TODO: use the first IPv4 addr for now
  std::string instance_uri =
      std::string(inet_ntoa(*((struct in_addr*) &(instance_addrs[0]))));
  Logger::nrf_app().debug("NF instance URI: %s", instance_uri.c_str());
  json_data["nfInstanceUri"] = instance_uri;

  // NF profile
  if ((event_type == NOTIFICATION_TYPE_NF_REGISTERED) or
      (event_type == NOTIFICATION_TYPE_NF_PROFILE_CHANGED)) {
    nlohmann::json json_profile = {};
    switch (profile.get()->get_nf_type()) {
      case NF_TYPE_AMF: {
        std::static_pointer_cast<amf_profile>(profile).get()->to_json(
            json_profile);
      } break;
      case NF_TYPE_SMF: {
        std::static_pointer_cast<smf_profile>(profile).get()->to_json(
            json_profile);
      } break;
      case NF_TYPE_UPF: {
        std::static_pointer_cast<upf_profile>(profile).get()->to_json(
            json_profile);
      } break;
      default: {
        profile.get()->to_json(json_profile);
      }
    }
    json_data["nfProfile"] = json_profile;
  }

  std::string body = json_data.dump();

  for (auto uri : uris) {
    responses[uri] = "";
    std::unique_ptr<std::string> httpData(new std::string());
    send_curl_multi(uri, body, responses[uri]);
  }

  perform_curl_multi(
      0);  // TODO: current time as parameter if curl is performed per event
}

//------------------------------------------------------------------------------
void nrf_client::subscribe_task_curl() {
  struct itimerspec its;
  its.it_value.tv_sec  = 100;               // TODO: to be updated 100 seconds
  its.it_value.tv_nsec = 10 * 1000 * 1000;  // 10ms
  const uint64_t interval =
      its.it_value.tv_sec * 1000 +
      its.it_value.tv_nsec / 1000000;  // convert sec, nsec to msec

  task_connection = m_event_sub.subscribe_task_tick(
      boost::bind(&nrf_client::perform_curl_multi, this, _1), interval, 0);
}
