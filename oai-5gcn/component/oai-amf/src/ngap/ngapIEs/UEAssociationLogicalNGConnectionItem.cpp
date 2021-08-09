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
 \author
 \date 2021
 \email: contact@openairinterface.org
 */

#include "UEAssociationLogicalNGConnectionItem.hpp"

extern "C" {
#include "dynamic_memory_check.h"
}

#include <iostream>
using namespace std;

namespace ngap {

UEAssociationLogicalNGConnectionItem::UEAssociationLogicalNGConnectionItem() {
  amfUeNgapId = nullptr;
  ranUeNgapId = nullptr;
}
//------------------------------------------------------------------------------
void UEAssociationLogicalNGConnectionItem::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }
}

//------------------------------------------------------------------------------
void UEAssociationLogicalNGConnectionItem::setRanUeNgapId(
    uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_DownlinkNASTransport_IEs_t* ie =
      (Ngap_DownlinkNASTransport_IEs_t*) calloc(
          1, sizeof(Ngap_DownlinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_DownlinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    cout << "Encode RAN_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }
}

//------------------------------------------------------------------------------
bool UEAssociationLogicalNGConnectionItem::encode(
    Ngap_UE_associatedLogicalNG_connectionItem_t& item) {
  item.aMF_UE_NGAP_ID = new Ngap_AMF_UE_NGAP_ID_t();
  amfUeNgapId->encode2AMF_UE_NGAP_ID(*item.aMF_UE_NGAP_ID);
  item.rAN_UE_NGAP_ID = new Ngap_RAN_UE_NGAP_ID_t();
  ranUeNgapId->encode2RAN_UE_NGAP_ID(*item.rAN_UE_NGAP_ID);
  return true;
}

//------------------------------------------------------------------------------
bool UEAssociationLogicalNGConnectionItem::encode(
    UEAssociationLogicalNGConnectionItem& item) {
  item.amfUeNgapId = new AMF_UE_NGAP_ID();
  item.amfUeNgapId = amfUeNgapId;
  item.ranUeNgapId = new RAN_UE_NGAP_ID();
  item.ranUeNgapId = ranUeNgapId;
  return true;
}

//------------------------------------------------------------------------------
bool UEAssociationLogicalNGConnectionItem::decode(
    Ngap_UE_associatedLogicalNG_connectionItem_t* item) {
  amfUeNgapId = new AMF_UE_NGAP_ID();
  if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(*item->aMF_UE_NGAP_ID)) {
    cout << "Decoded NGAP AMF_UE_NGAP_ID IE error" << endl;
    return false;
  }

  ranUeNgapId = new RAN_UE_NGAP_ID();
  if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(*item->rAN_UE_NGAP_ID)) {
    cout << "Decoded NGAP RAN_UE_NGAP_ID IE error" << endl;
    return false;
  }
  return true;
}

}  // namespace ngap
