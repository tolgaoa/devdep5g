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

/*! \file amf_module_from_config.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#ifndef _AMF_MODULE_FROM_CONFIG_H_
#define _AMF_MODULE_FROM_CONFIG_H_

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <libconfig.h++>

#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProcedureCode.h"

#define MODULES_CONFIG_STRING_AMF_MODULES "MODULES"
#define MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE "NGAP_MESSAGE"
#define MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_NAME "MSG_NAME"
#define MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_PROCEDURECODE           \
  "ProcedureCode"
#define MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_TYPEOFMSG "TypeOfMessage"

using namespace libconfig;

namespace config {

class amf_modules {
 public:
  int load(const std::string& config_file);
  void display();
  void makeModulesAlive();

 private:
  std::string msgName;  // vector to store more msgs
  Ngap_NGAP_PDU_PR typeOfMsg;
  Ngap_ProcedureCode_t procedureCode;
  //  NGSetupRequestMsg *ngSetupRequest;
};
}  // namespace config

#endif
