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
/*! \file itti.hpp
   \brief
   \author  Lionel GAUTHIER
   \date 2018
   \email: lionel.gauthier@eurecom.fr
*/
#ifndef SRC_OAI_ITTI_ITTI_HPP_INCLUDED_
#define SRC_OAI_ITTI_ITTI_HPP_INCLUDED_

#include <chrono>
#include <condition_variable>
//#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <set>
#include <stdint.h>
#include <thread>
#include "itti_msg.hpp"
#include "thread_sched.hpp"

typedef volatile enum task_state_s {
  TASK_STATE_NOT_CONFIGURED,
  TASK_STATE_STARTING,
  TASK_STATE_READY,
  TASK_STATE_ENDED,
  TASK_STATE_MAX,
} task_state_t;

typedef uint32_t timer_id_t;
#define ITTI_INVALID_TIMER_ID (timer_id_t) 0

class itti_timer {
 public:
  itti_timer(
      const timer_id_t id, const task_id_t task_id, const uint32_t interval_sec,
      const uint32_t interval_us, uint64_t arg1_user, uint64_t arg2_user)
      : id(id), task_id(task_id), arg1_user(arg1_user), arg2_user(arg2_user) {
    time_out = std::chrono::system_clock::now() +
               std::chrono::seconds(interval_sec) +
               std::chrono::microseconds(interval_us);
  }
  itti_timer(
      const timer_id_t id, const task_id_t task_id,
      const std::chrono::system_clock::time_point time_out, uint64_t arg1_user,
      uint64_t arg2_user)
      : id(id),
        task_id(task_id),
        time_out(time_out),
        arg1_user(arg1_user),
        arg2_user(arg2_user) {}
  itti_timer(const itti_timer& t)
      : id(t.id),
        task_id(t.task_id),
        time_out(t.time_out),
        arg1_user(t.arg1_user),
        arg2_user(t.arg2_user) {}
  // itti_timer(itti_timer&& t) noexcept : id(std::move(t.id)),
  // task_id(std::move(t.task_id)) , time_out(std::move(t.time_out)) {}

  bool operator<(const itti_timer& t) const { return time_out < t.time_out; }
  ~itti_timer() {}
  timer_id_t id;
  task_id_t task_id;
  std::chrono::system_clock::time_point time_out;
  uint64_t arg1_user;
  uint64_t arg2_user;
};

//------------------------------------------------------------------------------
struct timer_comparator {
  bool operator()(const itti_timer& left, const itti_timer& right) const {
    return (left.time_out < right.time_out);
  }
};

class itti_task_ctxt {
 public:
  explicit itti_task_ctxt(const task_id_t task_id)
      : task_id(task_id),
        m_state(),
        task_state(TASK_STATE_STARTING),
        msg_queue(),
        m_queue(),
        c_queue() {}
  ~itti_task_ctxt() {}

  const task_id_t task_id;
  /*
   * pthread associated with the thread
   */
  // std::thread::id                      thread_id;
  std::thread thread;
  /*
   * State of the thread
   */
  std::mutex m_state;
  volatile task_state_t task_state;

  std::queue<std::shared_ptr<itti_msg>> msg_queue;
  std::mutex m_queue;
  std::condition_variable c_queue;
};

class itti_mw {
 private:
  itti_task_ctxt* itti_task_ctxts[TASK_MAX];

  /*
   * Current message number. Incremented every call to send_msg_to_task
   */
  unsigned long msg_number;
  timer_id_t timer_id;
  std::mutex m_timer_id;
  std::thread timer_thread;

  std::atomic<int> created_tasks;
  std::atomic<int> ready_tasks;

  std::set<itti_timer, timer_comparator> timers;
  itti_timer current_timer;
  std::mutex m_timers;
  std::condition_variable c_timers;

  std::mutex m_timeout;
  std::condition_variable c_timeout;

  bool terminate;

  static void timer_manager_task(const util::thread_sched_params& sched_params);

 public:
  itti_mw();
  itti_mw(itti_mw const&) = delete;
  void operator=(itti_mw const&) = delete;
  ~itti_mw();

  void start(const util::thread_sched_params& sched_params);

  timer_id_t increment_timer_id();
  unsigned int increment_message_number();

  /** \brief Send a broadcast message to every task
   \param message_p Pointer to the message to send
   @returns < 0 on failure, 0 otherwise
   **/
  int send_broadcast_msg(std::shared_ptr<itti_msg> message);

  /** \brief Send a message to a task (could be itself)
   \param message message to send
   @returns -1 on failure, 0 otherwise
   **/
  int send_msg(std::shared_ptr<itti_msg> message);

  /** \brief Retrieves a message in the queue associated to task_id.
   * If the queue is empty, the thread is blocked till a new message arrives.
   \param task_id Task ID of the receiving task
   \param received_msg Pointer to the allocated message
   **/
  std::shared_ptr<itti_msg> receive_msg(task_id_t task_id);

  /** \brief Try to retrieves a message in the queue associated to task_id.
   \param task_id Task ID of the receiving task
   \param received_msg Pointer to the allocated message
   **/
  std::shared_ptr<itti_msg> poll_msg(task_id_t task_id);

  /** \brief Start thread associated to the task
   * \param task_id task to start
   * \param start_routine entry point for the task
   * \param args_p Optional argument to pass to the start routine
   * @returns -1 on failure, 0 otherwise
   **/
  int create_task(
      const task_id_t task_id, void (*start_routine)(void*), void* args_p);

  /** \brief Notify ITTI of a started thread
   * \param task_id of started task
   * \param start_routine entry point for the task
   * \param args_p Optional argument to pass to the start routine
   * @returns -1 on failure, 0 otherwise
   **/
  int notify_task_ready(const task_id_t task_id);

  /** \brief Indicates to ITTI if newly created tasks should wait for all tasks
   *to be ready \param wait_tasks non 0 to make new created tasks to wait, 0 to
   *let created tasks to run
   **/
  // void wait_ready(int wait_tasks);

  /** \brief Mark the task as in ready state
   * \param task_id task to mark as ready
   **/
  // void mark_task_ready(task_id_t task_id);

  /** \brief handle signals and wait for all threads to join when the process
   *complete. This function should be called from the main thread after having
   *created all ITTI tasks.
   **/
  void wait_tasks_end(void);

  /** \brief Send a termination message to all tasks.
   * \param src_task_id task that is broadcasting the message.
   **/
  int send_terminate_msg(task_id_t src_task_id);

  /** \brief Request a new timer
   *  \param interval_sec timer interval in seconds
   *  \param interval_us  timer interval in micro seconds
   *  \param task_id      task id of the task requesting the timer
   *  @returns 0 on failure, timer id otherwise
   **/
  timer_id_t timer_setup(
      uint32_t interval_sec, uint32_t interval_us, task_id_t task_id,
      uint64_t arg1_user = 0, uint64_t arg2_user = 0);

  /** \brief Remove the timer from list
   *  \param timer_id unique timer id
   *  \param task_id task id of the task that requested the timer
   *  @returns -1 on failure, 0 otherwise
   **/
  int timer_remove(timer_id_t timer_id);

  static void signal_handler(int signum);
};

#endif /* SRC_OAI_ITTI_ITTI_HPP_INCLUDED_ */
