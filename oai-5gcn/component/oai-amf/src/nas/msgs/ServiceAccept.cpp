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

/*! \file
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "ServiceAccept.hpp"

#include "3gpp_ts24501.hpp"
#include "logger.hpp"
using namespace nas;

//------------------------------------------------------------------------------
ServiceAccept::ServiceAccept() {
  plain_header                   = NULL;
  ie_PDU_session_status          = NULL;
  ie_session_reactivation_result = NULL;
}

ServiceAccept::~ServiceAccept() {
  if (plain_header) delete plain_header;
  if (ie_PDU_session_status) delete ie_PDU_session_status;
  if (ie_session_reactivation_result) delete ie_session_reactivation_result;
}
//------------------------------------------------------------------------------
void ServiceAccept::setHeader(uint8_t security_header_type) {
  plain_header = new NasMmPlainHeader();
  plain_header->setHeader(EPD_5GS_MM_MSG, security_header_type, SERVICE_ACCEPT);
}

//------------------------------------------------------------------------------
void ServiceAccept::setPDU_session_status(uint16_t value) {
  ie_PDU_session_status = new PDU_Session_Status(0x50, value);
}

//------------------------------------------------------------------------------
void ServiceAccept::setPDU_session_reactivation_result(uint16_t value) {
  ie_session_reactivation_result =
      new PDU_Session_Reactivation_Result(0x26, value);
}

//------------------------------------------------------------------------------
int ServiceAccept::encode2buffer(uint8_t* buf, int len) {
  if (!plain_header) {
    Logger::nas_mm().error("Missing message header");
    return -1;
  }
  int encoded_size = 0;
  if (!(plain_header->encode2buffer(buf, len))) return -1;
  encoded_size += 3;
  if (ie_PDU_session_status)
    encoded_size += ie_PDU_session_status->encode2buffer(
        buf + encoded_size, len - encoded_size);
  if (ie_session_reactivation_result)
    encoded_size += ie_session_reactivation_result->encode2buffer(
        buf + encoded_size, len - encoded_size);
  return encoded_size;
}
