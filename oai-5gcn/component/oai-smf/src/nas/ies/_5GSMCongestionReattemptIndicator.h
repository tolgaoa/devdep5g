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

#ifndef __5GSMCONGESTIONREATTEMPTINDICATOR_H_
#define __5GSMCONGESTIONREATTEMPTINDICATOR_H_

#include <stdint.h>

#define _5GSM_CONGESTION_REATTEMPT_INDICATOR_MINIMUM_LENGTH 3
#define _5GSM_CONGESTION_REATTEMPT_INDICATOR_MAXIMUM_LENGTH 3

#define _5GSM_CONGESTION_REATTEMPT_INDICATOR_MINIMUM_LENGTH_TLV 3
#define _5GSM_CONGESTION_REATTEMPT_INDICATOR_MAXIMUM_LENGTH_TLV 3

#define THE_BACKOFF_TIMER_IS_APPLIED_IN_THE_REGISTERED_PLMN 0
#define THE_BACKOFF_TIMER_IS_APPLIED_IN_ALL_PLMNS 1

typedef struct {
  uint8_t abo : 1;
} _5GSMCongestionReattemptIndicator;

int encode__5gsm_congestion_reattempt_indicator(
    _5GSMCongestionReattemptIndicator _5gsmcongestionreattemptindicator,
    uint8_t iei, uint8_t* buffer, uint32_t len);
int decode__5gsm_congestion_reattempt_indicator(
    _5GSMCongestionReattemptIndicator* _5gsmcongestionreattemptindicator,
    uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
