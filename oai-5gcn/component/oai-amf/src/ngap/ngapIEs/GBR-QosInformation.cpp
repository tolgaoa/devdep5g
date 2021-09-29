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

#include "GBR-QosInformation.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
GBR_QosInformation::GBR_QosInformation() {
  notificationControl     = nullptr;
  maximumPacketLossRateDL = nullptr;
  maximumPacketLossRateUL = nullptr;
  maximumFlowBitRateDL    = 0;
  maximumFlowBitRateUL    = 0;
  guaranteedFlowBitRateDL = 0;
  guaranteedFlowBitRateUL = 0;
}

//------------------------------------------------------------------------------
GBR_QosInformation::~GBR_QosInformation() {}

//------------------------------------------------------------------------------
void GBR_QosInformation::setGBR_QosInformation(
    long m_maximumFlowBitRateDL, long m_maximumFlowBitRateUL,
    long m_guaranteedFlowBitRateDL, long m_guaranteedFlowBitRateUL,
    NotificationControl* m_notificationControl,
    PacketLossRate* m_maximumPacketLossRateDL,
    PacketLossRate* m_maximumPacketLossRateUL) {
  maximumFlowBitRateDL    = m_maximumFlowBitRateDL;
  maximumFlowBitRateUL    = m_maximumFlowBitRateUL;
  guaranteedFlowBitRateDL = m_guaranteedFlowBitRateDL;
  guaranteedFlowBitRateUL = m_guaranteedFlowBitRateUL;

  notificationControl     = m_notificationControl;
  maximumPacketLossRateDL = m_maximumPacketLossRateDL;
  maximumPacketLossRateUL = m_maximumPacketLossRateUL;
}

//------------------------------------------------------------------------------
bool GBR_QosInformation::getGBR_QosInformation(
    long& m_maximumFlowBitRateDL, long& m_maximumFlowBitRateUL,
    long& m_guaranteedFlowBitRateDL, long& m_guaranteedFlowBitRateUL,
    NotificationControl*& m_notificationControl,
    PacketLossRate*& m_maximumPacketLossRateDL,
    PacketLossRate*& m_maximumPacketLossRateUL) {
  m_maximumFlowBitRateDL    = maximumFlowBitRateDL;
  m_maximumFlowBitRateUL    = maximumFlowBitRateUL;
  m_guaranteedFlowBitRateDL = guaranteedFlowBitRateDL;
  m_guaranteedFlowBitRateUL = guaranteedFlowBitRateUL;

  m_notificationControl     = notificationControl;
  m_maximumPacketLossRateDL = maximumPacketLossRateDL;
  m_maximumPacketLossRateUL = maximumPacketLossRateUL;

  return true;
}

//------------------------------------------------------------------------------
bool GBR_QosInformation::encode2GBR_QosInformation(
    Ngap_GBR_QosInformation_t* gBR_QosInformation) {
  gBR_QosInformation->maximumFlowBitRateDL.size = 6;
  gBR_QosInformation->maximumFlowBitRateDL.buf =
      (uint8_t*) calloc(1, gBR_QosInformation->maximumFlowBitRateDL.size);
  if (!gBR_QosInformation->maximumFlowBitRateDL.buf) return false;

  for (int i = 0; i < gBR_QosInformation->maximumFlowBitRateDL.size; i++) {
    gBR_QosInformation->maximumFlowBitRateDL.buf[i] =
        (maximumFlowBitRateDL & (0xff0000000000 >> i * 8)) >>
        ((gBR_QosInformation->maximumFlowBitRateDL.size - i - 1) * 8);
  }

  gBR_QosInformation->maximumFlowBitRateUL.size = 6;
  gBR_QosInformation->maximumFlowBitRateUL.buf =
      (uint8_t*) calloc(1, gBR_QosInformation->maximumFlowBitRateUL.size);
  if (!gBR_QosInformation->maximumFlowBitRateUL.buf) return false;

  for (int i = 0; i < gBR_QosInformation->maximumFlowBitRateUL.size; i++) {
    gBR_QosInformation->maximumFlowBitRateUL.buf[i] =
        (maximumFlowBitRateUL & (0xff0000000000 >> i * 8)) >>
        ((gBR_QosInformation->maximumFlowBitRateUL.size - i - 1) * 8);
  }

  gBR_QosInformation->guaranteedFlowBitRateDL.size = 6;
  gBR_QosInformation->guaranteedFlowBitRateDL.buf =
      (uint8_t*) calloc(1, gBR_QosInformation->guaranteedFlowBitRateDL.size);
  if (!gBR_QosInformation->guaranteedFlowBitRateDL.buf) return false;

  for (int i = 0; i < gBR_QosInformation->guaranteedFlowBitRateDL.size; i++) {
    gBR_QosInformation->guaranteedFlowBitRateDL.buf[i] =
        (guaranteedFlowBitRateDL & (0xff0000000000 >> i * 8)) >>
        ((gBR_QosInformation->guaranteedFlowBitRateDL.size - i - 1) * 8);
  }

  gBR_QosInformation->guaranteedFlowBitRateUL.size = 6;
  gBR_QosInformation->guaranteedFlowBitRateUL.buf =
      (uint8_t*) calloc(1, gBR_QosInformation->guaranteedFlowBitRateUL.size);
  if (!gBR_QosInformation->guaranteedFlowBitRateUL.buf) return false;

  for (int i = 0; i < gBR_QosInformation->guaranteedFlowBitRateUL.size; i++) {
    gBR_QosInformation->guaranteedFlowBitRateUL.buf[i] =
        (guaranteedFlowBitRateUL & (0xff0000000000 >> i * 8)) >>
        ((gBR_QosInformation->guaranteedFlowBitRateUL.size - i - 1) * 8);
  }

  if (notificationControl) {
    Ngap_NotificationControl_t* nc = (Ngap_NotificationControl_t*) calloc(
        1, sizeof(Ngap_NotificationControl_t));
    if (!nc) return false;
    if (!notificationControl->encode2NotificationControl(nc)) return false;
    gBR_QosInformation->notificationControl = nc;
  }
  if (maximumPacketLossRateDL) {
    Ngap_PacketLossRate_t* mplrd =
        (Ngap_PacketLossRate_t*) calloc(1, sizeof(Ngap_PacketLossRate_t));
    if (!mplrd) return false;
    if (!maximumPacketLossRateDL->encode2PacketLossRate(mplrd)) return false;
    gBR_QosInformation->maximumPacketLossRateDL = mplrd;
  }
  if (maximumPacketLossRateUL) {
    Ngap_PacketLossRate_t* mplru =
        (Ngap_PacketLossRate_t*) calloc(1, sizeof(Ngap_PacketLossRate_t));
    if (!mplru) return false;
    if (!maximumPacketLossRateUL->encode2PacketLossRate(mplru)) return false;
    gBR_QosInformation->maximumPacketLossRateUL = mplru;
  }

  return true;
}

