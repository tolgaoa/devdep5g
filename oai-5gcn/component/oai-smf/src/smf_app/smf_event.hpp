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

/*! \file smf_event.hpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: tien-thinh.nguyen@eurecom.fr
 */

#ifndef FILE_SMF_EVENT_HPP_SEEN
#define FILE_SMF_EVENT_HPP_SEEN

#include <boost/signals2.hpp>
namespace bs2 = boost::signals2;

#include "smf.h"
#include "smf_event_sig.hpp"

namespace smf {
class smf_event {
 public:
  smf_event(){};
  smf_event(smf_event const&) = delete;
  void operator=(smf_event const&) = delete;

  static smf_event& get_instance() {
    static smf_event instance;
    return instance;
  }

  // class register/handle event
  friend class smf_app;
  friend class smf_context;
  friend class smf_profile;

  /*
   * Subscribe to SM Context Status Notification signal
   * @param [const sm_context_status_sig_t::slot_type&] sig:  slot_type
   * parameter
   * @return boost::signals2::connection: the connection between the signal and
   * the slot
   */
  bs2::connection subscribe_sm_context_status(
      const sm_context_status_sig_t::slot_type& sig);

  /*
   * Subscribe to Event Exposure Event: PDU Session Release
   * @param [const ee_pdu_session_release_sig_t::slot_type&] sig:  slot_type
   * parameter
   * @return boost::signals2::connection: the connection between the signal and
   * the slot
   */
  bs2::connection subscribe_ee_pdu_session_release(
      const ee_pdu_session_release_sig_t::slot_type& sig);

 private:
  sm_context_status_sig_t
      sm_context_status;  // Signal for SM Context status update
  ee_pdu_session_release_sig_t
      ee_pdu_session_release;  // Signal for SM Context status update
};
}  // namespace smf
#endif /* FILE_SMF_EVENT_HPP_SEEN */
