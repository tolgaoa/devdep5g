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

#include "NFStatusNotifyApi.h"
#include "Helpers.h"
#include "smf_config.hpp"

extern smf::smf_config smf_cfg;

namespace oai {
namespace smf_server {
namespace api {

using namespace oai::smf_server::helpers;
using namespace oai::smf_server::model;

NFStatusNotifyApi::NFStatusNotifyApi(
    std::shared_ptr<Pistache::Rest::Router> rtr) {
  router = rtr;
}

void NFStatusNotifyApi::init() {
  setupRoutes();
}

void NFStatusNotifyApi::setupRoutes() {
  using namespace Pistache::Rest;

  Routes::Post(
      *router, base + smf_cfg.sbi_api_version + "/subscriptions",
      Routes::bind(&NFStatusNotifyApi::notify_nf_status_handler, this));

  // Default handler, called when a route is not found
  router->addCustomHandler(
      Routes::bind(&NFStatusNotifyApi::notify_nf_status_default_handler, this));
}

void NFStatusNotifyApi::notify_nf_status_handler(
    const Pistache::Rest::Request& request,
    Pistache::Http::ResponseWriter response) {
  // Getting the body param
  NotificationData notificationData;

  try {
    nlohmann::json::parse(request.body()).get_to(notificationData);
    this->receive_nf_status_notification(notificationData, response);
  } catch (nlohmann::detail::exception& e) {
    // send a 400 error
    response.send(Pistache::Http::Code::Bad_Request, e.what());
    return;
  } catch (Pistache::Http::HttpError& e) {
    response.send(static_cast<Pistache::Http::Code>(e.code()), e.what());
    return;
  } catch (std::exception& e) {
    // send a 500 error
    response.send(Pistache::Http::Code::Internal_Server_Error, e.what());
    return;
  }
}

void NFStatusNotifyApi::notify_nf_status_default_handler(
    const Pistache::Rest::Request&, Pistache::Http::ResponseWriter response) {
  response.send(
      Pistache::Http::Code::Not_Found, "The requested method does not exist");
}

}  // namespace api
}  // namespace smf_server
}  // namespace oai
