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

#include "InitialContextSetupResponse.hpp"

extern "C" {
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "dynamic_memory_check.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

#include <iostream>

namespace ngap {

//------------------------------------------------------------------------------
InitialContextSetupResponseMsg::InitialContextSetupResponseMsg() {
  initialContextSetupResponsePdu              = NULL;
  initialContextSetupResponseIEs              = NULL;
  amfUeNgapId                                 = NULL;
  ranUeNgapId                                 = NULL;
  pduSessionResourceSetupResponseList         = NULL;
  pduSessionResourceFailedToSetupResponseList = NULL;
}

//------------------------------------------------------------------------------
InitialContextSetupResponseMsg::~InitialContextSetupResponseMsg() {}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setMessageType() {
  if (!initialContextSetupResponsePdu)
    initialContextSetupResponsePdu =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType initialContextSetupResponsePduTypeIE;
  initialContextSetupResponsePduTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_InitialContextSetup);
  initialContextSetupResponsePduTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_successfulOutcome);
  initialContextSetupResponsePduTypeIE.setCriticality(Ngap_Criticality_reject);
  initialContextSetupResponsePduTypeIE.setValuePresent(
      Ngap_SuccessfulOutcome__value_PR_InitialContextSetupResponse);

  if (initialContextSetupResponsePduTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_InitialContextSetup &&
      initialContextSetupResponsePduTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_successfulOutcome &&
      initialContextSetupResponsePduTypeIE.getCriticality() ==
          Ngap_Criticality_reject) {
    initialContextSetupResponsePduTypeIE.encode2pdu(
        initialContextSetupResponsePdu);
    initialContextSetupResponseIEs =
        &(initialContextSetupResponsePdu->choice.successfulOutcome->value.choice
              .InitialContextSetupResponse);
  } else {
    std::cout << "[Warning] This information doesn't refer to "
                 "InitialContextSetupResponse message!"
              << std::endl;
  }
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    std::cout << "Encode AMF_UE_NGAP_ID IE error" << std::endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) std::cout << "Encode AMF_UE_NGAP_ID IE error" << std::endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setRanUeNgapId(uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    std::cout << "Encode RAN_UE_NGAP_ID IE error" << std::endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0) std::cout << "Encode RAN_UE_NGAP_ID IE error" << std::endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t> list) {
  if (!pduSessionResourceSetupResponseList)
    pduSessionResourceSetupResponseList =
        new PDUSessionResourceSetupListCxtRes();

  PDUSessionResourceSetupItemCxtRes* m_pduSessionResourceSetupItemCxtRes =
      new PDUSessionResourceSetupItemCxtRes[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);

    m_pduSessionResourceSetupItemCxtRes[i].setPDUSessionResourceSetupItemCxtRes(
        m_pDUSessionID, list[i].pduSessionResourceSetupResponseTransfer);
  }

  pduSessionResourceSetupResponseList->setPDUSessionResourceSetupListCxtRes(
      m_pduSessionResourceSetupItemCxtRes, list.size());

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceSetupListCxtRes;

  int ret = pduSessionResourceSetupResponseList
                ->encode2PDUSessionResourceSetupListCxtRes(
                    &ie->value.choice.PDUSessionResourceSetupListCxtRes);
  if (!ret) {
    std::cout << "Encode PDUSessionResourceSetupListCxtRes IE error"
              << std::endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    std::cout << "Encode PDUSessionResourceSetupListCxtRes IE error"
              << std::endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void InitialContextSetupResponseMsg::setPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t> list) {
  if (!pduSessionResourceFailedToSetupResponseList)
    pduSessionResourceFailedToSetupResponseList =
        new PDUSessionResourceFailedToSetupListCxtRes();

  PDUSessionResourceFailedToSetupItemCxtRes*
      m_pduSessionResourceFailedToSetupItemCxtRes =
          new PDUSessionResourceFailedToSetupItemCxtRes[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);

    m_pduSessionResourceFailedToSetupItemCxtRes[i]
        .setPDUSessionResourceFailedToSetupItemCxtRes(
            m_pDUSessionID,
            list[i].pduSessionResourceSetupUnsuccessfulTransfer);
  }

  pduSessionResourceFailedToSetupResponseList
      ->setPDUSessionResourceFailedToSetupListCxtRes(
          m_pduSessionResourceFailedToSetupItemCxtRes, list.size());

  Ngap_InitialContextSetupResponseIEs_t* ie =
      (Ngap_InitialContextSetupResponseIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupResponseIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtRes;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListCxtRes;

  int ret =
      pduSessionResourceFailedToSetupResponseList
          ->encode2PDUSessionResourceFailedToSetupListCxtRes(
              &ie->value.choice.PDUSessionResourceFailedToSetupListCxtRes);
  if (!ret) {
    std::cout << "Encode PDUSessionResourceFailedToSetupListCxtRes IE error"
              << std::endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    std::cout << "Encode PDUSessionResourceFailedToSetupListCxtRes IE error"
              << std::endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int InitialContextSetupResponseMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, initialContextSetupResponsePdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, initialContextSetupResponsePdu, buf,
      buf_size);
  std::cout << "er.encoded(" << er.encoded << ")" << std::endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool InitialContextSetupResponseMsg::decodefrompdu(
    Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  initialContextSetupResponsePdu = ngap_msg_pdu;

  if (initialContextSetupResponsePdu->present ==
      Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (initialContextSetupResponsePdu->choice.successfulOutcome &&
        initialContextSetupResponsePdu->choice.successfulOutcome
                ->procedureCode == Ngap_ProcedureCode_id_InitialContextSetup &&
        initialContextSetupResponsePdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        initialContextSetupResponsePdu->choice.successfulOutcome->value
                .present ==
            Ngap_SuccessfulOutcome__value_PR_InitialContextSetupResponse) {
      initialContextSetupResponseIEs =
          &initialContextSetupResponsePdu->choice.successfulOutcome->value
               .choice.InitialContextSetupResponse;
    } else {
      std::cout << "Check InitialContextSetupResponse message error!"
                << std::endl;
      return false;
    }
  } else {
    std::cout << "MessageType error!" << std::endl;
    return false;
  }
  for (int i = 0; i < initialContextSetupResponseIEs->protocolIEs.list.count;
       i++) {
    switch (initialContextSetupResponseIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_InitialContextSetupResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  initialContextSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            std::cout << "Decoded NGAP AMF_UE_NGAP_ID IE error" << std::endl;
            return false;
          }
        } else {
          std::cout << "Decoded NGAP AMF_UE_NGAP_ID IE error" << std::endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        // TODO: to be verified
        if (/*initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&*/
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                ->value.present ==
            Ngap_InitialContextSetupResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  initialContextSetupResponseIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            std::cout << "Decoded NGAP RAN_UE_NGAP_ID IE error" << std::endl;
            return false;
          }
        } else {
          std::cout << "Decoded NGAP RAN_UE_NGAP_ID IE error" << std::endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListCxtRes: {
        if (initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceSetupListCxtRes) {
          pduSessionResourceSetupResponseList =
              new PDUSessionResourceSetupListCxtRes();
          if (!pduSessionResourceSetupResponseList
                   ->decodefromPDUSessionResourceSetupListCxtRes(
                       &initialContextSetupResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice.PDUSessionResourceSetupListCxtRes)) {
            std::cout
                << "Decoded NGAP PDUSessionResourceSetupListCxtRes IE error"
                << std::endl;
            return false;
          }
        } else {
          std::cout << "Decoded NGAP PDUSessionResourceSetupListCxtRes IE error"
                    << std::endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtRes: {
        if (initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupResponseIEs__value_PR_PDUSessionResourceFailedToSetupListCxtRes) {
          pduSessionResourceFailedToSetupResponseList =
              new PDUSessionResourceFailedToSetupListCxtRes();
          if (!pduSessionResourceFailedToSetupResponseList
                   ->decodefromPDUSessionResourceFailedToSetupListCxtRes(
                       &initialContextSetupResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceFailedToSetupListCxtRes)) {
            std::cout << "Decoded NGAP "
                         "PDUSessionResourceFailedToSetupListCxtRes IE error"
                      << std::endl;
            return false;
          }
        } else {
          std::cout << "Decoded NGAP PDUSessionResourceFailedToSetupListCxtRes "
                       "IE error!"
                    << std::endl;
          return false;
        }
      } break;

      default: {
        std::cout << "Decoded NGAP message PDU error" << std::endl;
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
unsigned long InitialContextSetupResponseMsg::getAmfUeNgapId() {
  if (!amfUeNgapId) return -1;
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
uint32_t InitialContextSetupResponseMsg::getRanUeNgapId() {
  if (!ranUeNgapId) return -1;
  return ranUeNgapId->getRanUeNgapId();
}

//------------------------------------------------------------------------------
bool InitialContextSetupResponseMsg::getPduSessionResourceSetupResponseList(
    std::vector<PDUSessionResourceSetupResponseItem_t>& list) {
  if (!pduSessionResourceSetupResponseList) return false;

  PDUSessionResourceSetupItemCxtRes* m_pduSessionResourceSetupItemCxtRes;
  int num = 0;
  pduSessionResourceSetupResponseList->getPDUSessionResourceSetupListCxtRes(
      m_pduSessionResourceSetupItemCxtRes, num);

  for (int i = 0; i < num; i++) {
    PDUSessionResourceSetupResponseItem_t response;

    PDUSessionID* m_pDUSessionID;
    m_pduSessionResourceSetupItemCxtRes[i].getPDUSessionResourceSetupItemCxtRes(
        m_pDUSessionID, response.pduSessionResourceSetupResponseTransfer);
    m_pDUSessionID->getPDUSessionID(response.pduSessionId);

    list.push_back(response);
  }

  return true;
}

//------------------------------------------------------------------------------
bool InitialContextSetupResponseMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupResponseList) return false;

  PDUSessionResourceFailedToSetupItemCxtRes*
      m_pduSessionResourceFailedToSetupItemCxtRes;
  int num = 0;
  pduSessionResourceFailedToSetupResponseList
      ->getPDUSessionResourceFailedToSetupListCxtRes(
          m_pduSessionResourceFailedToSetupItemCxtRes, num);

  for (int i = 0; i < num; i++) {
    PDUSessionResourceFailedToSetupItem_t failedToResponse;

    PDUSessionID* m_pDUSessionID;
    m_pduSessionResourceFailedToSetupItemCxtRes[i]
        .getPDUSessionResourceFailedToSetupItemCxtRes(
            m_pDUSessionID,
            failedToResponse.pduSessionResourceSetupUnsuccessfulTransfer);
    m_pDUSessionID->getPDUSessionID(failedToResponse.pduSessionId);

    list.push_back(failedToResponse);
  }

  return true;
}

}  // namespace ngap
