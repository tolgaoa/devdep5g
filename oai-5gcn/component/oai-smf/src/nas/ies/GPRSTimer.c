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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "TLVEncoder.h"
#include "TLVDecoder.h"
#include "GPRSTimer.h"

int encode_gprs_timer(
    GPRSTimer gprstimer, uint8_t iei, uint8_t* buffer, uint32_t len) {
  uint32_t encoded  = 0;
  uint8_t timeValue = 0;
  CHECK_PDU_POINTER_AND_LENGTH_ENCODER(
      buffer,
      ((iei > 0) ? GPRS_TIMER_MINIMUM_LENGTH_TV :
                   GPRS_TIMER_MINIMUM_LENGTH_TV - 1),
      len);

  if (iei > 0) {
    *buffer = iei;
    encoded++;
  }

  timeValue = (gprstimer.unit << 5) | gprstimer.timeValue;
  ENCODE_U8(buffer + encoded, timeValue, encoded);

  return encoded;
}

int decode_gprs_timer(
    GPRSTimer* gprstimer, uint8_t iei, uint8_t* buffer, uint32_t len) {
  int decoded       = 0;
  uint8_t timeValue = 0;

  if (iei > 0) {
    CHECK_IEI_DECODER(iei, *buffer);
    decoded++;
  }

  DECODE_U8(buffer + decoded, timeValue, decoded);
  gprstimer->unit      = (uint8_t)((timeValue & 0xe0) >> 5);
  gprstimer->timeValue = (uint8_t)(timeValue & 0x1f);

  return decoded;
}
