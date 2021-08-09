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

/*
 *  itti_msg_n4_restore.hpp
 *
 *  Created on: March 27, 2019
 *  Author: lionel.gauthier@eurecom.fr
 */

#ifndef ITTI_MSG_N4_RESTORE_HPP_INCLUDED_
#define ITTI_MSG_N4_RESTORE_HPP_INCLUDED_

#include "3gpp_29.244.h"
#include "itti_msg.hpp"
#include <set>

class itti_n4_restore : public itti_msg {
 public:
  itti_n4_restore(const task_id_t origin, const task_id_t destination)
      : itti_msg(RESTORE_N4_SESSIONS, origin, destination), sessions() {}
  itti_n4_restore(const itti_n4_restore& i)
      : itti_msg(i), sessions(i.sessions) {}
  itti_n4_restore(
      const itti_n4_restore& i, const task_id_t orig, const task_id_t dest)
      : itti_n4_restore(i) {
    origin      = orig;
    destination = dest;
  }
  const char* get_msg_name() { return "N4_RESTORE"; };

  std::set<pfcp::fseid_t> sessions;
};

#endif /* ITTI_MSG_N4_RESTORE_HPP_INCLUDED_ */
