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

/*! \file nrf_subscription.hpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#ifndef FILE_NRF_SUBSCRIPTION_HPP_SEEN
#define FILE_NRF_SUBSCRIPTION_HPP_SEEN

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
#include "3gpp_29.510.h"
#include "logger.hpp"
#include "nrf_event.hpp"
#include "nrf_profile.hpp"

namespace oai {
namespace nrf {
namespace app {
using namespace std;

class nrf_subscription {
 public:
  nrf_subscription(nrf_event& ev) : m_event_sub(ev) {
    validity_time = boost::posix_time::from_iso_string("20991231T235959Z");
  };

  nrf_subscription(nrf_subscription const&) = delete;

  virtual ~nrf_subscription() {
    Logger::nrf_app().debug("Delete NRF Subscription instance...");
    if (ev_connection.connected()) ev_connection.disconnect();
  }

  void operator=(nrf_subscription const&) = delete;

  /*
   * Set the subscription id
   * @param [const std::string &]: sub_id: Subscription id
   * @return void
   */
  void set_subscription_id(const std::string& sub_id);

  /*
   * Get the subscription id
   * @param [std::string &]: sub_id: Subscription id
   * @return void
   */
  void get_subscription_id(std::string& sub_id) const;

  /*
   * Get the subscription id
   * @param [void]
   * @return subscription id
   */
  std::string get_subscription_id() const;

  /*
   * Set the notification URI (URI where the notification will be sent to)
   * @param [const std::string &]: notification_uri: Notification URI
   * @return void
   */
  void set_notification_uri(const std::string& notification_uri);

  /*
   * Get the notification URI (URI where the notification will be sent to)
   * @param [std::string &]: notification_uri: Notification URI
   * @return void
   */
  void get_notification_uri(std::string& notification_uri) const;

  /*
   * Set the subscription condition
   * @param [const subscription_condition_t &]: c: Subscription condition
   * @return void
   */
  void set_sub_condition(const subscription_condition_t& c);

  /*
   * Get the subscription condition
   * @param [const subscription_condition_t &]: c: Subscription condition
   * @return void
   */
  void get_sub_condition(subscription_condition_t& c) const;
  // subscription_condition_t get_sub_condition() const;

  /*
   * Set the notification events
   * @param [const std::vector<uint8_t> &]: ev_types: Array of the notification
   * event types
   * @return void
   */
  void set_notif_events(const std::vector<uint8_t>& ev_types);

  /*
   * Add a notification type to the list of notification events
   * @param [const uint8_t &]: ev_type: A notification type
   * @return void
   */
  void add_notif_event(const uint8_t& ev_type);

  /*
   * Get the notification events
   * @param [std::vector<uint8_t> &]: ev_types: Array of the notification event
   * types
   * @return void
   */
  void get_notif_events(std::vector<uint8_t>& ev_types) const;

  /*
   * Get the notification events
   * @param [void]
   * @return Array of the notification event types
   */
  std::vector<uint8_t> get_notif_events() const;

  /*
   * Set the validity time
   * @param [const boost::posix_time::ptime &]t:  validity time
   * @return
   */
  void set_validity_time(const boost::posix_time::ptime& t);

  /*
   * Get the validity time
   * @param [boost::posix_time::ptime &]t:  validity time
   * @return
   */
  void get_validity_time(boost::posix_time::ptime& t) const;

  /*
   * Get the validity time
   * @param [void]
   * @return [boost::posix_time::ptime &] validity time
   */
  boost::posix_time::ptime get_validity_time() const;

  /*
   * Subscribe to be notified when a new NF registered to the NRF
   * @param void
   * @return void
   */
  void subscribe_nf_status_registered();

  /*
   * Handle the NF Status Registered event
   * @param [const std::shared_ptr<nrf_profile> &]: profile: pointer to the new
   * registered NF profile
   * @return void
   */
  void handle_nf_status_registered(const std::shared_ptr<nrf_profile>& profile);

  /*
   * Display all the members of a subscription
   * @param [void]
   * @return void
   */
  void display();

 private:
  std::string nf_status_notification_uri;
  std::string subscription_id;
  subscription_condition_t sub_condition;
  std::vector<uint8_t> notif_events;
  nrf_event& m_event_sub;
  bs2::connection ev_connection;
  boost::posix_time::ptime validity_time;
};
}  // namespace app
}  // namespace nrf
}  // namespace oai

#endif /* FILE_NRF_SUBSCRIPTION_HPP_SEEN */
