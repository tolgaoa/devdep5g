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

/*! \file nrf_search_result.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#include "nrf_search_result.hpp"

#include <nlohmann/json.hpp>

#include "logger.hpp"

using namespace oai::nrf::app;

//------------------------------------------------------------------------------
void nrf_search_result::set_search_id(const std::string& id) {
  search_id = id;
}

//------------------------------------------------------------------------------
void nrf_search_result::get_search_id(std::string& id) const {
  id = search_id;
}

//------------------------------------------------------------------------------
std::string nrf_search_result::get_search_id() const {
  return search_id;
}

//------------------------------------------------------------------------------
void nrf_search_result::set_nf_instances(
    const std::vector<std::shared_ptr<nrf_profile>>& instances) {
  nf_instances = instances;
}

//------------------------------------------------------------------------------
void nrf_search_result::add_nf_instance(
    const std::shared_ptr<nrf_profile>& instance) {
  nf_instances.push_back(instance);
}

//------------------------------------------------------------------------------
void nrf_search_result::get_nf_instances(
    std::vector<std::shared_ptr<nrf_profile>>& instances) const {
  instances = nf_instances;
}

//------------------------------------------------------------------------------
std::vector<std::shared_ptr<nrf_profile>> nrf_search_result::get_nf_instances()
    const {
  return nf_instances;
}

//------------------------------------------------------------------------------
void nrf_search_result::set_validity_period(const uint64_t& p) {
  validity_period = p;
}

//------------------------------------------------------------------------------
void nrf_search_result::get_validity_period(uint64_t& p) const {
  p = validity_period;
}

//------------------------------------------------------------------------------
uint64_t nrf_search_result::get_validity_period() const {
  return validity_period;
}

//------------------------------------------------------------------------------
void nrf_search_result::set_num_nf_inst_complete(const uint32_t& n) {
  num_nf_inst_complete = n;
}

//------------------------------------------------------------------------------
void nrf_search_result::get_num_nf_inst_complete(uint32_t& n) const {
  n = num_nf_inst_complete;
}

//------------------------------------------------------------------------------
void nrf_search_result::set_limit_nf_instances(const uint32_t& l) {
  limit_nf_instances = l;
}

//------------------------------------------------------------------------------
void nrf_search_result::get_limit_nf_instances(uint32_t& l) const {
  l = limit_nf_instances;
}

//------------------------------------------------------------------------------
uint32_t nrf_search_result::get_limit_nf_instances() const {
  return limit_nf_instances;
}
//------------------------------------------------------------------------------
void nrf_search_result::display() {
  Logger::nrf_app().debug("Search result information");
  Logger::nrf_app().debug("\tSearch ID: %s", search_id.c_str());

  Logger::nrf_app().debug("\tValidity period: %d", validity_period);

  Logger::nrf_app().debug("\tNF instances information");
  for (auto n : nf_instances) {
    n.get()->display();
  }
}

//------------------------------------------------------------------------------
void nrf_search_result::to_json(
    nlohmann::json& data, const uint32_t& limit_nfs) const {
  data = {};

  data["validityPeriod"] = validity_period;
  data["nfInstances"]    = nlohmann::json::array();
  uint32_t limit         = limit_nfs > 0 ? limit_nfs : nf_instances.size();
  int i                  = 0;
  for (auto instance : nf_instances) {
    if (i < limit) {
      nlohmann::json instance_json = {};
      instance.get()->to_json(instance_json);
      data["nfInstances"].push_back(instance_json);
    } else {
      break;
    }
    i++;
  }
  data["searchId"] = search_id;
}
