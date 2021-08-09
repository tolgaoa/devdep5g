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

/*! \file itti_msg.cpp
   \brief
   \author  Lionel GAUTHIER
   \date 2018
   \email: lionel.gauthier@eurecom.fr
*/
#include "itti_msg.hpp"
#include "itti.hpp"

extern itti_mw* itti_inst;

itti_msg::itti_msg()
    : msg_type(ITTI_MSG_TYPE_NONE), origin(TASK_NONE), destination(TASK_NONE) {
  msg_num = itti_inst->increment_message_number();
};

itti_msg::itti_msg(
    const itti_msg_type_t msg_type, task_id_t origin, task_id_t destination)
    : msg_type(msg_type), origin(origin), destination(destination) {
  msg_num = itti_inst->increment_message_number();
};

itti_msg::itti_msg(const itti_msg& i)
    : msg_type(i.msg_type),
      msg_num(i.msg_num),
      origin(i.origin),
      destination(i.destination){};

const char* itti_msg::get_msg_name() {
  return "UNINITIALIZED";
}
