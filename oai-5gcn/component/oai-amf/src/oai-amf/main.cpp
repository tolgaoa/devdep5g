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

#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
#include <thread>

#include "AMFApiServer.hpp"
#include "amf_app.hpp"
#include "amf_config.hpp"
#include "amf_module_from_config.hpp"
#include "amf_statistics.hpp"
#include "itti.hpp"
#include "logger.hpp"
#include "ngap_app.hpp"
#include "normalizer.hh"
#include "options.hpp"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"

extern void hexStr2Byte(const char* src, unsigned char* dest, int len);
extern void print_buffer(
    const std::string app, const std::string commit, uint8_t* buf, int len);
extern void ue_gnb_simulator();

using namespace config;
using namespace amf_application;

amf_config amf_cfg;
amf_modules modules;
itti_mw* itti_inst    = nullptr;
amf_app* amf_app_inst = nullptr;
statistics stacs;

//------------------------------------------------------------------------------
int main(int argc, char** argv) {
  srand(time(NULL));

  if (!Options::parse(argc, argv)) {
    std::cout << "Options::parse() failed" << std::endl;
    return 1;
  }

  Logger::init("AMF", Options::getlogStdout(), Options::getlogRotFilelog());
  Logger::amf_app().startup("Options parsed!");

  amf_cfg.load(Options::getlibconfigConfig());
  amf_cfg.display();
  modules.load(Options::getlibconfigConfig());
  modules.display();

  itti_inst = new itti_mw();
  itti_inst->start(amf_cfg.itti.itti_timer_sched_params);

  amf_app_inst = new amf_app(amf_cfg);
  amf_app_inst->allRegistredModulesInit(modules);

  Logger::amf_app().debug("Initiating AMF server endpoints");
  Pistache::Address addr(
      std::string(inet_ntoa(*((struct in_addr*) &amf_cfg.n11.addr4))),
      Pistache::Port(amf_cfg.n11.port));
  AMFApiServer amfApiServer(addr, amf_app_inst);
  amfApiServer.init(2);
  std::thread amf_api_manager(&AMFApiServer::start, amfApiServer);

  Logger::amf_app().debug("Initiating Done!");
  pause();
  return 0;
}
