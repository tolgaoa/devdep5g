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

#ifndef _GPRSTIMER_H_
#define _GPRSTIMER_H_

#include <stdint.h>
#include "bstrlib.h"

#define GPRS_TIMER_MINIMUM_LENGTH 2
#define GPRS_TIMER_MAXIMUM_LENGTH 2

#define GPRS_TIMER_MINIMUM_LENGTH_TV 2
#define GPRS_TIMER_MAXIMUM_LENGTH_TV 2

#define GPRSTIMER_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_2_SECONDS 0b000
#define GPRSTIMER_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1_MINUTE 0b001
#define GPRSTIMER_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_DECIHOURS 0b010
#define GPRSTIMER_VALUE_INDICATES_THAT_THE_TIMER_IS_DEACTIVATED 0b111

typedef struct {
  uint8_t unit : 3;
  uint8_t timeValue : 5;
} GPRSTimer;

int encode_gprs_timer(
    GPRSTimer gprstimer, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_gprs_timer(
    GPRSTimer* gprstimer, uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
