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
/*! \file itti.cpp
   \brief
   \author  Lionel GAUTHIER
   \date 2018
   \email: lionel.gauthier@eurecom.fr
*/
#include "logger.hpp"
#include "itti.hpp"
#include "common_defs.h"
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <signal.h>
#include <time.h>
#include <algorithm>
#include <csignal>

extern itti_mw* itti_inst;

static itti_timer null_timer(
    ITTI_INVALID_TIMER_ID, TASK_NONE, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);

//------------------------------------------------------------------------------
void itti_mw::timer_manager_task(
    const util::thread_sched_params& sched_params) {
  Logger::itti().info("Starting timer_manager_task");
  sched_params.apply(TASK_ITTI_TIMER, Logger::itti());
  while (true) {
    if (itti_inst->terminate) return;
    {
      std::unique_lock<std::mutex> lx(itti_inst->m_timers);
      while (itti_inst->timers.empty()) {
        itti_inst->c_timers.wait(lx);
      }
      std::set<itti_timer>::iterator it = itti_inst->timers.begin();
      itti_inst->current_timer          = std::ref(*it);
      itti_inst->timers.erase(it);
      lx.unlock();

      // check time-out
      if (itti_inst->current_timer.time_out >
          std::chrono::system_clock::now()) {
        std::unique_lock<std::mutex> lto(itti_inst->m_timeout);
        auto diff = itti_inst->current_timer.time_out -
                    std::chrono::system_clock::now();
        auto rc = itti_inst->c_timeout.wait_for(lto, diff);
        lto.unlock();
        if (std::cv_status::timeout == rc) {
          // signal time-out
          itti_msg_timeout mto(
              TASK_ITTI_TIMER, itti_inst->current_timer.task_id,
              itti_inst->current_timer.id, itti_inst->current_timer.arg1_user,
              itti_inst->current_timer.arg2_user);
          std::shared_ptr<itti_msg_timeout> msgsh =
              std::make_shared<itti_msg_timeout>(mto);
          int ret = itti_inst->send_msg(msgsh);
        } else {
          // other timer required ?
          itti_inst->m_timers.lock();
          if (itti_inst->current_timer.id != ITTI_INVALID_TIMER_ID) {
            itti_inst->timers.insert(itti_inst->current_timer);
          }
          itti_inst->current_timer = null_timer;
          itti_inst->m_timers.unlock();
          // cout << "cv.wait released by other triggered timer end" << endl;
        }
      } else {
        // signal time-out
        itti_msg_timeout mto(
            TASK_ITTI_TIMER, itti_inst->current_timer.task_id,
            itti_inst->current_timer.id, itti_inst->current_timer.arg1_user,
            itti_inst->current_timer.arg2_user);
        std::shared_ptr<itti_msg_timeout> msgsh =
            std::make_shared<itti_msg_timeout>(mto);
        itti_inst->send_msg(msgsh);
      }
    }
  }
}
//------------------------------------------------------------------------------
itti_mw::itti_mw()
    : timer_id(0),
      msg_number(0),
      created_tasks(0),
      ready_tasks(0),
      m_timers(),
      current_timer(null_timer),
      m_timeout(),
      m_timer_id(),
      terminate(false) {
  std::fill(itti_task_ctxts, itti_task_ctxts + TASK_MAX, nullptr);
}

//------------------------------------------------------------------------------
itti_mw::~itti_mw() {
  std::cout << "~itti()" << std::endl;
  timer_thread.detach();
  // wake up thread timer if necessary
  std::unique_lock<std::mutex> l2(m_timeout);
  c_timeout.notify_one();

  for (int t = TASK_FIRST; t < TASK_MAX; t++) {
    if (itti_task_ctxts[t]) {
      delete itti_task_ctxts[t];
    }
  }
  std::cout << "~itti() Done!" << std::endl;
}

//------------------------------------------------------------------------------
void itti_mw::start(const util::thread_sched_params& sched_params) {
  Logger::itti().startup("Starting...");
  timer_thread = std::thread(timer_manager_task, sched_params);
  Logger::itti().startup("Started");
}
//------------------------------------------------------------------------------
timer_id_t itti_mw::increment_timer_id() {
  return ++timer_id;
}

//------------------------------------------------------------------------------
unsigned int itti_mw::increment_message_number() {
  return __sync_fetch_and_add(&msg_number, 1);
}

