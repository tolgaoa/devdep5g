/*
 * Copyright (c) 2017 Sprint
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
#include "pid_file.hpp"
#include "smf_app.hpp"
#include "smf_config.hpp"
#include "smf-api-server.h"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#include "smf-http2-server.h"

#include <iostream>
#include <thread>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>  // srand
#include <unistd.h>  // get_pid(), pause()

using namespace smf;
using namespace util;
using namespace std;
using namespace oai::smf_server::api;

itti_mw* itti_inst                    = nullptr;
async_shell_cmd* async_shell_cmd_inst = nullptr;
smf_app* smf_app_inst                 = nullptr;
smf_config smf_cfg;
SMFApiServer* smf_api_server_1     = nullptr;
smf_http2_server* smf_api_server_2 = nullptr;

void send_heartbeat_to_tasks(const uint32_t sequence);

//------------------------------------------------------------------------------
void send_heartbeat_to_tasks(const uint32_t sequence) {
  itti_msg_ping* itti_msg = new itti_msg_ping(TASK_SMF_APP, TASK_ALL, sequence);
  std::shared_ptr<itti_msg_ping> i = std::shared_ptr<itti_msg_ping>(itti_msg);
  int ret                          = itti_inst->send_broadcast_msg(i);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_ALL", i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void my_app_signal_handler(int s) {
  std::cout << "Caught signal " << s << std::endl;
  Logger::system().startup("exiting");
  itti_inst->send_terminate_msg(TASK_SMF_APP);
  itti_inst->wait_tasks_end();
  std::cout << "Freeing Allocated memory..." << std::endl;
  if (async_shell_cmd_inst) delete async_shell_cmd_inst;
  async_shell_cmd_inst = nullptr;
  std::cout << "Async Shell CMD memory done." << std::endl;
  if (smf_api_server_1) {
    smf_api_server_1->shutdown();
    delete smf_api_server_1;
    smf_api_server_1 = nullptr;
  }
  if (smf_api_server_2) {
    smf_api_server_2->stop();
    delete smf_api_server_2;
    smf_api_server_2 = nullptr;
  }
  std::cout << "SMF API Server memory done." << std::endl;
  if (itti_inst) delete itti_inst;
  itti_inst = nullptr;
  std::cout << "ITTI memory done." << std::endl;
  if (smf_app_inst) delete smf_app_inst;
  smf_app_inst = nullptr;
  std::cout << "SMF APP memory done." << std::endl;
  std::cout << "Freeing Allocated memory done" << std::endl;
  exit(0);
}
//------------------------------------------------------------------------------
int main(int argc, char** argv) {
  srand(time(NULL));

  // Command line options
  if (!Options::parse(argc, argv)) {
    std::cout << "Options::parse() failed" << std::endl;
    return 1;
  }

  // Logger
  Logger::init("smf", Options::getlogStdout(), Options::getlogRotFilelog());
  Logger::smf_app().startup("Options parsed");

  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = my_app_signal_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction(SIGINT, &sigIntHandler, NULL);

  // Config
  smf_cfg.load(Options::getlibconfigConfig());
  smf_cfg.display();

  // Inter-task Interface
  itti_inst = new itti_mw();
  itti_inst->start(smf_cfg.itti.itti_timer_sched_params);

  // system command
  async_shell_cmd_inst =
      new async_shell_cmd(smf_cfg.itti.async_cmd_sched_params);

  // SMF application layer
  smf_app_inst = new smf_app(Options::getlibconfigConfig());

  // PID file
  // Currently hard-coded value. TODO: add as config option.
  string pid_file_name = get_exe_absolute_path("/var/run", smf_cfg.instance);
  if (!is_pid_file_lock_success(pid_file_name.c_str())) {
    Logger::smf_app().error("Lock PID file %s failed\n", pid_file_name.c_str());
    exit(-EDEADLK);
  }

  // SMF Pistache API server (HTTP1)
  Pistache::Address addr(
      std::string(inet_ntoa(*((struct in_addr*) &smf_cfg.sbi.addr4))),
      Pistache::Port(smf_cfg.sbi.port));
  smf_api_server_1 = new SMFApiServer(addr, smf_app_inst);
  smf_api_server_1->init(2);
  // smf_api_server_1->start();
  std::thread smf_http1_manager(&SMFApiServer::start, smf_api_server_1);
  // SMF NGHTTP API server (HTTP2)
  smf_api_server_2 = new smf_http2_server(
      conv::toString(smf_cfg.sbi.addr4), smf_cfg.sbi_http2_port, smf_app_inst);
  // smf_api_server_2->start();
  std::thread smf_http2_manager(&smf_http2_server::start, smf_api_server_2);

  smf_http1_manager.join();
  smf_http2_manager.join();

  FILE* fp             = NULL;
  std::string filename = fmt::format("/tmp/smf_{}.status", getpid());
  fp                   = fopen(filename.c_str(), "w+");
  fprintf(fp, "STARTED\n");
  fflush(fp);
  fclose(fp);

  pause();
  return 0;
}
