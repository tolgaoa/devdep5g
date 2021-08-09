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

#include "NGSetupFailure.hpp"

extern "C" {
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
NGSetupFailureMsg::NGSetupFailureMsg() {
  ngSetupFailurePdu = NULL;
  ngSetupFailureIEs = NULL;
  cause             = NULL;
  timeToWait        = NULL;
  // criticalityDiagnostics = NULL;
}

//------------------------------------------------------------------------------
NGSetupFailureMsg::~NGSetupFailureMsg() {}

//------------------------------------------------------------------------------
// Encapsulation
void NGSetupFailureMsg::setMessageType() {
  if (!ngSetupFailurePdu)
    ngSetupFailurePdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType NgSetupFailureMessageTypeIE;
  NgSetupFailureMessageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGSetup);
  NgSetupFailureMessageTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_unsuccessfulOutcome);
  NgSetupFailureMessageTypeIE.setValuePresent(
      Ngap_UnsuccessfulOutcome__value_PR_NGSetupFailure);

  if (NgSetupFailureMessageTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_NGSetup &&
      NgSetupFailureMessageTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_unsuccessfulOutcome) {
    NgSetupFailureMessageTypeIE.encode2pdu(ngSetupFailurePdu);
    ngSetupFailureIEs = &(ngSetupFailurePdu->choice.unsuccessfulOutcome->value
                              .choice.NGSetupFailure);
  } else {
    cout << "[warning] This information doesn't refer to NGSetupFailure "
            "Message!!!"
         << endl;
  }
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::addCauseIE() {
  Ngap_NGSetupFailureIEs_t* ie =
      (Ngap_NGSetupFailureIEs_t*) calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupFailureIEs__value_PR_Cause;

  cause->encode2Cause(&ie->value.choice.Cause);

  int ret = ASN_SEQUENCE_ADD(&ngSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode Cause IE error" << endl;
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::addTimeToWaitIE() {
  Ngap_NGSetupFailureIEs_t* ie =
      (Ngap_NGSetupFailureIEs_t*) calloc(1, sizeof(Ngap_NGSetupFailureIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_TimeToWait;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupFailureIEs__value_PR_TimeToWait;

  timeToWait->encode2TimeToWait(&ie->value.choice.TimeToWait);

  int ret = ASN_SEQUENCE_ADD(&ngSetupFailureIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode TimeToWait IE error" << endl;
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork cause_value, e_Ngap_TimeToWait time_to_wait) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  cause->setValue(cause_value);
  addCauseIE();

  if (!timeToWait) timeToWait = new TimeToWait();
  timeToWait->setValue(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork cause_value) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  cause->setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseTransport(
    e_Ngap_CauseTransport cause_value, e_Ngap_TimeToWait time_to_wait) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_transport);
  cause->setValue(cause_value);
  addCauseIE();

  if (!timeToWait) timeToWait = new TimeToWait();
  timeToWait->setValue(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseTransport(e_Ngap_CauseTransport cause_value) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_transport);
  cause->setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseNas(
    e_Ngap_CauseNas cause_value, e_Ngap_TimeToWait time_to_wait) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_nas);
  cause->setValue(cause_value);
  addCauseIE();

  if (!timeToWait) timeToWait = new TimeToWait();
  timeToWait->setValue(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseNas(e_Ngap_CauseNas cause_value) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_nas);
  cause->setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseProtocol(
    e_Ngap_CauseProtocol cause_value, e_Ngap_TimeToWait time_to_wait) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_protocol);
  cause->setValue(cause_value);
  addCauseIE();

  if (!timeToWait) timeToWait = new TimeToWait();
  timeToWait->setValue(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseProtocol(e_Ngap_CauseProtocol cause_value) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_protocol);
  cause->setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseMisc(
    e_Ngap_CauseMisc cause_value, e_Ngap_TimeToWait time_to_wait) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_misc);
  cause->setValue(cause_value);
  addCauseIE();

  if (!timeToWait) timeToWait = new TimeToWait();
  timeToWait->setValue(time_to_wait);
  addTimeToWaitIE();
}

