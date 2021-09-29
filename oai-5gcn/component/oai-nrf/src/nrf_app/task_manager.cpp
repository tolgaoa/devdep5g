/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 *file except in compliance with the License. You may obtain a copy of the
 *License at
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

/*! \file task_manager.cpp
 \brief
 \author
 \company Eurecom
 \date 2020
 \email: Tien-Thinh.Nguyen@eurecom.fr
 */

#include "task_manager.hpp"

#include <unistd.h>
#include <iostream>
#include <thread>

#include "logger.hpp"

using namespace oai::nrf::app;

//------------------------------------------------------------------------------
task_manager::task_manager(nrf_event& ev) : event_sub_(ev) {
  struct itimerspec its;

  sfd = timerfd_create(CLOCK_MONOTONIC, 0);

  /* Start the timer */
  its.it_value.tv_sec     = 0;
  its.it_value.tv_nsec    = 1000 * 1000;
  its.it_interval.tv_sec  = its.it_value.tv_sec;
  its.it_interval.tv_nsec = its.it_value.tv_nsec;

  if (timerfd_settime(sfd, TFD_TIMER_ABSTIME, &its, NULL) == -1) {
    Logger::nrf_app().error("Failed to set timer for task manager");
  }
}

//------------------------------------------------------------------------------
void task_manager::run() {
  manage_tasks();
}

//------------------------------------------------------------------------------
void task_manager::manage_tasks() {
  // starting from current time
  uint64_t t = std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch())
                   .count();

  while (1) {
    event_sub_.task_tick(t);
    t++;
    wait_for_cycle();
  }
}

//------------------------------------------------------------------------------
void task_manager::wait_for_cycle() {
  uint64_t exp;
  ssize_t res;

  if (sfd > 0) {
    res = read(sfd, &exp, sizeof(exp));
    if ((res < 0) || (res != sizeof(exp))) {
      Logger::nrf_app().error("Failed in task manager timer wait");
    }
  }
}
