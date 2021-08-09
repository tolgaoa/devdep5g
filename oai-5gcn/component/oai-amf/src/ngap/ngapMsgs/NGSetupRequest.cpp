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

#include "NGSetupRequest.hpp"

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
NGSetupRequestMsg::NGSetupRequestMsg() {
  ngSetupRequestPdu = NULL;
  ngSetupRequestIEs = NULL;

  globalRanNodeId  = NULL;
  ranNodeName      = NULL;
  supportedTAList  = NULL;
  defaultPagingDrx = NULL;
}

//------------------------------------------------------------------------------
NGSetupRequestMsg::~NGSetupRequestMsg() {}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setMessageType() {
  if (!ngSetupRequestPdu)
    ngSetupRequestPdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType NgSetupRequestMessageTypeIE;
  NgSetupRequestMessageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGSetup);
  NgSetupRequestMessageTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_initiatingMessage);
  NgSetupRequestMessageTypeIE.setValuePresent(
      Ngap_InitiatingMessage__value_PR_NGSetupRequest);

  if (NgSetupRequestMessageTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_NGSetup &&
      NgSetupRequestMessageTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_initiatingMessage) {
    NgSetupRequestMessageTypeIE.encode2pdu(ngSetupRequestPdu);
    ngSetupRequestIEs = &(ngSetupRequestPdu->choice.initiatingMessage->value
                              .choice.NGSetupRequest);
  } else {
    cout << "[warning] This information doesn't refer to NGSetupRequest "
            "Message!!!"
         << endl;
  }
}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setGlobalRanNodeID(
    const std::string mcc, const std::string mnc,
    Ngap_GlobalRANNodeID_PR ranNodeType, uint32_t ranNodeId) {
  GlobalRanNodeId globalRanNodeIdIE;
  globalRanNodeIdIE.setChoiceOfRanNodeId(ranNodeType);
  GlobalgNBId globalgNBId;
  PlmnId plmn;
  plmn.setMccMnc(mcc, mnc);
  GNB_ID gnbid;
  gnbid.setValue(ranNodeId);
  globalgNBId.setGlobalgNBId(&plmn, &gnbid);
  globalRanNodeIdIE.setGlobalgNBID(&globalgNBId);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_GlobalRANNodeID;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID;

  int ret = globalRanNodeIdIE.encode2GlobalRANNodeID(
      &ie->value.choice.GlobalRANNodeID);
  if (!ret) {
    cout << "encode GlobalRANNodeID IE error" << endl;
    return;
  }

  ret = ASN_SEQUENCE_ADD(&ngSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode GlobalRANNodeID IE error" << endl;
}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setRanNodeName(const std::string ranNodeName) {
  RanNodeName ranNodeNameIE;
  ranNodeNameIE.setValue(ranNodeName);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RANNodeName;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_RANNodeName;

  int ret = ranNodeNameIE.encode2RanNodeName(&ie->value.choice.RANNodeName);
  if (!ret) {
    cout << "encode RanNodeName IE error" << endl;
    return;
  }

  ret = ASN_SEQUENCE_ADD(&ngSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode RANNodeName IE error" << endl;
}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setSupportedTAList(
    const std::vector<struct SupportedItem_s> list) {
  if (list.size() == 0) {
    cout << "[Warning] Setup failed, vector is empty!!!" << endl;
    return;
  }

  SupportedTAList supportedTAListIE;
  SupportedTaItem* supportedTaItems =
      new SupportedTaItem[list.size()]();  // maximum: 256
  for (int i = 0; i < list.size(); i++) {
    TAC* tac = new TAC();
    tac->setTac(list[i].tac);
    supportedTaItems[i].setTac(tac);
    BroadcastPLMNItem* broadcastPlmnItems =
        new BroadcastPLMNItem[list[i].b_plmn_list.size()]();
    for (int j = 0; j < list[i].b_plmn_list.size(); j++) {
      PlmnId* broadPlmn = new PlmnId();
      broadPlmn->setMccMnc(
          list[i].b_plmn_list[j].mcc, list[i].b_plmn_list[j].mnc);
      S_NSSAI* snssai = new S_NSSAI[list[i].b_plmn_list[j].slice_list.size()]();
      for (int k = 0; k < list[i].b_plmn_list[j].slice_list.size(); k++) {
        snssai[k].setSst(list[i].b_plmn_list[j].slice_list[k].sst);
        if (list[i].b_plmn_list[j].slice_list[k].sd.size())
          snssai[k].setSd(list[i].b_plmn_list[j].slice_list[k].sd);
      }
      broadcastPlmnItems[j].setPlmnSliceSupportList(
          broadPlmn, snssai, list[i].b_plmn_list[j].slice_list.size());
    }
    supportedTaItems[i].setBroadcastPlmnList(
        broadcastPlmnItems, list[i].b_plmn_list.size());
  }
  supportedTAListIE.setSupportedTaItems(supportedTaItems, list.size());

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_SupportedTAList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_SupportedTAList;

  int ret = supportedTAListIE.encode2SupportedTAList(
      &ie->value.choice.SupportedTAList);
  if (!ret) {
    cout << "encode SupportedTAList IE error" << endl;
    return;
  }

  ret = ASN_SEQUENCE_ADD(&ngSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode SupportedTAList IE error" << endl;
}

//------------------------------------------------------------------------------
void NGSetupRequestMsg::setDefaultPagingDRX(e_Ngap_PagingDRX value) {
  DefaultPagingDRX defaultPagingDRXIE;
  defaultPagingDRXIE.setValue(value);

  Ngap_NGSetupRequestIEs_t* ie =
      (Ngap_NGSetupRequestIEs_t*) calloc(1, sizeof(Ngap_NGSetupRequestIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_DefaultPagingDRX;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupRequestIEs__value_PR_PagingDRX;

  int ret =
      defaultPagingDRXIE.encode2DefaultPagingDRX(ie->value.choice.PagingDRX);
  if (!ret) {
    cout << "encode DefaultPagingDRX IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&ngSetupRequestIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode DefaultPagingDRX IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int NGSetupRequestMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, ngSetupRequestPdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, ngSetupRequestPdu, buf, buf_size);
  printf("er.encoded(%ld)\n", er.encoded);
  return er.encoded;
}

//------------------------------------------------------------------------------
bool NGSetupRequestMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  ngSetupRequestPdu = ngap_msg_pdu;

  if (ngSetupRequestPdu->present == Ngap_NGAP_PDU_PR_initiatingMessage) {
    if (ngSetupRequestPdu->choice.initiatingMessage &&
        ngSetupRequestPdu->choice.initiatingMessage->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngSetupRequestPdu->choice.initiatingMessage->criticality ==
            Ngap_Criticality_reject &&
        ngSetupRequestPdu->choice.initiatingMessage->value.present ==
            Ngap_InitiatingMessage__value_PR_NGSetupRequest) {
      ngSetupRequestIEs = &ngSetupRequestPdu->choice.initiatingMessage->value
                               .choice.NGSetupRequest;
      for (int i = 0; i < ngSetupRequestIEs->protocolIEs.list.count; i++) {
        switch (ngSetupRequestIEs->protocolIEs.list.array[i]->id) {
          case Ngap_ProtocolIE_ID_id_GlobalRANNodeID: {
            if (ngSetupRequestIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                ngSetupRequestIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_GlobalRANNodeID) {
              globalRanNodeId = new GlobalRanNodeId();
              if (!globalRanNodeId->decodefromGlobalRANNodeID(
                      &ngSetupRequestIEs->protocolIEs.list.array[i]
                           ->value.choice.GlobalRANNodeID)) {
                cout << "Decoded NGAP GlobalRanNodeId IE error!" << endl;
                return false;
              }
            } else {
              cout << "Decoded NGAP GlobalRanNodeId IE error" << endl;
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_RANNodeName: {
            if (ngSetupRequestIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                ngSetupRequestIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_RANNodeName) {
              ranNodeName = new RanNodeName();
              if (!ranNodeName->decodefromRanNodeName(
                      &ngSetupRequestIEs->protocolIEs.list.array[i]
                           ->value.choice.RANNodeName)) {
                cout << "Decoded NGAP RanNodeName IE error" << endl;
                return false;
              }
            } else {
              cout << "Decoded NGAP RanNodeName IE error" << endl;
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_SupportedTAList: {
            if (ngSetupRequestIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_reject &&
                ngSetupRequestIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_SupportedTAList) {
              supportedTAList = new SupportedTAList();
              if (!supportedTAList->decodefromSupportedTAList(
                      &ngSetupRequestIEs->protocolIEs.list.array[i]
                           ->value.choice.SupportedTAList)) {
                cout << "Decoded NGAP SupportedTAList IE error" << endl;
                return false;
              }
            } else {
              cout << "Decoded NGAP SupportedTAList IE error" << endl;
              return false;
            }
          } break;
          case Ngap_ProtocolIE_ID_id_DefaultPagingDRX: {
            if (ngSetupRequestIEs->protocolIEs.list.array[i]->criticality ==
                    Ngap_Criticality_ignore &&
                ngSetupRequestIEs->protocolIEs.list.array[i]->value.present ==
                    Ngap_NGSetupRequestIEs__value_PR_PagingDRX) {
              defaultPagingDrx = new DefaultPagingDRX();
              if (!defaultPagingDrx->decodefromDefaultPagingDRX(
                      ngSetupRequestIEs->protocolIEs.list.array[i]
                          ->value.choice.PagingDRX)) {
                cout << "Decoded NGAP DefaultPagingDRX IE error" << endl;
                return false;
              }
            } else {
              cout << "Decoded NGAP DefaultPagingDRX IE error" << endl;
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
      cout << "Check NGSetupRequest message error!!!";
      return false;
    }
  } else {
    cout << "Check NGSetupRequest message error!!!";
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupRequestMsg::getGlobalGnbID(
    uint32_t& gnbId, std::string& mcc, std::string& mnc) {
  if (!globalRanNodeId) return false;
  if (globalRanNodeId->getChoiceOfRanNodeId() !=
      Ngap_GlobalRANNodeID_PR_globalGNB_ID)
    return false;
  GlobalgNBId* globalgNBId;
  globalRanNodeId->getGlobalgNBID(globalgNBId);
  PlmnId* plmn;
  GNB_ID* gnbid;
  globalgNBId->getGlobalgNBId(plmn, gnbid);
  plmn->getMcc(mcc);
  plmn->getMnc(mnc);
  gnbId = gnbid->getValue();
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupRequestMsg::getRanNodeName(std::string& name) {
  if (!ranNodeName) return false;
  ranNodeName->getValue(name);
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupRequestMsg::getSupportedTAList(
    std::vector<struct SupportedItem_s>& list) {
  if (!supportedTAList) return false;
  SupportedTaItem* supportedTaItem;
  int numOfsupportedTaItem;
  supportedTAList->getSupportedTaItems(supportedTaItem, numOfsupportedTaItem);
  for (int i = 0; i < numOfsupportedTaItem; i++) {
    TAC* tac;
    supportedTaItem[i].getTac(tac);
    SupportedItem_t supporteditem_data;
    supporteditem_data.tac = tac->getTac();

    BroadcastPLMNItem* broadcastPLMNItem;
    int numOfbroadcastPLMNItem;
    supportedTaItem[i].getBroadcastPlmnList(
        broadcastPLMNItem, numOfbroadcastPLMNItem);
    for (int j = 0; j < numOfbroadcastPLMNItem; j++) {
      PlmnId* plmnId_decode;
      S_NSSAI* snssai_decode;
      int numofsnssai;
      broadcastPLMNItem[j].getPlmnSliceSupportList(
          plmnId_decode, snssai_decode, numofsnssai);
      PlmnSliceSupport_t broadcastplmn_data;
      plmnId_decode->getMcc(broadcastplmn_data.mcc);
      plmnId_decode->getMnc(broadcastplmn_data.mnc);
      for (int k = 0; k < numofsnssai; k++) {
        SliceSupportItem_t slicesupport_data;
        snssai_decode[k].getSst(slicesupport_data.sst);
        snssai_decode[k].getSd(slicesupport_data.sd);
        broadcastplmn_data.slice_list.push_back(slicesupport_data);
      }
      supporteditem_data.b_plmn_list.push_back(broadcastplmn_data);
    }
    list.push_back(supporteditem_data);
  }

  return true;
}

//------------------------------------------------------------------------------
int NGSetupRequestMsg::getDefaultPagingDRX() {
  if (!defaultPagingDrx) return -1;
  return defaultPagingDrx->getValue();
}
}  // namespace ngap
