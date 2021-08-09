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

/*! \file amf_module_from_config.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "amf_module_from_config.hpp"

#include <iostream>
#include <string>

#include "logger.hpp"

namespace config {

//------------------------------------------------------------------------------
int amf_modules::load(const std::string& config_file) {
  Logger::amf_app().debug(
      "\nLoad AMF module configuration file (%s)", config_file.c_str());
  Config cfg;
  try {
    cfg.readFile(config_file.c_str());
  } catch (const FileIOException& fioex) {
    Logger::amf_app().error(
        "I/O error while reading file %s - %s", config_file.c_str(),
        fioex.what());
    throw;
  } catch (const ParseException& pex) {
    Logger::amf_app().error(
        "Parse error at %s:%d - %s", pex.getFile(), pex.getLine(),
        pex.getError());
    throw;
  }
  const Setting& root = cfg.getRoot();
  try {
    const Setting& modules = root[MODULES_CONFIG_STRING_AMF_MODULES];
  } catch (const SettingNotFoundException& nfex) {
    Logger::amf_app().error("%s : %s", nfex.what(), nfex.getPath());
    return -1;
  }
  const Setting& modules = root[MODULES_CONFIG_STRING_AMF_MODULES];
  const Setting& msg = modules[MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE];
  int count          = msg.getLength();
  for (int i = 0; i < count; i++) {
    const Setting& item = msg[i];
    std::string typeOfMessage;
    int procedure_code;
    item.lookupValue(
        MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_NAME, msgName);
    item.lookupValue(
        MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_PROCEDURECODE,
        procedure_code);
    item.lookupValue(
        MODULES_CONFIG_STRING_AMF_MODULES_NGAP_MESSAGE_TYPEOFMSG,
        typeOfMessage);
    procedureCode = (Ngap_ProcedureCode_t) procedure_code;
    if (!(typeOfMessage.compare("initialMessage"))) {
      typeOfMsg = Ngap_NGAP_PDU_PR_initiatingMessage;
    } else if (!(typeOfMessage.compare("successfuloutcome"))) {
      typeOfMsg = Ngap_NGAP_PDU_PR_successfulOutcome;
    } else if (!(typeOfMessage.compare("unsuccessfuloutcome"))) {
      typeOfMsg = Ngap_NGAP_PDU_PR_unsuccessfulOutcome;
    } else {
      Logger::config().error("wrong NGAP message configuration");
    }
  }
  return 1;
}

//------------------------------------------------------------------------------
void amf_modules::display() {
  Logger::config().info(
      "============    AMF Registered Modules   ============");
  Logger::config().info("NGAP Message Modules:");
  Logger::config().info(
      "- %s(Procedure code %d, Type of Msg %d)\n", msgName.c_str(),
      procedureCode, typeOfMsg);
}

}  // namespace config
