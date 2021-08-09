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

#include "UEContextReleaseComplete.hpp"

#include <iostream>
#include <memory>

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
UEContextReleaseCompleteMsg::UEContextReleaseCompleteMsg() {
  pdu                     = nullptr;
  ies                     = nullptr;
  amfUeNgapId             = nullptr;
  ranUeNgapId             = nullptr;
  userLocationInformation = nullptr;
}

//------------------------------------------------------------------------------
UEContextReleaseCompleteMsg::~UEContextReleaseCompleteMsg() {
  delete pdu;
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setMessageType() {
  if (!pdu) pdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));
  MessageType msgType;
  msgType.setProcedureCode(Ngap_ProcedureCode_id_UEContextRelease);
  msgType.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
  msgType.setCriticality(Ngap_Criticality_ignore);
  msgType.setValuePresent(
      Ngap_SuccessfulOutcome__value_PR_UEContextReleaseComplete);
  msgType.encode2pdu(pdu);
  ies = &(pdu->choice.successfulOutcome->value.choice.UEContextReleaseComplete);
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);
  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID;
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
unsigned long UEContextReleaseCompleteMsg::getAmfUeNgapId() const {
  if (amfUeNgapId) return amfUeNgapId->getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setRanUeNgapId(uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);
  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID;
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
uint32_t UEContextReleaseCompleteMsg::getRanUeNgapId() const {
  if (ranUeNgapId) return ranUeNgapId->getRanUeNgapId();
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::setUserLocationInfoNR(
    struct NrCgi_s cig, struct Tai_s tai) {
  if (!userLocationInformation)
    userLocationInformation = new UserLocationInformation();

  // userLocationInformation->setInformation(UserLocationInformationEUTRA *
  // informationEUTRA);
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

  Ngap_UEContextReleaseComplete_IEs* ie =
      (Ngap_UEContextReleaseComplete_IEs*) calloc(
          1, sizeof(Ngap_UEContextReleaseComplete_IEs));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UEContextReleaseComplete_IEs__value_PR_UserLocationInformation;

  int ret = userLocationInformation->encodefromUserLocationInformation(
      &ie->value.choice.UserLocationInformation);
  if (!ret) {
    cout << "encode UserLocationInformation IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }
  ret = ASN_SEQUENCE_ADD(&ies->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UserLocationInformation IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UEContextReleaseCompleteMsg::getUserLocationInfoNR(
    struct NrCgi_s& cig, struct Tai_s& tai) {
  // TODO:
  /*
    if (userLocationInformation) {
      UserLocationInformationNR* informationNR = new
    UserLocationInformationNR();
      userLocationInformation->getInformation(informationNR);

      //std::unique_ptr<NR_CGI> nR_CGI = std::make_unique<NR_CGI>();

       NR_CGI* nR_CGI = new NR_CGI();
      TAI* tai_nr = new TAI();
      informationNR->getInformationNR(nR_CGI, tai_nr);
      PlmnId* plmnId_cgi             = new PlmnId();
      NRCellIdentity* nRCellIdentity = new NRCellIdentity();

      nR_CGI->getNR_CGI(plmnId_cgi, nRCellIdentity);
      cig.nrCellID = nRCellIdentity->getNRCellIdentity();
      plmnId_cgi->getMcc(cig.mcc);
      plmnId_cgi->getMnc(cig.mnc);

      PlmnId* plmnId = new PlmnId();
      TAC* tac       = new TAC();
      tai_nr->getTAI(plmnId, tac);

      plmnId->getMcc(tai.mcc);
      plmnId->getMnc(tai.mnc);
      tai.tac = tac->getTac() & 0x00ffffff;
    }
    */
}

//------------------------------------------------------------------------------
int UEContextReleaseCompleteMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, pdu);
  asn_enc_rval_t er =
      aper_encode_to_buffer(&asn_DEF_Ngap_NGAP_PDU, NULL, pdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
bool UEContextReleaseCompleteMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  pdu = ngap_msg_pdu;
  if (pdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (pdu->choice.successfulOutcome &&
        pdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_UEContextRelease &&
        pdu->choice.successfulOutcome->criticality == Ngap_Criticality_reject &&
        pdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_UEContextReleaseComplete) {
      ies =
          &pdu->choice.successfulOutcome->value.choice.UEContextReleaseComplete;
    } else {
      cout << "Check UEContextReleaseComplete message error" << endl;
      return false;
    }
  } else {
    cout << "typeOfMessage of UEContextReleaseComplete is not SuccessfulOutcome"
         << endl;
    return false;
  }
  // TODO

  for (int i = 0; i < ies->protocolIEs.list.count; i++) {
    switch (ies->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ies->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_AMF_UE_NGAP_ID) {
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
                Ngap_Criticality_ignore &&
            ies->protocolIEs.list.array[i]->value.present ==
                Ngap_UEContextReleaseComplete_IEs__value_PR_RAN_UE_NGAP_ID) {
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
        // TODO: User Location Information
        // TODO: Information on Recommended Cells and RAN Nodes for Paging
        // TODO: PDU Session Resource List
        // TODO: Criticality Diagnostics
    }
  }

  return true;
}
