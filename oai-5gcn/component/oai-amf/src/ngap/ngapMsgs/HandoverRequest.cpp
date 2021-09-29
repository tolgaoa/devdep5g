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

#include "HandoverRequest.hpp"

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

HandoverRequest::HandoverRequest() {
  amfUeNgapId                         = NULL;
  handovertype                        = NULL;
  cause                               = NULL;
  ueAggregateMaximumBitRate           = NULL;
  ueSecurityCapabilities              = NULL;
  SecurityContext                     = NULL;
  PDUSessionResourceSetupList         = NULL;
  allowedNSSAI                        = NULL;
  SourceToTarget_TransparentContainer = NULL;
  mobilityrestrictionlist             = NULL;
  guami                               = NULL;
  handoverRequestPdu                  = NULL;
  handoverRequestIEs                  = NULL;
}
HandoverRequest::~HandoverRequest() {}

unsigned long HandoverRequest::getAmfUeNgapId() {
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}

/*bool HandoverRequest::decodefrompdu(Ngap_NGAP_PDU_t *ngap_msg_pdu)
{
        handoverCommandPdu = ngap_msg_pdu;

        if (handoverCommandPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome)
        {
                if (handoverCommandPdu->choice.successfulOutcome &&
handoverCommandPdu->choice.successfulOutcome->procedureCode ==
Ngap_ProcedureCode_id_HandoverPreparation &&
handoverCommandPdu->choice.successfulOutcome->criticality ==
Ngap_Criticality_reject &&
handoverRequiredPdu->choice.successfulOutcome->value.present ==
Ngap_SuccessfulOutcome__value_PR_HandoverCommand)
                {
                        handoverCommandIEs =
&handoverCommandIEs->choice.successfulOutcome->value.choice.HandoverCommand;
                }
                else
                {
                        cout << "Check HandoverCommand message error!!!" <<
endl; return false;
                }
        }
        else
        {
                cout << "HandoverRequired MessageType error!!!" << endl;
                return false;
        }
        for (int i = 0; i < handoverCommandIEs->protocolIEs.list.count; i++)
        {
                switch (handoverCommandIEs->protocolIEs.list.array[i]->id)
                {
                case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
                        if
(handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
Ngap_Criticality_reject &&
handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
Ngap_HandoverCommandIEs__value_PR_AMF_UE_NGAP_ID)
                        {
                                amfUeNgapId = new AMF_UE_NGAP_ID();
                                if
(!amfUeNgapId->decodefromAMF_UE_NGAP_ID(handoverCommandIEs->protocolIEs.list.array[i]->value.choice.AMF_UE_NGAP_ID))
                                {
                                        cout << "decoded ngap AMF_UE_NGAP_ID IE
error" << endl; return false;
                                }
                        }
                        else
                        {
                                cout << "decoded ngap AMF_UE_NGAP_ID IE error"
<< endl; return false;
                        }
                }break;
                case Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID: {
                        if
(handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
Ngap_Criticality_reject &&
handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
Ngap_HandoverCommandIEs__value_PR_RAN_UE_NGAP_ID)
                        {
                                ranUeNgapId = new RAN_UE_NGAP_ID();
                                if
(!ranUeNgapId->decodefromRAN_UE_NGAP_ID(handoverCommandIEs->protocolIEs.list.array[i]->value.choice.RAN_UE_NGAP_ID))
                                {
                                        cout << "decoded ngap RAN_UE_NGAP_ID IE
error" << endl; return false;
                                }
                        }
                        else
                        {
                                cout << "decoded ngap RAN_UE_NGAP_ID IE error"
<< endl; return false;
                        }
                }break;
                case Ngap_ProtocolIE_ID_id_HandoverType: {
                        if
(handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
Ngap_Criticality_reject &&
handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
Ngap_HandoverCommandIEs__value_PR_HandoverType)
                        {
                                ngap_handovertype = new Ngap_HandoverType_t();
                                ngap_handovertype =
handoverCommandIEs->protocolIEs.list.array[i]->value.choice.HandoverType;
                        }
                        else
                        {
                                cout << "decoded ngap Handover Type IE error" <<
endl; return false;
                        }
                }break;
                case Ngap_ProtocolIE_ID_id_PDUSessionResourceHandoverList: {
                        if
(handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
Ngap_Criticality_ignore &&
handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceHandoverList)
                        {
                        }
                        else
                        {
                                cout << "decoded ngap
PDUSessionResourceHandoverList IE error" << endl; return false;
                        }
                }break;
                case Ngap_ProtocolIE_ID_id_PDUSessionResourceToReleaseListHOCmd:
{ if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
Ngap_Criticality_ignore &&
handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
Ngap_HandoverCommandIEs__value_PR_PDUSessionResourceToReleaseListHOCmd)
                        {

                        }
                        else
                        {
                                cout << "decoded ngap
PDUSessionResourceToReleaseListHOCmd IE error" << endl; return false;
                        }
                }break;
                case Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer:
{ if (handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
Ngap_Criticality_reject &&
handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
Ngap_HandoverCommandIEs__value_PR_TargetToSource_TransparentContainer)
                        {
                        }
                        else
                        {
                                cout << "decoded ngap
TargetToSource_TransparentContainer IE error" << endl; return false;
                        }
                }break;
                case Ngap_ProtocolIE_ID_id_CriticalityDiagnostics: {
                        if
(handoverCommandIEs->protocolIEs.list.array[i]->criticality ==
Ngap_Criticality_ignore &&
handoverCommandIEs->protocolIEs.list.array[i]->value.present ==
Ngap_HandoverCommandIEs__value_PR_CriticalityDiagnostics)
                        {
                        }
                        else
                        {
                                cout << "decoded ngap CriticalityDiagnostics IE
error" << endl; return false;
                        }
                }break;
                default: {
                        cout << "decoded ngap message pdu error" << endl;
                        return false;
                }
                }
        }

        return true;
}*/

