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

/*! \file smf_event.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2019
 \email: tien-thinh.nguyen@eurecom.fr
 */

#include "smf_event.hpp"
#include "itti.hpp"
#include "smf_app.hpp"
#include "smf_subscription.hpp"

using namespace smf;
extern smf::smf_app* smf_app_inst;
extern itti_mw* itti_inst;

//------------------------------------------------------------------------------
bs2::connection smf_event::subscribe_sm_context_status(
    const sm_context_status_sig_t::slot_type& sig) {
  return sm_context_status.connect(sig);
}

//------------------------------------------------------------------------------
bs2::connection smf_event::subscribe_ee_pdu_session_release(
    const ee_pdu_session_release_sig_t::slot_type& sig) {
  return ee_pdu_session_release.connect(sig);
}
