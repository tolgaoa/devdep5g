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

/*! \file mysql_db.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _MYSQL_DB_HANDLERS_H_
#define _MYSQL_DB_HANDLERS_H_

#include <mysql/mysql.h>
#include <pthread.h>

#include <string>

#define KEY_LENGTH (16)
#define SQN_LENGTH (6)
#define RAND_LENGTH (16)

typedef struct {
  uint8_t key[KEY_LENGTH];
  uint8_t sqn[SQN_LENGTH];
  uint8_t opc[KEY_LENGTH];
  uint8_t rand[RAND_LENGTH];
} mysql_auth_info_t;

typedef struct {
  // mysql reference connector object
  MYSQL* db_conn;
  std::string server;
  std::string user;
  std::string password;
  std::string database;

  pthread_mutex_t db_cs_mutex;
} database_t;

#endif
