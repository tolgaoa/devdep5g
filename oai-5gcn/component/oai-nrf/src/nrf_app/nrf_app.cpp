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

/*! \file nrf_app.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#include "nrf_app.hpp"

#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/time_formatters.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

#include "3gpp_29.500.h"
#include "3gpp_29.510.h"
#include "api_conversions.hpp"
#include "common_defs.h"
#include "logger.hpp"
#include "nrf_client.hpp"
#include "nrf_config.hpp"
#include "nrf_jwt.hpp"
#include "nrf_search_result.hpp"

using namespace oai::nrf::app;
using namespace oai::nrf::model;
using namespace std::chrono;

extern nrf_app* nrf_app_inst;
extern nrf_config nrf_cfg;
nrf_client* nrf_client_inst = nullptr;
nrf_jwt* nrf_jwt_inst       = nullptr;

//------------------------------------------------------------------------------
nrf_app::nrf_app(const std::string& config_file, nrf_event& ev)
    : m_event_sub(ev),
      m_instance_id2nrf_profile(),
      m_subscription_id2nrf_subscription(),
      m_search_id2search_result() {
  Logger::nrf_app().startup("Starting...");

  try {
    nrf_client_inst = new nrf_client(ev);
    nrf_jwt_inst    = new nrf_jwt();
  } catch (std::exception& e) {
    Logger::nrf_app().error("Cannot create NRF_APP: %s", e.what());
    throw;
  }

  // subscribe to NF status
  subscribe_nf_status();

  Logger::nrf_app().startup("Started");
}

nrf_app::~nrf_app() {
  Logger::nrf_app().debug("Delete NRF_APP instance...");
  for (auto i : connections) {
    if (i.connected()) i.disconnect();
  }
  if (nrf_client_inst) delete nrf_client_inst;
}

//------------------------------------------------------------------------------
void nrf_app::generate_uuid() {
  nrf_instance_id = to_string(boost::uuids::random_generator()());
}

//------------------------------------------------------------------------------
void nrf_app::handle_register_nf_instance(
    const std::string& nf_instance_id,
    const oai::nrf::model::NFProfile& nf_profile, int& http_code,
    const uint8_t http_version,
    oai::nrf::model::ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle Register NF Instance/Update NF Instance (HTTP version %d)",
      http_version);

  // Check if nfInstanceID is a valid UUID (version 4)
  if (!api_conv::validate_uuid(nf_instance_id)) {
    http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
    Logger::nrf_app().debug(
        "Bad UUID format for NF Instance ID (%s)", nf_instance_id.c_str());
    problem_details.setCause(
        protocol_application_error_e2str[MANDATORY_QUERY_PARAM_INCORRECT]);
    return;
  }

  nf_type_t type = api_conv::string_to_nf_type(nf_profile.getNfType());
  // Create a new NF profile or Update an existing NF profile
  Logger::nrf_app().debug(
      "NF Profile with ID %s, NF type %s", nf_instance_id.c_str(),
      nf_profile.getNfType().c_str());

  std::shared_ptr<nrf_profile> sn = {};
  switch (type) {
    case NF_TYPE_AMF: {
      sn = std::make_shared<amf_profile>(m_event_sub);
    } break;

    case NF_TYPE_SMF: {
      sn = std::make_shared<smf_profile>(m_event_sub);
    } break;

    case NF_TYPE_UPF: {
      sn = std::make_shared<upf_profile>(m_event_sub);
    } break;

    default: {
      sn = std::make_shared<nrf_profile>(m_event_sub);
    }
  }

  // convert to nrf_profile
  if (api_conv::profile_api_to_nrf_profile(nf_profile, sn)) {
    // set default value for hearbeattimer
    sn.get()->set_nf_heartBeat_timer(HEART_BEAT_TIMER);
    if (is_profile_exist(nf_instance_id))
      http_code = HTTP_STATUS_CODE_200_OK;
    else
      http_code = HTTP_STATUS_CODE_201_CREATED;
    // add to the DB
    add_nf_profile(nf_instance_id, sn);
    Logger::nrf_app().debug("Added/Updated NF Profile to the DB");

    // Heartbeart management for this NF profile
    // get current time
    uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      std::chrono::system_clock::now().time_since_epoch())
                      .count();
    sn.get()->subscribe_heartbeat_timeout_nfregistration(ms);

    // Notify NF status change event
    m_event_sub.nf_status_registered(nf_instance_id);  // from nrf_app

    // display the info
    Logger::nrf_app().debug(
        "Added/Updated NF Profile with the NF instance info");
    sn.get()->display();
  } else {
    // error
    Logger::nrf_app().warn(
        "Cannot convert a NF profile generated from OpenAPI to an AMF profile "
        "(profile ID %s)",
        nf_instance_id.c_str());
    http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
    problem_details.setCause(
        protocol_application_error_e2str[MANDATORY_IE_INCORRECT]);
  }
}

//------------------------------------------------------------------------------
void nrf_app::handle_update_nf_instance(
    const std::string& nf_instance_id, const std::vector<PatchItem>& patchItem,
    int& http_code, const uint8_t http_version,
    oai::nrf::model::ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle Update NF Instance request (HTTP version %d)", http_version);

  // Find the profile corresponding to the instance ID
  std::shared_ptr<nrf_profile> sn = {};
  sn                              = find_nf_profile(nf_instance_id);
  bool op_success                 = true;
  bool is_heartbeart_procedure    = false;

  if (sn.get() != nullptr) {
    for (auto p : patchItem) {
      patch_op_type_t op = api_conv::string_to_patch_operation(p.getOp());
      // Verify Path
      if ((p.getPath().substr(0, 1).compare("/") != 0) or
          (p.getPath().length() < 2)) {
        Logger::nrf_app().warn(
            "Bad value for operation path: %s ", p.getPath().c_str());
        http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
        problem_details.setCause(
            protocol_application_error_e2str[MANDATORY_IE_INCORRECT]);
        return;
      }

      std::string path = p.getPath().substr(1);

      switch (op) {
        case PATCH_OP_REPLACE: {
          if (path.compare("nfStatus") == 0) {
            is_heartbeart_procedure = true;
            Logger::nrf_app().debug("NF Heart-Beat procedure!");
          }
          if (sn.get()->replace_profile_info(path, p.getValue())) {
            update_nf_profile(nf_instance_id, sn);
            http_code = HTTP_STATUS_CODE_200_OK;
          } else {
            op_success = false;
          }
        } break;

        case PATCH_OP_ADD: {
          if (sn.get()->add_profile_info(path, p.getValue())) {
            update_nf_profile(nf_instance_id, sn);
            http_code = HTTP_STATUS_CODE_200_OK;
          } else {
            op_success = false;
          }
        } break;

        case PATCH_OP_REMOVE: {
          if (sn.get()->remove_profile_info(path)) {
            update_nf_profile(nf_instance_id, sn);
            http_code = HTTP_STATUS_CODE_200_OK;
          } else {
            op_success = false;
          }
        } break;

        default: {
          Logger::nrf_app().warn("Requested operation is not valid!");
          op_success = false;
        }
      }

      if (!op_success) {
        http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
        problem_details.setCause(
            protocol_application_error_e2str[MANDATORY_IE_INCORRECT]);
      } else {
        if (!is_heartbeart_procedure)
          // update successful,
          // Notify NF status change event
          m_event_sub.nf_status_profile_changed(
              nf_instance_id);  // from nrf_app
      }
    }

    // for NF Heartbeat procedure
    if (is_heartbeart_procedure and (http_code == HTTP_STATUS_CODE_200_OK)) {
      http_code   = HTTP_STATUS_CODE_204_NO_CONTENT;
      uint64_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch())
                        .count();

      Logger::nrf_app().debug("NF update for Heartbeat, current time %ld", ms);
      // If this happens before the first Heartbeattimer expires -> remove this
      // timer
      if (sn.get()->unsubscribe_heartbeat_timeout_nfregistration()) {
        // Sleep 10ms to avoid Boost connection related issue
        unsigned int microsecond = 50000;  // 50ms
        usleep(microsecond);
        ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::system_clock::now().time_since_epoch())
                 .count();
        Logger::nrf_app().debug(
            "Subscribe to HBT (NF update), current time %ld", ms);
        sn.get()->subscribe_heartbeat_timeout_nfupdate(ms);
      }

      // sn.get()->subscribe_heartbeat_timeout_nfupdate(ms);
      // update NF updated flag
      sn.get()->set_status_updated(true);
      // update NF status
      sn.get()->set_nf_status("REGISTERED");
      return;
    }

  } else {
    Logger::nrf_app().debug(
        "NF Profile with ID %s does not exit", nf_instance_id.c_str());
    http_code = HTTP_STATUS_CODE_404_NOT_FOUND;
    problem_details.setCause(
        protocol_application_error_e2str[RESOURCE_URI_STRUCTURE_NOT_FOUND]);
  }
}

//------------------------------------------------------------------------------
void nrf_app::handle_get_nf_instances(
    const std::string& nf_type, std::vector<std::string>& uris,
    const uint32_t& limit_item, int& http_code, const uint8_t http_version,
    oai::nrf::model::ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle Retrieve a collection of NF Instances (HTTP version %d)",
      http_version);
  // TODO: verify if the NF Service Consumer is allowed to retrieve the
  // registered NF instances

  std::vector<std::shared_ptr<nrf_profile>> profiles = {};
  nf_type_t type = api_conv::string_to_nf_type(nf_type);
  if (type == NF_TYPE_UNKNOWN) {
    Logger::nrf_app().debug("Unknown requested nf_type: %s", nf_type.c_str());
    http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
    problem_details.setCause(
        protocol_application_error_e2str[MANDATORY_IE_INCORRECT]);
    return;
  }

  http_code = HTTP_STATUS_CODE_200_OK;
  find_nf_profiles(type, profiles);

  if (profiles.size() == 0) {
    Logger::nrf_app().debug("No profile found (NF type: %s)", nf_type.c_str());
  }

  for (auto profile : profiles) {
    std::string instance_uri;
    std::vector<struct in_addr> profile_addresses = {};
    profile.get()->get_nf_ipv4_addresses(profile_addresses);
    // TODO: use the first IP addr
    if (profile_addresses.size() > 0) {
      instance_uri =
          std::string(inet_ntoa(*((struct in_addr*) &profile_addresses[0])));
      uris.push_back(instance_uri);
    }
    profile.get()->display();
  }
}

//------------------------------------------------------------------------------
void nrf_app::handle_get_nf_instance(
    const std::string& nf_instance_id, std::shared_ptr<nrf_profile>& profile,
    int& http_code, const uint8_t http_version,
    ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle Retrieve an NF Instance (HTTP version %d)", http_version);

  // TODO:  If the NF Service Consumer is not allowed to retrieve the NF profile
  // of this specific registered NF instance, the  NRF shall return "403
  // Forbidden" status code.

  profile = find_nf_profile(nf_instance_id);
  if (profile.get() == nullptr) {
    Logger::nrf_app().debug(
        "Profile with profile ID %s not found", nf_instance_id.c_str());
    http_code = HTTP_STATUS_CODE_404_NOT_FOUND;
    problem_details.setCause(
        protocol_application_error_e2str[RESOURCE_URI_STRUCTURE_NOT_FOUND]);
    return;
  } else {
    Logger::nrf_app().debug(
        "Profile with profile ID %s", nf_instance_id.c_str());
    profile.get()->display();
    http_code = HTTP_STATUS_CODE_200_OK;
    return;
  }
}

//------------------------------------------------------------------------------
void nrf_app::handle_deregister_nf_instance(
    const std::string& nf_instance_id, int& http_code,
    const uint8_t http_version, ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle Deregister an NF Instance (HTTP version %d)", http_version);

  std::shared_ptr<nrf_profile> profile = {};
  profile                              = find_nf_profile(nf_instance_id);

  if (profile.get() != nullptr) {
    // TODO: notify NF status changed event
    // Notify NF status deregistered event
    m_event_sub.nf_status_deregistered(profile);  // from nrf_app

    if (remove_nf_profile(nf_instance_id)) {
      Logger::nrf_app().debug(
          "Removed NF profile with profile ID %s", nf_instance_id.c_str());
      http_code = HTTP_STATUS_CODE_204_NO_CONTENT;
      return;
    } else {
      http_code = HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR;
      problem_details.setCause(
          protocol_application_error_e2str[SYSTEM_FAILURE]);
      return;
    }
  } else {
    Logger::nrf_app().debug(
        "Profile with profile ID %s not found", nf_instance_id.c_str());
    http_code = HTTP_STATUS_CODE_404_NOT_FOUND;
    problem_details.setCause(
        protocol_application_error_e2str[RESOURCE_URI_STRUCTURE_NOT_FOUND]);
    return;
  }
}

//------------------------------------------------------------------------------
void nrf_app::handle_create_subscription(
    const SubscriptionData& subscription_data, std::string& sub_id,
    int& http_code, const uint8_t http_version,
    ProblemDetails& problem_details) {
  std::string evsub_id;

  Logger::nrf_app().info(
      "Handle Create a new subscription (HTTP version %d)", http_version);
  std::shared_ptr<nrf_subscription> ss =
      std::make_shared<nrf_subscription>(m_event_sub);

  // convert to nrf_subscription
  if (api_conv::subscription_api_to_nrf_subscription(subscription_data, ss)) {
    if (authorize_subscription(ss)) {
      // generate a subscription ID
      generate_ev_subscription_id(evsub_id);
      ss.get()->set_subscription_id(evsub_id);

      // subscribe to NF status registered
      // subscribe_nf_status(evsub_id);  // from nrf_app
      // subscribe to NF status
      // ss.get()->subscribe_nf_status_registered(); //from subscription
      // add to the DB
      add_subscription(evsub_id, ss);
      Logger::nrf_app().debug("Added a subscription to the DB");
      // display the info
      ss.get()->display();
      // assign info for API server
      http_code = HTTP_STATUS_CODE_201_CREATED;
      sub_id    = evsub_id;
      return;
    } else {
      Logger::nrf_app().debug("Subscription is not authorized!");
      http_code = HTTP_STATUS_CODE_401_UNAUTHORIZED;
      return;
    }

  } else {
    // error
    Logger::nrf_app().warn(
        "Cannot convert the subscription data (from OpenAPI) to an NRF "
        "subscription data");
    http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
    problem_details.setCause(
        protocol_application_error_e2str[MANDATORY_IE_INCORRECT]);
  }
}

//------------------------------------------------------------------------------
void nrf_app::handle_remove_subscription(
    const std::string& sub_id, int& http_code, const uint8_t http_version,
    ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle Remove an existing subscription (HTTP version %d)", http_version);

  // Remove the subscription
  if (remove_subscription(sub_id)) {
    http_code = HTTP_STATUS_CODE_204_NO_CONTENT;
  } else {
    // error
    http_code = HTTP_STATUS_CODE_404_NOT_FOUND;
    problem_details.setCause(
        protocol_application_error_e2str[SUBSCRIPTION_NOT_FOUND]);
  }
}

//------------------------------------------------------------------------------
void nrf_app::handle_update_subscription(
    const std::string& sub_id, const std::vector<PatchItem>& patchItem,
    int& http_code, const uint8_t http_version,
    ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle an Update of subscription to NF Instance (HTTP version %d)",
      http_version);

  // Find the existing subscription
  std::shared_ptr<nrf_subscription> ss = {};

  ss              = find_subscription(sub_id);
  bool op_success = false;

  if (ss.get() != nullptr) {
    // patchItem should contain only 1 element
    for (auto p : patchItem) {
      patch_op_type_t op = api_conv::string_to_patch_operation(p.getOp());
      // Verify Path
      if ((p.getPath().substr(0, 1).compare("/") != 0) or
          (p.getPath().length() < 2)) {
        Logger::nrf_app().warn(
            "Bad value for operation path: %s ", p.getPath().c_str());
        http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
        problem_details.setCause(
            protocol_application_error_e2str[MANDATORY_IE_INCORRECT]);
        return;
      }

      std::string path = p.getPath().substr(1);

      switch (op) {
        case PATCH_OP_REPLACE: {
          if (path.compare("validityTime") == 0) {
            try {
              // TODO: (section 5.2.2.5.6, Update of Subscription to NF
              // Instances, 3GPP TS 29.510 V16.0.0 (2019-06)) if the NRF accepts
              // the extension of the lifetime of the subscription, but it
              // assigns a validity time different than the value suggested by
              // the NF Service Consumer, a "200 OK" response code shall be
              // returned
              boost::posix_time::ptime pt(
                  boost::posix_time::from_iso_string(p.getValue()));
              ss.get()->set_validity_time(pt);
              Logger::nrf_app().debug(
                  "New validity time: %s", p.getValue().c_str());
              Logger::nrf_app().debug("Updated a subscription to the DB");
              // display the info
              ss.get()->display();
              http_code  = HTTP_STATUS_CODE_204_NO_CONTENT;
              op_success = true;
            } catch (std::exception& e) {
              std::cout << "  Exception: " << e.what() << std::endl;
            }
          }
        } break;

        default: {
          Logger::nrf_app().warn("Requested operation is not valid!");
        }
      }

      if (!op_success) {
        http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
        problem_details.setCause(
            protocol_application_error_e2str[MANDATORY_IE_INCORRECT]);
      }
    }
  } else {
    Logger::nrf_app().debug(
        "Subscription with ID %s does not exit", sub_id.c_str());
    http_code = HTTP_STATUS_CODE_404_NOT_FOUND;
    problem_details.setCause(
        protocol_application_error_e2str[SUBSCRIPTION_NOT_FOUND]);
  }
}

//------------------------------------------------------------------------------
void nrf_app::handle_search_nf_instances(
    const std::string& target_nf_type, const std::string& requester_nf_type,
    const std::string& requester_nf_instance_id, uint32_t& limit_nfs,
    std::string& search_id, int& http_code, const uint8_t http_version,
    ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle NFDiscover to discover the set of NF Instances (HTTP version %d)",
      http_version);

  // Check if requester is allowed to discover the NF services
  if (!is_service_discover_allowed(
          requester_nf_instance_id, requester_nf_type)) {
    http_code = HTTP_STATUS_CODE_403_FORBIDDEN;
    Logger::nrf_app().debug(
        "Requester (instance id %s) is not allowed to discover the NF "
        "instances",
        requester_nf_instance_id.c_str());
    problem_details.setCause(
        protocol_application_error_e2str[MODIFICATION_NOT_ALLOWED]);
    return;
  }

  nf_type_t target_type    = api_conv::string_to_nf_type(target_nf_type);
  nf_type_t requester_type = api_conv::string_to_nf_type(requester_nf_type);

  if ((target_type == NF_TYPE_UNKNOWN) or (requester_type == NF_TYPE_UNKNOWN)) {
    Logger::nrf_app().debug("Unknown target type/requester type");
    http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
    problem_details.setCause(
        protocol_application_error_e2str[OPTIONAL_IE_INCORRECT]);
    return;
  }

  Logger::nrf_app().debug(
      "target nf type %s, requester nf type %s, requester nf instance id %s",
      target_nf_type.c_str(), requester_nf_type.c_str(),
      requester_nf_instance_id.c_str());

  std::shared_ptr<nrf_search_result> ss = std::make_shared<nrf_search_result>();
  // generate a search ID and assign to the search result
  generate_search_id(search_id);
  ss.get()->set_search_id(search_id);

  // set search result
  std::vector<std::shared_ptr<nrf_profile>> profiles = {};
  find_nf_profiles(target_type, profiles);
  if (profiles.size() > 0) {
    ss.get()->set_nf_instances(profiles);
  }
  ss.get()->set_limit_nf_instances(limit_nfs);
  ss.get()->set_num_nf_inst_complete(limit_nfs);

  if (profiles.size() > limit_nfs) {
    ss.get()->set_num_nf_inst_complete(profiles.size());
  }

  // set validity period
  ss.get()->set_validity_period(100000);  // 100s
  // add to the DB
  add_search_result(search_id, ss);
  Logger::nrf_app().debug(
      "Added a search result with ID %s to the DB", search_id.c_str());
  ss.get()->display();
  http_code = HTTP_STATUS_CODE_200_OK;
}

//------------------------------------------------------------------------------
void nrf_app::handle_access_token_request(
    const std::string& request_body, AccessTokenRsp& access_token_rsp,
    int& http_code, const uint8_t http_version,
    ProblemDetails& problem_details) {
  Logger::nrf_app().info(
      "Handle a request to request an OAuth2 access token from NRF (HTTP "
      "version %d)",
      http_version);

  std::map<std::string, std::string> access_token_req;
  // Process request_body
  std::vector<std::string> key_values;
  boost::split(
      key_values, request_body, boost::is_any_of("&"),
      boost::token_compress_on);
  for (auto p : key_values) {
    std::vector<std::string> key_value;
    boost::split(key_value, p, boost::is_any_of("="), boost::token_compress_on);
    if (key_value.size() != 2) {
      // error
      Logger::nrf_app().debug("Invalid key-value tuple");
    } else {
      access_token_req[key_value[0]] = key_value[1];
    }

    Logger::nrf_app().debug(
        "(Key, Value): %s, %s", key_value[0].c_str(), key_value[1].c_str());
  }

  // validate the Access Token Request
  if ((access_token_req.count("grant_type") == 0) or
      (access_token_req.count("nfInstanceId") == 0) or
      (access_token_req.count("scope") == 0)) {
    http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
    problem_details.setCause(
        protocol_application_error_e2str[MANDATORY_IE_MISSING]);
    Logger::nrf_app().info("Grant type/nfInstanceId/scope missing");
    return;
  }

  // TODO: authorize NF service consumer

  // Generate signature
  std::string signature = {};
  bool result           = false;
  if ((access_token_req.count("nfType") > 0) and
      (access_token_req.count("targetNfType") > 0)) {
    nf_type_t nf_type =
        api_conv::string_to_nf_type(access_token_req.at("nfType"));
    nf_type_t target_nf_type =
        api_conv::string_to_nf_type(access_token_req.at("targetNfType"));

    result = nrf_jwt_inst->generate_signature(
        access_token_req.at("nfInstanceId"), access_token_req.at("scope"),
        access_token_req.at("nfType"), access_token_req.at("targetNfType"),
        nrf_instance_id, signature);

  } else if (access_token_req.count("targetNfInstanceId") > 0) {
    result = nrf_jwt_inst->generate_signature(
        access_token_req.at("nfInstanceId"), access_token_req.at("scope"),
        access_token_req.at("targetNfInstanceId"), nrf_instance_id, signature);
  }

  if (!result) {
    http_code = HTTP_STATUS_CODE_400_BAD_REQUEST;
    problem_details.setCause(
        protocol_application_error_e2str[MANDATORY_QUERY_PARAM_INCORRECT]);
    Logger::nrf_app().info("Mandatory IE incorrect");
    return;
  }

  // AccessTokenRsp
  access_token_rsp.setAccessToken(signature);
  access_token_rsp.setTokenType("Bearer");
  // TODO: Expires_in, Scope;
  http_code = HTTP_STATUS_CODE_200_OK;
}

//------------------------------------------------------------------------------
bool nrf_app::add_nf_profile(
    const std::string& profile_id, const std::shared_ptr<nrf_profile>& p) {
  /*
   //if profile with this id exist, update
   if (instance_id2nrf_profile.count(profile_id) > 0) {
   Logger::nrf_app().info(
   "Update a NF profile to the list (profile ID %s)",
   profile_id.c_str());
   instance_id2nrf_profile.at(profile_id) = p;
   } else {
   //if not, add to the list
   Logger::nrf_app().info(
   "Insert a NF profile to the list (profile ID %s)",
   profile_id.c_str());
   instance_id2nrf_profile.emplace(profile_id, p);
   }*/

  // Create or update if profile exist
  std::unique_lock lock(m_instance_id2nrf_profile);
  instance_id2nrf_profile[profile_id] = p;

  return true;
}

