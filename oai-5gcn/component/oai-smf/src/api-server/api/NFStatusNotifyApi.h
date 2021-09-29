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

/*
 * NFStatusNotifyApi.h
 *
 *
 */

#ifndef NFStatusNotifyApi_H_
#define NFStatusNotifyApi_H_

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/http_headers.h>
#include <pistache/optional.h>

#include "NotificationData.h"
#include "ProblemDetails.h"

namespace oai {
namespace smf_server {
namespace api {

using namespace oai::smf_server::model;

class NFStatusNotifyApi {
 public:
  NFStatusNotifyApi(std::shared_ptr<Pistache::Rest::Router>);
  virtual ~NFStatusNotifyApi() {}
  void init();

  const std::string base = "/nsmf-nfstatus-notify/";

 private:
  void setupRoutes();

  void notify_nf_status_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);
  void notify_nf_status_default_handler(
      const Pistache::Rest::Request& request,
      Pistache::Http::ResponseWriter response);

  std::shared_ptr<Pistache::Rest::Router> router;

  /// <summary>
  ///
  /// </summary>
  /// <remarks>
  ///
  /// </remarks>
  /// <param name="NotificationData"></param>
  virtual void receive_nf_status_notification(
      const NotificationData& notificationData,
      Pistache::Http::ResponseWriter& response) = 0;
};

}  // namespace api
}  // namespace smf_server
}  // namespace oai

#endif /* NFStatusNotifyApi_H_ */
