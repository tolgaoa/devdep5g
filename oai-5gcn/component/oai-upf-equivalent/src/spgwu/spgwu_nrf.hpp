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

/*! \file spgwu_nrf.hpp
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN
 \company Eurecom
 \date 2021
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr
 */

#ifndef FILE_SPGWU_NRF_HPP_SEEN
#define FILE_SPGWU_NRF_HPP_SEEN

#include <map>
#include <thread>

#include "itti.hpp"
#include <curl/curl.h>
#include "3gpp_29.510.h"
#include "spgwu_profile.hpp"

namespace spgwu {

#define TASK_SPGWU_NRF_TIMEOUT_NRF_HEARTBEAT (1)
#define TASK_SPGWU_NRF_TIMEOUT_NRF_DEREGISTRATION (2)

class spgwu_nrf {
 private:
  std::thread::id thread_id;
  std::thread thread;

  spgwu_profile upf_profile;    // UPF profile
  std::string upf_instance_id;  // UPF instance id
  timer_id_t timer_nrf_heartbeat;

 public:
  spgwu_nrf();
  spgwu_nrf(spgwu_nrf const&) = delete;
  void operator=(spgwu_nrf const&) = delete;

  /*
   * Send NF instance registration to NRF
   * @param [std::string &] url: NRF's URL
   * @return void
   */
  void send_register_nf_instance(std::string& url);

  /*
   * Send NF instance registration to NRF
   * @param [std::string &] url: NRF's URL
   * @param [nlohmann::json &] data: Json data to be sent
   * @return void
   */
  void send_update_nf_instance(std::string& url, nlohmann::json& data);

  /*
   * Send NF deregister to NRF
   * @param [std::string &] url: NRF's URL
   * @return void
   */
  void send_deregister_nf_instance(std::string& url);

  /*
   * Trigger NF instance registration to NRF
   * @param [void]
   * @return void
   */
  void register_to_nrf();

  /*
   * Generate a random UUID for UPF instance
   * @param [void]
   * @return void
   */
  void generate_uuid();

  /*
   * Generate a UPF profile for this instance
   * @param [void]
   * @return void
   */
  void generate_upf_profile();

  /*
   * will be executed when NRF Heartbeat timer expires
   * @param [timer_id_t] timer_id
   * @param [uint64_t] arg2_user
   * @return void
   */
  void timer_nrf_heartbeat_timeout(timer_id_t timer_id, uint64_t arg2_user);

  /*
   * will be executed when NRF Heartbeat timer expires
   * @param [timer_id_t] timer_id
   * @param [uint64_t] arg2_user
   * @return void
   */
  void timer_nrf_deregistration(timer_id_t timer_id, uint64_t arg2_user);
};
}  // namespace spgwu
#endif /* FILE_SPGWU_NRF_HPP_SEEN */
