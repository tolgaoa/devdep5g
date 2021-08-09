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

/*! \file nrf_event.cpp
 \brief
 \author
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#include "nrf_event.hpp"
#include <iostream>
#include "nrf_app.hpp"
#include "nrf_event_sig.hpp"

using namespace oai::nrf::app;

//------------------------------------------------------------------------------
bs2::connection nrf_event::subscribe_task_tick(
    const task_sig_t::slot_type& sig, uint64_t period, uint64_t start) {
  /* Wrap the actual callback in a lambda. The latter checks whether the
   * current time is after start time, and ensures that the callback is only
   * called every X ms with X being the period time. This way, it is possible
   * to register to be notified every X ms instead of every ms, which provides
   * greater freedom to implementations. */
  auto f = [period, start, sig](uint64_t t) {
    if (t >= start && (t - start) % period == 0) sig(t);
  };
  return task_tick.connect(f);
}

//------------------------------------------------------------------------------
bs2::connection nrf_event::subscribe_task_tick_extended(
    const task_sig_t::extended_slot_type& sig, uint64_t period,
    uint64_t start) {
  /* Wrap the actual callback in a lambda. The latter checks whether the
   * current time is after start time, and ensures that the callback is only
   * called every X ms with X being the period time. This way, it is possible
   * to register to be notified every X ms instead of every ms, which provides
   * greater freedom to implementations. */
  auto f = [period, start, sig](const bs2::connection& c, uint64_t t) {
    if (t >= start && (t - start) % period == 0) sig(c, t);
  };
  return task_tick.connect_extended(f);
}

//------------------------------------------------------------------------------
bs2::connection nrf_event::subscribe_nf_status_change(
    const nf_status_change_sig_t::slot_type& sig) {
  return nf_status_change.connect(sig);
}

//------------------------------------------------------------------------------
bs2::connection nrf_event::subscribe_nf_status_registered(
    const nf_status_sig_t::slot_type& sig) {
  return nf_status_registered.connect(sig);
}

//------------------------------------------------------------------------------
bs2::connection nrf_event::subscribe_nf_status_deregistered(
    const nf_deregistered_sig_t::slot_type& sig) {
  return nf_status_deregistered.connect(sig);
}

//------------------------------------------------------------------------------
bs2::connection nrf_event::subscribe_nf_status_profile_changed(
    const nf_status_sig_t::slot_type& sig) {
  return nf_status_profile_changed.connect(sig);
}
