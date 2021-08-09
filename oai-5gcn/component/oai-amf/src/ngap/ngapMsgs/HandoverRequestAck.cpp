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

#include "HandoverRequestAck.hpp"

#include "GTP-TEID.hpp"
#include "String2Value.hpp"
#include "TransportLayerAddress.hpp"
extern "C" {
#include "Ngap_GTPTunnel.h"
#include "Ngap_NGAP-PDU.h"
#include "asn_codecs.h"
#include "constr_TYPE.h"
#include "constraints.h"
#include "per_decoder.h"
#include "per_encoder.h"
}

#include <iostream>
#include <vector>

using namespace std;

namespace ngap {

HandoverRequestAck::HandoverRequestAck() {
  amfUeNgapId                         = nullptr;
  ranUeNgapId                         = nullptr;
  pduSessionResourceAdmittedList      = nullptr;
  PDUSessionResourceFailedToSetupList = nullptr;
  TargetToSource_TransparentContainer = nullptr;
  CriticalityDiagnostics              = nullptr;
  handoverRequestAckPdu               = nullptr;
  handoverRequestAckIEs               = nullptr;
  handovertype                        = nullptr;
}

HandoverRequestAck::~HandoverRequestAck() {}
unsigned long HandoverRequestAck::getAmfUeNgapId() {
  return amfUeNgapId->getAMF_UE_NGAP_ID();
}
void HandoverRequestAck::setMessageType() {
  if (!handoverRequestAckPdu)
    handoverRequestAckPdu =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

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
    handoverRequestMessageTypeIE.encode2pdu(handoverRequestAckPdu);
    handoverRequestAckIEs = &(handoverRequestAckPdu->choice.successfulOutcome
                                  ->value.choice.HandoverRequestAcknowledge);
  } else {
    cout << "[warning] This information doesn't refer to HandoverRequest "
            "Message!!!"
         << endl;
  }
}
uint32_t HandoverRequestAck::getRanUeNgapId() {
  return ranUeNgapId->getRanUeNgapId();
}
OCTET_STRING_t HandoverRequestAck::getTargetToSource_TransparentContainer() {
  return *TargetToSource_TransparentContainer;
}

bool HandoverRequestAck::getPDUSessionResourceAdmittedList(
    std::vector<PDUSessionResourceAdmittedItem_t>& list) {
  if (!pduSessionResourceAdmittedList) return false;

  PDUSessionResourceAdmittedItem* m_pduSessionResourceAdmittedItem;
  int num = 0;
  pduSessionResourceAdmittedList->getPDUSessionResourceAdmittedList(
      m_pduSessionResourceAdmittedItem, num);

  for (int i = 0; i < num; i++) {
    PDUSessionResourceAdmittedItem_t response;

    PDUSessionID* m_pDUSessionID;
    m_pduSessionResourceAdmittedItem[i].getPDUSessionResourceAdmittedItem(
        m_pDUSessionID, response.handoverRequestAcknowledgeTransfer);
    m_pDUSessionID->getPDUSessionID(response.pduSessionId);
    list.push_back(response);
  }

  return true;
}

