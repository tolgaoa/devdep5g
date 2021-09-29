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

#ifndef _DOWNLINKRANSTATUSTRANSFER_H_
#define _DOWNLINKRANSTATUSTRANSFER_H_
#include "AMF-UE-NGAP-ID.hpp"
#include "MessageType.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "RANStatusTransferTransparentContainer.hpp"
#include "dRBStatusDL18.hpp"
extern "C" {
#include "Ngap_DRB-ID.h"
#include "Ngap_InitiatingMessage.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
#include "Ngap_RANStatusTransfer-TransparentContainer.h"
#include "Ngap_UplinkRANStatusTransfer.h"
}
namespace ngap {
class DownlinkRANStatusTransfer {
 public:
  DownlinkRANStatusTransfer();
  virtual ~DownlinkRANStatusTransfer();
  void setmessagetype();
  void setAmfUeNgapId(unsigned long id);  // 40 bits
  void setRanUeNgapId(uint32_t id);       // 32 bits
  void setRANStatusTransfer_TransparentContainer(
      long drb_id, long ul_pcdp, long ul_hfn_pdcp, long dl_pcdp,
      long dl_hfn_pdcp);
  int encodetobuffer(uint8_t* buf, int buf_size);

 private:
  Ngap_NGAP_PDU_t* DownlinkranstatustransferPDU;
  Ngap_DownlinkRANStatusTransfer_t* DownlinkranstatustransferIEs;
  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  RANStatusTransferTransparentContainer* ranStatusTransfer_TransparentContainer;
};
}  // namespace ngap
#endif
