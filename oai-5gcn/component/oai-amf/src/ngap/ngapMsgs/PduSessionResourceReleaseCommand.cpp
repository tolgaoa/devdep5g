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
 \date 2020
 \email: contact@openairinterface.org
 */

#include "PduSessionResourceReleaseCommand.hpp"

#include "logger.hpp"

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
PduSessionResourceReleaseCommandMsg::PduSessionResourceReleaseCommandMsg() {
  pduSessionResourceReleaseCommandPdu = NULL;
  pduSessionResourceReleaseCommandIEs = NULL;
  amfUeNgapId                         = NULL;
  ranUeNgapId                         = NULL;
  ranPagingPriority                   = NULL;
  nasPdu                              = NULL;
  pduSessionResourceToReleaseList     = NULL;
}

//------------------------------------------------------------------------------
PduSessionResourceReleaseCommandMsg::~PduSessionResourceReleaseCommandMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setMessageType() {
  if (!pduSessionResourceReleaseCommandPdu)
    pduSessionResourceReleaseCommandPdu =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType initialMessageTypeIE;
  initialMessageTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_PDUSessionResourceRelease);

  initialMessageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
  initialMessageTypeIE.setCriticality(Ngap_Criticality_reject);
  initialMessageTypeIE.setValuePresent(
      Ngap_InitiatingMessage__value_PR_PDUSessionResourceReleaseCommand);

  if (initialMessageTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_PDUSessionResourceRelease &&
      initialMessageTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_initiatingMessage &&
      initialMessageTypeIE.getCriticality() == Ngap_Criticality_reject) {
    initialMessageTypeIE.encode2pdu(pduSessionResourceReleaseCommandPdu);

    pduSessionResourceReleaseCommandIEs =
        &(pduSessionResourceReleaseCommandPdu->choice.initiatingMessage->value
              .choice.PDUSessionResourceReleaseCommand);
  } else {
    Logger::nas_mm().warn(
        "This information doesn't refer to PDUSessionResourceReleaseCommand "
        "Message");
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setRanUeNgapId(
    uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setRanPagingPriority(
    uint8_t priority) {
  if (!ranPagingPriority) ranPagingPriority = new RANPagingPriority();

  ranPagingPriority->setRANPagingPriority(priority);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RANPagingPriority;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RANPagingPriority;

  int ret = ranPagingPriority->encode2RANPagingPriority(
      ie->value.choice.RANPagingPriority);
  if (!ret) {
    Logger::nas_mm().warn("Encode RANPagingPriority IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode RANPagingPriority IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setNasPdu(
    uint8_t* nas, size_t sizeofnas) {
  if (!nasPdu) nasPdu = new NAS_PDU();

  nasPdu->setNasPdu(nas, sizeofnas);

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU;

  int ret = nasPdu->encode2octetstring(ie->value.choice.NAS_PDU);
  if (!ret) {
    Logger::nas_mm().warn("encode NAS_PDU IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode NAS_PDU IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::setPduSessionResourceToReleaseList(
    std::vector<PDUSessionResourceToReleaseItem_t> list) {
  if (!pduSessionResourceToReleaseList)
    pduSessionResourceToReleaseList =
        new PDUSessionResourceToReleaseListRelCmd();
  PDUSessionResourceToReleaseItemRelCmd*
      m_pduSessionResourceToReleaseItemRelCmd =
          new PDUSessionResourceToReleaseItemRelCmd[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);
    m_pduSessionResourceToReleaseItemRelCmd[i]
        .setPDUSessionResourceToReleaseItemRelCmd(
            m_pDUSessionID, list[i].pduSessionResourceReleaseCommandTransfer);
  }

  pduSessionResourceToReleaseList->setPDUSessionResourceToReleaseListRelCmd(
      m_pduSessionResourceToReleaseItemRelCmd, list.size());

  Ngap_PDUSessionResourceReleaseCommandIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseCommandIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd;

  int ret = pduSessionResourceToReleaseList
                ->encode2PDUSessionResourceToReleaseListRelCmd(
                    &ie->value.choice.PDUSessionResourceToReleaseListRelCmd);
  if (!ret) {
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceToReleaseListRelCmd IE error");
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseCommandIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceToReleaseListRelCmd IE error");
}

//------------------------------------------------------------------------------
int PduSessionResourceReleaseCommandMsg::encode2buffer(
    uint8_t* buf, int buf_size) {
  asn_fprint(
      stderr, &asn_DEF_Ngap_NGAP_PDU, pduSessionResourceReleaseCommandPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, pduSessionResourceReleaseCommandPdu, buf,
      buf_size);
  Logger::nas_mm().debug("er.encoded( %d)", er.encoded);

  return er.encoded;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseCommandMsg::encode2buffer_new(
    char* buf, int& encoded_size) {
  char* buffer = (char*) calloc(1, 512);  // TODO: remove hardcoded value
  asn_fprint(
      stderr, &asn_DEF_Ngap_NGAP_PDU, pduSessionResourceReleaseCommandPdu);
  encoded_size = aper_encode_to_new_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, pduSessionResourceReleaseCommandPdu,
      (void**) &buffer);

  Logger::nas_mm().debug("er.encoded ( %d)", encoded_size);
  memcpy((void*) buf, (void*) buffer, encoded_size);
  free(buffer);
}

//------------------------------------------------------------------------------
// Decapsulation
bool PduSessionResourceReleaseCommandMsg::decodefrompdu(
    Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  pduSessionResourceReleaseCommandPdu = ngap_msg_pdu;

  if (pduSessionResourceReleaseCommandPdu->present ==
      Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (pduSessionResourceReleaseCommandPdu->choice.initiatingMessage &&
        pduSessionResourceReleaseCommandPdu->choice.initiatingMessage
                ->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceRelease &&
        pduSessionResourceReleaseCommandPdu->choice.initiatingMessage
                ->criticality == Ngap_Criticality_reject &&
        pduSessionResourceReleaseCommandPdu->choice.initiatingMessage->value
                .present ==
            Ngap_InitiatingMessage__value_PR_PDUSessionResourceReleaseCommand) {
      pduSessionResourceReleaseCommandIEs =
          &pduSessionResourceReleaseCommandPdu->choice.initiatingMessage->value
               .choice.PDUSessionResourceReleaseCommand;
    } else {
      cout << "Check PDUSessionResourceReleaseCommand message error!!!" << endl;
      return false;
    }
  } else {
    Logger::nas_mm().warn("MessageType error");
    return false;
  }

  for (int i = 0;
       i < pduSessionResourceReleaseCommandIEs->protocolIEs.list.count; i++) {
    switch (
        pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            Logger::nas_mm().warn("Decoded NGAP AMF_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP AMF_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            Logger::nas_mm().warn("Decoded NGAP RAN_UE_NGAP_ID IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP RAN_UE_NGAP_ID IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_RANPagingPriority: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_RANPagingPriority) {
          ranPagingPriority = new RANPagingPriority();
          if (!ranPagingPriority->decodefromRANPagingPriority(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.RANPagingPriority)) {
            Logger::nas_mm().warn("Decoded NGAP RANPagingPriority IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP RANPagingPriority IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_NAS_PDU) {
          nasPdu = new NAS_PDU();
          if (!nasPdu->decodefromoctetstring(
                  pduSessionResourceReleaseCommandIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.NAS_PDU)) {
            Logger::nas_mm().warn("Decoded NGAP NAS_PDU IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn("Decoded NGAP NAS_PDU IE error");
          return false;
        }
      } break;

      case Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListRelCmd: {
        if (pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceReleaseCommandIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseCommandIEs__value_PR_PDUSessionResourceToReleaseListRelCmd) {
          pduSessionResourceToReleaseList =
              new PDUSessionResourceToReleaseListRelCmd();
          if (!pduSessionResourceToReleaseList
                   ->decodefromPDUSessionResourceToReleaseListRelCmd(

                       &pduSessionResourceReleaseCommandIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceToReleaseListRelCmd)) {
            Logger::nas_mm().warn(
                "Decoded NGAP PDUSessionResourceToReleaseListRelCmd IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn(
              "Decoded NGAP PDUSessionResourceToReleaseListRelCmd IE error");
          return false;
        }
      } break;
      default: {
        Logger::nas_mm().warn("Decoded NGAP message PDU error");
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
unsigned long PduSessionResourceReleaseCommandMsg::getAmfUeNgapId() {
  if (!amfUeNgapId) return -1;
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
uint32_t PduSessionResourceReleaseCommandMsg::getRanUeNgapId() {
  if (!ranUeNgapId) return -1;
  return ranUeNgapId->getRanUeNgapId();
}

//------------------------------------------------------------------------------
int PduSessionResourceReleaseCommandMsg::getRanPagingPriority() {
  if (!ranPagingPriority) return -1;
  return ranPagingPriority->getRANPagingPriority();
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::getNasPdu(
    uint8_t*& nas, size_t& sizeofnas) {
  if (!nasPdu) return false;
  if (!nasPdu->getNasPdu(nas, sizeofnas)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseCommandMsg::getPduSessionResourceToReleaseList(
    std::vector<PDUSessionResourceToReleaseItem_t>& list) {
  if (!pduSessionResourceToReleaseList) return false;

  PDUSessionResourceToReleaseItemRelCmd*
      m_pduSessionResourceToReleaseItemRelCmd;
  int num = 0;
  pduSessionResourceToReleaseList->getPDUSessionResourceToReleaseListRelCmd(
      m_pduSessionResourceToReleaseItemRelCmd, num);

  for (int i = 0; i < num; i++) {
    PDUSessionResourceToReleaseItem_t rel;

    PDUSessionID* m_pDUSessionID;

    m_pduSessionResourceToReleaseItemRelCmd[i]
        .getPDUSessionResourceToReleaseItemRelCmd(
            m_pDUSessionID, rel.pduSessionResourceReleaseCommandTransfer);

    m_pDUSessionID->getPDUSessionID(rel.pduSessionId);

    list.push_back(rel);
  }

  return true;
}

}  // namespace ngap
