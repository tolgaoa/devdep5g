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

#include "DownlinkRANStatusTransfer.hpp"

#include <iostream>
#include <vector>

extern "C" {
#include "dynamic_memory_check.h"
}

using namespace std;
namespace ngap {
DownlinkRANStatusTransfer::DownlinkRANStatusTransfer() {
  amfUeNgapId                            = NULL;
  ranUeNgapId                            = NULL;
  ranStatusTransfer_TransparentContainer = NULL;
  DownlinkranstatustransferIEs           = NULL;
  DownlinkranstatustransferPDU           = NULL;
}
DownlinkRANStatusTransfer::~DownlinkRANStatusTransfer() {}
void DownlinkRANStatusTransfer::setAmfUeNgapId(unsigned long id) {
  if (!amfUeNgapId) amfUeNgapId = new AMF_UE_NGAP_ID();
  amfUeNgapId->setAMF_UE_NGAP_ID(id);

  Ngap_DownlinkRANStatusTransferIEs_t* ie =
      (Ngap_DownlinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkRANStatusTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_AMF_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkRANStatusTransferIEs__value_PR_AMF_UE_NGAP_ID;

  int ret = amfUeNgapId->encode2AMF_UE_NGAP_ID(ie->value.choice.AMF_UE_NGAP_ID);
  if (!ret) {
    cout << "encode AMF_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&DownlinkranstatustransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode AMF_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}
void DownlinkRANStatusTransfer::setRanUeNgapId(uint32_t id) {
  if (!ranUeNgapId) ranUeNgapId = new RAN_UE_NGAP_ID();
  ranUeNgapId->setRanUeNgapId(id);

  Ngap_DownlinkRANStatusTransferIEs_t* ie =
      (Ngap_DownlinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkRANStatusTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_RAN_UE_NGAP_ID;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkRANStatusTransferIEs__value_PR_RAN_UE_NGAP_ID;

  int ret = ranUeNgapId->encode2RAN_UE_NGAP_ID(ie->value.choice.RAN_UE_NGAP_ID);
  if (!ret) {
    cout << "encode RAN_UE_NGAP_ID IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(&DownlinkranstatustransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode RAN_UE_NGAP_ID IE error" << endl;
  // free_wrapper((void**) &ie);
}

void DownlinkRANStatusTransfer::setRANStatusTransfer_TransparentContainer(
    long drb_id, long ul_pcdp, long ul_hfn_pdcp, long dl_pcdp,
    long dl_hfn_pdcp) {
  if (!ranStatusTransfer_TransparentContainer) {
    ranStatusTransfer_TransparentContainer =
        new RANStatusTransferTransparentContainer();
  }
  Ngap_DRB_ID_t* dRB_id = (Ngap_DRB_ID_t*) calloc(1, sizeof(Ngap_DRB_ID_t));
  dRB_id                = &drb_id;
  COUNTValueForPDCP_SN18* UL_value =
      (COUNTValueForPDCP_SN18*) calloc(1, sizeof(COUNTValueForPDCP_SN18));
  UL_value->setvalue(ul_pcdp, ul_hfn_pdcp);
  COUNTValueForPDCP_SN18* DL_value =
      (COUNTValueForPDCP_SN18*) calloc(1, sizeof(COUNTValueForPDCP_SN18));
  DL_value->setvalue(dl_pcdp, dl_hfn_pdcp);
  dRBStatusUL18* UL18 = (dRBStatusUL18*) calloc(1, sizeof(dRBStatusUL18));
  UL18->setcountvalue(UL_value);
  DRBStatusDL18* DL18 = (DRBStatusDL18*) calloc(1, sizeof(DRBStatusDL18));
  DL18->setcountvalue(DL_value);
  dRBStatusDL* DL = (dRBStatusDL*) calloc(1, sizeof(dRBStatusDL));
  DL->setDRBStatusDL18(DL18);
  dRBStatusUL* UL = (dRBStatusUL*) calloc(1, sizeof(dRBStatusUL));
  UL->setdRBStatusUL(UL18);
  dRBSubjectItem* m_item = (dRBSubjectItem*) calloc(1, sizeof(dRBSubjectItem));
  m_item->setdRBSubjectItem(dRB_id, UL, DL);
  dRBSubjectList* m_list = (dRBSubjectList*) calloc(1, sizeof(dRBSubjectList));
  m_list->setdRBSubjectItem(m_item, 1);
  ranStatusTransfer_TransparentContainer->setdRBSubject_list(m_list);
  Ngap_DownlinkRANStatusTransferIEs_t* ie =
      (Ngap_DownlinkRANStatusTransferIEs_t*) calloc(
          1, sizeof(Ngap_DownlinkRANStatusTransferIEs_t));
  ie->id = Ngap_ProtocolIE_ID_id_RANStatusTransfer_TransparentContainer;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_DownlinkRANStatusTransferIEs__value_PR_RANStatusTransfer_TransparentContainer;
  bool ret = ranStatusTransfer_TransparentContainer
                 ->encoderanstatustransfer_transparentcontainer(
                     &ie->value.choice.RANStatusTransfer_TransparentContainer);
  if (!ret) {
    cout << "encode ranstatustransfer_transparentcontainer error" << endl;
    // free_wrapper((void**) &dRB_id);
    free_wrapper((void**) &UL_value);
    free_wrapper((void**) &DL_value);
    free_wrapper((void**) &UL18);
    free_wrapper((void**) &DL18);
    free_wrapper((void**) &DL);
    free_wrapper((void**) &UL);
    free_wrapper((void**) &m_item);
    free_wrapper((void**) &m_list);
    free_wrapper((void**) &ie);
  }
  if (ASN_SEQUENCE_ADD(&DownlinkranstatustransferIEs->protocolIEs.list, ie) !=
      0) {
    cout << "encode ranstatustransfer_transparentcontainer error 2" << endl;
  }
  /* free_wrapper((void**) &dRB_id);
   free_wrapper((void**) &UL_value);
   free_wrapper((void**) &DL_value);
   free_wrapper((void**) &UL18);
   free_wrapper((void**) &DL18);
   free_wrapper((void**) &DL);
   free_wrapper((void**) &UL);
   free_wrapper((void**) &m_list);
   free_wrapper((void**) &m_item);
   free_wrapper((void**) &ie);
   */
}
void DownlinkRANStatusTransfer::setmessagetype() {
  if (!DownlinkranstatustransferPDU) {
    DownlinkranstatustransferPDU =
        (Ngap_NGAP_PDU_t*) calloc(1, sizeof(Ngap_NGAP_PDU_t));
  }
  MessageType downlinkranstatustransfermessageIEs;
  downlinkranstatustransfermessageIEs.setProcedureCode(
      Ngap_ProcedureCode_id_DownlinkRANStatusTransfer);
  downlinkranstatustransfermessageIEs.setTypeOfMessage(
      Ngap_NGAP_PDU_PR_initiatingMessage);
  downlinkranstatustransfermessageIEs.setCriticality(Ngap_Criticality_ignore);
  downlinkranstatustransfermessageIEs.setValuePresent(
      Ngap_InitiatingMessage__value_PR_DownlinkRANStatusTransfer);
  if (downlinkranstatustransfermessageIEs.getProcedureCode() ==
          Ngap_ProcedureCode_id_DownlinkRANStatusTransfer &&
      downlinkranstatustransfermessageIEs.getTypeOfMessage() ==
          Ngap_NGAP_PDU_PR_initiatingMessage) {
    downlinkranstatustransfermessageIEs.encode2pdu(
        DownlinkranstatustransferPDU);
    DownlinkranstatustransferIEs =
        &(DownlinkranstatustransferPDU->choice.initiatingMessage->value.choice
              .DownlinkRANStatusTransfer);
  } else {
    cout << "[warning] This information doesn't refer to "
            "downlinkranstatustransfer Message!!!"
         << endl;
  }
}
int DownlinkRANStatusTransfer::encodetobuffer(uint8_t* buf, int buf_size) {
  asn_fprint(stderr, &asn_DEF_Ngap_NGAP_PDU, DownlinkranstatustransferPDU);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_NGAP_PDU, NULL, DownlinkranstatustransferPDU, buf,
      buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}
}  // namespace ngap
