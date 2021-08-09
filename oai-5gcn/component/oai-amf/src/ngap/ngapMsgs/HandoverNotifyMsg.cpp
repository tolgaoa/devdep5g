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

/*! \file HandoverNotifyMsg.cpp
 \brief
 \author  niuxiansheng-niu, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */
#include "HandoverNotifyMsg.hpp"

extern "C" {
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
HandoverNotifyMsg::HandoverNotifyMsg() {
  amfUeNgapId             = nullptr;
  ranUeNgapId             = nullptr;
  userLocationInformation = nullptr;
  handoverNotifyPdu       = nullptr;
  handoverNotifyIEs       = nullptr;
}

HandoverNotifyMsg::~HandoverNotifyMsg(){};
unsigned long HandoverNotifyMsg::getAmfUeNgapId() {
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}
int HandoverNotifyMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, handoverNotifyPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, handoverNotifyPdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}
bool HandoverNotifyMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  handoverNotifyPdu = ngap_msg_pdu;

  if (handoverNotifyPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (handoverNotifyPdu->choice.initiatingMessage &&
        handoverNotifyPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_HandoverNotification &&
        handoverNotifyPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        handoverNotifyPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_HandoverNotify) {
      handoverNotifyIEs = &handoverNotifyPdu->choice.initiatingMessage->value
                               .choice.HandoverNotify;
    } else {
      cout << "Check HandoverNotify message error!!!" << endl;
      return false;
    }
  } else {
    cout << "HandoverNotify MessageType error!!!" << endl;
    return false;
  }
  for (int i = 0; i < handoverNotifyIEs->protocolIEs.list.count; i++) {
    switch (handoverNotifyIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverNotifyIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverNotifyIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverNotifyIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  handoverNotifyIEs->protocolIEs.list.array[i]
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
        if (handoverNotifyIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverNotifyIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverNotifyIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  handoverNotifyIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        if (handoverNotifyIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverNotifyIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverNotifyIEs__value_PR_UserLocationInformation) {
          userLocationInformation = new UserLocationInformation();
          if (!userLocationInformation->decodefromUserLocationInformation(
                  &handoverNotifyIEs->protocolIEs.list.array[i]
                       ->value.choice.UserLocationInformation)) {
            cout << "decoded ngap UserLocationInformation IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap UserLocationInformation IE error" << endl;
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
void HandoverNotifyMsg::setUserLocationInfoNR(
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

  Ngap_HandoverNotifyIEs_t* ie =
      (Ngap_HandoverNotifyIEs_t*) calloc(1, sizeof(Ngap_HandoverNotifyIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_HandoverNotifyIEs__value_PR_UserLocationInformation;

  int ret = userLocationInformation->encodefromUserLocationInformation(
      &ie->value.choice.UserLocationInformation);
  if (!ret) {
    cout << "encode UserLocationInformation IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverNotifyIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UserLocationInformation IE error" << endl;
  // free_wrapper((void**) &ie);
}
uint32_t HandoverNotifyMsg::getRanUeNgapId() {
  return ranUeNgapId->getRanUeNgapId();
}
bool HandoverNotifyMsg::getUserLocationInfoNR(
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
