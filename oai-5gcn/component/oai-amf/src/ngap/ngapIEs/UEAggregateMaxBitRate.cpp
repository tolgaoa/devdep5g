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

#include "UEAggregateMaxBitRate.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
UEAggregateMaxBitRate::UEAggregateMaxBitRate() {
  ueaggregatemaxbitratedl = 0;
  ueaggregatemaxbitrateul = 0;
}

//------------------------------------------------------------------------------
UEAggregateMaxBitRate::~UEAggregateMaxBitRate() {}

//------------------------------------------------------------------------------
void UEAggregateMaxBitRate::setUEAggregateMaxBitRate(
    long bit_rate_downlink, long bit_rate_uplink) {
  ueaggregatemaxbitratedl = bit_rate_downlink;
  ueaggregatemaxbitrateul = bit_rate_uplink;
}

//------------------------------------------------------------------------------
bool UEAggregateMaxBitRate::getUEAggregateMaxBitRate(
    long& bit_rate_downlink, long& bit_rate_uplink) {
  bit_rate_downlink = ueaggregatemaxbitratedl;
  bit_rate_uplink   = ueaggregatemaxbitrateul;
  return true;
}

//------------------------------------------------------------------------------
bool UEAggregateMaxBitRate::encode2UEAggregateMaxBitRate(
    Ngap_UEAggregateMaximumBitRate_t& ueAggregateMaxBitRate) {
  ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.size = 4;
  ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.buf  = (uint8_t*) calloc(
      1, ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.size);
  if (!ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.buf) return false;

  for (int i = 0; i < ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.size;
       i++) {
    ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.buf[i] =
        (ueaggregatemaxbitratedl & (0xff000000 >> i * 8)) >>
        ((ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.size - i - 1) * 8);
  }

  ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.size = 4;
  ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.buf  = (uint8_t*) calloc(
      1, ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.size);
  if (!ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.buf) return false;

  for (int i = 0; i < ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.size;
       i++) {
    ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.buf[i] =
        (ueaggregatemaxbitrateul & (0xff000000 >> i * 8)) >>
        ((ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.size - i - 1) * 8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool UEAggregateMaxBitRate::decodefromUEAggregateMaxBitRate(
    Ngap_UEAggregateMaximumBitRate_t& ueAggregateMaxBitRate) {
  if (!ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.buf) return false;
  if (!ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.buf) return false;

  ueaggregatemaxbitratedl = 0;
  ueaggregatemaxbitrateul = 0;

  for (int i = 0; i < ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.size;
       i++) {
    ueaggregatemaxbitratedl = ueaggregatemaxbitratedl << 8;
    ueaggregatemaxbitratedl |=
        ueAggregateMaxBitRate.uEAggregateMaximumBitRateDL.buf[i];
  }
  for (int i = 0; i < ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.size;
       i++) {
    ueaggregatemaxbitrateul = ueaggregatemaxbitrateul << 8;
    ueaggregatemaxbitrateul |=
        ueAggregateMaxBitRate.uEAggregateMaximumBitRateUL.buf[i];
  }

  return true;
}

}  // namespace ngap