//------------------------------------------------------------------------------
bool nrf_app::update_nf_profile(
    const std::string& profile_id, const std::shared_ptr<nrf_profile>& p) {
  std::unique_lock lock(m_instance_id2nrf_profile);
  // if profile with this id exist, return false
  if (instance_id2nrf_profile.count(profile_id) > 0) {
    // if not, update to the list
    Logger::nrf_app().info(
        "Updated the NF profile (profile ID %s)", profile_id.c_str());
    instance_id2nrf_profile.at(profile_id) = p;
    return true;
  } else {
    Logger::nrf_app().info("NF profile (ID %s) not found", profile_id.c_str());
    return false;
  }
}

//------------------------------------------------------------------------------
std::shared_ptr<nrf_profile> nrf_app::find_nf_profile(
    const std::string& profile_id) const {
  // Logger::nrf_app().info("Find a NF profile with ID %s", profile_id.c_str());

  std::shared_lock lock(m_instance_id2nrf_profile);
  if (instance_id2nrf_profile.count(profile_id) > 0) {
    return instance_id2nrf_profile.at(profile_id);
  } else {
    Logger::nrf_app().info("NF profile (ID %s) not found", profile_id.c_str());
    return nullptr;
  }
}

//------------------------------------------------------------------------------
bool nrf_app::find_nf_profile(
    const std::string& profile_id, std::shared_ptr<nrf_profile>& p) const {
  // Logger::nrf_app().info("Find a NF profile with ID %s", profile_id.c_str());

  std::shared_lock lock(m_instance_id2nrf_profile);
  if (instance_id2nrf_profile.count(profile_id) > 0) {
    p = instance_id2nrf_profile.at(profile_id);
    return true;
  } else {
    Logger::nrf_app().info("NF profile (ID %s) not found", profile_id.c_str());
    return false;
  }
}

