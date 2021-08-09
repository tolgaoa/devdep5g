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

#ifndef MM_MSG_H_
#define MM_MSG_H_

#include "mmMsgDef.h"
#include "AuthenticationRequest.h"
#include "AuthenticationResponse.h"
#include "AuthenticationResult.h"
#include "AuthenticationFailure.h"
#include "AuthenticationReject.h"
#include "RegistrationRequest.h"
#include "RegistrationAccept.h"
#include "RegistrationComplete.h"
#include "RegistrationReject.h"
#include "ULNASTransport.h"
#include "DLNASTransport.h"
#include "DeregistrationRequest.h"
#include "ServiceRequest.h"
#include "ServiceAccept.h"
#include "ServiceReject.h"
#include "ConfigurationUpdateCommand.h"
#include "ConfigurationUpdateComplete.h"
#include "IdentityRequest.h"
#include "IdentityResponse.h"
#include "Notification.h"
#include "NotificationResponse.h"
#include "SecurityModeCommand.h"
#include "SecurityModeComplete.h"
#include "SecurityModeReject.h"
#include "_5GMMStatus.h"

/*
 * Structure of EMM plain NAS message
 * ----------------------------------
 */
typedef union {
  mm_msg_header_t header;
  authentication_request_msg authentication_request;
  authentication_response_msg authentication_response;
  authentication_result_msg authentication_result;
  authentication_failure_msg authentication_failure;
  authentication_reject_msg authentication_reject;
  registration_request_msg registration_request;

  registration_accept_msg registration_accept;
  registration_complete_msg registration_complete;
  registration_reject_msg registration_reject;

  deregistration_request_msg deregistration_request;
  service_request_msg service_request;
  service_accept_msg service_accept;
  service_reject_msg service_reject;

  configuration_update_command_msg configuration_update_command;
  configuration_update_complete_msg configuration_update_complete;
  identity_request_msg identity_request;
  identity_response_msg identity_response;

  notification_msg notification;
  notification_response_msg notification_response;
  security_mode_command_msg security_mode_command;
  security_mode_complete_msg security_mode_complete;
  security_mode_reject_msg security_mode_reject;

  _5gmm_status_msg _5gmm_status;
} MM_msg;

int mm_msg_decode(MM_msg* msg, uint8_t* buffer, uint32_t len);

int fivegmm_msg_encode(MM_msg* msg, uint8_t* buffer, uint32_t len);

#endif