//------------------------------------------------------------------------------
bool GBR_QosInformation::decodefromGBR_QosInformation(
    Ngap_GBR_QosInformation_t* gBR_QosInformation) {
  if (!gBR_QosInformation->maximumFlowBitRateDL.buf) return false;
  if (!gBR_QosInformation->maximumFlowBitRateUL.buf) return false;
  if (!gBR_QosInformation->guaranteedFlowBitRateDL.buf) return false;
  if (!gBR_QosInformation->guaranteedFlowBitRateUL.buf) return false;

  maximumFlowBitRateDL    = 0;
  maximumFlowBitRateUL    = 0;
  guaranteedFlowBitRateDL = 0;
  guaranteedFlowBitRateUL = 0;

  for (int i = 0; i < gBR_QosInformation->maximumFlowBitRateDL.size; i++) {
    maximumFlowBitRateDL = maximumFlowBitRateDL << 8;
    maximumFlowBitRateDL |= gBR_QosInformation->maximumFlowBitRateDL.buf[i];
  }
  for (int i = 0; i < gBR_QosInformation->maximumFlowBitRateUL.size; i++) {
    maximumFlowBitRateUL = maximumFlowBitRateUL << 8;
    maximumFlowBitRateUL |= gBR_QosInformation->maximumFlowBitRateUL.buf[i];
  }
  for (int i = 0; i < gBR_QosInformation->guaranteedFlowBitRateDL.size; i++) {
    guaranteedFlowBitRateDL = guaranteedFlowBitRateDL << 8;
    guaranteedFlowBitRateDL |=
        gBR_QosInformation->guaranteedFlowBitRateDL.buf[i];
  }
  for (int i = 0; i < gBR_QosInformation->guaranteedFlowBitRateUL.size; i++) {
    guaranteedFlowBitRateUL = guaranteedFlowBitRateUL << 8;
    guaranteedFlowBitRateUL |=
        gBR_QosInformation->guaranteedFlowBitRateUL.buf[i];
  }

  if (gBR_QosInformation->notificationControl) {
    notificationControl = new NotificationControl();
    if (!notificationControl->decodefromNotificationControl(
            gBR_QosInformation->notificationControl))
      return false;
  }
  if (gBR_QosInformation->maximumPacketLossRateDL) {
    maximumPacketLossRateDL = new PacketLossRate();
    if (!maximumPacketLossRateDL->decodefromPacketLossRate(
            gBR_QosInformation->maximumPacketLossRateDL))
      return false;
  }
  if (gBR_QosInformation->maximumPacketLossRateUL) {
    maximumPacketLossRateUL = new PacketLossRate();
    if (!maximumPacketLossRateUL->decodefromPacketLossRate(
            gBR_QosInformation->maximumPacketLossRateUL))
      return false;
  }

  return true;
}
}  // namespace ngap