//------------------------------------------------------------------------------
void nrf_app::find_nf_profiles(
    const nf_type_t& nf_type,
    std::vector<std::shared_ptr<nrf_profile>>& profiles) const {
  std::shared_lock lock(m_instance_id2nrf_profile);
  for (auto profile : instance_id2nrf_profile) {
    if (profile.second.get()->get_nf_type() == nf_type) {
      profiles.push_back(profile.second);
    }
  }
}

//------------------------------------------------------------------------------
bool nrf_app::is_profile_exist(const std::string& profile_id) const {
  Logger::nrf_app().info(
      "Check if a profile with this ID %s exist", profile_id.c_str());

  std::shared_lock lock(m_instance_id2nrf_profile);
  if (instance_id2nrf_profile.count(profile_id) > 0) {
    return true;
  } else {
    Logger::nrf_app().info("NF profile (ID %s) not found", profile_id.c_str());
    return false;
  }
}

//------------------------------------------------------------------------------
bool nrf_app::remove_nf_profile(const std::shared_ptr<nrf_profile>& snp) {
  std::string key;
  snp.get()->get_nf_instance_id(key);
  std::unique_lock lock(m_instance_id2nrf_profile);
  if (instance_id2nrf_profile.erase(key)) {
    Logger::nrf_app().info(
        "Removed NF profile (ID %s) from the list", key.c_str());
    return true;
  } else {
    Logger::nrf_app().info(
        "Remove_NF_profile, profile not found (ID %s)", key.c_str());
    return false;
  }
}

