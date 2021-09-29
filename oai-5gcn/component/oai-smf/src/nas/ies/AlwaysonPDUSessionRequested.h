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

#ifndef _ALWAYSONPDUSESSIONREQUESTED_H_
#define _ALWAYSONPDUSESSIONREQUESTED_H_

#include <stdint.h>
#include <stdbool.h>
#include "bstrlib.h"

#define ALWAYSON_PDU_SESSION_REQUESTED_MINIMUM_LENGTH 1
#define ALWAYSON_PDU_SESSION_REQUESTED_MAXIMUM_LENGTH 1

#define ALWAYSON_PDU_SESSION_NOT_REQUESTED 0
#define ALWAYSON_PDU_SESSION_REQUESTED 1

typedef struct {
  bool apsr_requested;
} AlwaysonPDUSessionRequested;

int encode_alwayson_pdu_session_requested(
    AlwaysonPDUSessionRequested alwaysonpdusessionrequested, uint8_t iei,
    uint8_t* buffer, uint32_t len);
int decode_alwayson_pdu_session_requested(
    AlwaysonPDUSessionRequested* alwaysonpdusessionrequested, uint8_t iei,
    uint8_t* buffer, uint32_t len);

#endif
