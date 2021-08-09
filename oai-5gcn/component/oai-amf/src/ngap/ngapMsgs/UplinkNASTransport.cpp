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

#include "UplinkNASTransport.hpp"

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
UplinkNASTransportMsg::UplinkNASTransportMsg() {
  uplinkNASTransportPdu   = NULL;
  uplinkNASTransportIEs   = NULL;
  amfUeNgapId             = NULL;
  ranUeNgapId             = NULL;
  nasPdu                  = NULL;
  userLocationInformation = NULL;
}

//------------------------------------------------------------------------------
UplinkNASTransportMsg::~UplinkNASTransportMsg() {}

//------------------------------------------------------------------------------
void UplinkNASTransportMsg::setMessageType() {
  if (!uplinkNASTransportPdu)
    uplinkNASTransportPdu =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType uplinkNASTransportPduTypeIE;
  uplinkNASTransportPduTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_UplinkNASTransport);
  uplinkNASTransportPduTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_initiatingMessage);
  uplinkNASTransportPduTypeIE.setCriticality(Ngap_Criticality_ignore);
  uplinkNASTransportPduTypeIE.setValuePresent(
      Ngap_InitiatingMessage__value_PR_UplinkNASTransport);

  if (uplinkNASTransportPduTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_UplinkNASTransport &&
      uplinkNASTransportPduTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_initiatingMessage &&
      uplinkNASTransportPduTypeIE.getCriticality() == Ngap_Criticality_ignore) {
    uplinkNASTransportPduTypeIE.encode2pdu(uplinkNASTransportPdu);
    uplinkNASTransportIEs = &(uplinkNASTransportPdu->choice.initiatingMessage
                                  ->value.choice.UplinkNASTransport);
  } else {
    std::cout << "[Warning] This information doesn't refer to "
                 "UplinkNASTransport Message!"
              << std::endl;
  }
}

