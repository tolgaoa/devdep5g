/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

/*! \file amf_client.cpp
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: tien-thinh.nguyen@eurecom.fr
 */

#include <curl/curl.h>
#include <unistd.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string>
#include <thread>

#define NF_CURL_TIMEOUT_MS 3000L

static std::size_t callback(const char *in, std::size_t size, std::size_t num,
                            std::string *out) {
  const std::size_t totalBytes(size * num);
  out->append(in, totalBytes);
  return totalBytes;
}

//------------------------------------------------------------------------------
bool send_curl(std::string url) {
  long httpCode = {0};
  nlohmann::json msg;

  // Fill the json part
  nlohmann::json json_data = {};
  msg["nfStatusNotificationUri"] =
      "http://192.168.1.23/nnrf-disc/v1/"
      "nf-instances?target-nf-type=SMF&requester-nf-type=AMF";
  msg["subscrCond"]["NfTypeCond"]["nfType"] = "SMF";
  msg["reqNotifEvents"] = nlohmann::json::array();
  msg["reqNotifEvents"].push_back("NF_REGISTERED");
  msg["reqNotifEvents"].push_back("NF_DEREGISTERED");

  std::string body = msg.dump();

  curl_global_init(CURL_GLOBAL_ALL);
  CURL *curl = curl = curl_easy_init();

  if (curl) {
    CURLcode res = {};
    struct curl_slist *headers = nullptr;
    // headers = curl_slist_append(headers, "charsets: utf-8");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NF_CURL_TIMEOUT_MS);

    std::unique_ptr<std::string> httpData(new std::string());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}

//------------------------------------------------------------------------------
void send_curl_multi(std::string url, uint32_t n) {
  int still_running = 0, numfds = 0, res = 0, msgs_left = 0;
  CURLMsg *curl_msg = nullptr;
  CURL *curl = nullptr;
  CURLcode return_code = {};
  int http_status_code = 0;
  int index = 0;
  CURLM *m_curl_multi = nullptr;
  char *curl_url = nullptr;
  std::vector<CURL *> handles;
  std::unique_ptr<std::string> httpData(new std::string());
  CURLM *curl_multi;

  curl_global_init(CURL_GLOBAL_ALL);
  curl_multi = curl_multi_init();

  // init header
  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "charsets: utf-8");

  nlohmann::json json_data = {};
  json_data["nfStatusNotificationUri"] =
      "http://192.168.1.23/nnrf-disc/v1/"
      "nf-instances?target-nf-type=SMF&requester-nf-type=AMF";
  json_data["subscrCond"]["NfTypeCond"]["nfType"] = "SMF";
  json_data["reqNotifEvents"] = nlohmann::json::array();
  json_data["reqNotifEvents"].push_back("NF_REGISTERED");
  json_data["reqNotifEvents"].push_back("NF_DEREGISTERED");

  std::string body = json_data.dump();

  // create and add an easy handle to a  multi curl request
  for (int i = 0; i < n; i++) {
    curl = curl_easy_init();
    if (curl) {
      // Logger::nrf_app().debug("Send notification to NF with URI: %s",
      //                         uri.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1);
      curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, NF_CURL_TIMEOUT_MS);
      // Hook up data handling function.
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &callback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, httpData.get());
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.length());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    }
    curl_multi_add_handle(curl_multi, curl);
    index++;
    handles.push_back(curl);
  }

  std::cout << "Number of handles: " << index << std::endl;
  curl_multi_perform(curl_multi, &still_running);
  // block until activity is detected on at least one of the handles or
  // MAX_WAIT_MSECS has passed.
  int i = 0;
  do {
    res = curl_multi_wait(curl_multi, NULL, 0, 1000, &numfds);
    if (res != CURLM_OK) {
      std::cout << "curl_multi_wait() returned" << res << std::endl;
    }
    curl_multi_perform(curl_multi, &still_running);
    i++;
  } while (still_running);

  i = 0;  // process multiple curl
  // read the messages
  while ((curl_msg = curl_multi_info_read(curl_multi, &msgs_left))) {
    if (curl_msg->msg == CURLMSG_DONE) {
      i++;
      curl = curl_msg->easy_handle;
      return_code = curl_msg->data.result;
      res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &curl_url);

      if (return_code != CURLE_OK) {
        continue;
      }
      // Get HTTP status code
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status_code);

      // TODO: remove handle from the multi session and end this handle now, or
      // later
      curl_multi_remove_handle(curl_multi, curl);
      curl_easy_cleanup(curl);

      std::vector<CURL *>::iterator it;
      it = find(handles.begin(), handles.end(), curl);
      if (it != handles.end()) {
        handles.erase(it);
        index--;
      }
    }
  }

  std::cout << "Processed messages: " << i << std::endl;

  // Remove handle, free memory
  for (int i = 0; i < index; i++) {
    curl_multi_remove_handle(curl_multi, handles[i]);
    curl_easy_cleanup(handles[i]);
  }
  curl_multi_cleanup(curl_multi);
  curl_global_cleanup();
  curl_slist_free_all(headers);
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
  bool cont = false;
  uint8_t context_id = 1;
  uint8_t pid = 1;

  std::string type = "Content-Type: application/json";
  std::string url = "http://192.168.1.23:8080/nnrf-nfm/v1/subscriptions";
  // send_curl(url);
  uint32_t number_subscriptions = 100;
  std::cout << "Number of subscriptions: " << number_subscriptions << std::endl;
  std::cout << "Sending..." << std::endl;
  send_curl_multi(url, number_subscriptions);
  std::cout << "Finished" << std::endl;
  return 0;
}