bool HandoverRequestAck::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  handoverRequestAckPdu = ngap_msg_pdu;

  if (handoverRequestAckPdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (handoverRequestAckPdu->choice.successfulOutcome &&
        handoverRequestAckPdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_HandoverResourceAllocation &&
        handoverRequestAckPdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        handoverRequestAckPdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge) {
      handoverRequestAckIEs = &handoverRequestAckPdu->choice.successfulOutcome
                                   ->value.choice.HandoverRequestAcknowledge;
    } else {
      cout << "Check handoverRequestAck message error!!!" << endl;
      return false;
    }
  } else {
    cout << "handoverRequestAck MessageType error!!!" << endl;
    return false;
  }
  for (int i = 0; i < handoverRequestAckIEs->protocolIEs.list.count; i++) {
    switch (handoverRequestAckIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID) {
          amfUeNgapId = new AMF_UE_NGAP_ID();
          if (!amfUeNgapId->decodefromAMF_UE_NGAP_ID(
                  handoverRequestAckIEs->protocolIEs.list.array[i]
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
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID) {
          ranUeNgapId = new RAN_UE_NGAP_ID();
          if (!ranUeNgapId->decodefromRAN_UE_NGAP_ID(
                  handoverRequestAckIEs->protocolIEs.list.array[i]
                      ->value.choice.RAN_UE_NGAP_ID)) {
            cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap RAN_UE_NGAP_ID IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionResourceAdmittedList: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceAdmittedList) {
          pduSessionResourceAdmittedList = new PDUSessionResourceAdmittedList();
          if (!pduSessionResourceAdmittedList
                   ->decodefromPDUSessionResourceAdmittedList(
                       &handoverRequestAckIEs->protocolIEs.list.array[i]
                            ->value.choice.PDUSessionResourceAdmittedList)) {
            cout << "decoded ngap PDUSessionResourceAdmittedList IE error"
                 << endl;
            return false;
          }
        } else {
          cout << "decoded ngap PDUSessionResourceAdmittedList Type IE error"
               << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer: {
        if (handoverRequestAckIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            handoverRequestAckIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_HandoverRequestAcknowledgeIEs__value_PR_TargetToSource_TransparentContainer) {
          TargetToSource_TransparentContainer =
              new Ngap_TargetToSource_TransparentContainer_t();
          *TargetToSource_TransparentContainer =
              handoverRequestAckIEs->protocolIEs.list.array[i]
                  ->value.choice.TargetToSource_TransparentContainer;
        } else {
          cout << "decoded ngap TargetToSource_TransparentContainer IE error"
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

int HandoverRequestAck::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, handoverRequestAckPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, handoverRequestAckPdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

/*	void HandoverRequestAck::setMessageType()
{
        if (!handoverRequestAckPdu) handoverRequestAckPdu =
(Ngap_NGAP_PDU_t*)calloc(1, sizeof(Ngap_NGAP_PDU_t));

        MessageType HandoverRequestAckMessageTypeIE;
        HandoverRequestAckMessageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_HandoverResourceAllocation);
        HandoverRequestAckMessageTypeIE.setTypeOfMessage(Ngap_NGAP_PDU_PR_successfulOutcome);
        HandoverRequestAckMessageTypeIE.setCriticality(Ngap_Criticality_reject);
        HandoverRequestAckMessageTypeIE.setValuePresent(Ngap_SuccessfulOutcome__value_PR_HandoverRequestAcknowledge);

        if (HandoverRequestAckMessageTypeIE.getProcedureCode() ==
Ngap_ProcedureCode_id_HandoverResourceAllocation &&
HandoverRequestAckMessageTypeIE.getTypeOfMessage() ==
Ngap_NGAP_PDU_PR_successfulOutcome)
        {
                HandoverRequestAckMessageTypeIE.encode2pdu(handoverRequestAckPdu);
                handoverRequestAckIEs =
&(handoverRequestAckPdu->choice.successfulOutcome->value.choice.HandoverRequestAcknowledge);
        }
        else
        {
                cout << "[warning] This information doesn't refer to
HandoverRequestAcknowledge Message!!!" << endl;
        }
}
*/
/*	void HandoverRequestAck::setAmfUeNgapId(unsigned long id)
{
        if (!amfUeNgapId)
                amfUeNgapId = new AMF_UE_NGAP_ID();
        amfUeNgapId->setAMF_UE_NGAP_ID(id);

        Ngap_HandoverRequestAcknowledgeIEs_t *ie =
(Ngap_HandoverRequestAcknowledgeIEs_t *)calloc(1,
sizeof(Ngap_HandoverRequestAcknowledgeIEs_t)); ie->id =
Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID; ie->criticality = Ngap_Criticality_reject;
        ie->value.present =
Ngap_HandoverRequestAcknowledgeIEs__value_PR_AMF_UE_NGAP_ID;

        int ret =
amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID); if (!ret)
        {
                cout << "encode AMF_UE_NGAP_ID IE error" << endl;
                return;
        }

        ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
        if (ret != 0) cout << "encode AMF_UE_NGAP_ID IE error" << endl;
}
*/
/*	void HandoverRequestAck::setRanUeNgapId(uint32_t ran_ue_ngap_id)
{
        if (!ranUeNgapId)
                ranUeNgapId = new RAN_UE_NGAP_ID();
        ranUeNgapId->setRanUeNgapId(ran_ue_ngap_id);

        Ngap_HandoverRequestAcknowledgeIEs_t *ie =
(Ngap_HandoverRequestAcknowledgeIEs_t *)calloc(1,
sizeof(Ngap_HandoverRequestAcknowledgeIEs_t)); ie->id =
Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID; ie->criticality = Ngap_Criticality_reject;
        ie->value.present =
Ngap_HandoverRequestAcknowledgeIEs__value_PR_RAN_UE_NGAP_ID;

        int ret =
ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID); if (!ret)
        {
                cout << "encode RAN_UE_NGAP_ID IE error" << endl;
                return;
        }

        ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie);
        if (ret != 0) cout << "encode RAN_UE_NGAP_ID IE error" << endl;
}

*/
/*	void
HandoverRequestAck::setPDUSessionResourceAdmittedList(std::vector<Ngap_PDUSessionResourceAdmittedItem_t>
list)
{
        if (!PDUSessionResourceAdmittedList)
                PDUSessionResourceAdmittedList = new
Ngap_PDUSessionResourceAdmittedList_t(); Ngap_HandoverRequestAcknowledgeIEs_t
*ie = (Ngap_HandoverRequestAcknowledgeIEs_t *)calloc(1,
sizeof(Ngap_HandoverRequestAcknowledgeIEs_t));

        for (int i = 0; i < list.size(); i++)
        {
                Ngap_PDUSessionResourceAdmittedItem_t *item =
(Ngap_PDUSessionResourceAdmittedItem_t *)calloc(1,
sizeof(Ngap_PDUSessionResourceAdmittedItem_t)); item->pDUSessionID =
list[i].pDUSessionID;


                Ngap_HandoverRequestAcknowledgeTransfer_t
*handoverrequestacknowledgetransfer;
                handoverrequestacknowledgetransfer->dL_NGU_UP_TNLInformation.present
= Ngap_UPTransportLayerInformation_PR_gTPTunnel;

                TransportLayerAddress transportlayeraddress;
                transportlayeraddress.setTransportLayerAddress("127.0.1.1");
                transportlayeraddress.encode2TransportLayerAddress((handoverrequestacknowledgetransfer->dL_NGU_UP_TNLInformation).choice.gTPTunnel->transportLayerAddress);

                GtpTeid gtpteid;
                gtpteid.setGtpTeid(159);
                gtpteid.encode2GtpTeid((handoverrequestacknowledgetransfer->dL_NGU_UP_TNLInformation.choice.gTPTunnel->gTP_TEID));



                item->handoverRequestAcknowledgeTransfer =
list[i].handoverRequestAcknowledgeTransfer; int ret =
ASN_SEQUENCE_ADD(&PDUSessionResourceAdmittedList->list, item); if (ret != 0)
cout << "encode PDUSessionResourceAdmittedList IE error" << endl;
        }

        ie->id = Ngap_ProtocolIE_ID_id_PDUSessionResourceAdmittedList;
        ie->criticality = Ngap_Criticality_ignore;
        ie->value.present =
Ngap_HandoverRequestAcknowledgeIEs__value_PR_PDUSessionResourceAdmittedList;
        ie->value.choice.PDUSessionResourceAdmittedList =
*PDUSessionResourceAdmittedList; int ret =
ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list, ie); if (ret != 0)
cout << "encode PDUSessionResourceAdmittedList IE error" << endl;

}
*/
/*	void
HandoverRequestAck::setTargetToSource_TransparentContainer(OCTET_STRING_t
targetTosource)
{
        if (!TargetToSource_TransparentContainer)
                TargetToSource_TransparentContainer = new
Ngap_TargetToSource_TransparentContainer_t();

        Ngap_HandoverRequestAcknowledgeIEs_t *ie =
(Ngap_HandoverRequestAcknowledgeIEs_t *)calloc(1,
sizeof(Ngap_HandoverRequestAcknowledgeIEs_t)); ie->id =
Ngap_ProtocolIE_ID_id_TargetToSource_TransparentContainer; ie->criticality =
Ngap_Criticality_reject; ie->value.present =
Ngap_HandoverRequestAcknowledgeIEs__value_PR_TargetToSource_TransparentContainer;
        ie->value.choice.TargetToSource_TransparentContainer = targetTosource;
        int ret = ASN_SEQUENCE_ADD(&handoverRequestAckIEs->protocolIEs.list,
ie); if (ret != 0) cout << "encode TargetToSource_TransparentContainer IE error"
<< endl;
}

*/

}  // namespace ngap
