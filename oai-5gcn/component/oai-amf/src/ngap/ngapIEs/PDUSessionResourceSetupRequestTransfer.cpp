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

#include "PDUSessionResourceSetupRequestTransfer.hpp"

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
PduSessionResourceSetupRequestTransferIE::
    PduSessionResourceSetupRequestTransferIE() {
  pduSessionResourceSetupRequestTransferIEs =
      (Ngap_PDUSessionResourceSetupRequestTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransfer_t));
  pduSessionAggregateMaximumBitRateIE = NULL;
  upTransportLayerInformation         = NULL;
  dataForwardingNotPossible           = NULL;
  pduSessionType                      = NULL;
  securityIndication                  = NULL;
  networkInstance                     = NULL;
  qosFlowSetupRequestList             = NULL;
}

//------------------------------------------------------------------------------
PduSessionResourceSetupRequestTransferIE::
    ~PduSessionResourceSetupRequestTransferIE() {}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::
    setPduSessionAggregateMaximumBitRate(
        long bit_rate_downlink, long bit_rate_uplink) {
  if (!pduSessionAggregateMaximumBitRateIE)
    pduSessionAggregateMaximumBitRateIE =
        new PduSessionAggregateMaximumBitRate();
  pduSessionAggregateMaximumBitRateIE->setPduSessionAggregateMaximumBitRate(
      bit_rate_downlink, bit_rate_uplink);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate;

  int ret = pduSessionAggregateMaximumBitRateIE
                ->encode2PduSessionAggregateMaximumBitRate(
                    ie->value.choice.PDUSessionAggregateMaximumBitRate);
  if (!ret) {
    cout << "encode PDUSessionAggregateMaximumBitRate IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0)
    cout << "encode PDUSessionAggregateMaximumBitRate IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setUL_NG_U_UP_TNL_Information(
    GtpTunnel_t upTnlInfo) {
  if (!upTransportLayerInformation)
    upTransportLayerInformation = new UpTransportLayerInformation();
  TransportLayerAddress* m_transportLayerAddress = new TransportLayerAddress();
  m_transportLayerAddress->setTransportLayerAddress(upTnlInfo.ip_address);
  GtpTeid* m_gtpTeid = new GtpTeid();
  m_gtpTeid->setGtpTeid(upTnlInfo.gtp_teid);
  upTransportLayerInformation->setUpTransportLayerInformation(
      m_transportLayerAddress, m_gtpTeid);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation;

  int ret = upTransportLayerInformation->encode2UpTransportLayerInformation(
      ie->value.choice.UPTransportLayerInformation);
  if (!ret) {
    cout << "encode UPTransportLayerInformation IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode UPTransportLayerInformation IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
// void
// PduSessionResourceSetupRequestTransferIE::setAdditionalUL_NG_U_UP_TNL_Information(std::vector<GtpTunnel>list);//O
void PduSessionResourceSetupRequestTransferIE::setDataForwardingNotPossible() {
  if (!dataForwardingNotPossible)
    dataForwardingNotPossible = new DataForwardingNotPossible();

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_DataForwardingNotPossible;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_DataForwardingNotPossible;

  int ret = dataForwardingNotPossible->encode2DataForwardingNotPossible(
      ie->value.choice.DataForwardingNotPossible);
  if (!ret) {
    cout << "encode DataForwardingNotPossible IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode DataForwardingNotPossible IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setPduSessionType(
    e_Ngap_PDUSessionType type) {
  if (!pduSessionType) pduSessionType = new PDUSessionType();

  pduSessionType->setPDUSessionType(type);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_PDUSessionType;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType;

  int ret =
      pduSessionType->encode2PDUSessionType(ie->value.choice.PDUSessionType);
  if (!ret) {
    cout << "encode PDUSessionType IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode PDUSessionType IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setSecurityIndication(
    e_Ngap_IntegrityProtectionIndication integrity_protection,
    e_Ngap_ConfidentialityProtectionIndication confidentiality_protection,
    e_Ngap_MaximumIntegrityProtectedDataRate maxIntProtDataRate) {
  if (!securityIndication) securityIndication = new SecurityIndication();

  IntegrityProtectionIndication* m_integrityProtectionIndication =
      new IntegrityProtectionIndication();
  ConfidentialityProtectionIndication* m_confidentialityProtectionIndication =
      new ConfidentialityProtectionIndication();
  MaximumIntegrityProtectedDataRate* m_maximumIntegrityProtectedDataRate =
      new MaximumIntegrityProtectedDataRate();
  m_integrityProtectionIndication->setIntegrityProtectionIndication(
      integrity_protection);
  m_confidentialityProtectionIndication->setConfidentialityProtectionIndication(
      confidentiality_protection);
  m_maximumIntegrityProtectedDataRate->setMaximumIntegrityProtectedDataRate(
      maxIntProtDataRate);
  securityIndication->setSecurityIndication(
      m_integrityProtectionIndication, m_confidentialityProtectionIndication,
      m_maximumIntegrityProtectedDataRate);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SecurityIndication;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication;

  int ret = securityIndication->encode2SecurityIndication(
      &ie->value.choice.SecurityIndication);
  if (!ret) {
    cout << "encode SecurityIndication IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode SecurityIndication IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setSecurityIndication(
    e_Ngap_IntegrityProtectionIndication integrity_protection,
    e_Ngap_ConfidentialityProtectionIndication confidentiality_protection) {
  if (!securityIndication) securityIndication = new SecurityIndication();

  IntegrityProtectionIndication* m_integrityProtectionIndication =
      new IntegrityProtectionIndication();
  ConfidentialityProtectionIndication* m_confidentialityProtectionIndication =
      new ConfidentialityProtectionIndication();

  m_integrityProtectionIndication->setIntegrityProtectionIndication(
      integrity_protection);
  m_confidentialityProtectionIndication->setConfidentialityProtectionIndication(
      confidentiality_protection);

  securityIndication->setSecurityIndication(
      m_integrityProtectionIndication, m_confidentialityProtectionIndication,
      NULL);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_SecurityIndication;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication;

  int ret = securityIndication->encode2SecurityIndication(
      &ie->value.choice.SecurityIndication);
  if (!ret) {
    cout << "encode SecurityIndication IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode SecurityIndication IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setNetworkInstance(long value) {
  if (!networkInstance) networkInstance = new NetworkInstance();

  networkInstance->setNetworkInstance(value);

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_NetworkInstance;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_NetworkInstance;

  int ret =
      networkInstance->encode2NetworkInstance(ie->value.choice.NetworkInstance);
  if (!ret) {
    cout << "encode NetworkInstance IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode NetworkInstance IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
void PduSessionResourceSetupRequestTransferIE::setQosFlowSetupRequestList(
    std::vector<QosFlowSetupReq_t> list) {
  if (!qosFlowSetupRequestList)
    qosFlowSetupRequestList = new QosFlowSetupRequestList();

  QosFlowSetupRequestItem* m_items = new QosFlowSetupRequestItem[list.size()]();

  for (int i = 0; i < list.size(); i++) {
    QosFlowIdentifier* m_qosFlowIdentifier = new QosFlowIdentifier();
    m_qosFlowIdentifier->setQosFlowIdentifier(list[i].qos_flow_id);

    QosCharacteristics* m_qosCharacteristics = new QosCharacteristics();
    if (list[i].qflqp.qosc.nonDy) {
      FiveQI* m_fiveQI                                 = new FiveQI();
      PriorityLevelQos* m_priorityLevelQos             = NULL;
      AveragingWindow* m_averagingWindow               = NULL;
      MaximumDataBurstVolume* m_maximumDataBurstVolume = NULL;

      m_fiveQI->setFiveQI(list[i].qflqp.qosc.nonDy->_5QI);
      if (list[i].qflqp.qosc.nonDy->priorityLevelQos) {
        m_priorityLevelQos = new PriorityLevelQos();
        m_priorityLevelQos->setPriorityLevelQos(
            *list[i].qflqp.qosc.nonDy->priorityLevelQos);
      }
      if (list[i].qflqp.qosc.nonDy->averagingWindow) {
        m_averagingWindow = new AveragingWindow();
        m_averagingWindow->setAveragingWindow(
            *list[i].qflqp.qosc.nonDy->averagingWindow);
      }
      if (list[i].qflqp.qosc.nonDy->maximumDataBurstVolume) {
        m_maximumDataBurstVolume = new MaximumDataBurstVolume();
        m_maximumDataBurstVolume->setMaximumDataBurstVolume(
            *list[i].qflqp.qosc.nonDy->maximumDataBurstVolume);
      }

      NonDynamic5QIDescriptor* m_nonDynamic5QIDescriptor =
          new NonDynamic5QIDescriptor();
      m_nonDynamic5QIDescriptor->setNonDynamic5QIDescriptor(
          m_fiveQI, m_priorityLevelQos, m_averagingWindow,
          m_maximumDataBurstVolume);

      m_qosCharacteristics->setQosCharacteristics(m_nonDynamic5QIDescriptor);
    } else {
      PriorityLevelQos* m_priorityLevelQos   = new PriorityLevelQos();
      PacketDelayBudget* m_packetDelayBudget = new PacketDelayBudget();
      PacketErrorRate* m_packetErrorRate     = new PacketErrorRate();
      FiveQI* m_fiveQI                       = NULL;
      DelayCritical* m_delayCritical         = NULL;
      AveragingWindow* m_averagingWindow     = NULL;
      MaximumDataBurstVolume* m_maximumDataBurstVolume = NULL;

      m_priorityLevelQos->setPriorityLevelQos(
          list[i].qflqp.qosc.dy->priorityLevelQos);
      m_packetDelayBudget->setPacketDelayBudget(
          list[i].qflqp.qosc.dy->packetDelayBudget);
      m_packetErrorRate->setPacketErrorRate(
          list[i].qflqp.qosc.dy->packetErrorRate.pERScalar,
          list[i].qflqp.qosc.dy->packetErrorRate.pERExponent);
      if (list[i].qflqp.qosc.dy->_5QI) {
        m_fiveQI = new FiveQI();
        m_fiveQI->setFiveQI(*list[i].qflqp.qosc.dy->_5QI);
      }
      if (list[i].qflqp.qosc.dy->delayCritical) {
        m_delayCritical = new DelayCritical();
        m_delayCritical->setDelayCritical(
            *list[i].qflqp.qosc.dy->delayCritical);
      }
      if (list[i].qflqp.qosc.dy->averagingWindow) {
        m_averagingWindow = new AveragingWindow();
        m_averagingWindow->setAveragingWindow(
            *list[i].qflqp.qosc.dy->averagingWindow);
      }
      if (list[i].qflqp.qosc.dy->maximumDataBurstVolume) {
        m_maximumDataBurstVolume = new MaximumDataBurstVolume();
        m_maximumDataBurstVolume->setMaximumDataBurstVolume(
            *list[i].qflqp.qosc.dy->maximumDataBurstVolume);
      }

      Dynamic5QIDescriptor* m_dynamic5QIDescriptor = new Dynamic5QIDescriptor();
      m_dynamic5QIDescriptor->setDynamic5QIDescriptor(
          m_priorityLevelQos, m_packetDelayBudget, m_packetErrorRate, m_fiveQI,
          m_delayCritical, m_averagingWindow, m_maximumDataBurstVolume);

      m_qosCharacteristics->setQosCharacteristics(m_dynamic5QIDescriptor);
    }

    PriorityLevelARP* m_priorityLevelARP = new PriorityLevelARP();
    m_priorityLevelARP->setPriorityLevelARP(list[i].qflqp.arp.priorityLevelARP);
    Pre_emptionCapability* m_pre_emptionCapability =
        new Pre_emptionCapability();
    m_pre_emptionCapability->setPre_emptionCapability(
        list[i].qflqp.arp.pre_emptionCapability);
    Pre_emptionVulnerability* m_pre_emptionVulnerability =
        new Pre_emptionVulnerability();
    m_pre_emptionVulnerability->setPre_emptionVulnerability(
        list[i].qflqp.arp.pre_emptionVulnerability);
    AllocationAndRetentionPriority* m_allocationAndRetentionPriority =
        new AllocationAndRetentionPriority();
    m_allocationAndRetentionPriority->setAllocationAndRetentionPriority(
        m_priorityLevelARP, m_pre_emptionCapability,
        m_pre_emptionVulnerability);

    GBR_QosInformation* m_gBR_QosInformation = NULL;
    if (list[i].qflqp.gbr_qos_info) {
      NotificationControl* m_notificationControl = NULL;
      if (list[i].qflqp.gbr_qos_info->notificationControl) {
        m_notificationControl = new NotificationControl();
        m_notificationControl->setNotificationControl(
            *list[i].qflqp.gbr_qos_info->notificationControl);
      }
      PacketLossRate* m_maximumPacketLossRateDL = NULL;
      if (list[i].qflqp.gbr_qos_info->maximumPacketLossRateDL) {
        m_maximumPacketLossRateDL = new PacketLossRate();
        m_maximumPacketLossRateDL->setPacketLossRate(
            *list[i].qflqp.gbr_qos_info->maximumPacketLossRateDL);
      }
      PacketLossRate* m_maximumPacketLossRateUL = NULL;
      if (list[i].qflqp.gbr_qos_info->maximumPacketLossRateUL) {
        m_maximumPacketLossRateUL = new PacketLossRate();
        m_maximumPacketLossRateUL->setPacketLossRate(
            *list[i].qflqp.gbr_qos_info->maximumPacketLossRateUL);
      }

      m_gBR_QosInformation = new GBR_QosInformation();
      m_gBR_QosInformation->setGBR_QosInformation(
          list[i].qflqp.gbr_qos_info->maximumFlowBitRateDL,
          list[i].qflqp.gbr_qos_info->maximumFlowBitRateUL,
          list[i].qflqp.gbr_qos_info->guaranteedFlowBitRateDL,
          list[i].qflqp.gbr_qos_info->guaranteedFlowBitRateUL,
          m_notificationControl, m_maximumPacketLossRateDL,
          m_maximumPacketLossRateUL);
    }

    ReflectiveQosAttribute* m_reflectiveQosAttribute = NULL;
    if (list[i].qflqp.reflectiveQosAttribute) {
      m_reflectiveQosAttribute = new ReflectiveQosAttribute();
      m_reflectiveQosAttribute->setReflectiveQosAttribute(
          *list[i].qflqp.reflectiveQosAttribute);
    }
    AdditionalQosFlowInformation* m_additionalQosFlowInformation = NULL;
    if (list[i].qflqp.additionalQosFlowInformation) {
      m_additionalQosFlowInformation = new AdditionalQosFlowInformation();
      m_additionalQosFlowInformation->setAdditionalQosFlowInformation(
          *list[i].qflqp.additionalQosFlowInformation);
    }

    QosFlowLevelQosParameters* m_qosFlowLevelQosParameters =
        new QosFlowLevelQosParameters();
    m_qosFlowLevelQosParameters->setQosFlowLevelQosParameters(
        m_qosCharacteristics, m_allocationAndRetentionPriority,
        m_gBR_QosInformation, m_reflectiveQosAttribute,
        m_additionalQosFlowInformation);

    m_items[i].setQosFlowSetupRequestItem(
        m_qosFlowIdentifier, m_qosFlowLevelQosParameters);
  }

  qosFlowSetupRequestList->setQosFlowSetupRequestList(m_items, list.size());

  Ngap_PDUSessionResourceSetupRequestTransferIEs_t* ie =
      (Ngap_PDUSessionResourceSetupRequestTransferIEs_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceSetupRequestTransferIEs_t));
  ie->id          = Ngap_ProtocolIE_ID_id_QosFlowSetupRequestList;
  ie->criticality = Ngap_Criticality_reject;
  ie->value.present =
      Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList;

  int ret = qosFlowSetupRequestList->encode2QosFlowSetupRequestList(
      &ie->value.choice.QosFlowSetupRequestList);
  if (!ret) {
    cout << "encode QosFlowSetupRequestList IE error" << endl;
    free_wrapper((void**) &ie);
    return;
  }

  ret = ASN_SEQUENCE_ADD(
      &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list, ie);
  if (ret != 0) cout << "encode QosFlowSetupRequestList IE error" << endl;
  // free_wrapper((void**) &ie);
}

//------------------------------------------------------------------------------
int PduSessionResourceSetupRequestTransferIE::encode2buffer(
    uint8_t* buf, int buf_size) {
  asn_fprint(
      stderr, &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer,
      pduSessionResourceSetupRequestTransferIEs);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer, NULL,
      pduSessionResourceSetupRequestTransferIEs, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  // asn_fprint(stderr, er.failed_type, er.structure_ptr);
  return er.encoded;
}

//------------------------------------------------------------------------------
// Decapsulation
bool PduSessionResourceSetupRequestTransferIE::decodefromIE(
    uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer,
      (void**) &pduSessionResourceSetupRequestTransferIEs, buf, buf_size);
  if (rc.code == RC_OK) {
    cout << "Decoded successfully" << endl;
  } else if (rc.code == RC_WMORE) {
    cout << "More data expected, call again" << endl;
    return false;
  } else {
    cout << "Failure to decode data" << endl;
    return false;
  }
  cout << endl;
  // asn_fprint(stderr, &asn_DEF_Ngap_PDUSessionResourceSetupRequestTransfer,
  // pduSessionResourceSetupRequestTransferIEs);

  for (int i = 0;
       i < pduSessionResourceSetupRequestTransferIEs->protocolIEs.list.count;
       i++) {
    switch (
        pduSessionResourceSetupRequestTransferIEs->protocolIEs.list.array[i]
            ->id) {
      case Ngap_ProtocolIE_ID_id_PDUSessionAggregateMaximumBitRate: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionAggregateMaximumBitRate) {
          pduSessionAggregateMaximumBitRateIE =
              new PduSessionAggregateMaximumBitRate();
          if (!pduSessionAggregateMaximumBitRateIE
                   ->decodefromPduSessionAggregateMaximumBitRate(
                       pduSessionResourceSetupRequestTransferIEs->protocolIEs
                           .list.array[i]
                           ->value.choice.PDUSessionAggregateMaximumBitRate)) {
            cout << "decoded ngap PDUSessionAggregateMaximumBitRate IE error"
                 << endl;
            return false;
          }
        } else {
          cout << "decoded ngap PDUSessionAggregateMaximumBitRate IE error"
               << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_UL_NGU_UP_TNLInformation: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_UPTransportLayerInformation) {
          upTransportLayerInformation = new UpTransportLayerInformation();
          if (!upTransportLayerInformation
                   ->decodefromUpTransportLayerInformation(
                       pduSessionResourceSetupRequestTransferIEs->protocolIEs
                           .list.array[i]
                           ->value.choice.UPTransportLayerInformation)) {
            cout << "decoded ngap UPTransportLayerInformation IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap UPTransportLayerInformation IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_DataForwardingNotPossible: {
        if ((pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                 .array[i]
                 ->criticality) == Ngap_Criticality_reject &&
            (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                 .array[i]
                 ->value.present ==
             Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_DataForwardingNotPossible)) {
          dataForwardingNotPossible = new DataForwardingNotPossible();
          if (!dataForwardingNotPossible->decodefromDataForwardingNotPossible(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.DataForwardingNotPossible)) {
            cout << "decoded ngap DataForwardingNotPossible IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap DataForwardingNotPossible IE error!" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_PDUSessionType: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_PDUSessionType) {
          pduSessionType = new PDUSessionType();
          if (!pduSessionType->decodefromPDUSessionType(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.PDUSessionType)) {
            cout << "decoded ngap PDUSessionType IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap PDUSessionType IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_SecurityIndication: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_SecurityIndication) {
          securityIndication = new SecurityIndication();
          if (!securityIndication->decodefromSecurityIndication(
                  &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                       .array[i]
                       ->value.choice.SecurityIndication)) {
            cout << "decoded ngap SecurityIndication IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap SecurityIndication IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_NetworkInstance: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_NetworkInstance) {
          networkInstance = new NetworkInstance();
          if (!networkInstance->decodefromNetworkInstance(
                  pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                      .array[i]
                      ->value.choice.NetworkInstance)) {
            cout << "decoded ngap NetworkInstance IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap NetworkInstance IE error" << endl;
          return false;
        }
      } break;
      case Ngap_ProtocolIE_ID_id_QosFlowSetupRequestList: {
        if (pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->criticality == Ngap_Criticality_reject &&
            pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                    .array[i]
                    ->value.present ==
                Ngap_PDUSessionResourceSetupRequestTransferIEs__value_PR_QosFlowSetupRequestList) {
          qosFlowSetupRequestList = new QosFlowSetupRequestList();
          if (!qosFlowSetupRequestList->decodefromQosFlowSetupRequestList(
                  &pduSessionResourceSetupRequestTransferIEs->protocolIEs.list
                       .array[i]
                       ->value.choice.QosFlowSetupRequestList)) {
            cout << "decoded ngap QosFlowSetupRequestList IE error" << endl;
            return false;
          }
        } else {
          cout << "decoded ngap QosFlowSetupRequestList IE error" << endl;
          return false;
        }
      } break;

      default: {
        cout << "decoded ngap message PduSessionResourceSetupRequestTransferIE "
                "error"
             << endl;
        return false;
      }
    }
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::
    getPduSessionAggregateMaximumBitRate(
        long& bit_rate_downlink, long& bit_rate_uplink) {
  if (!pduSessionAggregateMaximumBitRateIE) return false;

  if (!pduSessionAggregateMaximumBitRateIE
           ->getPduSessionAggregateMaximumBitRate(
               bit_rate_downlink, bit_rate_uplink))
    return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getUL_NG_U_UP_TNL_Information(
    GtpTunnel_t& upTnlInfo) {
  if (!upTransportLayerInformation) return false;

  TransportLayerAddress* m_transportLayerAddress;
  GtpTeid* m_gtpTeid;
  if (!upTransportLayerInformation->getUpTransportLayerInformation(
          m_transportLayerAddress, m_gtpTeid))
    return false;
  if (!m_transportLayerAddress->getTransportLayerAddress(upTnlInfo.ip_address))
    return false;
  if (!m_gtpTeid->getGtpTeid(upTnlInfo.gtp_teid)) return false;

  return true;
}

//------------------------------------------------------------------------------
// bool getAdditionalUL_NG_U_UP_TNL_Information(std::vector<GtpTunnel>&list);//O
bool PduSessionResourceSetupRequestTransferIE::getDataForwardingNotPossible() {
  if (!dataForwardingNotPossible) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getPduSessionType(long& type) {
  if (!pduSessionType) return false;

  if (!pduSessionType->getPDUSessionType(type)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getSecurityIndication(
    long& integrity_protection, long& confidentiality_protection,
    long& maxIntProtDataRate) {
  if (!securityIndication) return false;

  IntegrityProtectionIndication* m_integrityProtectionIndication;
  ConfidentialityProtectionIndication* m_confidentialityProtectionIndication;
  MaximumIntegrityProtectedDataRate* m_maximumIntegrityProtectedDataRate;

  securityIndication->getSecurityIndication(
      m_integrityProtectionIndication, m_confidentialityProtectionIndication,
      m_maximumIntegrityProtectedDataRate);

  if (!m_integrityProtectionIndication->getIntegrityProtectionIndication(
          integrity_protection))
    return false;
  if (!m_confidentialityProtectionIndication
           ->getConfidentialityProtectionIndication(confidentiality_protection))
    return false;
  if (m_maximumIntegrityProtectedDataRate)
    m_maximumIntegrityProtectedDataRate->getMaximumIntegrityProtectedDataRate(
        maxIntProtDataRate);
  else
    maxIntProtDataRate = -1;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getNetworkInstance(long& value) {
  if (!networkInstance) return false;

  if (!networkInstance->getNetworkInstance(value)) return false;

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceSetupRequestTransferIE::getQosFlowSetupRequestList(
    std::vector<QosFlowSetupReq_t>& list) {
  if (!qosFlowSetupRequestList) return false;

  QosFlowSetupRequestItem* m_items;
  int m_numofitems;
  if (!qosFlowSetupRequestList->getQosFlowSetupRequestList(
          m_items, m_numofitems))
    return false;
  for (int i = 0; i < m_numofitems; i++) {
    QosFlowIdentifier* m_qosFlowIdentifier;
    QosFlowLevelQosParameters* m_qosFlowLevelQosParameters;

    m_items[i].getQosFlowSetupRequestItem(
        m_qosFlowIdentifier, m_qosFlowLevelQosParameters);

    QosFlowSetupReq_t qosflowsetuprequest;
    if (!m_qosFlowIdentifier->getQosFlowIdentifier(
            qosflowsetuprequest.qos_flow_id))
      return false;
    QosCharacteristics* m_qosCharacteristics;
    AllocationAndRetentionPriority* m_allocationAndRetentionPriority;
    GBR_QosInformation* m_gBR_QosInformation;
    ReflectiveQosAttribute* m_reflectiveQosAttribute;
    AdditionalQosFlowInformation* m_additionalQosFlowInformation;
    if (!m_qosFlowLevelQosParameters->getQosFlowLevelQosParameters(
            m_qosCharacteristics, m_allocationAndRetentionPriority,
            m_gBR_QosInformation, m_reflectiveQosAttribute,
            m_additionalQosFlowInformation))
      return false;

    if (m_qosCharacteristics->QosCharacteristicsPresent() ==
        Ngap_QosCharacteristics_PR_nonDynamic5QI) {
      qosflowsetuprequest.qflqp.qosc.nonDy =
          (NonDynamic5QI_t*) calloc(1, sizeof(NonDynamic5QI_t));
      NonDynamic5QIDescriptor* m_nonDynamic5QIDescriptor;
      m_qosCharacteristics->getQosCharacteristics(m_nonDynamic5QIDescriptor);
      FiveQI* m_fiveQI;
      PriorityLevelQos* m_priorityLevelQos;
      AveragingWindow* m_averagingWindow;
      MaximumDataBurstVolume* m_maximumDataBurstVolume;
      m_nonDynamic5QIDescriptor->getNonDynamic5QIDescriptor(
          m_fiveQI, m_priorityLevelQos, m_averagingWindow,
          m_maximumDataBurstVolume);

      m_fiveQI->getFiveQI(qosflowsetuprequest.qflqp.qosc.nonDy->_5QI);
      if (m_priorityLevelQos) {
        qosflowsetuprequest.qflqp.qosc.nonDy->priorityLevelQos =
            (long*) calloc(1, sizeof(long));
        m_priorityLevelQos->getPriorityLevelQos(
            *qosflowsetuprequest.qflqp.qosc.nonDy->priorityLevelQos);
      } else {
        qosflowsetuprequest.qflqp.qosc.nonDy->priorityLevelQos = NULL;
      }
      if (m_averagingWindow) {
        qosflowsetuprequest.qflqp.qosc.nonDy->averagingWindow =
            (long*) calloc(1, sizeof(long));
        m_averagingWindow->getAveragingWindow(
            *qosflowsetuprequest.qflqp.qosc.nonDy->averagingWindow);
      } else {
        qosflowsetuprequest.qflqp.qosc.nonDy->averagingWindow = NULL;
      }
      if (m_maximumDataBurstVolume) {
        qosflowsetuprequest.qflqp.qosc.nonDy->maximumDataBurstVolume =
            (long*) calloc(1, sizeof(long));
        m_maximumDataBurstVolume->getMaximumDataBurstVolume(
            *qosflowsetuprequest.qflqp.qosc.nonDy->maximumDataBurstVolume);
      } else {
        qosflowsetuprequest.qflqp.qosc.nonDy->maximumDataBurstVolume = NULL;
      }
    } else if (
        m_qosCharacteristics->QosCharacteristicsPresent() ==
        Ngap_QosCharacteristics_PR_dynamic5QI) {
      qosflowsetuprequest.qflqp.qosc.dy =
          (Dynamic5QI_t*) calloc(1, sizeof(Dynamic5QI_t));
      Dynamic5QIDescriptor* m_dynamic5QIDescriptor;
      m_qosCharacteristics->getQosCharacteristics(m_dynamic5QIDescriptor);
      PriorityLevelQos* m_priorityLevelQos;
      PacketDelayBudget* m_packetDelayBudget;
      PacketErrorRate* m_packetErrorRate;

      FiveQI* m_fiveQI;
      DelayCritical* m_delayCritical;
      AveragingWindow* m_averagingWindow;
      MaximumDataBurstVolume* m_maximumDataBurstVolume;
      m_dynamic5QIDescriptor->getDynamic5QIDescriptor(
          m_priorityLevelQos, m_packetDelayBudget, m_packetErrorRate, m_fiveQI,
          m_delayCritical, m_averagingWindow, m_maximumDataBurstVolume);

      m_priorityLevelQos->getPriorityLevelQos(
          qosflowsetuprequest.qflqp.qosc.dy->priorityLevelQos);
      m_packetDelayBudget->getPacketDelayBudget(
          qosflowsetuprequest.qflqp.qosc.dy->packetDelayBudget);
      m_packetErrorRate->getPacketErrorRate(
          qosflowsetuprequest.qflqp.qosc.dy->packetErrorRate.pERScalar,
          qosflowsetuprequest.qflqp.qosc.dy->packetErrorRate.pERExponent);
      if (m_fiveQI) {
        qosflowsetuprequest.qflqp.qosc.dy->_5QI =
            (long*) calloc(1, sizeof(long));
        m_fiveQI->getFiveQI(*qosflowsetuprequest.qflqp.qosc.dy->_5QI);
      } else {
        qosflowsetuprequest.qflqp.qosc.dy->_5QI = NULL;
      }
      if (m_delayCritical) {
        qosflowsetuprequest.qflqp.qosc.dy->delayCritical =
            (e_Ngap_DelayCritical*) calloc(1, sizeof(e_Ngap_DelayCritical));
        m_delayCritical->getDelayCritical(
            *qosflowsetuprequest.qflqp.qosc.dy->delayCritical);
      } else {
        qosflowsetuprequest.qflqp.qosc.dy->delayCritical = NULL;
      }
      if (m_averagingWindow) {
        qosflowsetuprequest.qflqp.qosc.dy->averagingWindow =
            (long*) calloc(1, sizeof(long));
        m_averagingWindow->getAveragingWindow(
            *qosflowsetuprequest.qflqp.qosc.dy->averagingWindow);
      } else {
        qosflowsetuprequest.qflqp.qosc.dy->averagingWindow = NULL;
      }
      if (m_maximumDataBurstVolume) {
        qosflowsetuprequest.qflqp.qosc.dy->maximumDataBurstVolume =
            (long*) calloc(1, sizeof(long));
        m_maximumDataBurstVolume->getMaximumDataBurstVolume(
            *qosflowsetuprequest.qflqp.qosc.dy->maximumDataBurstVolume);
      } else {
        qosflowsetuprequest.qflqp.qosc.dy->maximumDataBurstVolume = NULL;
      }
    } else
      return false;

    PriorityLevelARP* m_priorityLevelARP;
    Pre_emptionCapability* m_pre_emptionCapability;
    Pre_emptionVulnerability* m_pre_emptionVulnerability;
    if (!m_allocationAndRetentionPriority->getAllocationAndRetentionPriority(
            m_priorityLevelARP, m_pre_emptionCapability,
            m_pre_emptionVulnerability))
      return false;
    m_priorityLevelARP->getPriorityLevelARP(
        qosflowsetuprequest.qflqp.arp.priorityLevelARP);
    m_pre_emptionCapability->getPre_emptionCapability(
        qosflowsetuprequest.qflqp.arp.pre_emptionCapability);
    m_pre_emptionVulnerability->getPre_emptionVulnerability(
        qosflowsetuprequest.qflqp.arp.pre_emptionVulnerability);

    if (m_gBR_QosInformation) {
      qosflowsetuprequest.qflqp.gbr_qos_info =
          (GBR_QosInformation_t*) calloc(1, sizeof(GBR_QosInformation_t));
      NotificationControl* m_notificationControl;
      PacketLossRate* m_maximumPacketLossRateDL;
      PacketLossRate* m_maximumPacketLossRateUL;
      m_gBR_QosInformation->getGBR_QosInformation(
          qosflowsetuprequest.qflqp.gbr_qos_info->maximumFlowBitRateDL,
          qosflowsetuprequest.qflqp.gbr_qos_info->maximumFlowBitRateUL,
          qosflowsetuprequest.qflqp.gbr_qos_info->guaranteedFlowBitRateDL,
          qosflowsetuprequest.qflqp.gbr_qos_info->guaranteedFlowBitRateUL,
          m_notificationControl, m_maximumPacketLossRateDL,
          m_maximumPacketLossRateUL);
      if (m_notificationControl) {
        qosflowsetuprequest.qflqp.gbr_qos_info->notificationControl =
            (e_Ngap_NotificationControl*) calloc(
                1, sizeof(e_Ngap_NotificationControl));
        m_notificationControl->getNotificationControl(
            *qosflowsetuprequest.qflqp.gbr_qos_info->notificationControl);
      } else {
        qosflowsetuprequest.qflqp.gbr_qos_info->notificationControl = NULL;
      }
      if (m_maximumPacketLossRateDL) {
        qosflowsetuprequest.qflqp.gbr_qos_info->maximumPacketLossRateDL =
            (long*) calloc(1, sizeof(long));
        m_maximumPacketLossRateDL->getPacketLossRate(
            *qosflowsetuprequest.qflqp.gbr_qos_info->maximumPacketLossRateDL);
      } else {
        qosflowsetuprequest.qflqp.gbr_qos_info->maximumPacketLossRateDL = NULL;
      }
      if (m_maximumPacketLossRateUL) {
        qosflowsetuprequest.qflqp.gbr_qos_info->maximumPacketLossRateUL =
            (long*) calloc(1, sizeof(long));
        m_maximumPacketLossRateUL->getPacketLossRate(
            *qosflowsetuprequest.qflqp.gbr_qos_info->maximumPacketLossRateUL);
      } else {
        qosflowsetuprequest.qflqp.gbr_qos_info->maximumPacketLossRateUL = NULL;
      }
    } else {
      qosflowsetuprequest.qflqp.gbr_qos_info = NULL;
    }
    if (m_reflectiveQosAttribute) {
      qosflowsetuprequest.qflqp.reflectiveQosAttribute =
          (e_Ngap_ReflectiveQosAttribute*) calloc(
              1, sizeof(e_Ngap_ReflectiveQosAttribute));
      m_reflectiveQosAttribute->getReflectiveQosAttribute(
          *qosflowsetuprequest.qflqp.reflectiveQosAttribute);
    } else {
      qosflowsetuprequest.qflqp.reflectiveQosAttribute = NULL;
    }
    if (m_additionalQosFlowInformation) {
      qosflowsetuprequest.qflqp.additionalQosFlowInformation =
          (e_Ngap_AdditionalQosFlowInformation*) calloc(
              1, sizeof(e_Ngap_AdditionalQosFlowInformation));
      m_additionalQosFlowInformation->getAdditionalQosFlowInformation(
          *qosflowsetuprequest.qflqp.additionalQosFlowInformation);
    } else {
      qosflowsetuprequest.qflqp.additionalQosFlowInformation = NULL;
    }

    list.push_back(qosflowsetuprequest);
  }

  return true;
}

}  // namespace ngap
