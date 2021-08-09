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

#include "UPTransportLayerInformation.hpp"

extern "C" {
#include "Ngap_GTPTunnel.h"
#include "dynamic_memory_check.h"
}

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
UpTransportLayerInformation::UpTransportLayerInformation() {
  transportLayerAddress = NULL;
  gtpTeid               = NULL;
}

//------------------------------------------------------------------------------
UpTransportLayerInformation::~UpTransportLayerInformation() {}

//------------------------------------------------------------------------------
void UpTransportLayerInformation::setUpTransportLayerInformation(
    TransportLayerAddress* m_transportLayerAddress, GtpTeid* m_gtpTeid) {
  transportLayerAddress = m_transportLayerAddress;
  gtpTeid               = m_gtpTeid;
}

//------------------------------------------------------------------------------
bool UpTransportLayerInformation::getUpTransportLayerInformation(
    TransportLayerAddress*& m_transportLayerAddress, GtpTeid*& m_gtpTeid) {
  m_transportLayerAddress = transportLayerAddress;
  m_gtpTeid               = gtpTeid;

  return true;
}

//------------------------------------------------------------------------------
bool UpTransportLayerInformation::encode2UpTransportLayerInformation(
    Ngap_UPTransportLayerInformation_t& upTransportLayerInfo) {
  upTransportLayerInfo.present = Ngap_UPTransportLayerInformation_PR_gTPTunnel;
  Ngap_GTPTunnel_t* gtptunnel =
      (Ngap_GTPTunnel_t*) calloc(1, sizeof(Ngap_GTPTunnel_t));
  if (!gtptunnel) return false;
  if (!transportLayerAddress->encode2TransportLayerAddress(
          gtptunnel->transportLayerAddress)) {
    free_wrapper((void**) &gtptunnel);
    return false;
  }

  if (!gtpTeid->encode2GtpTeid(gtptunnel->gTP_TEID)) {
    free_wrapper((void**) &gtptunnel);
    return false;
  }
  upTransportLayerInfo.choice.gTPTunnel = gtptunnel;
  return true;
}

//------------------------------------------------------------------------------
bool UpTransportLayerInformation::decodefromUpTransportLayerInformation(
    Ngap_UPTransportLayerInformation_t& upTransportLayerInfo) {
  if (upTransportLayerInfo.present !=
      Ngap_UPTransportLayerInformation_PR_gTPTunnel)
    return false;
  if (!upTransportLayerInfo.choice.gTPTunnel) return false;
  transportLayerAddress = new TransportLayerAddress();
  gtpTeid               = new GtpTeid();

  if (!transportLayerAddress->decodefromTransportLayerAddress(
          upTransportLayerInfo.choice.gTPTunnel->transportLayerAddress))
    return false;
  if (!gtpTeid->decodefromGtpTeid(
          upTransportLayerInfo.choice.gTPTunnel->gTP_TEID))
    return false;

  return true;
}

}  // namespace ngap
