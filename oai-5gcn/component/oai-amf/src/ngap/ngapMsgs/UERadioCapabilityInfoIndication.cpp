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

#include "UERadioCapabilityInfoIndication.hpp"

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
UeRadioCapabilityInfoIndicationMsg::UeRadioCapabilityInfoIndicationMsg() {
  ueRadioCapabilityInfoIndicationPdu = NULL;
  ueRadioCapabilityInfoIndicationIEs = NULL;
  amfUeNgapId                        = NULL;
  ranUeNgapId                        = NULL;
  ueRadioCapability                  = NULL;
  ueRadioCapabilityForPaging         = NULL;
}

//------------------------------------------------------------------------------
UeRadioCapabilityInfoIndicationMsg::~UeRadioCapabilityInfoIndicationMsg() {}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setMessageType() {
  if (!ueRadioCapabilityInfoIndicationPdu)
    ueRadioCapabilityInfoIndicationPdu =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType UeRadioCapabilityInfoIndicationPduTypeIE;
  UeRadioCapabilityInfoIndicationPduTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_UERadioCapabilityInfoIndication);
  UeRadioCapabilityInfoIndicationPduTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_initiatingMessage);
  UeRadioCapabilityInfoIndicationPduTypeIE.setCriticality(
      Ngap_Criticality_ignore);
  UeRadioCapabilityInfoIndicationPduTypeIE.setValuePresent(
      Ngap_InitiatingMessage__value_PR_UERadioCapabilityInfoIndication);

  if (UeRadioCapabilityInfoIndicationPduTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_UERadioCapabilityInfoIndication &&
      UeRadioCapabilityInfoIndicationPduTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_initiatingMessage &&
      UeRadioCapabilityInfoIndicationPduTypeIE.getCriticality() ==
          Ngap_Criticality_ignore) {
    UeRadioCapabilityInfoIndicationPduTypeIE.encode2pdu(
        ueRadioCapabilityInfoIndicationPdu);
    ueRadioCapabilityInfoIndicationIEs =
        &(ueRadioCapabilityInfoIndicationPdu->choice.initiatingMessage->value
              .choice.UERadioCapabilityInfoIndication);
  } else {
    cout << "[warning] This information doesn't refer to "
            "UERadioCapabilityInfoIndication Message!!!"
         << endl;
  }
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_UERadioCapabilityInfoIndicationIEs_t* ie =
      (Ngap_UERadioCapabilityInfoIndicationIEs_t*) calloc(
          1, sizeof(Ngap_UERadioCapabilityInfoIndicationIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AMF_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setRanUeNgapId(
    uint32_t ran_ue_ngap_id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

  Ngap_UERadioCapabilityInfoIndicationIEs_t* ie =
      (Ngap_UERadioCapabilityInfoIndicationIEs_t*) calloc(
          1, sizeof(Ngap_UERadioCapabilityInfoIndicationIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    cout << "encode RAN_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode RAN_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setUERadioCapability(
    uint8_t* buf, size_t size) {
  if (!ueRadioCapability) ueRadioCapability = new UERadioCapability();

  ueRadioCapability->setUERadioCapability(buf, size);

  Ngap_UERadioCapabilityInfoIndicationIEs_t* ie =
      (Ngap_UERadioCapabilityInfoIndicationIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UERadioCapability;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapability;

  int ret = ueRadioCapability->encode2UERadioCapability(
      ie->value.choice.UERadioCapability);
  if (!ret) {
    cout << "encode UERadioCapability IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UERadioCapability IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void UeRadioCapabilityInfoIndicationMsg::setUERadioCapabilityForPaging(
    uint8_t* nr, size_t sizeofnr, uint8_t* eutra, size_t sizeofeutra) {
  if (!ueRadioCapabilityForPaging)
    ueRadioCapabilityForPaging = new UERadioCapabilityForPaging();
  UERadioCapabilityForPagingOfNR* m_ueRadioCapabilityForPagingOfNR       = NULL;
  UERadioCapabilityForPagingOfEUTRA* m_ueRadioCapabilityForPagingOfEUTRA = NULL;
  if (nr && sizeofnr > 0) {
    m_ueRadioCapabilityForPagingOfNR = new UERadioCapabilityForPagingOfNR();
    m_ueRadioCapabilityForPagingOfNR->setUERadioCapabilityForPagingOfNR(
        nr, sizeofnr);
  }
  if (eutra && sizeofeutra > 0) {
    m_ueRadioCapabilityForPagingOfEUTRA =
        new UERadioCapabilityForPagingOfEUTRA();
    m_ueRadioCapabilityForPagingOfEUTRA->setUERadioCapabilityForPagingOfEUTRA(
        eutra, sizeofeutra);
  }
  ueRadioCapabilityForPaging->setUERadioCapabilityForPaging(
      m_ueRadioCapabilityForPagingOfNR, m_ueRadioCapabilityForPagingOfEUTRA);

  Ngap_UERadioCapabilityInfoIndicationIEs_t* ie =
      (Ngap_UERadioCapabilityInfoIndicationIEs_t*) calloc(
          1, sizeof(Ngap_UERadioCapabilityInfoIndicationIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UERadioCapabilityForPaging;
  ie->criticality = Ngap_Criticality_ignore;
  ie->value.present =
      Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapabilityForPaging;

  int ret = ueRadioCapabilityForPaging->encode2UERadioCapabilityForPaging(
      &ie->value.choice.UERadioCapabilityForPaging);
  if (!ret) {
    cout << "encode UERadioCapabilityForPaging IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UERadioCapabilityForPaging IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int UeRadioCapabilityInfoIndicationMsg::encode2buffer(
    uint8_t* buf, int buf_size) {
  asn_fprint(
      stderr, &asn_DEF_Ngap_NGAP_PDU, ueRadioCapabilityInfoIndicationPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, ueRadioCapabilityInfoIndicationPdu, buf,
      buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool UeRadioCapabilityInfoIndicationMsg::decodefrompdu(
    Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  ueRadioCapabilityInfoIndicationPdu = ngap_msg_pdu;

  if (ueRadioCapabilityInfoIndicationPdu->present ==
      Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ueRadioCapabilityInfoIndicationPdu->choice.initiatingMessage &&
        ueRadioCapabilityInfoIndicationPdu->choice.initiatingMessage
                ->procedureCode ==
            Ngap_ProcedureCode_id_UERadioCapabilityInfoIndication &&
        ueRadioCapabilityInfoIndicationPdu->choice.initiatingMessage
                ->criticality == Ngap_Criticality_ignore &&
        ueRadioCapabilityInfoIndicationPdu->choice.initiatingMessage->value
                .present ==
            Ngap_InitiatingMessage__value_PR_UERadioCapabilityInfoIndication) {
      ueRadioCapabilityInfoIndicationIEs =
          &ueRadioCapabilityInfoIndicationPdu->choice.initiatingMessage->value
               .choice.UERadioCapabilityInfoIndication;
    } else {
      cout << "Check UERadioCapabilityInfoIndication message error!!!" << endl;
      return false;
    }
  } else {
    cout << "MessageType error!!!" << endl;
    return false;
  }
  for (int i = 0;
       i < ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.count; i++) {
    switch (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
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
        if (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_reject &&
            ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UERadioCapability: {
        if (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapability) {
          ueRadioCapability = new UERadioCapability();
          if (!ueRadioCapability->decodefromUERadioCapability(
                  ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                      ->value.choice.UERadioCapability)) {
            cout << "decoded ngap UERadioCapability IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap UERadioCapability IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UERadioCapabilityForPaging: {
        if (ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->criticality == Ngap_Criticality_ignore &&
            ueRadioCapabilityInfoIndicationIEs->protocolIEs.list.array[i]
                    ->value.present ==
                Ngap_UERadioCapabilityInfoIndicationIEs__value_PR_UERadioCapabilityForPaging) {
          ueRadioCapabilityForPaging = new UERadioCapabilityForPaging();
          if (!ueRadioCapabilityForPaging->decodefromUERadioCapabilityForPaging(
                  &ueRadioCapabilityInfoIndicationIEs->protocolIEs.list
                       .array[i]
                       ->value.choice.UERadioCapabilityForPaging)) {
            cout << "decoded ngap UERadioCapabilityForPaging IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap UERadioCapabilityForPaging IE error" << endl;
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
bool UeRadioCapabilityInfoIndicationMsg::getAmfUeNgapId(unsigned long& id) {
  if (!amfUeNgapId) return false;
  id = amfUeNgapId->getAMF_UE_NGAP_ID();
  return true;
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityInfoIndicationMsg::getRanUeNgapId(uint32_t& id) {
  if (!ranUeNgapId) return false;
  id = ranUeNgapId->getRanUeNgapId();
  return true;
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityInfoIndicationMsg::getUERadioCapability(
    uint8_t*& buf, size_t& size) {
  if (!ueRadioCapability) return false;
  if (!ueRadioCapability->getUERadioCapability(buf, size)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool UeRadioCapabilityInfoIndicationMsg::getUERadioCapabilityForPaging(
    uint8_t*& nr, size_t& sizeofnr, uint8_t*& eutra, size_t& sizeofeutra) {
  if (!ueRadioCapabilityForPaging) return false;
  UERadioCapabilityForPagingOfNR* m_ueRadioCapabilityForPagingOfNR;
  UERadioCapabilityForPagingOfEUTRA* m_ueRadioCapabilityForPagingOfEUTRA;
  if (!ueRadioCapabilityForPaging->getUERadioCapabilityForPaging(
          m_ueRadioCapabilityForPagingOfNR,
          m_ueRadioCapabilityForPagingOfEUTRA))
    return false;

  if (m_ueRadioCapabilityForPagingOfNR) {
    if (!m_ueRadioCapabilityForPagingOfNR->getUERadioCapabilityForPagingOfNR(
            nr, sizeofnr))
      return false;
  }
  if (m_ueRadioCapabilityForPagingOfEUTRA) {
    if (!m_ueRadioCapabilityForPagingOfEUTRA
             ->getUERadioCapabilityForPagingOfEUTRA(eutra, sizeofeutra))
      return false;
  }

  return true;
}

}  // namespace ngap
