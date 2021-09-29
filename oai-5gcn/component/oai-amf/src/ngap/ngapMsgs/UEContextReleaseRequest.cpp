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

#include "UEContextReleaseRequest.hpp"

#include <iostream>
extern "C" {
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
UEContextReleaseRequestMsg::UEContextReleaseRequestMsg() {
  pdu         = nullptr;
  ies         = nullptr;
  amfUeNgapId = nullptr;
  ranUeNgapId = nullptr;
  causeValue  = nullptr;
}

//------------------------------------------------------------------------------
UEContextReleaseRequestMsg::~UEContextReleaseRequestMsg() {
  delete pdu;
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::setMessageType() {
  if (!pdu) pdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));
  MessageType msgType;
  msgType.setProcedureCode(Ngap_ProcedureCode_id_UEContextReleaseRequest);
  msgType.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
  msgType.setCriticality(Ngap_Criticality_ignore);
  msgType.setValuePresent(
      Ngap_InitiatingMessage__value_PR_UEContextReleaseRequest);
  msgType.encode2pdu(pdu);
  ies = &(pdu->choice.initiatingMessage->value.choice.UEContextReleaseRequest);
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_AMF_UE_NGAP_ID;
  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AMF_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::setRanUeNgapId(uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_RAN_UE_NGAP_ID;
  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    cout << "encode RAN_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) cout << "encode RAN_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::setCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork cause_value) {
  if (!causeValue) causeValue = new Cause();
  causeValue->setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  causeValue->setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void UEContextReleaseRequestMsg::addCauseIE() {
  Ngap_UEContextReleaseRequest_IEs* ie =
      (Ngap_UEContextReleaseRequest_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseRequest_IEs));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_UEContextReleaseRequest_IEs__value_PR_Cause;
  causeValue->encode2Cause(&ie->value.choice.Cause);
  int ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) cout << "encode Cause IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int UEContextReleaseRequestMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, pdu);
  asn_enc_rval_t er =
      aper_encode_to_buffer(&asn_DEF_Ngap_NGAP_PDU, NULL, pdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
bool UEContextReleaseRequestMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  pdu = ngap_msg_pdu;
  if (pdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (pdu->choice.initiatingMessage &&
        pdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UEContextReleaseRequest &&
        pdu->choice.initiatingMessage->criticality == Ngap_Criticality_ignore &&
        pdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UEContextReleaseRequest) {
      ies =
          &pdu->choice.initiatingMessage->value.choice.UEContextReleaseRequest;
    } else {
      cout << "Check UEContextReleaseRequest message error" << endl;
      return false;
    }
  } else {
    cout << "typeOfMessage of UEContextReleaseRequest is not initiatingMessage"
         << endl;
    return false;
  }
  for (int i = 0; i < ies->protocolIEs.list.count; i++) {
    switch (ies->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  ies->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            cout << "decode AMF_UE_NGAP_ID error" << endl;
            return false;
          }
        } else {
          cout << "IE AMF_UE_NGAP_ID is not correct" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  ies->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            cout << "decode RAN_UE_NGAP_ID error" << endl;
            return false;
          }
        } else {
          cout << "IE RAN_UE_NGAP_ID is not correct" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseRequest_IEs__value_PR_Cause) {
          causeValue = new Cause();
          if (!causeValue->decodefromCause(
                  &ies->protocolIEs.list.array[i]->value.choice.Cause)) {
            cout << "decode Cause error" << endl;
            return false;
          }
        } else {
          cout << "IE Cause is not correct" << endl;
          return false;
        }
      } break;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool UEContextReleaseRequestMsg::getCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork& causeRadioNetwork) {
  if (causeValue->getValue() < 0) {
    cout << "Get Cause value from UEContextReleaseRequest Error!!!" << endl;
    return false;
  }
  causeRadioNetwork = (e_Ngap_CauseRadioNetwork) causeValue->getValue();
  return true;
}

//------------------------------------------------------------------------------
unsigned long UEContextReleaseRequestMsg::getAmfUeNgapId() {
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
uint32_t UEContextReleaseRequestMsg::getRanUeNgapId() {
  return ranUeNgapId->getRanUeNgapId();
}
