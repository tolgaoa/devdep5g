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

#include "HandoverCommandMsg.hpp"
extern "C" {
#include "Ngap_NGAP-PDU.h"
#include "Ngap_PDUSessionResourceHandoverItem.h"
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "dynamic_memory_check.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

#include <iostream>
#include <vector>

using namespace std;

namespace ngap {

HandoverCommandMsg::HandoverCommandMsg() {
  amfUeNgapId                          = NULL;
  ranUeNgapId                          = NULL;
  ngap_handovertype                    = NULL;
  NASSecurityParametersFromNGRAN       = NULL;
  PDUSessionResourceHandoverList       = NULL;
  PDUSessionResourceToReleaseListHOCmd = NULL;
  TargetToSource_TransparentContainer  = NULL;
  CriticalityDiagnostics               = NULL;
  handoverCommandPdu                   = NULL;
  handoverCommandIEs                   = NULL;
}
HandoverCommandMsg::~HandoverCommandMsg() {}

unsigned long HandoverCommandMsg::getAmfUeNgapId() {
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

uint32_t HandoverCommandMsg::getRanUeNgapId() {
  return ranUeNgapId->getRanUeNgapId();
}

bool HandoverCommandMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  handoverCommandPdu = ngap_msg_pdu;

  if (handoverCommandPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (handoverCommandPdu->choice.successfulOutcome &&
        handoverCommandPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_HandoverPreparation &&
        handoverCommandPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        handoverCommandPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_HandoverCommand) {
      handoverCommandIEs = &handoverCommandPdu->choice.successfulOutcome->value
                                .choice.HandoverCommand;
    } else {
      cout << "Check HandoverCommand message error!!!" << endl;
      return false;
    }
  } else {
    cout << "HandoverRequired MessageType error!!!" << endl;
    return false;
  }
  for (int i = 0; i < handoverCommandIEs->protocolIEs.list.count; i++) {
    switch (handoverCommandIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  handoverCommandIEs->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            cout << "decoded ngap AMF_UE_NGAP_ID IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap AMF_UE_NGAP_ID IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  handoverCommandIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_HandoverType: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_HandoverType) {
          ngap_handovertype  = new Ngap_HandoverType_t();
          *ngap_handovertype = handoverCommandIEs->protocolIEs.list.array[i]
                                   ->value.choice.HandoverType;
        } else {
          cout << "decoded ngap Handover Type IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList) {
        } else {
          cout << "decoded ngap PDUSessionResourceHandoverList IE error"
               << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListHOCmd: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceToReleaseListHOCmd) {
        } else {
          cout << "decoded ngap PDUSessionResourceToReleaseListHOCmd IE error"
               << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer) {
        } else {
          cout << "decoded ngap TargetToSource_TransparentContainer IE error"
               << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverCommandIEs__value_PR_CriticalityDiagnostics) {
        } else {
          cout << "decoded ngap CriticalityDiagnostics IE error" << endl;
          return false;
        }
      } break;
      default: {
        cout << "decoded ngap message pdu error" << endl;
        return false;
      }
    }
  }

  return true;
}

int HandoverCommandMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, handoverCommandPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, handoverCommandPdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

void HandoverCommandMsg::setMessageType() {
  if (!handoverCommandPdu)
    handoverCommandPdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType HandoverCommandMessageTypeIE;
  HandoverCommandMessageTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_HandoverPreparation);
  HandoverCommandMessageTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_successfulOutcome);
  HandoverCommandMessageTypeIE.setCriticality(Ngap_Criticality_reject);
  HandoverCommandMessageTypeIE.setValuePresent(
      Ngap_SuccessfulOutcome__value_PR_HandoverCommand);

  if (HandoverCommandMessageTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_HandoverPreparation &&
      HandoverCommandMessageTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_successfulOutcome) {
    HandoverCommandMessageTypeIE.encode2pdu(handoverCommandPdu);
    handoverCommandIEs = &(handoverCommandPdu->choice.successfulOutcome->value
                               .choice.HandoverCommand);
  } else {
    cout << "[warning] This information doesn't refer to HandoverCommand "
            "Message!!!"
         << endl;
  }
}

void HandoverCommandMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AMF_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverCommandMsg::setRanUeNgapId(uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    cout << "encode RAN_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode RAN_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverCommandMsg::setHandoverType(long type) {
  if (!ngap_handovertype) ngap_handovertype = new Ngap_HandoverType_t();
  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_HandoverType;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverCommandIEs__value_PR_HandoverType;
  ie->value.choice.HandoverType = type;
  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode HandoverType IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverCommandMsg::setPduSessionResourceHandoverList(
    std::vector<PDUSessionResourceHandoverItem_t> list) {
  if (!PDUSessionResourceHandoverList)
    PDUSessionResourceHandoverList =
        new Ngap_PDUSessionResourceHandoverList_t();
  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));

  Ngap_PDUSessionResourceHandoverItem_t* item =
      (Ngap_PDUSessionResourceHandoverItem_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceHandoverItem_t));
  for (int i = 0; i < list.size(); i++) {
    item->pDUSessionID            = list[i].pduSessionId;
    item->handoverCommandTransfer = list[i].HandoverCommandTransfer;
    int ret = ASN_SEQUENCE_ADD(&PDUSessionResourceHandoverList->list, item);
    if (ret != 0)
      cout << "encode PDUSessionResourceHandoverListItem IE error" << endl;
  }

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList;
  ie->value.choice.PDUSessionResourceHandoverList =
      *PDUSessionResourceHandoverList;
  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0)
    cout << "encode PDUSessionResourceHandoverList IE error" << endl;
  // free_wrapper((void**) &item);
  // free_wrapper((void**) &ie);
}

void HandoverCommandMsg::setTargetToSource_TransparentContainer(
    OCTET_STRING_t targetTosource) {
  if (!TargetToSource_TransparentContainer)
    TargetToSource_TransparentContainer =
        new Ngap_TargetToSource_TransparentContainer_t();

  Ngap_HandoverCommandIEs_t* ie =
      (Ngap_HandoverCommandIEs_t*) calloc(1, sizeof(Ngap_HandoverCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer;
  ie->value.choice.TargetToSource_TransparentContainer = targetTosource;
  int ret = ASN_SEQUENCE_ADD(&handoverCommandIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode HandoverType IE error" << endl;
  // free_wrapper((void**) &ie);
}

}  // namespace ngap
