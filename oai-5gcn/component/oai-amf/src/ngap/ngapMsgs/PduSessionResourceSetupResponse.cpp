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

#include "PduSessionResourceSetupResponse.hpp"

extern "C" {
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "dynamic_memory_check.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseMsg::PduSessionResourceSetupResponseMsg() {
  pduSessionResourceSetupResponsePdu          = NULL;
  pduSessionResourceSetupResponseIEs          = NULL;
  amfUeNgapId                                 = NULL;
  ranUeNgapId                                 = NULL;
  pduSessionResourceSetupResponseList         = NULL;
  pduSessionResourceFailedToSetupResponseList = NULL;
}

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseMsg::~PduSessionResourceSetupResponseMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setMessageType() {
  if (!pduSessionResourceSetupResponsePdu)
    pduSessionResourceSetupResponsePdu =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType pduSessionResourceSetupResponsePduTypeIE;
  pduSessionResourceSetupResponsePduTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_PDUSessionResourceSetup);
  pduSessionResourceSetupResponsePduTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_successfulOutcome);
  pduSessionResourceSetupResponsePduTypeIE.setCriticality(
      Ngap_Criticality_reject);
  pduSessionResourceSetupResponsePduTypeIE.setValuePresent(
      Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceSetupResponse);

  if (pduSessionResourceSetupResponsePduTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_PDUSessionResourceSetup &&
      pduSessionResourceSetupResponsePduTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_successfulOutcome &&
      pduSessionResourceSetupResponsePduTypeIE.getCriticality() ==
          Ngap_Criticality_reject) {
    pduSessionResourceSetupResponsePduTypeIE.encode2pdu(
        pduSessionResourceSetupResponsePdu);
    pduSessionResourceSetupResponseIEs =
        &(pduSessionResourceSetupResponsePdu->choice.successfulOutcome->value
              .choice.PDUSessionResourceSetupResponse);
  } else {
    cout << "[warning] This information doesn't refer to "
            "PDUSessionResourceSetupResponse Message!!!"
         << endl;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AMF_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setRanUeNgapId(
    uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    cout << "encode RAN_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode RAN_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t> list) {
  if (!pduSessionResourceSetupResponseList)
    pduSessionResourceSetupResponseList =
        new PDUSessionResourceSetupListSURes();

  PDUSessionResourceSetupItemSURes* m_pduSessionResourceSetupItemSURes =
      new PDUSessionResourceSetupItemSURes[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);

    m_pduSessionResourceSetupItemSURes[i].setPDUSessionResourceSetupItemSURes(
        m_pDUSessionID, list[i].pduSessionResourceSetupResponseTransfer);
  }

  pduSessionResourceSetupResponseList->setPDUSessionResourceSetupListSURes(
      m_pduSessionResourceSetupItemSURes, list.size());

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes;

  int ret = pduSessionResourceSetupResponseList
                ->encode2PDUSessionResourceSetupListSURes(
                    &ie->value.choice.PDUSessionResourceSetupListSURes);
  if (!ret) {
    cout << "encode PDUSessionResourceSetupListSURes IE error" << endl;
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    cout << "encode PDUSessionResourceSetupListSURes IE error" << endl;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseMsg::setPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t> list) {
  if (!pduSessionResourceFailedToSetupResponseList)
    pduSessionResourceFailedToSetupResponseList =
        new PDUSessionResourceFailedToSetupListSURes();

  PDUSessionResourceFailedToSetupItemSURes*
      m_pduSessionResourceFailedToSetupItemSURes =
          new PDUSessionResourceFailedToSetupItemSURes[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);

    m_pduSessionResourceFailedToSetupItemSURes[i]
        .setPDUSessionResourceFailedToSetupItemSURes(
            m_pDUSessionID,
            list[i].pduSessionResourceSetupUnsuccessfulTransfer);
  }

  pduSessionResourceFailedToSetupResponseList
      ->setPDUSessionResourceFailedToSetupListSURes(
          m_pduSessionResourceFailedToSetupItemSURes, list.size());

  Ngap_PDUSessionResourceSetupResponseIEs_t* ie =
      (Ngap_PDUSessionResourceSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes;

  int ret = pduSessionResourceFailedToSetupResponseList
                ->encode2PDUSessionResourceFailedToSetupListSURes(
                    &ie->value.choice.PDUSessionResourceFailedToSetupListSURes);
  if (!ret) {
    cout << "encode PDUSessionResourceFailedToSetupListSURes IE error" << endl;
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    cout << "encode PDUSessionResourceFailedToSetupListSURes IE error" << endl;
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupResponseMsg::encode2buffer(
    uint8_t* buf, int buf_size) {
  asn_fprint(
      stderr, &asn_DEF_Ngap_NGAP_PDU, pduSessionResourceSetupResponsePdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, pduSessionResourceSetupResponsePdu, buf,
      buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool PduSessionResourceSetupResponseMsg::decodefrompdu(
    Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  pduSessionResourceSetupResponsePdu = ngap_msg_pdu;
  // asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU,
  // pduSessionResourceSetupResponsePdu);

  if (pduSessionResourceSetupResponsePdu->present ==
      Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (pduSessionResourceSetupResponsePdu->choice.successfulOutcome &&
        pduSessionResourceSetupResponsePdu->choice.successfulOutcome
                ->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceSetup &&
        pduSessionResourceSetupResponsePdu->choice.successfulOutcome
                ->criticality == Ngap_Criticality_reject &&
        pduSessionResourceSetupResponsePdu->choice.successfulOutcome->value
                .present ==
            Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceSetupResponse) {
      pduSessionResourceSetupResponseIEs =
          &pduSessionResourceSetupResponsePdu->choice.successfulOutcome->value
               .choice.PDUSessionResourceSetupResponse;
    } else {
      cout << "Check PDUSessionResourceSetupResponse message error!!!" << endl;
      return false;
    }
  } else {
    cout << "MessageType error!!!" << endl;
    return false;
  }
  for (int i = 0;
       i < pduSessionResourceSetupResponseIEs->protocolIEs.list.count; i++) {
    switch (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_PDUSessionResourceSetupResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
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
        // TODO: to be verified
        if (/*pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_PDUSessionResourceSetupResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListSURes: {
        if (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceSetupListSURes) {
          pduSessionResourceSetupResponseList =
              new PDUSessionResourceSetupListSURes();
          if (!pduSessionResourceSetupResponseList
                   ->decodefromPDUSessionResourceSetupListSURes(
                       &pduSessionResourceSetupResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice.PDUSessionResourceSetupListSURes)) {
            cout << "decoded ngap PDUSessionResourceSetupListSURes IE error"
                 << endl;
            return false;
          }
        } else {
          cout << "decoded ngap PDUSessionResourceSetupListSURes IE error"
               << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListSURes: {
        if (pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListSURes) {
          pduSessionResourceFailedToSetupResponseList =
              new PDUSessionResourceFailedToSetupListSURes();
          if (!pduSessionResourceFailedToSetupResponseList
                   ->decodefromPDUSessionResourceFailedToSetupListSURes(
                       &pduSessionResourceSetupResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceFailedToSetupListSURes)) {
            cout << "decoded ngap PDUSessionResourceFailedToSetupListSURes IE "
                    "error"
                 << endl;
            return false;
          }
        } else {
          cout << "decoded ngap PDUSessionResourceFailedToSetupListSURes IE "
                  "error!"
               << endl;
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

//------------------------------------------------------------------------------
unsigned long PduSessionResourceSetupResponseMsg::getAmfUeNgapId() {
  if (!amfUeNgapId) return -1;
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
uint32_t PduSessionResourceSetupResponseMsg::getRanUeNgapId() {
  if (!ranUeNgapId) return -1;
  return ranUeNgapId->getRanUeNgapId();
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::getPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  if (!pduSessionResourceSetupResponseList) return false;

  PDUSessionResourceSetupItemSURes* m_pduSessionResourceSetupItemSURes;
  int num = 0;
  pduSessionResourceSetupResponseList->getPDUSessionResourceSetupListSURes(
      m_pduSessionResourceSetupItemSURes, num);

  for (int i = 0; i < num; i++) {
    PDUSessionResourceSetupResponseItem_t response;

    PDUSessionID* m_pDUSessionID;
    m_pduSessionResourceSetupItemSURes[i].getPDUSessionResourceSetupItemSURes(
        m_pDUSessionID, response.pduSessionResourceSetupResponseTransfer);
    m_pDUSessionID->getPDUSessionID(response.pduSessionId);

    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupResponseList) return false;

  PDUSessionResourceFailedToSetupItemSURes*
      m_pduSessionResourceFailedToSetupItemSURes;
  int num = 0;
  pduSessionResourceFailedToSetupResponseList
      ->getPDUSessionResourceFailedToSetupListSURes(
          m_pduSessionResourceFailedToSetupItemSURes, num);

  for (int i = 0; i < num; i++) {
    PDUSessionResourceFailedToSetupItem_t failedToResponse;

    PDUSessionID* m_pDUSessionID;
    m_pduSessionResourceFailedToSetupItemSURes[i]
        .getPDUSessionResourceFailedToSetupItemSURes(
            m_pDUSessionID,
            failedToResponse.pduSessionResourceSetupUnsuccessfulTransfer);
    m_pDUSessionID->getPDUSessionID(failedToResponse.pduSessionId);

    list.push_back(failedToResponse);
  }

  return true;
}

}  // namespace ngap