//------------------------------------------------------------------------------
bool nrf_app::remove_nf_profile(const std::string& profile_id) {
  std::unique_lock lock(m_instance_id2nrf_profile);
  if (instance_id2nrf_profile.erase(profile_id)) {
    Logger::nrf_app().info(
        "Removed NF profile (ID %s) from the list", profile_id.c_str());
    return true;
  } else {
    Logger::nrf_app().info(
        "Remove_NF_profile, profile not found (ID %s)", profile_id.c_str());
    return false;
  }
}

//------------------------------------------------------------------------------
bool nrf_app::add_subscription(
    const std::string& sub_id, const std::shared_ptr<nrf_subscription>& s) {
  std::unique_lock lock(m_subscription_id2nrf_subscription);
  /*
   //if profile with this id exist, update
   if (subscrition_id2nrf_subscription.count(sub_id) > 0) {
   Logger::nrf_app().info(
   "Update a subscription to the list (Subscription ID %s)",
   sub_id.c_str());
   subscrition_id2nrf_subscription.at(sub_id) = s;
   } else {
   //if not, add to the list
   Logger::nrf_app().info(
   "Insert a subscription to the list (Subscription ID %s)",
   sub_id.c_str());
   subscrition_id2nrf_subscription.emplace(sub_id, s);
   }*/
  // Create or update if subscription exist
  subscrition_id2nrf_subscription[sub_id] = s;
  return true;
}

