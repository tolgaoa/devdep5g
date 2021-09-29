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

/*! \file smf_event_sig.hpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: tien-thinh.nguyen@eurecom.fr
 */

#ifndef FILE_SMF_EVENT_SIG_HPP_SEEN
#define FILE_SMF_EVENT_SIG_HPP_SEEN

#include <boost/signals2.hpp>
#include <string>
#include "3gpp_24.007.h"

namespace bs2 = boost::signals2;

namespace smf {

// Signal for PDU session status
// SCID, PDU Session Status, HTTP version
typedef bs2::signal_type<
    void(scid_t, const std::string&, uint8_t),
    bs2::keywords::mutex_type<bs2::dummy_mutex>>::type sm_context_status_sig_t;

// Signal for Event exposure
// PDU session Release, SUPI, PDU SessionID, HTTP version
typedef bs2::signal_type<
    void(supi64_t, pdu_session_id_t, uint8_t),
    bs2::keywords::mutex_type<bs2::dummy_mutex>>::type
    ee_pdu_session_release_sig_t;

// TODO: ee_ue_ip_address_change_sig_t; //UI IP Address, UE ID
// TODO: Access Type Change
// TODO: UP Path Change
// TODO: PLMN Change
// TODO: Downlink data delivery status

}  // namespace smf
#endif /* FILE_SMF_EVENT_SIG_HPP_SEEN */
