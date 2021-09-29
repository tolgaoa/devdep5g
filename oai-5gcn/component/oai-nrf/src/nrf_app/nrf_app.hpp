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

/*! \file nrf_app.hpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#ifndef FILE_NRF_APP_HPP_SEEN
#define FILE_NRF_APP_HPP_SEEN

#include <string>
#include "AccessTokenRsp.h"
#include "NFProfile.h"
#include "PatchItem.h"
#include "ProblemDetails.h"
#include "SubscriptionData.h"
#include "nrf_event.hpp"
#include "nrf_profile.hpp"
#include "nrf_search_result.hpp"
#include "nrf_subscription.hpp"
#include "uint_generator.hpp"

namespace oai {
namespace nrf {
namespace app {

using namespace oai::nrf::model;
class nrf_config;
class nrf_app {
 public:
  explicit nrf_app(const std::string& config_file, nrf_event& ev);
  nrf_app(nrf_app const&) = delete;
  void operator=(nrf_app const&) = delete;

  virtual ~nrf_app();

  /*
   * Generate a random UUID for NRF instance
   * @param [void]
   * @return void
   */
  void generate_uuid();

  /*
   * Handle a Register NF Instance request
   * @param [const std::string &] nf_instance_id: Instance ID
   * @param [NFProfile &] nf_profile: NF profile
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_register_nf_instance(
      const std::string& nf_instance_id, const NFProfile& nf_profile,
      int& http_code, const uint8_t http_version,
      ProblemDetails& problem_details);

  /*
   * Handle a Get NF Instance Information
   * @param [const std::string &] nf_type: NF's type
   * @param [std::vector<std::string> &] uris: List of registered NFs in the NRF
   *                                           that satisfy the retrieval filter
   * criteria
   * @param [const uint32_t &] limit_item: Maximum number of items returned
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_get_nf_instances(
      const std::string& nf_type, std::vector<std::string>& uris,
      const uint32_t& limit_item, int& http_code, const uint8_t http_version,
      oai::nrf::model::ProblemDetails& problem_details);

  /*
   * Handle a Update NF Instance request
   * @param [const std::string &] nf_instance_id: Instance ID
   * @param [const std::vector<PatchItem> &] patchItem: List of modifications
   * need to be applied
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_update_nf_instance(
      const std::string& nf_instance_id,
      const std::vector<PatchItem>& patchItem, int& http_code,
      const uint8_t http_version, ProblemDetails& problem_details);

  /*
   * Handle a Get NF Instance request
   * @param [const std::string &] nf_instance_id: Instance ID
   * @param [nrf_profile &] profile: NF profile
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_get_nf_instance(
      const std::string& nf_instance_id, std::shared_ptr<nrf_profile>& profile,
      int& http_code, const uint8_t http_version,
      ProblemDetails& problem_details);

  /*
   * Handle De-register a given NF Instance
   * @param [const std::string &] nf_instance_id: Instance ID
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_deregister_nf_instance(
      const std::string& nf_instance_id, int& http_code,
      const uint8_t http_version, ProblemDetails& problem_details);

  /*
   * Handle a Register NF Instance request
   * @param [SubscriptionData &] subscription_data: Subscription data
   * @param [std::string &] sub_id: Subscription ID
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_create_subscription(
      const SubscriptionData& subscription_data, std::string& sub_id,
      int& http_code, const uint8_t http_version,
      ProblemDetails& problem_details);

  /*
   * Handle a NFStatusUnSubscribe request (removes an existing subscription)
   * @param [const std::string &] sub_id: Subscription ID
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_remove_subscription(
      const std::string& sub_id, int& http_code, const uint8_t http_version,
      ProblemDetails& problem_details);

  /*
   * Handle a Update of Subscription to NF Instances
   * @param [const std::string &] sub_id: Subscription ID
   * @param [const std::vector<PatchItem> &] patchItem: List of modifications
   * need to be applied
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_update_subscription(
      const std::string& sub_id, const std::vector<PatchItem>& patchItem,
      int& http_code, const uint8_t http_version,
      ProblemDetails& problem_details);

  /*
   * Handle NFDiscover to discover the set of NF Instances
   * @param [const std::string &] target_nf_type: target NF type
   * @param [const std::string &] requester_nf_type: Requester NF type
   * @param [const std::string &] requester_nf_instance_id: Requester NF
   * instance id
   * @param [uint32_t &] limit_nfs: Maximum number of NFProfiles to be returned
   * in the response:
   * @param [std::string &] search_id: Store search result ID
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_search_nf_instances(
      const std::string& target_nf_type, const std::string& requester_nf_type,
      const std::string& requester_nf_instance_id, uint32_t& limit_nfs,
      std::string& search_id, int& http_code, const uint8_t http_version,
      ProblemDetails& problem_details);

  /*
   * Handle a Register NF Instance request
   * @param [const std::string &] request_body: includes access token request
   * @param [AccessTokenRsp &] access_token_rsp: Access token response
   * @param [int &] http_code: HTTP code used to return to the consumer
   * @param [const uint8_t] http_version: HTTP version
   * @param [ProblemDetails &] problem_details: Store details of the error
   * @return void
   */
  void handle_access_token_request(
      const std::string& request_body, AccessTokenRsp& access_token_rsp,
      int& http_code, const uint8_t http_version,
      ProblemDetails& problem_details);

  /*
   * Insert a nrf profile
   * @param [const std::string &] profile_id: Profile ID
   * @param [std::shared_ptr<nrf_profile> &] p: profile to be added
   * @return true if successful, otherwise, return false
   */
  bool add_nf_profile(
      const std::string& profile_id, const std::shared_ptr<nrf_profile>& p);

  /*
   * Update a nrf profile
   * @param [const std::string &] profile_id: Profile ID
   * @param [std::shared_ptr<nrf_profile> &] p: profile to be added
   * @return true if successful, otherwise, return false
   */
  bool update_nf_profile(
      const std::string& profile_id, const std::shared_ptr<nrf_profile>& p);

  /*
   * Find a nf profile with its ID
   * @param [const std::string &] profile_id: Profile ID
   * @return shared pointer to the profile if found
   */
  std::shared_ptr<nrf_profile> find_nf_profile(
      const std::string& profile_id) const;

  /*
   * Find a nf profile with its ID
   * @param [const std::string &] profile_id: Profile ID
   * @return shared pointer to the profile if found
   */
  bool find_nf_profile(
      const std::string& profile_id, std::shared_ptr<nrf_profile>& p) const;

  /*
   * Find a list of nf profiles with a type
   * @param [const nf_type_t &] nf_type: Type of NF profile
   * @param [std::vector<std::shared_ptr<nrf_profile>> &] profiles: Store list
   * of corresponding profiles
   * @return shared pointer to the profile if found
   */
  void find_nf_profiles(
      const nf_type_t& nf_type,
      std::vector<std::shared_ptr<nrf_profile>>& profiles) const;

  /*
   * Check if a profile with an ID exist
   * @param [const std::string &] profile_id: Profile ID
   * @return true if profile exist, otherwise, return false
   */
  bool is_profile_exist(const std::string& profile_id) const;

  /*
   * Remove a nf profile from the list
   * @param [std::shared_ptr<nrf_profile> &] snp: profile to be removed
   * @return true if successful, otherwise, return false
   */
  bool remove_nf_profile(const std::shared_ptr<nrf_profile>& snp);

  /*
   * Remove a nf profile from the list
   * @param [std::string &] profile_id: ID of the profile to be removed
   * @return true if successful, otherwise, return false
   */
  bool remove_nf_profile(const std::string& profile_id);

  /*
   * Add a subscription
   * @param [const std::string &] sub_id: Subscription ID
   * @param [std::shared_ptr<nrf_subscription> &] s: shared_pointer to the
   * subscription to be added
   * @return true if successful, otherwise, return false
   */
  bool add_subscription(
      const std::string& sub_id, const std::shared_ptr<nrf_subscription>& s);

  /*
   * Remove a subscription from the list
   * @param [std::string &] sub_id: ID of the subscription to be removed
   * @return true if successful, otherwise, return false
   */
  bool remove_subscription(const std::string& sub_id);

  /*
   * Find a subscription with its ID
   * @param [const std::string &] sub_id: Subscription ID
   * @return shared pointer to the subscription if found, otherwise nullptr
   */
  std::shared_ptr<nrf_subscription> find_subscription(
      const std::string& sub_id) const;

  /*
   * Subscribe to the task tick event
   * @param [uint64_t &] ms: Current time in ms
   * @return void
   */
  void subscribe_task_tick(uint64_t ms);

  /*
   * Handle when hearbeart timer expires
   * @param [uint64_t] ms: current time in milliseconds
   * @return void
   */
  void handle_heartbeart_timeout(uint64_t ms);

  /*
   * Verify whether a subscription is authorized
   * @param [std::shared_ptr<nrf_subscription> &] s: shared_pointer to the
   * subscription
   * @return true if this sub is authorized, otherwise, return false
   */
  bool authorize_subscription(const std::shared_ptr<nrf_subscription>& s) const;

  /*
   * Generate an unique ID for the new subscription
   * @param [const std::string &] sub_id: the generated ID
   * @return void
   */
  void generate_ev_subscription_id(std::string& sub_id);

  /*
   * Generate an unique ID for the new subscription
   * @param void
   * @return the generated ID
   */
  evsub_id_t generate_ev_subscription_id();

  /*
   * Subscribe to the nf status events
   * @param void
   * @return void
   */
  void subscribe_nf_status();

  /*
   * Subscribe to the event when a new NF registers to the NRF
   * @param void
   * @return void
   */
  void subscribe_nf_status_registered();

  /*
   * Handle NF status registered event
   * @param [const std::string &] profile_id: Profile ID of the registered NF
   * @return void
   */
  void handle_nf_status_registered(const std::string& profile_id);

  /*
   * Subscribe to the event when a NF de-registers from the NRF
   * @param void
   * @return void
   */
  void subscribe_nf_status_deregistered();

  /*
   * Handle NF status deregistered event
   * @param [const std::shared_ptr<nrf_profile> &] profile: pointer to the
   * deregistered NF
   * @return void
   */
  void handle_nf_status_deregistered(const std::shared_ptr<nrf_profile>& p);

  /*
   * Subscribe to the event when a registered NF changes its profile
   * @param void
   * @return void
   */
  void subscribe_nf_status_profile_changed();

  /*
   * Handle NF status profile changed event
   * @param [const std::string &] profile_id: Profile ID of the NF
   * @return void
   */
  void handle_nf_status_profile_changed(const std::string& profile_id);

  /*
   * Get the list of subscriptions to the profile with notification type
   * @param [const std::string &] profile_id: Profile ID of the NF
   * @param [const uint8_t &] notification_type: requested notification type
   * @param [std::vector<std::string> &] uris: vector stores list of uri of
   * subscribed NFs
   * @return void
   */
  void get_subscription_list(
      const std::string& profile_id, const uint8_t& notification_type,
      std::vector<std::string>& uris) const;

  /*
   * Verify whether the requester is allowed to discover the NF services
   * @param [const std::string &] requester_instance_id: Requester instance ID
   * @param [const std::string &] requester_nf_type: Requester nf type
   * @return void
   */
  bool is_service_discover_allowed(
      const std::string& requester_instance_id,
      const std::string& requester_nf_type);

  /*
   * Generate an unique ID for the search result
   * @param [const std::string &] search_id: the generated search ID
   * @return void
   */
  void generate_search_id(std::string& search_id);

  /*
   * Add a search result to the DB
   * @param [const std::string &] id: Search ID
   * @param [const std::shared_ptr<nrf_search_result> &] s: Pointer to the
   * search result
   * @return void
   */
  bool add_search_result(
      const std::string& id, const std::shared_ptr<nrf_search_result>& s);

  /*
   * Find a search result with its ID
   * @param [const std::string &] search_id: Search ID
   * @param [std::shared_ptr<nrf_search_result> &] s: Pointer to the search
   * result
   * @return true if found, otherwise false
   */
  bool find_search_result(
      const std::string& search_id,
      std::shared_ptr<nrf_search_result>& p) const;

 private:
  std::string nrf_instance_id;  // NRF instance id
  std::map<std::string, std::shared_ptr<nrf_profile>> instance_id2nrf_profile;
  mutable std::shared_mutex m_instance_id2nrf_profile;

  std::map<std::string, std::shared_ptr<nrf_subscription>>
      subscrition_id2nrf_subscription;
  mutable std::shared_mutex m_subscription_id2nrf_subscription;
  nrf_event& m_event_sub;
  util::uint_generator<uint32_t> evsub_id_generator;
  std::vector<bs2::connection> connections;

  util::uint_generator<uint32_t> search_id_generator;
  std::map<std::string, std::shared_ptr<nrf_search_result>>
      search_id2search_result;
  mutable std::shared_mutex m_search_id2search_result;
};
}  // namespace app
}  // namespace nrf
}  // namespace oai
#include "nrf_config.hpp"

#endif /* FILE_NRF_APP_HPP_SEEN */
