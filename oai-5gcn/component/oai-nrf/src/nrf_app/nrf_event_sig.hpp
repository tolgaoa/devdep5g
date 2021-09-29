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

/*! \file nrf_event_sig.hpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#ifndef FILE_NRF_EVENT_SIG_HPP_SEEN
#define FILE_NRF_EVENT_SIG_HPP_SEEN

#include <boost/signals2.hpp>

namespace bs2 = boost::signals2;

namespace oai {
namespace nrf {
namespace app {

class nrf_profile;

typedef bs2::signal_type<
    void(uint64_t), bs2::keywords::mutex_type<bs2::dummy_mutex>>::type
    task_sig_t;

// Signal for NF Status
// Subscription ID, NF Status
typedef bs2::signal_type<
    void(const std::string&), bs2::keywords::mutex_type<bs2::dummy_mutex>>::type
    nf_status_sig_t;

typedef bs2::signal_type<
    void(const std::shared_ptr<nrf_profile>& p),
    bs2::keywords::mutex_type<bs2::dummy_mutex>>::type nf_deregistered_sig_t;

typedef bs2::signal_type<
    void(const std::shared_ptr<nrf_profile>& p),
    bs2::keywords::mutex_type<bs2::dummy_mutex>>::type nf_status_change_sig_t;

}  // namespace app
}  // namespace nrf
}  // namespace oai

#endif /* FILE_NRF_EVENT_SIG_HPP_SEEN */
