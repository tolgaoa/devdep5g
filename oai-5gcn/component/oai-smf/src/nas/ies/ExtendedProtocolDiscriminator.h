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

#ifndef EXTENDED_PROTOCOL_DISCRIMINATOR_H_
#define EXTENDED_PROTOCOL_DISCRIMINATOR_H_

#include <stdint.h>
#include "bstrlib.h"

#define EXTENDED_PROTOCOL_DISCRIMINATOR_MINIMUM_LENGTH 1
#define EXTENDED_PROTOCOL_DISCRIMINATOR_MAXIMUM_LENGTH 1

typedef uint8_t ExtendedProtocolDiscriminator;

int encode_extended_protocol_discriminator(
    ExtendedProtocolDiscriminator extendedprotocoldiscriminator, uint8_t iei,
    uint8_t* buffer, uint32_t len);
int decode_extended_protocol_discriminator(
    ExtendedProtocolDiscriminator* extendedprotocoldiscriminator, uint8_t iei,
    uint8_t* buffer, uint32_t len);

#endif