//------------------------------------------------------------------------------
bool nrf_app::remove_subscription(const std::string& sub_id) {
  std::unique_lock lock(m_subscription_id2nrf_subscription);
  if (subscrition_id2nrf_subscription.erase(sub_id)) {
    Logger::nrf_app().info(
        "Removed subscription (ID %s) from the list", sub_id.c_str());
    return true;
  } else {
    Logger::nrf_app().info(
        "Remove_subscription, subscription not found (ID %s)", sub_id.c_str());
    return false;
  }
}

//------------------------------------------------------------------------------
std::shared_ptr<nrf_subscription> nrf_app::find_subscription(
    const std::string& sub_id) const {
  // Logger::nrf_app().info("Find a subscription with ID %s", sub_id.c_str());

  std::shared_lock lock(m_subscription_id2nrf_subscription);
  if (subscrition_id2nrf_subscription.count(sub_id) > 0) {
    return subscrition_id2nrf_subscription.at(sub_id);
  } else {
    Logger::nrf_app().info("Subscription (ID %s) not found", sub_id.c_str());
    return nullptr;
  }
}

//------------------------------------------------------------------------------
void nrf_app::subscribe_task_tick(uint64_t ms) {
  struct itimerspec its;
  its.it_value.tv_sec  = 20;  // seconds
  its.it_value.tv_nsec = 0;   // 100 * 1000 * 1000; //100ms

  const uint64_t interval =
      its.it_value.tv_sec * 1000 +
      its.it_value.tv_nsec / 1000000;  // convert sec, nsec to msec

  Logger::nrf_app().debug("subscribe task_tick: %d", ms);
  m_event_sub.subscribe_task_tick(
      boost::bind(&nrf_app::handle_heartbeart_timeout, this, _1), interval,
      ms % 20000);
}

