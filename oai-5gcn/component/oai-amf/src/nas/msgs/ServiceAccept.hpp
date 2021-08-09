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
#ifndef _SERVICE_ACCEPT_H_

#define _SERVICE_ACCEPT_H_

#include <stdint.h>

#include <string>

#include "nas_ie_header.hpp"

namespace nas {

class ServiceAccept {
 public:
  ServiceAccept();
  ~ServiceAccept();

 public:
  void setHeader(uint8_t security_header_type);
  void setPDU_session_status(uint16_t value);
  void setPDU_session_reactivation_result(uint16_t);
  int encode2buffer(uint8_t* buf, int len);

 private:
  NasMmPlainHeader* plain_header;
  PDU_Session_Status* ie_PDU_session_status;
  PDU_Session_Reactivation_Result* ie_session_reactivation_result;
};

}  // namespace nas

#endif
