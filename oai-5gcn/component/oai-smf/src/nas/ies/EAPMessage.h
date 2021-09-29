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

#ifndef _EAPMESSAGE_H_
#define _EAPMESSAGE_H_

#include <stdint.h>
#include "bstrlib.h"

#define EAP_MESSAGE_MINIMUM_LENGTH 7
#define EAP_MESSAGE_MAXIMUM_LENGTH 1503

#define EAP_MESSAGE_MINIMUM_LENGTH_TLVE 7
#define EAP_MESSAGE_MAXIMUM_LENGTH_TLVE 1503
#define EAP_MESSAGE_MINIMUM_LENGTH_LVE 6
#define EAP_MESSAGE_MAXIMUM_LENGTH_LVE 1502

typedef bstring EAPMessage;

int encode_eap_message(
    EAPMessage eapmessage, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_eap_message(
    EAPMessage* eapmessage, uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