//------------------------------------------------------------------------------
int itti_mw::create_task(
    const task_id_t task_id, void (*start_routine)(void*), void* args_p) {
  if (nullptr == start_routine) {
    Logger::itti().error("Null start routine for task %d", task_id);
    return RETURNerror;
  }
  if ((TASK_FIRST <= task_id) && (TASK_MAX > task_id)) {
    if (itti_task_ctxts[task_id] == nullptr) {
      itti_task_ctxts[task_id] = new itti_task_ctxt(task_id);
      {
        std::unique_lock<std::mutex> lk(itti_task_ctxts[task_id]->m_state);
        if (itti_task_ctxts[task_id]->task_state == TASK_STATE_NOT_CONFIGURED) {
          itti_task_ctxts[task_id]->task_state = TASK_STATE_STARTING;
        }
        created_tasks++;
        lk.unlock();
      }
      itti_task_ctxts[task_id]->thread = std::thread(start_routine, args_p);
      while ((itti_task_ctxts[task_id]->task_state != TASK_STATE_READY) &&
             (itti_task_ctxts[task_id]->task_state != TASK_STATE_ENDED))
        usleep(1000);
      return 0;
    } else {
      Logger::itti().error("Try to start an already started task %d", task_id);
    }
  } else {
    Logger::itti().error("Bad task id %d", task_id);
  }
  return RETURNerror;
}

//------------------------------------------------------------------------------
int itti_mw::notify_task_ready(const task_id_t task_id) {
  if (itti_task_ctxts[task_id]) {
    itti_task_ctxts[task_id]->m_state.lock();
    if (itti_task_ctxts[task_id]->task_state == TASK_STATE_STARTING) {
      itti_task_ctxts[task_id]->task_state = TASK_STATE_READY;
      itti_task_ctxts[task_id]->m_state.unlock();
      return RETURNok;
    }
    itti_task_ctxts[task_id]->m_state.unlock();
    Logger::itti().error(
        "Notify task ready, bad state %d",
        itti_task_ctxts[task_id]->task_state);
  } else {
    Logger::itti().error("Notify task ready, task not starting %d", task_id);
  }
  return RETURNerror;
}

//------------------------------------------------------------------------------
int itti_mw::send_msg(std::shared_ptr<itti_msg> message) {
  if ((TASK_FIRST <= message->destination) &&
      (TASK_MAX > message->destination)) {
    if (itti_task_ctxts[message->destination]) {
      if (itti_task_ctxts[message->destination]->task_state ==
          TASK_STATE_READY) {
        std::unique_lock<std::mutex> l(
            itti_task_ctxts[message->destination]->m_queue);
        // res =
        itti_task_ctxts[message->destination]->msg_queue.push(message);
        itti_task_ctxts[message->destination]->c_queue.notify_one();
        return RETURNok;
      } else if (
          itti_task_ctxts[message->destination]->task_state ==
          TASK_STATE_ENDED) {
        Logger::itti().warn(
            "Unicast message number %lu can not be sent from %d to %d, ended "
            "destination task!",
            message->msg_num, message->origin, message->destination);
        return RETURNerror;
      }
    } else {
      Logger::itti().warn(
          "Unicast message number %lu can not be sent from %d to %d, not "
          "started destination task!",
          message->msg_num, message->origin, message->destination);
    }
  } else if (message->destination == TASK_ALL) {
    return send_broadcast_msg(message);
  }
  return RETURNerror;
}

//------------------------------------------------------------------------------
int itti_mw::send_broadcast_msg(std::shared_ptr<itti_msg> message) {
  if (TASK_ALL == message->destination) {
    for (int t = TASK_FIRST; t < TASK_MAX; t++) {
      if (itti_task_ctxts[t]) {
        if (itti_task_ctxts[t]->task_state == TASK_STATE_READY) {
          std::unique_lock<std::mutex> l(itti_task_ctxts[t]->m_queue);
          itti_task_ctxts[t]->msg_queue.push(message);
          itti_task_ctxts[t]->c_queue.notify_one();
        } else if (itti_task_ctxts[t]->task_state == TASK_STATE_ENDED) {
          Logger::itti().warn(
              "Broadcast message number %lu can not be sent from %d to %d, "
              "ended destination task!",
              message->msg_num, message->origin, t);
        } else {
          Logger::itti().warn(
              "Broadcast message number %lu can not be sent from %d to %d, "
              "unknown state %d !",
              message->msg_num, message->origin, t,
              itti_task_ctxts[t]->task_state);
        }
      }
    }
    return RETURNok;
  }
  return RETURNerror;
}