//------------------------------------------------------------------------------
void NGSetupFailureMsg::setCauseMisc(e_Ngap_CauseMisc cause_value) {
  if (!cause) cause = new Cause();
  cause->setChoiceOfCause(Ngap_Cause_PR_misc);
  cause->setValue(cause_value);
  addCauseIE();
}

//------------------------------------------------------------------------------
int NGSetupFailureMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, ngSetupFailurePdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, ngSetupFailurePdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool NGSetupFailureMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  ngSetupFailurePdu = ngap_msg_pdu;
  if (ngSetupFailurePdu->present == Ngap_NGAP_PDU_PR_unsuccessfulOutcome) {
    if (ngSetupFailurePdu->choice.unsuccessfulOutcome &&
        ngSetupFailurePdu->choice.unsuccessfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngSetupFailurePdu->choice.unsuccessfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngSetupFailurePdu->choice.unsuccessfulOutcome->value.present ==
            Ngap_UnsuccessfulOutcome__value_PR_NGSetupFailure) {
      ngSetupFailureIEs = &ngSetupFailurePdu->choice.initiatingMessage->value
                               .choice.NGSetupFailure;
    } else {
      cout << "Check NGSetupFailure message error!!!" << endl;
      return false;
    }
  } else {
    cout << "MessageType error!!!" << endl;
    return false;
  }
  for (int i = 0; i < ngSetupFailureIEs->protocolIEs.list.count; i++) {
    switch (ngSetupFailureIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_Cause: {
        if (ngSetupFailureIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ngSetupFailureIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupFailureIEs__value_PR_Cause) {
          cause = new Cause();
          if (!cause->decodefromCause(
                  &ngSetupFailureIEs->protocolIEs.list.array[i]
                       ->value.choice.Cause)) {
            cout << "decoded ngap Cause IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap Cause IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TimeToWait: {
        if (ngSetupFailureIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ngSetupFailureIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupFailureIEs__value_PR_TimeToWait) {
          timeToWait = new TimeToWait();
          if (!timeToWait->decodefromTimeToWait(
                  &ngSetupFailureIEs->protocolIEs.list.array[i]
                       ->value.choice.TimeToWait)) {
            cout << "decoded ngap TimeToWait IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap TimeToWait IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
        cout << "decoded ngap: This is CriticalityDiagnostics IE" << endl;
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
bool NGSetupFailureMsg::getCauseType(Ngap_Cause_PR& causePresent) {
  if (cause->getChoiceOfCause() < 0) {
    return false;
  }
  causePresent = cause->getChoiceOfCause();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork& causeRadioNetwork) {
  if (cause->getValue() < 0) {
    return false;
  }
  causeRadioNetwork = (e_Ngap_CauseRadioNetwork) cause->getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseTransport(
    e_Ngap_CauseTransport& causeTransport) {
  if (cause->getValue() < 0) {
    return false;
  }
  causeTransport = (e_Ngap_CauseTransport) cause->getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseNas(e_Ngap_CauseNas& causeNas) {
  if (cause->getValue() < 0) {
    return false;
  }
  causeNas = (e_Ngap_CauseNas) cause->getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseProtocol(e_Ngap_CauseProtocol& causeProtocol) {
  if (cause->getValue() < 0) {
    return false;
  }
  causeProtocol = (e_Ngap_CauseProtocol) cause->getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getCauseMisc(e_Ngap_CauseMisc& causeMisc) {
  if (cause->getValue() < 0) {
    return false;
  }
  causeMisc = (e_Ngap_CauseMisc) cause->getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupFailureMsg::getTime2Wait(e_Ngap_TimeToWait& time) {
  if (timeToWait->getValue() < 0) {
    return false;
  }
  time = (e_Ngap_TimeToWait) timeToWait->getValue();
  return true;
}

}  // namespace ngap
