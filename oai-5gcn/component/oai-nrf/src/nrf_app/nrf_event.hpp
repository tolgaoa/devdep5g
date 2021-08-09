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

/*! \file nrf_event.hpp
 \brief
 \author
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#ifndef FILE_NRF_EVENT_HPP_SEEN
#define FILE_NRF_EVENT_HPP_SEEN

#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

#include "nrf.h"
#include "nrf_event_sig.hpp"
#include "task_manager.hpp"

namespace oai {
namespace nrf {
namespace app {

class task_manager;
// class nrf_profile;

class nrf_event {
 public:
  nrf_event(){};
  nrf_event(nrf_event const&) = delete;
  void operator=(nrf_event const&) = delete;

  static nrf_event& get_instance() {
    static nrf_event instance;
    return instance;
  }

  // class register/handle event
  friend class nrf_app;
  friend class task_manager;
  friend class nrf_profile;

  /*
   * Subscribe to the task tick event
   * @param [const task_sig_t::slot_type &] sig
   * @param [uint64_t] period: interval between two events
   * @param [uint64_t] start:
   * @return void
   */
  bs2::connection subscribe_task_tick(
      const task_sig_t::slot_type& sig, uint64_t period, uint64_t start = 0);

  /*
   * Subscribe to the extended task tick event
   * @param [const task_sig_t::slot_type &] sig
   * @param [uint64_t] period: interval between two events
   * @param [uint64_t] start:
   * @return void
   */
  bs2::connection subscribe_task_tick_extended(
      const task_sig_t::extended_slot_type& sig, uint64_t period,
      uint64_t start = 0);

  /*
   * Subscribe to the nf status change event
   * @param [const task_sig_t::slot_type &] sig
   * @param [uint64_t] period: interval between two events
   * @param [uint64_t] start:
   * @return void
   */
  bs2::connection subscribe_nf_status_change(
      const nf_status_change_sig_t::slot_type& sig);

  bs2::connection subscribe_nf_status_registered(
      const nf_status_sig_t::slot_type& sig);

  bs2::connection subscribe_nf_status_deregistered(
      const nf_deregistered_sig_t::slot_type& sig);

  bs2::connection subscribe_nf_status_profile_changed(
      const nf_status_sig_t::slot_type& sig);

 private:
  task_sig_t task_tick;
  nf_status_change_sig_t nf_status_change;
  nf_status_sig_t nf_status_registered;
  nf_deregistered_sig_t nf_status_deregistered;
  nf_status_sig_t nf_status_profile_changed;
};
}  // namespace app
}  // namespace nrf
}  // namespace oai

#endif /* FILE_NRF_EVENT_HPP_SEEN */