//------------------------------------------------------------------------------
void nrf_app::handle_heartbeart_timeout(uint64_t ms) {
  Logger::nrf_app().info("handle_heartbeart_timeout %d", ms);
}

//------------------------------------------------------------------------------
bool nrf_app::authorize_subscription(
    const std::shared_ptr<nrf_subscription>& s) const {
  // TODO:
  return true;
}

//------------------------------------------------------------------------------
void nrf_app::generate_ev_subscription_id(std::string& sub_id) {
  sub_id = std::to_string(evsub_id_generator.get_uid());
}

//------------------------------------------------------------------------------
evsub_id_t nrf_app::generate_ev_subscription_id() {
  return evsub_id_generator.get_uid();
}

//------------------------------------------------------------------------------
void nrf_app::subscribe_nf_status() {
  // depending on the type of subscription, subscribe to the corresponding event
  // for now subscribe to all events
  subscribe_nf_status_registered();
  subscribe_nf_status_deregistered();
  subscribe_nf_status_profile_changed();
}

//------------------------------------------------------------------------------
void nrf_app::subscribe_nf_status_registered() {
  Logger::nrf_app().debug("Subscribe to NF status registered event");
  bs2::connection c = m_event_sub.subscribe_nf_status_registered(
      boost::bind(&nrf_app::handle_nf_status_registered, this, _1));
  connections.push_back(c);
}

