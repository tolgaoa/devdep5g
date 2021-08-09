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

#include "PduSessionAggregateMaximumBitRate.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PduSessionAggregateMaximumBitRate::PduSessionAggregateMaximumBitRate() {
  aggregatemaxbitratedl = 0;
  aggregatemaxbitrateul = 0;
}

//------------------------------------------------------------------------------
PduSessionAggregateMaximumBitRate::~PduSessionAggregateMaximumBitRate() {}

//------------------------------------------------------------------------------
void PduSessionAggregateMaximumBitRate::setPduSessionAggregateMaximumBitRate(
    long bit_rate_downlink, long bit_rate_uplink) {
  aggregatemaxbitratedl = bit_rate_downlink;
  aggregatemaxbitrateul = bit_rate_uplink;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::getPduSessionAggregateMaximumBitRate(
    long& bit_rate_downlink, long& bit_rate_uplink) {
  bit_rate_downlink = aggregatemaxbitratedl;
  bit_rate_uplink   = aggregatemaxbitrateul;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::
    encode2PduSessionAggregateMaximumBitRate(
        Ngap_PDUSessionAggregateMaximumBitRate_t&
            pdusessionAggregateMaxBitRate) {
  pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL.size = 6;
  pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL
      .buf = (uint8_t*) calloc(
      1,
      pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL.size);
  if (!pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL.buf)
    return false;

  for (int i = 0;
       i <
       pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL.size;
       i++) {
    pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL.buf[i] =
        (aggregatemaxbitratedl & (0xff0000000000 >> i * 8)) >>
        ((pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL
              .size -
          i - 1) *
         8);
  }

  pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL.size = 6;
  pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL
      .buf = (uint8_t*) calloc(
      1,
      pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL.size);
  if (!pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL.buf)
    return false;

  for (int i = 0;
       i <
       pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL.size;
       i++) {
    pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL.buf[i] =
        (aggregatemaxbitrateul & (0xff0000000000 >> i * 8)) >>
        ((pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL
              .size -
          i - 1) *
         8);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionAggregateMaximumBitRate::
    decodefromPduSessionAggregateMaximumBitRate(
        Ngap_PDUSessionAggregateMaximumBitRate_t&
            pdusessionAggregateMaxBitRate) {
  if (!pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL.buf)
    return false;
  if (!pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL.buf)
    return false;

  aggregatemaxbitratedl = 0;
  aggregatemaxbitrateul = 0;

  for (int i = 0;
       i <
       pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateDL.size;
       i++) {
    aggregatemaxbitratedl = aggregatemaxbitratedl << 8;
    aggregatemaxbitratedl |= pdusessionAggregateMaxBitRate
                                 .pDUSessionAggregateMaximumBitRateDL.buf[i];
  }
  for (int i = 0;
       i <
       pdusessionAggregateMaxBitRate.pDUSessionAggregateMaximumBitRateUL.size;
       i++) {
    aggregatemaxbitrateul = aggregatemaxbitrateul << 8;
    aggregatemaxbitrateul |= pdusessionAggregateMaxBitRate
                                 .pDUSessionAggregateMaximumBitRateUL.buf[i];
  }

  return true;
}

}  // namespace ngap
