/*
 * Copyright (c) 2019 EURECOM
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "async_shell_cmd.hpp"
#include "common_defs.h"
#include "itti.hpp"
#include "logger.hpp"
#include "options.hpp"
#include "pfcp_switch.hpp"
#include "pid_file.hpp"
#include "spgwu_app.hpp"
#include "spgwu_config.hpp"

#include <boost/asio.hpp>
#include <iostream>
#include <algorithm>
#include <thread>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>  // get_pid(), pause()
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace spgwu;
using namespace util;
using namespace std;

itti_mw* itti_inst                    = nullptr;
async_shell_cmd* async_shell_cmd_inst = nullptr;
pfcp_switch* pfcp_switch_inst         = nullptr;
spgwu_app* spgwu_app_inst             = nullptr;
spgwu_config spgwu_cfg;
boost::asio::io_service io_service;

//------------------------------------------------------------------------------
void my_app_signal_handler(int s) {
  std::cout << "Caught signal " << s << std::endl;
  Logger::system().startup("exiting");
  itti_inst->send_terminate_msg(TASK_SPGWU_APP);
  itti_inst->wait_tasks_end();
  std::cout << "Freeing Allocated memory..." << std::endl;
  if (async_shell_cmd_inst) delete async_shell_cmd_inst;
  async_shell_cmd_inst = nullptr;
  std::cout << "Async Shell CMD memory done." << std::endl;
  if (itti_inst) delete itti_inst;
  itti_inst = nullptr;
  std::cout << "ITTI memory done." << std::endl;
  if (spgwu_app_inst) delete spgwu_app_inst;
  spgwu_app_inst = nullptr;
  std::cout << "SPGW-U APP memory done." << std::endl;
  std::cout << "Freeing Allocated memory done" << std::endl;
  exit(0);
}
//------------------------------------------------------------------------------
// We are doing a check to see if an existing process already runs this program.
// We have seen that running at least twice this program in a container may lead
// to the container host to crash.
int my_check_redundant_process(char* exec_name) {
  FILE* fp;
  char* cmd = new char[200];
  std::vector<std::string> words;
  int result     = 0;
  size_t retSize = 0;

  // Retrieving only the executable name
  boost::split(words, exec_name, boost::is_any_of("/"));
  memset(cmd, 1, 200);
  sprintf(
      cmd, "ps aux | grep -v grep | grep -v nohup | grep -c %s",
      words[words.size() - 1].c_str());
  fp = popen(cmd, "r");

  // clearing the buffer
  memset(cmd, 1, 200);
  retSize = fread(cmd, 1, 200, fp);
  fclose(fp);

  // if something wrong, then we don't know
  if (retSize == 0) {
    delete[] cmd;
    return 10;
  }

  result = atoi(cmd);
  delete[] cmd;
  return result;
}
//------------------------------------------------------------------------------
int main(int argc, char** argv) {
  // Checking if another instance of SPGW-U is running
  int nb_processes = my_check_redundant_process(argv[0]);
  if (nb_processes > 1) {
    std::cout << "An instance of " << argv[0] << "is maybe already called!"
              << std::endl;
    return -1;
  }

  // Command line options
  if (!Options::parse(argc, argv)) {
    std::cout << "Options::parse() failed" << std::endl;
    return 1;
  }

  // Logger
  Logger::init("spgwu", Options::getlogStdout(), Options::getlogRotFilelog());

  Logger::spgwu_app().startup("Options parsed");

  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = my_app_signal_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Config
  spgwu_cfg.load(Options::getlibconfigConfig());
  spgwu_cfg.display();

  // Inter task Interface
  itti_inst = new itti_mw();
  itti_inst->start(spgwu_cfg.itti.itti_timer_sched_params);

  // system command
  async_shell_cmd_inst =
      new async_shell_cmd(spgwu_cfg.itti.async_cmd_sched_params);

  // PGW application layer
  spgwu_app_inst = new spgwu_app(Options::getlibconfigConfig());

  // PID file
  // Currently hard-coded value. TODO: add as config option.
  string pid_file_name = get_exe_absolute_path("/var/run", spgwu_cfg.instance);
  if (!is_pid_file_lock_success(pid_file_name.c_str())) {
    Logger::spgwu_app().error(
        "Lock PID file %s failed\n", pid_file_name.c_str());
    exit(-EDEADLK);
  }

  FILE* fp             = NULL;
  std::string filename = fmt::format("/tmp/spgwu_{}.status", getpid());
  fp                   = fopen(filename.c_str(), "w+");
  fprintf(fp, "STARTED\n");
  fflush(fp);
  fclose(fp);

  // once all udp servers initialized
  io_service.run();

  pause();
  return 0;
}