//------------------------------------------------------------------------------
void nrf_app::handle_nf_status_registered(const std::string& profile_id) {
  Logger::nrf_app().info(
      "Handle NF status registered event, profile id %s", profile_id.c_str());

  std::shared_ptr<nrf_profile> profile = {};
  Logger::nrf_app().info("\tFind a NF profile with ID %s", profile_id.c_str());
  find_nf_profile(profile_id, profile);
  if (profile.get() != nullptr) {
    std::vector<std::string> notification_uris = {};
    get_subscription_list(
        profile_id, NOTIFICATION_TYPE_NF_REGISTERED, notification_uris);
    // send notifications
    if (notification_uris.size() > 0)
      nrf_client_inst->notify_subscribed_event(
          profile, NOTIFICATION_TYPE_NF_REGISTERED, notification_uris);
    else
      Logger::nrf_app().debug("\tNo subscription found");

  } else {
    Logger::nrf_app().error(
        "\tNF profile not found, profile id %s", profile_id.c_str());
  }
}

//------------------------------------------------------------------------------
void nrf_app::subscribe_nf_status_deregistered() {
  Logger::nrf_app().debug("Subscribe to NF status deregistered event");
  bs2::connection c = m_event_sub.subscribe_nf_status_deregistered(
      boost::bind(&nrf_app::handle_nf_status_deregistered, this, _1));
  connections.push_back(c);
}

//------------------------------------------------------------------------------
void nrf_app::handle_nf_status_deregistered(
    const std::shared_ptr<nrf_profile>& p) {
  Logger::nrf_app().info(
      "Handle NF status deregistered event, profile id %s",
      p.get()->get_nf_instance_id().c_str());

  std::vector<std::string> notification_uris = {};
  get_subscription_list(
      p.get()->get_nf_instance_id(), NOTIFICATION_TYPE_NF_DEREGISTERED,
      notification_uris);
  // send notifications
  if (notification_uris.size() > 0)
    nrf_client_inst->notify_subscribed_event(
        p, NOTIFICATION_TYPE_NF_DEREGISTERED, notification_uris);
  else
    Logger::nrf_app().debug("\tNo subscription found");
}

//------------------------------------------------------------------------------
void nrf_app::subscribe_nf_status_profile_changed() {
  Logger::nrf_app().debug("Subscribe to NF status profile changed event");
  bs2::connection c = m_event_sub.subscribe_nf_status_profile_changed(
      boost::bind(&nrf_app::handle_nf_status_profile_changed, this, _1));
  connections.push_back(c);
}

//------------------------------------------------------------------------------
void nrf_app::handle_nf_status_profile_changed(const std::string& profile_id) {
  Logger::nrf_app().info(
      "Handle NF status profile changed event, profile id %s",
      profile_id.c_str());
  std::shared_ptr<nrf_profile> profile = {};
  Logger::nrf_app().info("\tFind a NF profile with ID %s", profile_id.c_str());
  find_nf_profile(profile_id, profile);
  if (profile.get() != nullptr) {
    std::vector<std::string> notification_uris = {};
    get_subscription_list(
        profile_id, NOTIFICATION_TYPE_NF_PROFILE_CHANGED, notification_uris);
    // Notification data includes NF profile (other alternative, includes
    // profile_changes)
    // send notifications
    if (notification_uris.size() > 0)
      nrf_client_inst->notify_subscribed_event(
          profile, NOTIFICATION_TYPE_NF_PROFILE_CHANGED, notification_uris);
    else
      Logger::nrf_app().debug("\tNo subscription found");
  } else {
    Logger::nrf_app().error(
        "NF profile not found, profile id %s", profile_id.c_str());
  }
}

