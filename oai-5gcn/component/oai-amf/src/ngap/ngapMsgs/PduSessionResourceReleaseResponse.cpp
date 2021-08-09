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

#include "PduSessionResourceReleaseResponse.hpp"

#include "logger.hpp"

extern "C" {
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "dynamic_memory_check.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
PduSessionResourceReleaseResponseMsg::PduSessionResourceReleaseResponseMsg() {
  pduSessionResourceReleaseResponsePdu = NULL;
  pduSessionResourceReleaseResponseIEs = NULL;
  amfUeNgapId                          = NULL;
  ranUeNgapId                          = NULL;
  pduSessionResourceReleasedList       = NULL;
  userLocationInformation              = NULL;
}

//------------------------------------------------------------------------------
PduSessionResourceReleaseResponseMsg::~PduSessionResourceReleaseResponseMsg() {}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setMessageType() {
  if (!pduSessionResourceReleaseResponsePdu)
    pduSessionResourceReleaseResponsePdu =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType messageTypeIE;
  messageTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_PDUSessionResourceRelease);

  messageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
  messageTypeIE.setCriticality(Ngap_Criticality_reject);

  messageTypeIE.setValuePresent(
      Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceReleaseResponse);

  if (messageTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_PDUSessionResourceRelease &&
      messageTypeIE.getTypeOfMessage() == Ngap_NGAP_PDU_PR_successfulOutcome &&
      messageTypeIE.getCriticality() == Ngap_Criticality_reject) {
    messageTypeIE.encode2pdu(pduSessionResourceReleaseResponsePdu);

    pduSessionResourceReleaseResponseIEs =
        &(pduSessionResourceReleaseResponsePdu->choice.successfulOutcome->value
              .choice.PDUSessionResourceReleaseResponse);
  } else {
    Logger::nas_mm().warn(
        "This information doesn't refer to PDUSessionResourceReleaseResponse "
        "Message");
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_PDUSessionResourceReleaseResponseIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseResponseIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode AMF_UE_NGAP_ID IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setRanUeNgapId(
    uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_PDUSessionResourceReleaseResponseIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseResponseIEs->protocolIEs.list, ie);
  if (ret != 0) Logger::nas_mm().warn("Encode RAN_UE_NGAP_ID IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setPduSessionResourceReleasedList(
    std::vector<PDUSessionResourceReleasedItem_t> list) {
  if (!pduSessionResourceReleasedList)
    pduSessionResourceReleasedList = new PDUSessionResourceReleasedListRelRes();
  PDUSessionResourceReleasedItemRelRes* m_pduSessionResourceReleasedItemRelRes =
      new PDUSessionResourceReleasedItemRelRes[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);

    m_pduSessionResourceReleasedItemRelRes[i]
        .setPDUSessionResourceReleasedItemRelRes(
            m_pDUSessionID, list[i].pduSessionResourceReleaseResponseTransfer);
  }

  pduSessionResourceReleasedList->setPDUSessionResourceReleasedListRelRes(
      m_pduSessionResourceReleasedItemRelRes, list.size());

  Ngap_PDUSessionResourceReleaseResponseIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseResponseIEs_t));

  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceReleasedListRelRes;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_PDUSessionResourceReleasedListRelRes;

  int ret = pduSessionResourceReleasedList
                ->encode2PDUSessionResourceReleasedListRelRes(
                    &ie->value.choice.PDUSessionResourceReleasedListRelRes);
  if (!ret) {
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceReleasedListRelRes IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::nas_mm().warn(
        "Encode PDUSessionResourceReleasedListRelRes IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::setUserLocationInfoNR(
    struct NrCgi_s cig, struct Tai_s tai) {
  if (!userLocationInformation)
    userLocationInformation = new UserLocationInformation();

  UserLocationInformationNR* informationNR = new UserLocationInformationNR();
  NR_CGI* nR_CGI                           = new NR_CGI();
  PlmnId* plmnId_cgi                       = new PlmnId();
  NRCellIdentity* nRCellIdentity           = new NRCellIdentity();
  plmnId_cgi->setMccMnc(cig.mcc, cig.mnc);
  nRCellIdentity->setNRCellIdentity(cig.nrCellID);
  nR_CGI->setNR_CGI(plmnId_cgi, nRCellIdentity);

  TAI* tai_nr        = new TAI();
  PlmnId* plmnId_tai = new PlmnId();
  plmnId_tai->setMccMnc(tai.mcc, tai.mnc);
  TAC* tac = new TAC();
  tac->setTac(tai.tac);
  tai_nr->setTAI(plmnId_tai, tac);
  informationNR->setInformationNR(nR_CGI, tai_nr);
  userLocationInformation->setInformation(informationNR);

  Ngap_PDUSessionResourceReleaseResponseIEs_t* ie =
      (Ngap_PDUSessionResourceReleaseResponseIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseResponseIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_UserLocationInformation;

  int ret = userLocationInformation->encodefromUserLocationInformation(
      &ie->value.choice.UserLocationInformation);
  if (!ret) {
    Logger::nas_mm().warn("Encode UserLocationInformation IE error");
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceReleaseResponseIEs->protocolIEs.list, ie);
  if (ret != 0)
    Logger::nas_mm().warn("Encode UserLocationInformation IE error");
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int PduSessionResourceReleaseResponseMsg::encode2buffer(
    uint8_t* buf, int buf_size) {
  asn_fprint(
      stderr, &asn_DEF_Ngap_NGAP_PDU, pduSessionResourceReleaseResponsePdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, pduSessionResourceReleaseResponsePdu, buf,
      buf_size);
  Logger::nas_mm().debug("er.encoded( %d)", er.encoded);

  return er.encoded;
}

//------------------------------------------------------------------------------
void PduSessionResourceReleaseResponseMsg::encode2buffer_new(
    char* buf, int& encoded_size) {
  char* buffer = (char*) calloc(1, 512);  // TODO: remove hardcoded value
  asn_fprint(
      stderr, &asn_DEF_Ngap_NGAP_PDU, pduSessionResourceReleaseResponsePdu);
  encoded_size = aper_encode_to_new_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, pduSessionResourceReleaseResponsePdu,
      (void**) &buffer);

  Logger::nas_mm().debug("er.encoded ( %d)", encoded_size);
  memcpy((void*) buf, (void*) buffer, encoded_size);
  free(buffer);
}

//------------------------------------------------------------------------------
// Decapsulation
bool PduSessionResourceReleaseResponseMsg::decodefrompdu(
    Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  pduSessionResourceReleaseResponsePdu = ngap_msg_pdu;

  if (pduSessionResourceReleaseResponsePdu->present ==
      Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (pduSessionResourceReleaseResponsePdu->choice.successfulOutcome &&
        pduSessionResourceReleaseResponsePdu->choice.successfulOutcome
                ->procedureCode ==
            Ngap_ProcedureCode_id_PDUSessionResourceRelease &&
        pduSessionResourceReleaseResponsePdu->choice.successfulOutcome
                ->criticality == Ngap_Criticality_reject &&
        pduSessionResourceReleaseResponsePdu->choice.successfulOutcome->value
                .present ==
            Ngap_SuccessfulOutcome__value_PR_PDUSessionResourceReleaseResponse) {
      pduSessionResourceReleaseResponseIEs =
          &pduSessionResourceReleaseResponsePdu->choice.successfulOutcome->value
               .choice.PDUSessionResourceReleaseResponse;
    } else {
      Logger::nas_mm().warn(
          "Check PDUSessionResourceReleaseResponse message error");
      return false;
    }
  } else {
    Logger::nas_mm().warn("MessageType error");
    return false;
  }

  for (int i = 0;
       i < pduSessionResourceReleaseResponseIEs->protocolIEs.list.count; i++) {
    switch (
        pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  pduSessionResourceReleaseResponseIEs->protocolIEs.list
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
        if (pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  pduSessionResourceReleaseResponseIEs->protocolIEs.list
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

      case Ngap_ProtocolIE_ID_id_PDUSessionResourceReleasedListRelRes: {
        if (pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            pduSessionResourceReleaseResponseIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceReleaseResponseIEs__value_PR_PDUSessionResourceReleasedListRelRes) {
          pduSessionResourceReleasedList =
              new PDUSessionResourceReleasedListRelRes();
          if (!pduSessionResourceReleasedList
                   ->decodefromPDUSessionResourceReleasedListRelRes(

                       &pduSessionResourceReleaseResponseIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceReleasedListRelRes)) {
            Logger::nas_mm().warn(
                "Decoded NGAP PDUSessionResourceReleasedListRelRes IE error");
            return false;
          }
        } else {
          Logger::nas_mm().warn(
              "Decoded NGAP PDUSessionResourceReleasedListRelRes IE error");
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
unsigned long PduSessionResourceReleaseResponseMsg::getAmfUeNgapId() {
  if (!amfUeNgapId) return -1;
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
uint32_t PduSessionResourceReleaseResponseMsg::getRanUeNgapId() {
  if (!ranUeNgapId) return -1;
  return ranUeNgapId->getRanUeNgapId();
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseResponseMsg::getPduSessionResourceReleasedList(
    std::vector<PDUSessionResourceReleasedItem_t>& list) {
  if (!pduSessionResourceReleasedList) return false;

  PDUSessionResourceReleasedItemRelRes* m_pduSessionResourceReleasedItemRelRes;
  int num = 0;
  pduSessionResourceReleasedList->getPDUSessionResourceReleasedListRelRes(
      m_pduSessionResourceReleasedItemRelRes, num);

  for (int i = 0; i < num; i++) {
    PDUSessionResourceReleasedItem_t rel;

    PDUSessionID* m_pDUSessionID;

    m_pduSessionResourceReleasedItemRelRes[i]
        .getPDUSessionResourceReleasedItemRelRes(
            m_pDUSessionID, rel.pduSessionResourceReleaseResponseTransfer);

    m_pDUSessionID->getPDUSessionID(rel.pduSessionId);

    list.push_back(rel);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceReleaseResponseMsg::getUserLocationInfoNR(
    struct NrCgi_s& cig, struct Tai_s& tai) {
  UserLocationInformationNR* informationNR;
  userLocationInformation->getInformation(informationNR);
  if (userLocationInformation->getChoiceOfUserLocationInformation() !=
      Ngap_UserLocationInformation_PR_userLocationInformationNR)
    return false;
  NR_CGI* nR_CGI;
  TAI* nR_TAI;
  informationNR->getInformationNR(nR_CGI, nR_TAI);
  PlmnId* cgi_plmnId;
  NRCellIdentity* nRCellIdentity;
  nR_CGI->getNR_CGI(cgi_plmnId, nRCellIdentity);
  cgi_plmnId->getMcc(cig.mcc);
  cgi_plmnId->getMnc(cig.mnc);
  cig.nrCellID = nRCellIdentity->getNRCellIdentity();

  PlmnId* tai_plmnId;
  TAC* tac;
  nR_TAI->getTAI(tai_plmnId, tac);
  tai_plmnId->getMcc(tai.mcc);
  tai_plmnId->getMnc(tai.mnc);
  tai.tac = tac->getTac();

  return true;
}

}  // namespace ngap
