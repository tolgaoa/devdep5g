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
 \date 2021
 \email: contact@openairinterface.org
 */

#include "NGReset.hpp"

extern "C" {
#include "Ngap_NGAP-PDU.h"
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

//------------------------------------------------------------------------------
NGResetMsg::NGResetMsg() {
  ngResetPdu = nullptr;
  ngResetIEs = nullptr;
  cause      = nullptr;
  resetType  = nullptr;
}
//------------------------------------------------------------------------------
NGResetMsg::~NGResetMsg() {}

//------------------------------------------------------------------------------
void NGResetMsg::setMessageType() {
  if (!ngResetPdu)
    ngResetPdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType NgResetMessageTypeIE;
  NgResetMessageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGReset);
  NgResetMessageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_initiatingMessage);
  NgResetMessageTypeIE.setValuePresent(
      Ngap_InitiatingMessage__value_PR_NGReset);

  if (NgResetMessageTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_NGReset &&
      NgResetMessageTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_initiatingMessage) {
    NgResetMessageTypeIE.encode2pdu(ngResetPdu);
    ngResetIEs = &(ngResetPdu->choice.initiatingMessage->value.choice.NGReset);
  } else {
    cout << "[Warning] This information doesn't refer to NGReset "
            "Message!!!"
         << endl;
  }
}

//------------------------------------------------------------------------------
void NGResetMsg::setCause(Ngap_Cause_t cause) {
  // this->cause = cause;
}

//------------------------------------------------------------------------------
void NGResetMsg::setResetType(Ngap_ResetType_t resetType) {
  // is->resetType = resetType;
}

void NGResetMsg::getCause(Ngap_Cause_t& cause) {
  // cause = this->cause;
}

void NGResetMsg::getResetType(Ngap_ResetType_t& resetType) {
  // resetType = this->resetType;
}

bool NGResetMsg::getResetType(ResetType& resetType) {
  if (!this->resetType) return false;

  if (this->resetType->getResetType() == Ngap_ResetType_PR_nG_Interface) {
    long ng_interface = 0;
    this->resetType->getResetType(ng_interface);
    resetType.setResetType(ng_interface);
  } else if (
      this->resetType->getResetType() == Ngap_ResetType_PR_partOfNG_Interface) {
    // TODO
  }
  return true;
}

//------------------------------------------------------------------------------
int NGResetMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, ngResetPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, ngResetPdu, buf, buf_size);
  printf("er.encoded(%ld)\n", er.encoded);
  return er.encoded;
}

//------------------------------------------------------------------------------
bool NGResetMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  ngResetPdu = ngap_msg_pdu;

  if (ngResetPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngResetPdu->choice.initiatingMessage &&
        ngResetPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_NGReset &&
        ngResetPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngResetPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_NGReset) {
      ngResetIEs = &ngResetPdu->choice.initiatingMessage->value.choice.NGReset;
      for (int i = 0; i < ngResetIEs->protocolIEs.list.count; i++) {
        switch (ngResetIEs->protocolIEs.list.array[i]->id) {
          case Ngap_ProtocolIE_ID_id_Cause: {
            if (ngResetIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                ngResetIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGResetIEs__value_PR_Cause) {
              cause = new Cause();
              if (!cause->decodefromCause(
                      &ngResetIEs->protocolIEs.list.array[i]
                           ->value.choice.Cause)) {
                cout << "Decoded NGAP Cause IE error" << endl;
                return false;
              }
            } else {
              cout << "Decoded NGAP Cause IE error" << endl;
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_ResetType: {
            if (ngResetIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                ngResetIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGResetIEs__value_PR_ResetType) {
              resetType = new ResetType();
              if (!resetType->decode(&ngResetIEs->protocolIEs.list.array[i]
                                          ->value.choice.ResetType)) {
                cout << "Decoded NGAP ResetType IE error" << endl;
                return false;
              }

            } else {
              cout << "Decoded NGAP ResetType IE error" << endl;
              return false;
            }
          } break;

          default: {
            cout << "Decoded NGAP message PDU error" << endl;
            return false;
          }
        }
      }
    } else {
      cout << "Check NGReset message error!!!";
      return false;
    }
  } else {
    cout << "Check NGReset message error!!!";
    return false;
  }
  return true;
}

}  // namespace ngap
