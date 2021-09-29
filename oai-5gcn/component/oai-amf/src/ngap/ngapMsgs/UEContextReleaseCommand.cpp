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

#include "UEContextReleaseCommand.hpp"

#include <iostream>
extern "C" {
#include "Ngap_UE-NGAP-ID-pair.h"
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "dynamic_memory_check.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

using namespace ngap;
using namespace std;

//------------------------------------------------------------------------------
UEContextReleaseCommandMsg::UEContextReleaseCommandMsg() {
  pdu         = nullptr;
  amfUeNgapId = nullptr;
  ranUeNgapId = nullptr;
  causeValue  = nullptr;
  ies         = nullptr;
}

UEContextReleaseCommandMsg::~UEContextReleaseCommandMsg() {
  if (pdu) ASN_STRUCT_FREE(asn_DEF_Ngap_NGAP_PDU, pdu);
  if (amfUeNgapId) delete amfUeNgapId;
  if (ranUeNgapId) delete ranUeNgapId;
  if (causeValue) delete causeValue;
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::setMessageType() {
  if (!pdu) pdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));
  MessageType msgType;
  msgType.setProcedureCode(Ngap_ProcedureCode_id_UEContextRelease);
  msgType.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
  msgType.setCriticality(Ngap_Criticality_reject);
  msgType.setValuePresent(
      Ngap_InitiatingMessage__value_PR_UEContextReleaseCommand);
  msgType.encode2pdu(pdu);
  ies = &(pdu->choice.initiatingMessage->value.choice.UEContextReleaseCommand);
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);
  Ngap_UEContextReleaseCommand_IEs_t* ie =
      (Ngap_UEContextReleaseCommand_IEs_t*) calloc(
          1, sizeof(Ngap_UEContextReleaseCommand_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UE_NGAP_IDs;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseCommand_IEs__value_PR_UE_NGAP_IDs;
  ie->value.choice.UE_NGAP_IDs.present = Ngap_UE_NGAP_IDs_PR_aMF_UE_NGAP_ID;
  int ret                              = amfUeNgapId->encode2AMF_UE_NGAP_ID(
      ie->value.choice.UE_NGAP_IDs.choice.aMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UE_NGAP_IDs IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::setUeNgapIdPair(
    unsigned long amfId, uint32_t ranId) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(amfId);
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ranId);
  Ngap_UEContextReleaseCommand_IEs_t* ie =
      (Ngap_UEContextReleaseCommand_IEs_t*) calloc(
          1, sizeof(Ngap_UEContextReleaseCommand_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UE_NGAP_IDs;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseCommand_IEs__value_PR_UE_NGAP_IDs;
  ie->value.choice.UE_NGAP_IDs.present = Ngap_UE_NGAP_IDs_PR_uE_NGAP_ID_pair;
  ie->value.choice.UE_NGAP_IDs.choice.uE_NGAP_ID_pair =
      (Ngap_UE_NGAP_ID_pair_t*) calloc(1, sizeof(Ngap_UE_NGAP_ID_pair_t));
  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(
      ie->value.choice.UE_NGAP_IDs.choice.uE_NGAP_ID_pair->aMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }
  ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(
      ie->value.choice.UE_NGAP_IDs.choice.uE_NGAP_ID_pair->rAN_UE_NGAP_ID);
  if (!ret) {
    cout << "encode RAN_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UE_NGAP_IDs IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::setCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork cause_value) {
  if (!causeValue) causeValue = new Cause();
  causeValue->setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  causeValue->setValue(cause_value);
  addCauseIE();
}

void UEContextReleaseCommandMsg::setCauseNas(e_Ngap_CauseNas cause_value) {
  if (!causeValue) causeValue = new Cause();
  causeValue->setChoiceOfCause(Ngap_Cause_PR_nas);
  causeValue->setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void UEContextReleaseCommandMsg::addCauseIE() {
  Ngap_UEContextReleaseCommand_IEs_t* ie =
      (Ngap_UEContextReleaseCommand_IEs_t*) calloc(
          1, sizeof(Ngap_UEContextReleaseCommand_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_UEContextReleaseCommand_IEs__value_PR_Cause;
  causeValue->encode2Cause(&ie->value.choice.Cause);
  int ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) cout << "encode Cause IE error" << endl;
}

//------------------------------------------------------------------------------
int UEContextReleaseCommandMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, pdu);
  asn_enc_rval_t er =
      aper_encode_to_buffer(&asn_DEF_Ngap_NGAP_PDU, NULL, pdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
bool UEContextReleaseCommandMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  pdu = ngap_msg_pdu;
  return true;
}