//------------------------------------------------------------------------------
int itti_mw::send_terminate_msg(task_id_t src_task_id) {
  itti_msg_terminate msg(src_task_id, TASK_ALL);
  terminate = true;
  std::shared_ptr<itti_msg_terminate> smsg =
      std::make_shared<itti_msg_terminate>(msg);
  int ret = itti_inst->send_broadcast_msg(smsg);

  return ret;
}
//------------------------------------------------------------------------------
std::shared_ptr<itti_msg> itti_mw::receive_msg(task_id_t task_id) {
  if ((TASK_FIRST <= task_id) && (TASK_MAX > task_id)) {
    if (itti_task_ctxts[task_id]) {
      std::unique_lock<std::mutex> lk(itti_task_ctxts[task_id]->m_queue);
      while (itti_task_ctxts[task_id]->msg_queue.empty()) {
        itti_task_ctxts[task_id]->c_queue.wait(lk);
      }
      std::shared_ptr<itti_msg> msg =
          itti_task_ctxts[task_id]->msg_queue.front();
      itti_task_ctxts[task_id]->msg_queue.pop();
      return msg;
    }
  }
  Logger::itti().warn("received message failed, bad task id");
  return nullptr;
}

//------------------------------------------------------------------------------
std::shared_ptr<itti_msg> itti_mw::poll_msg(task_id_t task_id) {
  if ((TASK_FIRST <= task_id) && (TASK_MAX > task_id)) {
    if (itti_task_ctxts[task_id]) {
      std::lock_guard<std::mutex> lk(itti_task_ctxts[task_id]->m_queue);
      if (!itti_task_ctxts[task_id]->msg_queue.empty()) {
        std::shared_ptr<itti_msg> msg =
            itti_task_ctxts[task_id]->msg_queue.front();
        itti_task_ctxts[task_id]->msg_queue.pop();
        return msg;
      }
    }
  }
  return nullptr;
}

//------------------------------------------------------------------------------
void itti_mw::wait_tasks_end(void) {
  Logger::itti().info("Waiting ITTI tasks closed");
  for (int task_idx = TASK_FIRST; task_idx < TASK_MAX; task_idx++) {
    if (itti_task_ctxts[task_idx]) {
      if (itti_task_ctxts[task_idx]->thread.joinable()) {
        itti_task_ctxts[task_idx]->thread.join();
        itti_task_ctxts[task_idx]->m_state.lock();
        itti_task_ctxts[task_idx]->task_state = TASK_STATE_ENDED;
        itti_task_ctxts[task_idx]->m_state.unlock();
      }
    }
  }
  Logger::itti().info("All ITTI tasks closed");
}

//------------------------------------------------------------------------------
timer_id_t itti_mw::timer_setup(
    uint32_t interval_sec, uint32_t interval_us, task_id_t task_id,
    uint64_t arg1_user, uint64_t arg2_user) {
  // Not sending to task timer
  if ((TASK_FIRST < task_id) && (TASK_MAX > task_id)) {
    itti_timer t(
        increment_timer_id(), task_id, interval_sec, interval_us, arg1_user,
        arg2_user);
    timer_id_t id = t.id;
    std::unique_lock<std::mutex> l(m_timers);
    timers.insert(t);
    c_timers.notify_one();

    // wake up thread timer if necessary
    std::unique_lock<std::mutex> l2(m_timeout);
    c_timeout.notify_one();
    return id;
  }
  return ITTI_INVALID_TIMER_ID;
}

//------------------------------------------------------------------------------
int itti_mw::timer_remove(timer_id_t timer_id) {
  std::lock_guard<std::mutex> lk(m_timers);
  if (current_timer.id == timer_id) {
    // cout << "timer_remove() current_timer.id == timer_id" << endl;
    current_timer = null_timer;
    c_timers.notify_one();
    // wake up thread timer if necessary
    std::unique_lock<std::mutex> l2(m_timeout);
    c_timeout.notify_one();
    return RETURNok;
  } else {
    std::set<itti_timer>::iterator it = itti_inst->timers.begin();
    while (it != itti_inst->timers.end()) {
      if (it->id == timer_id) {
        itti_inst->timers.erase(it);
        return RETURNok;
      }
      ++it;
    }
  }
  Logger::itti().trace("Removing timer 0x%lx: Not found", timer_id);
  return RETURNerror;
}