int HandoverRequest::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, handoverRequestPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, handoverRequestPdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

void HandoverRequest::setMessageType() {
  if (!handoverRequestPdu)
    handoverRequestPdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType handoverRequestMessageTypeIE;
  handoverRequestMessageTypeIE.setProcedureCode(
      Ngap_ProcedureCode_id_HandoverResourceAllocation);
  handoverRequestMessageTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_initiatingMessage);
  handoverRequestMessageTypeIE.setCriticality(Ngap_Criticality_reject);
  handoverRequestMessageTypeIE.setValuePresent(
      Ngap_InitiatingMessage__value_PR_HandoverRequest);

  if (handoverRequestMessageTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_HandoverResourceAllocation &&
      handoverRequestMessageTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_initiatingMessage) {
    handoverRequestMessageTypeIE.encode2pdu(handoverRequestPdu);
    handoverRequestIEs = &(handoverRequestPdu->choice.initiatingMessage->value
                               .choice.HandoverRequest);
  } else {
    cout << "[warning] This information doesn't refer to HandoverRequest "
            "Message!!!"
         << endl;
  }
}
void HandoverRequest::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AMF_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverRequest::setHandoverType(long type)  // 0--intra5gs
{
  if (!handovertype) handovertype = new Ngap_HandoverType_t();
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_HandoverType;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_HandoverType;
  ie->value.choice.HandoverType = type;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode HandoverType IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverRequest::setCause(Ngap_Cause_PR m_causePresent, long value)  //
{
  if (!cause) cause = new Cause;
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_Cause;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_Cause;

  cause->setChoiceOfCause(m_causePresent);
  cause->setValue(value);
  cause->encode2Cause(&(ie->value.choice.Cause));
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode Cause IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverRequest::setUEAggregateMaximumBitRate(
    long bit_rate_downlink, long bit_rate_uplink) {
  if (!ueAggregateMaximumBitRate)
    ueAggregateMaximumBitRate = new UEAggregateMaxBitRate;

  ueAggregateMaximumBitRate->setUEAggregateMaxBitRate(
      bit_rate_downlink, bit_rate_uplink);

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UEAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestIEs__value_PR_UEAggregateMaximumBitRate;
  ueAggregateMaximumBitRate->encode2UEAggregateMaxBitRate(
      ie->value.choice.UEAggregateMaximumBitRate);

  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UEAggregateMaximumBitRate IE error" << endl;
  // free_wrapper((void**) &ie);
}
void HandoverRequest::setUESecurityCapabilities(
    uint16_t m_NR_EncryptionAlgs, uint16_t m_NR_IntegrityProtectionAlgs,
    uint16_t m_E_UTRA_EncryptionAlgs,
    uint16_t m_E_UTRA_IntegrityProtectionAlgs) {
  if (!ueSecurityCapabilities)
    ueSecurityCapabilities = new UESecurityCapabilities;

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_UESecurityCapabilities;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_UESecurityCapabilities;
  ueSecurityCapabilities->setUESecurityCapabilities(
      m_NR_EncryptionAlgs, m_NR_IntegrityProtectionAlgs,
      m_E_UTRA_EncryptionAlgs, m_E_UTRA_IntegrityProtectionAlgs);
  ueSecurityCapabilities->encode2UESecurityCapabilities(
      (ie->value.choice.UESecurityCapabilities));

  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UESecurityCapabilities IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverRequest::setGUAMI(
    PlmnId* m_plmnId, AMFRegionID* m_aMFRegionID, AMFSetID* m_aMFSetID,
    AMFPointer* m_aMFPointer) {
  if (!guami) guami = new GUAMI;

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GUAMI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_GUAMI;
  guami->setGUAMI(m_plmnId, m_aMFRegionID, m_aMFSetID, m_aMFPointer);
  guami->encode2GUAMI(&(ie->value.choice.GUAMI));

  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode GUAMI IE error" << endl;
  // free_wrapper((void**) &ie);
}
void HandoverRequest::setAllowedNSSAI(std::vector<S_NSSAI> list) {
  if (!allowedNSSAI) allowedNSSAI = new Ngap_AllowedNSSAI_t();

  for (int i = 0; i < list.size(); i++) {
    Ngap_AllowedNSSAI_Item_t* item =
        (Ngap_AllowedNSSAI_Item_t*) calloc(1, sizeof(Ngap_AllowedNSSAI_Item_t));
    // item->s_NSSAI = list[i].s_NSSAI;
    // S_NSSAI *s_NSSAI = new S_NSSAI();
    list[i].encode2S_NSSAI(&item->s_NSSAI);
    int ret = ASN_SEQUENCE_ADD(&allowedNSSAI->list, item);
    if (ret != 0)
      cout << "encode PDUSessionResourceHandoverListItem IE error" << endl;
  }
  asn_fprint(stderr, &asn_DEF_Ngap_AllowedNSSAI, allowedNSSAI);
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AllowedNSSAI;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_AllowedNSSAI;
  ie->value.choice.AllowedNSSAI = *allowedNSSAI;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AllowedNSSAI IE error" << endl;
  // free_wrapper((void**) &ie);
}
void HandoverRequest::setSecurityContext(long count, uint8_t* buffer) {
  if (!SecurityContext) SecurityContext = new Ngap_SecurityContext_t();

  SecurityKey securitykey;
  securitykey.setSecurityKey(buffer);
  securitykey.encode2bitstring(SecurityContext->nextHopNH);
  SecurityContext->nextHopChainingCount = count;

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_SecurityContext;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_SecurityContext;
  ie->value.choice.SecurityContext = *SecurityContext;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode SecurityContext IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverRequest::setPduSessionResourceSetupList(
    std::vector<PDUSessionResourceSetupRequestItem_t> list) {
  if (!PDUSessionResourceSetupList)
    PDUSessionResourceSetupList = new PDUSessionResourceSetupListHOReq();
  PDUSessionResourceSetupItemHOReq* m_pduSessionResourceSetupItemHOReq =
      new PDUSessionResourceSetupItemHOReq[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    PDUSessionID* m_pDUSessionID = new PDUSessionID();
    m_pDUSessionID->setPDUSessionID(list[i].pduSessionId);
    S_NSSAI* m_s_NSSAI = new S_NSSAI();
    m_s_NSSAI->setSst(list[i].s_nssai.sst);
    if (list[i].s_nssai.sd.size()) m_s_NSSAI->setSd(list[i].s_nssai.sd);
    m_pduSessionResourceSetupItemHOReq[i].setPDUSessionResourceSetupItemHOReq(
        m_pDUSessionID, m_s_NSSAI,
        list[i].pduSessionResourceSetupRequestTransfer);
  }

  PDUSessionResourceSetupList->setPDUSessionResourceSetupListHOReq(
      m_pduSessionResourceSetupItemHOReq, list.size());

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionResourceSetupListHOReq;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestIEs__value_PR_PDUSessionResourceSetupListHOReq;

  int ret =
      PDUSessionResourceSetupList->encode2PDUSessionResourceSetupListHOReq(
          &ie->value.choice.PDUSessionResourceSetupListHOReq);
  if (!ret) {
    cout << "encode PDUSessionResourceSetupListSUReq IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    cout << "encode PDUSessionResourceSetupListSUReq IE error" << endl;
  // free_wrapper((void**) &ie);
}

void HandoverRequest::setSourceToTarget_TransparentContainer(
    OCTET_STRING_t sourceTotarget) {
  if (!SourceToTarget_TransparentContainer)
    SourceToTarget_TransparentContainer =
        new Ngap_SourceToTarget_TransparentContainer_t();

  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SourceToTarget_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_HandoverRequestIEs__value_PR_SourceToTarget_TransparentContainer;
  ie->value.choice.SourceToTarget_TransparentContainer = sourceTotarget;
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0)
    cout << "encode SourceToTarget_TransparentContainer IE error" << endl;
  // free_wrapper((void**) &ie);
}
void HandoverRequest::setMobilityRestrictionList(PlmnId* m_plmnId) {
  if (!mobilityrestrictionlist) {
    mobilityrestrictionlist = new MobilityRestrictionList();
  }
  Ngap_HandoverRequestIEs_t* ie =
      (Ngap_HandoverRequestIEs_t*) calloc(1, sizeof(Ngap_HandoverRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_MobilityRestrictionList;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_HandoverRequestIEs__value_PR_MobilityRestrictionList;
  mobilityrestrictionlist->setMobilityRestrictionList(m_plmnId);
  mobilityrestrictionlist->encodeMobilityRestrictionList(
      &(ie->value.choice.MobilityRestrictionList));
  int ret = ASN_SEQUENCE_ADD(&handoverRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode MobilityRestrictionList IE error" << endl;
  // free_wrapper((void**) &ie);
}

}  // namespace ngap
