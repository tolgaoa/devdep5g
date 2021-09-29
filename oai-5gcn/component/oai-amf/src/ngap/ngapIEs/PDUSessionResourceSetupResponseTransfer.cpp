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

#include "PDUSessionResourceSetupResponseTransfer.hpp"

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
PduSessionResourceSetupResponseTransferIE::
    PduSessionResourceSetupResponseTransferIE() {
  pduSessionResourceSetupResponseTransferIEs =
      (Ngap_PDUSessionResourceSetupResponseTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupResponseTransfer_t));
  dlQoSFlowPerTNLInformation           = NULL;
  additionalDLQoSFlowPerTNLInformation = NULL;
  securityResult                       = NULL;
}

//------------------------------------------------------------------------------
PduSessionResourceSetupResponseTransferIE::
    ~PduSessionResourceSetupResponseTransferIE() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseTransferIE::setDLQoSFlowPerTNLInformation(
    GtpTunnel_t uptlinfo, std::vector<AssociatedQosFlow_t> list) {
  if (!dlQoSFlowPerTNLInformation)
    dlQoSFlowPerTNLInformation = new DLQoSFlowPerTNLInformation();
  UpTransportLayerInformation* m_uPTransportLayerInformation =
      new UpTransportLayerInformation();
  TransportLayerAddress* m_transportLayerAddress = new TransportLayerAddress();
  GtpTeid* m_gtpTeid                             = new GtpTeid();
  m_transportLayerAddress->setTransportLayerAddress(uptlinfo.ip_address);
  m_gtpTeid->setGtpTeid(uptlinfo.gtp_teid);
  m_uPTransportLayerInformation->setUpTransportLayerInformation(
      m_transportLayerAddress, m_gtpTeid);

  AssociatedQosFlowList* m_associatedQosFlowList = new AssociatedQosFlowList();
  AssociatedQosFlowItem* m_associatedQosFlowItem =
      new AssociatedQosFlowItem[list.size()]();
  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier* m_qosFlowIdentifier = new QosFlowIdentifier();
    m_qosFlowIdentifier->setQosFlowIdentifier(list[i].qosFlowIdentifier);
    if (list[i].qosFlowMappingIndication) {
      m_associatedQosFlowItem[i].setAssociatedQosFlowItem(
          *list[i].qosFlowMappingIndication, m_qosFlowIdentifier);
    } else {
      m_associatedQosFlowItem[i].setAssociatedQosFlowItem(m_qosFlowIdentifier);
    }
  }
  m_associatedQosFlowList->setAssociatedQosFlowList(
      m_associatedQosFlowItem, list.size());

  dlQoSFlowPerTNLInformation->setDLQoSFlowPerTNLInformation(
      m_uPTransportLayerInformation, m_associatedQosFlowList);

  int ret = dlQoSFlowPerTNLInformation->encode2DLQoSFlowPerTNLInformation(
      &pduSessionResourceSetupResponseTransferIEs->qosFlowPerTNLInformation);
  if (!ret) {
    cout << "encode DLQoSFlowPerTNLInformation IE error" << endl;
    return;
  }
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseTransferIE::
    setAdditionalDLQoSFlowPerTNLInformation(
        GtpTunnel_t uptlinfo, std::vector<AssociatedQosFlow_t> list) {
  if (!additionalDLQoSFlowPerTNLInformation)
    additionalDLQoSFlowPerTNLInformation = new DLQoSFlowPerTNLInformation();
  UpTransportLayerInformation* m_uPTransportLayerInformation =
      new UpTransportLayerInformation();
  TransportLayerAddress* m_transportLayerAddress = new TransportLayerAddress();
  GtpTeid* m_gtpTeid                             = new GtpTeid();
  m_transportLayerAddress->setTransportLayerAddress(uptlinfo.ip_address);
  m_gtpTeid->setGtpTeid(uptlinfo.gtp_teid);
  m_uPTransportLayerInformation->setUpTransportLayerInformation(
      m_transportLayerAddress, m_gtpTeid);

  AssociatedQosFlowList* m_associatedQosFlowList = new AssociatedQosFlowList();
  AssociatedQosFlowItem* m_associatedQosFlowItem =
      new AssociatedQosFlowItem[list.size()]();
  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier* m_qosFlowIdentifier = new QosFlowIdentifier();
    m_qosFlowIdentifier->setQosFlowIdentifier(list[i].qosFlowIdentifier);
    if (list[i].qosFlowMappingIndication) {
      m_associatedQosFlowItem[i].setAssociatedQosFlowItem(
          *list[i].qosFlowMappingIndication, m_qosFlowIdentifier);
    } else {
      m_associatedQosFlowItem[i].setAssociatedQosFlowItem(m_qosFlowIdentifier);
    }
  }
  m_associatedQosFlowList->setAssociatedQosFlowList(
      m_associatedQosFlowItem, list.size());

  additionalDLQoSFlowPerTNLInformation->setDLQoSFlowPerTNLInformation(
      m_uPTransportLayerInformation, m_associatedQosFlowList);

  Ngap_QosFlowPerTNLInformation_t* ie =
      (Ngap_QosFlowPerTNLInformation_t*) calloc(
          1, sizeof(Ngap_QosFlowPerTNLInformation_t));
  int ret =
      additionalDLQoSFlowPerTNLInformation->encode2DLQoSFlowPerTNLInformation(
          ie);
  if (!ret) {
    cout << "encode AdditionalDLQoSFlowPerTNLInformation IE error" << endl;
    return;
  }
  pduSessionResourceSetupResponseTransferIEs
      ->additionalQosFlowPerTNLInformation = ie;
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupResponseTransferIE::setSecurityResult(
    e_Ngap_IntegrityProtectionResult integrityProtectionResult,
    e_Ngap_ConfidentialityProtectionResult confidentialityProtectionResult) {
  if (!securityResult) securityResult = new SecurityResult();

  IntegrityProtectionResult* m_integrityProtectionResult =
      new IntegrityProtectionResult();
  m_integrityProtectionResult->setIntegrityProtectionResult(
      integrityProtectionResult);
  ConfidentialityProtectionResult* m_confidentialityProtectionResult =
      new ConfidentialityProtectionResult();
  m_confidentialityProtectionResult->setConfidentialityProtectionResult(
      confidentialityProtectionResult);
  securityResult->setSecurityResult(
      m_integrityProtectionResult, m_confidentialityProtectionResult);

  Ngap_SecurityResult_t* ie =
      (Ngap_SecurityResult_t*) calloc(1, sizeof(Ngap_SecurityResult_t));
  int ret = securityResult->encode2SecurityResult(ie);
  if (!ret) {
    cout << "encode SecurityResult IE error" << endl;
    return;
  }
  pduSessionResourceSetupResponseTransferIEs->securityResult = ie;
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupResponseTransferIE::encode2buffer(
    uint8_t* buf, int buf_size) {
  asn_fprint(
      stderr, &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer,
      pduSessionResourceSetupResponseTransferIEs);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer, NULL,
      pduSessionResourceSetupResponseTransferIEs, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool PduSessionResourceSetupResponseTransferIE::decodefromIE(
    uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer,
      (void**) &pduSessionResourceSetupResponseTransferIEs, buf, buf_size);
  if (rc.code == RC_OK) {
    cout << "Decoded successfully" << endl;
  } else if (rc.code == RC_WMORE) {
    cout << "More data expected, call again" << endl;
    return false;
  } else {
    cout << "Failure to decode data" << endl;
    return false;
  }
  cout << "rc.consumed to decode = " << rc.consumed << endl;
  cout << endl;
  // asn_fprint(stderr, &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer,
  // pduSessionResourceSetupResponseTransferIEs);

  dlQoSFlowPerTNLInformation = new DLQoSFlowPerTNLInformation();
  if (!dlQoSFlowPerTNLInformation->decodefromDLQoSFlowPerTNLInformation(
          &pduSessionResourceSetupResponseTransferIEs
               ->qosFlowPerTNLInformation)) {
    cout << "decoded ngap DLQoSFlowPerTNLInformation IE error" << endl;
    return false;
  }

  if (pduSessionResourceSetupResponseTransferIEs
          ->additionalQosFlowPerTNLInformation) {
    additionalDLQoSFlowPerTNLInformation = new DLQoSFlowPerTNLInformation();
    if (!additionalDLQoSFlowPerTNLInformation
             ->decodefromDLQoSFlowPerTNLInformation(
                 pduSessionResourceSetupResponseTransferIEs
                     ->additionalQosFlowPerTNLInformation)) {
      cout << "decoded ngap AdditionalDLQoSFlowPerTNLInformation IE error"
           << endl;
      return false;
    }
  }
  if (pduSessionResourceSetupResponseTransferIEs->securityResult) {
    securityResult = new SecurityResult();
    if (!securityResult->decodefromSecurityResult(
            pduSessionResourceSetupResponseTransferIEs->securityResult)) {
      cout << "decoded ngap SecurityResult IE error" << endl;
      return false;
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseTransferIE::getDLQoSFlowPerTNLInformation(
    GtpTunnel_t& uptlinfo, std::vector<AssociatedQosFlow_t>& list) {
  if (!dlQoSFlowPerTNLInformation) return false;
  UpTransportLayerInformation* m_uPTransportLayerInformation;
  AssociatedQosFlowList* m_associatedQosFlowList;
  dlQoSFlowPerTNLInformation->getDLQoSFlowPerTNLInformation(
      m_uPTransportLayerInformation, m_associatedQosFlowList);
  TransportLayerAddress* m_transportLayerAddress;
  GtpTeid* m_gtpTeid;
  m_uPTransportLayerInformation->getUpTransportLayerInformation(
      m_transportLayerAddress, m_gtpTeid);
  m_transportLayerAddress->getTransportLayerAddress(uptlinfo.ip_address);
  m_gtpTeid->getGtpTeid(uptlinfo.gtp_teid);

  AssociatedQosFlowItem* m_associatedQosFlowItem;
  int numofitem;
  m_associatedQosFlowList->getAssociatedQosFlowList(
      m_associatedQosFlowItem, numofitem);
  for (int i = 0; i < numofitem; i++) {
    AssociatedQosFlow_t AssociatedQosFlow_str;
    long m_qosFlowMappingIndication;
    QosFlowIdentifier* m_qosFlowIdentifier;
    m_associatedQosFlowItem[i].getAssociatedQosFlowItem(
        m_qosFlowMappingIndication, m_qosFlowIdentifier);
    m_qosFlowIdentifier->getQosFlowIdentifier(
        AssociatedQosFlow_str.qosFlowIdentifier);
    if (m_qosFlowMappingIndication < 0) {
      AssociatedQosFlow_str.qosFlowMappingIndication = NULL;
    } else {
      AssociatedQosFlow_str.qosFlowMappingIndication =
          (e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication*) calloc(
              1,
              sizeof(e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication));
      *AssociatedQosFlow_str.qosFlowMappingIndication =
          (e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication)
              m_qosFlowMappingIndication;
    }

    list.push_back(AssociatedQosFlow_str);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseTransferIE::
    getAdditionalDLQoSFlowPerTNLInformation(
        GtpTunnel_t& uptlinfo, std::vector<AssociatedQosFlow_t>& list) {
  if (!additionalDLQoSFlowPerTNLInformation) return false;
  UpTransportLayerInformation* m_uPTransportLayerInformation;
  AssociatedQosFlowList* m_associatedQosFlowList;
  additionalDLQoSFlowPerTNLInformation->getDLQoSFlowPerTNLInformation(
      m_uPTransportLayerInformation, m_associatedQosFlowList);
  TransportLayerAddress* m_transportLayerAddress;
  GtpTeid* m_gtpTeid;
  m_uPTransportLayerInformation->getUpTransportLayerInformation(
      m_transportLayerAddress, m_gtpTeid);
  m_transportLayerAddress->getTransportLayerAddress(uptlinfo.ip_address);
  m_gtpTeid->getGtpTeid(uptlinfo.gtp_teid);

  AssociatedQosFlowItem* m_associatedQosFlowItem;
  int numofitem;
  m_associatedQosFlowList->getAssociatedQosFlowList(
      m_associatedQosFlowItem, numofitem);
  for (int i = 0; i < numofitem; i++) {
    AssociatedQosFlow_t AssociatedQosFlow_str;
    long m_qosFlowMappingIndication;
    QosFlowIdentifier* m_qosFlowIdentifier;
    m_associatedQosFlowItem[i].getAssociatedQosFlowItem(
        m_qosFlowMappingIndication, m_qosFlowIdentifier);
    m_qosFlowIdentifier->getQosFlowIdentifier(
        AssociatedQosFlow_str.qosFlowIdentifier);
    if (m_qosFlowMappingIndication < 0) {
      AssociatedQosFlow_str.qosFlowMappingIndication = NULL;
    } else {
      AssociatedQosFlow_str.qosFlowMappingIndication =
          (e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication*) calloc(
              1,
              sizeof(e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication));

      *AssociatedQosFlow_str.qosFlowMappingIndication =
          (e_Ngap_AssociatedQosFlowItem__qosFlowMappingIndication)
              m_qosFlowMappingIndication;
    }

    list.push_back(AssociatedQosFlow_str);
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupResponseTransferIE::getSecurityResult(
    long& integrityProtectionResult, long& confidentialityProtectionResult) {
  if (!securityResult) return false;

  IntegrityProtectionResult* m_integrityProtectionResult;
  ConfidentialityProtectionResult* m_confidentialityProtectionResult;
  securityResult->getSecurityResult(
      m_integrityProtectionResult, m_confidentialityProtectionResult);

  m_integrityProtectionResult->getIntegrityProtectionResult(
      integrityProtectionResult);
  m_confidentialityProtectionResult->getConfidentialityProtectionResult(
      confidentialityProtectionResult);

  return true;
}
}  // namespace ngap
