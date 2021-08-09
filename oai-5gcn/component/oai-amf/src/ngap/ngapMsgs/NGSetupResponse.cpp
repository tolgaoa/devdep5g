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

#include "NGSetupResponse.hpp"

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
NGSetupResponseMsg::NGSetupResponseMsg() {
  ngSetupResponsePdu  = NULL;
  ngSetupResponsIEs   = NULL;
  amfName             = NULL;
  servedGUAMIList     = NULL;
  relativeAmfCapacity = NULL;
  plmnSupportList     = NULL;
  // criticalityDiagnostics = NULL;
}

//------------------------------------------------------------------------------
NGSetupResponseMsg::~NGSetupResponseMsg() {}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setMessageType() {
  if (!ngSetupResponsePdu)
    ngSetupResponsePdu = (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));

  MessageType NgSetupResponseMessageTypeIE;
  NgSetupResponseMessageTypeIE.setProcedureCode(Ngap_ProcedureCode_id_NGSetup);
  NgSetupResponseMessageTypeIE.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_successfulOutcome);
  NgSetupResponseMessageTypeIE.setValuePresent(
      Ngap_SuccessfulOutcome__value_PR_NGSetupResponse);

  if (NgSetupResponseMessageTypeIE.getProcedureCode() ==
          Ngap_ProcedureCode_id_NGSetup &&
      NgSetupResponseMessageTypeIE.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_successfulOutcome) {
    NgSetupResponseMessageTypeIE.encode2pdu(ngSetupResponsePdu);
    ngSetupResponsIEs = &(ngSetupResponsePdu->choice.successfulOutcome->value
                              .choice.NGSetupResponse);
  } else {
    cout << "[warning] This information doesn't refer to NGSetupRespons "
            "Message!!!"
         << endl;
  }
}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setAMFName(const std::string name) {
  amfName = new AmfName();
  amfName->setValue(name);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_AMFName;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_AMFName;

  int ret = amfName->encode2AmfName(&ie->value.choice.AMFName);
  if (!ret) {
    cout << "encode AmfName IE error" << endl;
    return;
  }

  ret = ASN_SEQUENCE_ADD(&ngSetupResponsIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AmfName IE error" << endl;
}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setGUAMIList(std::vector<struct GuamiItem_s> list) {
  servedGUAMIList = new ServedGUAMIList();
  ServedGUAMIItem* servedGUAMIItem;
  servedGUAMIItem = new ServedGUAMIItem[list.size()]();
  for (int i = 0; i < list.size(); i++) {
    GUAMI* guami          = new GUAMI();
    PlmnId* plmnid        = new PlmnId();
    AMFRegionID* regionID = new AMFRegionID();
    AMFSetID* setID       = new AMFSetID();
    AMFPointer* pointer   = new AMFPointer();

    plmnid->setMccMnc(list[i].mcc, list[i].mnc);
    regionID->setAMFRegionID(list[i].regionID);
    setID->setAMFSetID(list[i].AmfSetID);
    pointer->setAMFPointer(list[i].AmfPointer);
    guami->setGUAMI(plmnid, regionID, setID, pointer);
    servedGUAMIItem[i].setGUAMI(guami);

    if (list[i].backupAMFName.size() > 0) {
      AmfName* backupamfname = new AmfName();
      backupamfname->setValue(list[i].backupAMFName);

      servedGUAMIItem[i].setBackupAMFName(backupamfname);
    }
  }
  servedGUAMIList->addServedGUAMIItems(servedGUAMIItem, list.size());

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_ServedGUAMIList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_ServedGUAMIList;

  servedGUAMIList->encode2ServedGUAMIList(&ie->value.choice.ServedGUAMIList);

  int ret = ASN_SEQUENCE_ADD(&ngSetupResponsIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode ServedGUAMIList IE error" << endl;
}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setRelativeAmfCapacity(long capacity) {
  relativeAmfCapacity = new RelativeAMFCapacity();
  relativeAmfCapacity->setValue(capacity);

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_RelativeAMFCapacity;
  ie->criticality   = Ngap_Criticality_ignore;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_RelativeAMFCapacity;

  relativeAmfCapacity->encode2RelativeAMFCapacity(
      &ie->value.choice.RelativeAMFCapacity);

  int ret = ASN_SEQUENCE_ADD(&ngSetupResponsIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode RelativeAMFCapacity IE error" << endl;
}

//------------------------------------------------------------------------------
void NGSetupResponseMsg::setPlmnSupportList(
    std::vector<PlmnSliceSupport_t> list) {
  plmnSupportList                  = new PLMNSupportList();
  PLMNSupportItem* plmnSupportItem = new PLMNSupportItem[list.size()]();
  for (int i = 0; i < list.size(); i++) {
    PlmnId* plmn = new PlmnId();
    plmn->setMccMnc(list[i].mcc, list[i].mnc);
    cout << "mcc = " << list[i].mcc << "  mnc = " << list[i].mnc << endl;
    S_NSSAI* snssai = new S_NSSAI[list[i].slice_list.size()]();
    for (int j = 0; j < list[i].slice_list.size(); j++) {
      snssai[j].setSst(list[i].slice_list[j].sst);
      if (list[i].slice_list[j].sd.size() &&
          (list[i].slice_list[j].sd.compare("None") &&
           list[i].slice_list[j].sd.compare("none"))) {
        snssai[j].setSd(list[i].slice_list[j].sd);
      }
    }
    plmnSupportItem[i].setPlmnSliceSupportList(
        plmn, snssai, list[i].slice_list.size());
  }
  plmnSupportList->addPLMNSupportItems(plmnSupportItem, list.size());

  Ngap_NGSetupResponseIEs_t* ie =
      (Ngap_NGSetupResponseIEs_t*) calloc(1, sizeof(Ngap_NGSetupResponseIEs_t));
  ie->id            = Ngap_ProtocolIE_ID_id_PLMNSupportList;
  ie->criticality   = Ngap_Criticality_reject;
  ie->value.present = Ngap_NGSetupResponseIEs__value_PR_PLMNSupportList;

  plmnSupportList->encode2PLMNSupportList(&ie->value.choice.PLMNSupportList);

  int ret = ASN_SEQUENCE_ADD(&ngSetupResponsIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode PLMNSupportList IE error" << endl;
}

//------------------------------------------------------------------------------
int NGSetupResponseMsg::encode2buffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, ngSetupResponsePdu);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, ngSetupResponsePdu, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool NGSetupResponseMsg::decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu) {
  ngSetupResponsePdu = ngap_msg_pdu;

  if (ngSetupResponsePdu->present == Ngap_NGAP_PDU_PR_successfulOutcome) {
    if (ngSetupResponsePdu->choice.successfulOutcome &&
        ngSetupResponsePdu->choice.successfulOutcome->procedureCode ==
            Ngap_ProcedureCode_id_NGSetup &&
        ngSetupResponsePdu->choice.successfulOutcome->criticality ==
            Ngap_Criticality_reject &&
        ngSetupResponsePdu->choice.successfulOutcome->value.present ==
            Ngap_SuccessfulOutcome__value_PR_NGSetupResponse) {
      ngSetupResponsIEs = &ngSetupResponsePdu->choice.successfulOutcome->value
                               .choice.NGSetupResponse;
    } else {
      cout << "Check NGSetupResponse message error!!!" << endl;
      return false;
    }
  } else {
    cout << "MessageType error!!!" << endl;
    cout << "ngSetupResponsePdu->present == " << ngSetupResponsePdu->present
         << endl;
    return false;
  }
  for (int i = 0; i < ngSetupResponsIEs->protocolIEs.list.count; i++) {
    switch (ngSetupResponsIEs->protocolIEs.list.array[i]->id) {
      case Ngap_ProtocolIE_ID_id_AMFName: {
        if (ngSetupResponsIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ngSetupResponsIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_AMFName) {
          amfName = new AmfName();
          if (!amfName->decodefromAmfName(
                  &ngSetupResponsIEs->protocolIEs.list.array[i]
                       ->value.choice.AMFName)) {
            cout << "decoded ngap AmfName IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap AmfName IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_ServedGUAMIList: {
        if (ngSetupResponsIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ngSetupResponsIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_ServedGUAMIList) {
          servedGUAMIList = new ServedGUAMIList();
          if (!servedGUAMIList->decodefromServedGUAMIList(
                  &ngSetupResponsIEs->protocolIEs.list.array[i]
                       ->value.choice.ServedGUAMIList)) {
            cout << "decoded ngap ServedGUAMIList IE error!" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap ServedGUAMIList IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_RelativeAMFCapacity: {
        if (ngSetupResponsIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_ignore &&
            ngSetupResponsIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_RelativeAMFCapacity) {
          relativeAmfCapacity = new RelativeAMFCapacity();
          if (!relativeAmfCapacity->decodefromRelativeAMFCapacity(
                  &ngSetupResponsIEs->protocolIEs.list.array[i]
                       ->value.choice.RelativeAMFCapacity)) {
            cout << "decoded ngap RelativeAMFCapacity IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap RelativeAMFCapacity IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PLMNSupportList: {
        if (ngSetupResponsIEs->protocolIEs.list.array[i]->criticality ==
                Ngap_Criticality_reject &&
            ngSetupResponsIEs->protocolIEs.list.array[i]->value.present ==
                Ngap_NGSetupResponseIEs__value_PR_PLMNSupportList) {
          plmnSupportList = new PLMNSupportList();
          if (!plmnSupportList->decodefromPLMNSupportList(
                  &ngSetupResponsIEs->protocolIEs.list.array[i]
                       ->value.choice.PLMNSupportList)) {
            cout << "decoded ngap PLMNSupportList IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap PLMNSupportList IE error!" << endl;
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
bool NGSetupResponseMsg::getAMFName(std::string& name) {
  if (!amfName) return false;
  amfName->getValue(name);
  return true;
}

//------------------------------------------------------------------------------
bool NGSetupResponseMsg::getGUAMIList(std::vector<struct GuamiItem_s>& list) {
  if (!servedGUAMIList) return false;
  ServedGUAMIItem* servedGUAMIItem;
  int numOfItem = 0;
  servedGUAMIList->getServedGUAMIItems(servedGUAMIItem, numOfItem);
  for (int i = 0; i < numOfItem; i++) {
    GuamiItem_t guamiitem_data;
    GUAMI* guami;
    servedGUAMIItem[i].getGUAMI(guami);
    PlmnId* plmnid;
    AMFRegionID* regionID;
    AMFSetID* setID;
    AMFPointer* pointer;
    guami->getGUAMI(plmnid, regionID, setID, pointer);
    plmnid->getMcc(guamiitem_data.mcc);
    plmnid->getMnc(guamiitem_data.mnc);
    regionID->getAMFRegionID(guamiitem_data.regionID);
    setID->getAMFSetID(guamiitem_data.AmfSetID);
    pointer->getAMFPointer(guamiitem_data.AmfPointer);

    AmfName* backupAMFName;
    if (servedGUAMIItem[i].getBackupAMFName(backupAMFName)) {
      backupAMFName->getValue(guamiitem_data.backupAMFName);
    } else {
      guamiitem_data.backupAMFName = "None";
    }

    list.push_back(guamiitem_data);
  }

  return true;
}

//------------------------------------------------------------------------------
long NGSetupResponseMsg::getRelativeAmfCapacity() {
  if (!relativeAmfCapacity) return -1;

  return relativeAmfCapacity->getValue();
}

//------------------------------------------------------------------------------
bool NGSetupResponseMsg::getPlmnSupportList(
    std::vector<PlmnSliceSupport_t>& list) {
  if (!plmnSupportList) return false;

  PLMNSupportItem* plmnsupportItemItem;
  int numOfItem = 0;
  plmnSupportList->getPLMNSupportItems(plmnsupportItemItem, numOfItem);
  for (int i = 0; i < numOfItem; i++) {
    PlmnSliceSupport_t plmnSupportItem_data;
    PlmnId* plmn;
    S_NSSAI* snssai;
    int numofsnssai = 0;
    plmnsupportItemItem[i].getPlmnSliceSupportList(plmn, snssai, numofsnssai);
    plmn->getMcc(plmnSupportItem_data.mcc);
    plmn->getMnc(plmnSupportItem_data.mnc);
    for (int j = 0; j < numofsnssai; j++) {
      SliceSupportItem_t sliceSupportItem_data;
      snssai[j].getSst(sliceSupportItem_data.sst);
      snssai[j].getSd(sliceSupportItem_data.sd);
      plmnSupportItem_data.slice_list.push_back(sliceSupportItem_data);
    }

    list.push_back(plmnSupportItem_data);
  }

  return true;
}

}  // namespace ngap