//------------------------------------------------------------------------------
void UplinkNASTransportMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_UplinkNASTransport_IEs_t* ie = (Ngap_UplinkNASTransport_IEs_t*) calloc(
      1, sizeof(Ngap_UplinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    std::cout << "Encode AMF_UE_NGAP_ID IE error" << std::endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&uplinkNASTransportIEs->protocolIEs.list, ie);
  if (ret != 0) std::cout << "Encode AMF_UE_NGAP_ID IE error" << std::endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UplinkNASTransportMsg::setRanUeNgapId(uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_UplinkNASTransport_IEs_t* ie = (Ngap_UplinkNASTransport_IEs_t*) calloc(
      1, sizeof(Ngap_UplinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    std::cout << "Encode RAN_UE_NGAP_ID IE error" << std::endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&uplinkNASTransportIEs->protocolIEs.list, ie);
  if (ret != 0) std::cout << "Encode RAN_UE_NGAP_ID IE error" << std::endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UplinkNASTransportMsg::setNasPdu(uint8_t* nas, size_t sizeofnas) {
  if (!nasPdu) nasPdu = new NAS_PDU();

  nasPdu->setNasPdu(nas, sizeofnas);

  Ngap_UplinkNASTransport_IEs_t* ie = (Ngap_UplinkNASTransport_IEs_t*) calloc(
      1, sizeof(Ngap_UplinkNASTransport_IEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_NAS_PDU;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_UplinkNASTransport_IEs__value_PR_NAS_PDU;

  int ret = nasPdu->encode2octetstring(ie->value.choice.NAS_PDU);
  if (!ret) {
    std::cout << "Encode NAS_PDU IE error" << std::endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&uplinkNASTransportIEs->protocolIEs.list, ie);
  if (ret != 0) std::cout << "Encode NAS_PDU IE error" << std::endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UplinkNASTransportMsg::setUserLocationInfoNR(
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

  Ngap_UplinkNASTransport_IEs_t* ie = (Ngap_UplinkNASTransport_IEs_t*) calloc(
      1, sizeof(Ngap_UplinkNASTransport_IEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UserLocationInformation;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UplinkNASTransport_IEs__value_PR_UserLocationInformation;

  int ret = userLocationInformation->encodefromUserLocationInformation(
      &ie->value.choice.UserLocationInformation);
  if (!ret) {
    std::cout << "Encode UserLocationInformation IE error" << std::endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&uplinkNASTransportIEs->protocolIEs.list, ie);
  if (ret != 0)
    std::cout << "Encode UserLocationInformation IE error" << std::endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int UplinkNASTransportMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, uplinkNASTransportPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, uplinkNASTransportPdu, buf, buf_size);
  std::cout << "er.encoded(" << er.encoded << ")" << std::endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool UplinkNASTransportMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  uplinkNASTransportPdu = ngap_msg_pdu;

  if (uplinkNASTransportPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (uplinkNASTransportPdu->choice.initiatingMessage &&
        uplinkNASTransportPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_UplinkNASTransport &&
        uplinkNASTransportPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_ignore &&
        uplinkNASTransportPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_UplinkNASTransport) {
      uplinkNASTransportIEs = &uplinkNASTransportPdu->choice.initiatingMessage
                                   ->value.choice.UplinkNASTransport;
    } else {
      std::cout << "Check UplinkNASTransport message error!!!" << std::endl;
      return false;
    }
  } else {
    std::cout << "MessageType error!!!" << std::endl;
    return false;
  }
  for (int i = 0; i < uplinkNASTransportIEs->protocolIEs.list.count; i++) {
    switch (uplinkNASTransportIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (uplinkNASTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            uplinkNASTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_UplinkNASTransport_IEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  uplinkNASTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.AMF_UE_NGAP_ID)) {
            std::cout << "decoded ngap AMF_UE_NGAP_ID IE error" << std::endl;
            return false;
          }
        } else {
          std::cout << "decoded ngap AMF_UE_NGAP_ID IE error" << std::endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
        if (uplinkNASTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            uplinkNASTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_UplinkNASTransport_IEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  uplinkNASTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            std::cout << "decoded ngap RAN_UE_NGAP_ID IE error" << std::endl;
            return false;
          }
        } else {
          std::cout << "decoded ngap RAN_UE_NGAP_ID IE error" << std::endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NAS_PDU: {
        if (uplinkNASTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            uplinkNASTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_UplinkNASTransport_IEs__value_PR_NAS_PDU) {
          nasPdu = new NAS_PDU();
          if (!nasPdu->decodefromoctetstring(
                  uplinkNASTransportIEs->protocolIEs.list.array[i]
                      ->value.choice.NAS_PDU)) {
            std::cout << "decoded ngap NAS_PDU IE error" << std::endl;
            return false;
          }
        } else {
          std::cout << "decoded ngap NAS_PDU IE error" << std::endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UserLocationInformation: {
        if (uplinkNASTransportIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            uplinkNASTransportIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_UplinkNASTransport_IEs__value_PR_UserLocationInformation) {
          userLocationInformation = new UserLocationInformation();
          if (!userLocationInformation->decodefromUserLocationInformation(
                  &uplinkNASTransportIEs->protocolIEs.list.array[i]
                       ->value.choice.UserLocationInformation)) {
            std::cout << "decoded ngap UserLocationInformation IE error"
                      << std::endl;
            return false;
          }
        } else {
          std::cout << "decoded ngap UserLocationInformation IE error"
                    << std::endl;
          return false;
        }
      } break;

      default: {
        std::cout << "decoded ngap message pdu error" << std::endl;
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
unsigned long UplinkNASTransportMsg::getAmfUeNgapId() {
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
uint32_t UplinkNASTransportMsg::getRanUeNgapId() {
  return ranUeNgapId->getRanUeNgapId();
}

//------------------------------------------------------------------------------
bool UplinkNASTransportMsg::getNasPdu(uint8_t*& nas, size_t& sizeofnas) {
  if (!nasPdu->getNasPdu(nas, sizeofnas)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool UplinkNASTransportMsg::getUserLocationInfoNR(
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
