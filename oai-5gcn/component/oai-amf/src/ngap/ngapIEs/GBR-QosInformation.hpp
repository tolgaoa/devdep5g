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

#ifndef _GBR_QOSINFORMATION_H_
#define _GBR_QOSINFORMATION_H_

#include "NotificationControl.hpp"
#include "PacketLossRate.hpp"

extern "C" {
#include "Ngap_GBR-QosInformation.h"
}

namespace ngap {

class GBR_QosInformation {
 public:
  GBR_QosInformation();
  virtual ~GBR_QosInformation();

  void setGBR_QosInformation(
      long m_maximumFlowBitRateDL, long m_maximumFlowBitRateUL,
      long m_guaranteedFlowBitRateDL, long m_guaranteedFlowBitRateUL,
      NotificationControl* m_notificationControl,
      PacketLossRate* m_maximumPacketLossRateDL,
      PacketLossRate* m_maximumPacketLossRateUL);
  bool getGBR_QosInformation(
      long& m_maximumFlowBitRateDL, long& m_maximumFlowBitRateUL,
      long& m_guaranteedFlowBitRateDL, long& m_guaranteedFlowBitRateUL,
      NotificationControl*& m_notificationControl,
      PacketLossRate*& m_maximumPacketLossRateDL,
      PacketLossRate*& m_maximumPacketLossRateUL);

  bool encode2GBR_QosInformation(Ngap_GBR_QosInformation_t*);
  bool decodefromGBR_QosInformation(Ngap_GBR_QosInformation_t*);

 private:
  long maximumFlowBitRateDL;
  long maximumFlowBitRateUL;
  long guaranteedFlowBitRateDL;
  long guaranteedFlowBitRateUL;
  NotificationControl* notificationControl; /* OPTIONAL */
  PacketLossRate* maximumPacketLossRateDL;  /* OPTIONAL */
  PacketLossRate* maximumPacketLossRateUL;  /* OPTIONAL */
};
}  // namespace ngap

#endif