//------------------------------------------------------------------------------
void nrf_app::get_subscription_list(
    const std::string& profile_id, const uint8_t& notification_type,
    std::vector<std::string>& uris) const {
  Logger::nrf_app().debug(
      "\tGet the list of subscriptions related to this profile, profile id %s",
      profile_id.c_str());

  std::shared_ptr<nrf_profile> profile = {};

  find_nf_profile(profile_id, profile);
  if (profile.get() == nullptr) {
    // error
    return;
  }

  for (auto s : subscrition_id2nrf_subscription) {
    Logger::nrf_app().info(
        "\tVerifying subscription, subscription id %s", s.first.c_str());
    std::string uri;
    s.second.get()->get_notification_uri(uri);

    // check notification event type
    bool match_notif_type = false;
    for (auto i : s.second.get()->get_notif_events()) {
      if (i == notification_type) {
        match_notif_type = true;
        break;
      }
    }
    if (!match_notif_type) continue;

    // check validity time
    boost::posix_time::ptime t(boost::posix_time::microsec_clock::local_time());
    Logger::nrf_app().debug(
        "\tCurrent time %s", boost::posix_time::to_iso_string(t).c_str());
    if (t > s.second.get()->get_validity_time()) {
      Logger::nrf_app().debug(
          "\tThis subscription expires, current time %s, validity time %s",
          boost::posix_time::to_iso_string(t).c_str(),
          boost::posix_time::to_iso_string(s.second.get()->get_validity_time())
              .c_str());
      continue;
    }

    subscription_condition_t condition = {};
    s.second.get()->get_sub_condition(condition);

    switch (condition.type) {
      case NF_INSTANCE_ID_COND: {
        if (profile_id.compare(condition.nf_instance_id) == 0) {
          uris.push_back(uri);
          Logger::nrf_app().debug(
              "\tSubscription id %s, uri %s", s.first.c_str(), uri.c_str());
        }

      } break;
      case NF_TYPE_COND: {
        std::string nf_type = nf_type_e2str[profile.get()->get_nf_type()];
        if (nf_type.compare(condition.nf_type) == 0) {
          uris.push_back(uri);
          Logger::nrf_app().debug(
              "\tSubscription id %s, uri %s", s.first.c_str(), uri.c_str());
        }
      } break;

      case SERVICE_NAME_COND: {
        std::string service_name;
        profile.get()->get_nf_instance_name(service_name);
        if (service_name.compare(condition.service_name) == 0) {
          uris.push_back(uri);
          Logger::nrf_app().debug(
              "\tSubscription id %s, uri %s", s.first.c_str(), uri.c_str());
        }

      } break;
      case AMF_COND: {
        std::string nf_type = nf_type_e2str[profile.get()->get_nf_type()];
        if (nf_type.compare("AMF") == 0) {
          amf_info_t info = {};
          std::static_pointer_cast<amf_profile>(profile).get()->get_amf_info(
              info);
          if ((info.amf_region_id.compare(condition.amf_info.amf_region_id) ==
               0) and
              (info.amf_set_id.compare(condition.amf_info.amf_set_id) == 0)) {
            uris.push_back(uri);
            Logger::nrf_app().debug(
                "\tSubscription id %s, uri %s", s.first.c_str(), uri.c_str());
          }
        }
      } break;

      case GUAMI_LIST_COND: {
        // TODO:
      } break;

      case NETWOTK_SLICE_COND: {
        // TODO:
      } break;

      case NF_GROUP_COND: {
        // TODO:
      } break;

      default: {
        // TODO:
      }
    }
  }
  // TODO:
}

//------------------------------------------------------------------------------
bool nrf_app::is_service_discover_allowed(
    const std::string& requester_instance_id,
    const std::string& requester_nf_type) {
  // TODO:
  return true;
}

//------------------------------------------------------------------------------
void nrf_app::generate_search_id(std::string& search_id) {
  search_id = std::to_string(search_id_generator.get_uid());
}

//------------------------------------------------------------------------------
bool nrf_app::add_search_result(
    const std::string& id, const std::shared_ptr<nrf_search_result>& s) {
  std::unique_lock lock(m_search_id2search_result);
  // Create or update if search result exist
  search_id2search_result[id] = s;
  return true;
}

bool nrf_app::find_search_result(
    const std::string& search_id, std::shared_ptr<nrf_search_result>& s) const {
  std::shared_lock lock(m_search_id2search_result);
  if (search_id2search_result.count(search_id) > 0) {
    s = search_id2search_result.at(search_id);
    return true;
  } else {
    Logger::nrf_app().info(
        "Search result (ID %s) not found", search_id.c_str());
    return false;
  }
}
