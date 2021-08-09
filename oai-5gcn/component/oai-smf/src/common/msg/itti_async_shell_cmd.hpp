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

/*! \file itti_async_shell_cmd.hpp
 \brief
 \author Lionel Gauthier
 \company Eurecom
 \email: lionel.gauthier@eurecom.fr
 */

#ifndef FILE_ITTI_ASYNC_SHELL_CMD_SEEN
#define FILE_ITTI_ASYNC_SHELL_CMD_SEEN

#include "itti_msg.hpp"

class itti_async_shell_cmd : public itti_msg {
 public:
  itti_async_shell_cmd(
      const task_id_t origin, const task_id_t destination,
      const std::string& system_cmd, bool is_abort_on_error,
      const char* src_file, const int src_line)
      : itti_msg(ASYNC_SHELL_CMD, origin, destination),
        system_command(system_cmd),
        is_abort_on_error(is_abort_on_error),
        src_file(src_file),
        src_line(src_line) {}
  itti_async_shell_cmd(const itti_async_shell_cmd& i)
      : itti_msg(i),
        system_command(i.system_command),
        is_abort_on_error(i.is_abort_on_error),
        src_file(i.src_file),
        src_line(i.src_line) {}
  const char* get_msg_name() { return typeid(itti_msg_ping).name(); };
  std::string system_command;
  bool is_abort_on_error;
  // debug
  std::string src_file;
  int src_line;
};

#endif /* FILE_ITTI_ASYNC_SHELL_CMD_SEEN */
