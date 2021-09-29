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

#include "InitialContextSetupFailure.hpp"

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
InitialContextSetupFailureMsg::InitialContextSetupFailureMsg() {
  initialContextSetupFailurePdu              = NULL;
  initialContextSetupFailureIEs              = NULL;
  amfUeNgapId                                = NULL;
  ranUeNgapId                                = NULL;
  pduSessionResourceFailedToSetupFailureList = NULL;
}

//------------------------------------------------------------------------------
InitialContextSetupFailureMsg::~InitialContextSetupFailureMsg() {}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setMessageType() {
  if (!initialContextSetupFailurePdu)
    initialContextSetupFailurePdu =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType initialContextSetupFailurePduTypeIE;
  initialContextSetupFailurePduTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_InitialContextSetup);
  initialContextSetupFailurePduTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_unsuccessfulOutcome);
  initialContextSetupFailurePduTypeIE.setCriticality(Ngap_Criticality_reject);
  initialContextSetupFailurePduTypeIE.setValuePresent(
      Ngap_UnsuccessfulOutcome__value_PR_InitialContextSetupFailure);

  if (initialContextSetupFailurePduTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_InitialContextSetup &&
      initialContextSetupFailurePduTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_unsuccessfulOutcome &&
      initialContextSetupFailurePduTypeIE.getCriticality() ==
          Ngap_Criticality_reject) {
    initialContextSetupFailurePduTypeIE.encode2pdu(
        initialContextSetupFailurePdu);
    initialContextSetupFailureIEs =
        &(initialContextSetupFailurePdu->choice.unsuccessfulOutcome->value
              .choice.InitialContextSetupFailure);
  } else {
    cout << "[warning] This information doesn't refer to "
            "InitialContextSetupFailure Message!!!"
         << endl;
  }
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AMF_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setRanUeNgapId(uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    cout << "encode RAN_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode RAN_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void InitialContextSetupFailureMsg::setPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t> list) {
  if (!pduSessionResourceFailedToSetupFailureList)
    pduSessionResourceFailedToSetupFailureList =
        new PDUSessionResourceFailedToSetupListCxtFail();

  PDUSessionResourceFailedToSetupItemCxtFail*
      m_pduSessionResourceFailedToSetupItemCxtFail =
          new PDUSessionResourceFailedToSetupItemCxtFail[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);

    m_pduSessionResourceFailedToSetupItemCxtFail[i]
        .setPDUSessionResourceFailedToSetupItemCxtFail(
            m_pDUSessionID,
            list[i].pduSessionResourceSetupUnsuccessfulTransfer);
  }

  pduSessionResourceFailedToSetupFailureList
      ->setPDUSessionResourceFailedToSetupListCxtFail(
          m_pduSessionResourceFailedToSetupItemCxtFail, list.size());

  Ngap_InitialContextSetupFailureIEs_t* ie =
      (Ngap_InitialContextSetupFailureIEs_t*) calloc(
          1, sizeof(Ngap_InitialContextSetupFailureIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtFail;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_InitialContextSetupFailureIEs__value_PR_PDUSessionResourceFailedToSetupListCxtFail;

  int ret =
      pduSessionResourceFailedToSetupFailureList
          ->encode2PDUSessionResourceFailedToSetupListCxtFail(
              &ie->value.choice.PDUSessionResourceFailedToSetupListCxtFail);
  if (!ret) {
    cout << "encode PDUSessionResourceFailedToSetupListCxtFail IE error"
         << endl;
    return;
  }

  ret = ASN_SEQUENCE_ADD(&initialContextSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0)
    cout << "encode PDUSessionResourceFailedToSetupListCxtFail IE error"
         << endl;
}

//------------------------------------------------------------------------------
int InitialContextSetupFailureMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, initialContextSetupFailurePdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, initialContextSetupFailurePdu, buf,
      buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool InitialContextSetupFailureMsg::decodefrompdu(
    Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  initialContextSetupFailurePdu = ngap_msg_pdu;

  if (initialContextSetupFailurePdu->present ==
      Ngap_NGAP_PDU_PR_unsuccessfulOutcome) {
    if (initialContextSetupFailurePdu->choice.unsuccessfulOutcome &&
        initialContextSetupFailurePdu->choice.unsuccessfulOutcome
                ->procedureCode == Ngap_ProcedureCode_id_InitialContextSetup &&
        initialContextSetupFailurePdu->choice.unsuccessfulOutcome
                ->criticality == Ngap_Criticality_reject &&
        initialContextSetupFailurePdu->choice.unsuccessfulOutcome->value
                .present ==
            Ngap_UnsuccessfulOutcome__value_PR_InitialContextSetupFailure) {
      initialContextSetupFailureIEs =
          &initialContextSetupFailurePdu->choice.unsuccessfulOutcome->value
               .choice.InitialContextSetupFailure;
    } else {
      cout << "Check InitialContextSetupFailure message error!!!" << endl;
      return false;
    }
  } else {
    cout << "MessageType error!!!" << endl;
    return false;
  }
  for (int i = 0; i < initialContextSetupFailureIEs->protocolIEs.list.count;
       i++) {
    switch (initialContextSetupFailureIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  initialContextSetupFailureIEs->protocolIEs.list.array[i]
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
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  initialContextSetupFailureIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceFailedToSetupListCxtFail: {
        if (initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            initialContextSetupFailureIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_InitialContextSetupFailureIEs__value_PR_PDUSessionResourceFailedToSetupListCxtFail) {
          pduSessionResourceFailedToSetupFailureList =
              new PDUSessionResourceFailedToSetupListCxtFail();
          if (!pduSessionResourceFailedToSetupFailureList
                   ->decodefromPDUSessionResourceFailedToSetupListCxtFail(
                       &initialContextSetupFailureIEs->protocolIEs.list
                            .array[i]
                            ->value.choice
                            .PDUSessionResourceFailedToSetupListCxtFail)) {
            cout << "decoded ngap PDUSessionResourceFailedToSetupListCxtFail "
                    "IE error"
                 << endl;
            return false;
          }
        } else {
          cout << "decoded ngap PDUSessionResourceFailedToSetupListCxtFail IE "
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
unsigned long InitialContextSetupFailureMsg::getAmfUeNgapId() {
  if (!amfUeNgapId) return -1;
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

//------------------------------------------------------------------------------
uint32_t InitialContextSetupFailureMsg::getRanUeNgapId() {
  if (!ranUeNgapId) return -1;
  return ranUeNgapId->getRanUeNgapId();
}

//------------------------------------------------------------------------------
bool InitialContextSetupFailureMsg::getPduSessionResourceFailedToSetupList(
    std::vector<PDUSessionResourceFailedToSetupItem_t>& list) {
  if (!pduSessionResourceFailedToSetupFailureList) return false;

  PDUSessionResourceFailedToSetupItemCxtFail*
      m_pduSessionResourceFailedToSetupItemCxtFail;
  int num = 0;
  pduSessionResourceFailedToSetupFailureList
      ->getPDUSessionResourceFailedToSetupListCxtFail(
          m_pduSessionResourceFailedToSetupItemCxtFail, num);

  for (int i = 0; i < num; i++) {
    PDUSessionResourceFailedToSetupItem_t failedToFailure;

    PDUSessionID* m_pDUSessionID;
    m_pduSessionResourceFailedToSetupItemCxtFail[i]
        .getPDUSessionResourceFailedToSetupItemCxtFail(
            m_pDUSessionID,
            failedToFailure.pduSessionResourceSetupUnsuccessfulTransfer);
    m_pDUSessionID->getPDUSessionID(failedToFailure.pduSessionId);

    list.push_back(failedToFailure);
  }

  return true;
}

}  // namespace ngap
