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
 \author  Keliang Du, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */
#ifndef _UPLINKRANSTATUSTRANSFER_H_
#define _UPLINKRANSTATUSTRANSFER_H_
#include "AMF-UE-NGAP-ID.hpp"
#include "NgapIEsStruct.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "RANStatusTransferTransparentContainer.hpp"
extern "C" {
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_RANStatusTransfer-TransparentContainer.h"
#include "Ngap_UplinkRANStatusTransfer.h"
}
namespace ngap {
class UplinkRANStatusTransfer {
 public:
  UplinkRANStatusTransfer();
  virtual ~UplinkRANStatusTransfer();
  unsigned long getAmfUeNgapId();  // return -1;
  uint32_t getRanUeNgapId();       // return -1;
  void getRANStatusTransfer_TransparentContainer(
      RANStatusTransferTransparentContainer*&
          ranstatustransfer_transparentcontainer);
  bool defromPDU(Ngap_NGAP_PDU_t* ngap_msg_pdu);

 private:
  Ngap_NGAP_PDU_t* UplinkRANStatusTransferPDU;
  Ngap_UplinkRANStatusTransfer_t* UplinkRANStatusTransferIEs;
  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  RANStatusTransferTransparentContainer* ranStatusTransfer_TransparentContainer;
};
}  // namespace ngap

#endif
