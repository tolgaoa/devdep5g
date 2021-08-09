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

/*! \file HandoverNotifyMsg.hpp
 \brief
 \author  niuxiansheng-niu, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */
#ifndef _HANDOVERNOTIFYMSG_H_
#define _HANDOVERNOTIFYMSG_H_

#include "AMF-UE-NGAP-ID.hpp"
#include "MessageType.hpp"
#include "NgapIEsStruct.hpp"
#include "RAN-UE-NGAP-ID.hpp"
#include "UserLocationInformation.hpp"

extern "C" {
#include "Ngap_HandoverNotify.h"
#include "Ngap_NGAP-PDU.h"
#include "Ngap_ProtocolIE-Field.h"
}

namespace ngap {
class HandoverNotifyMsg {
 public:
  HandoverNotifyMsg();
  virtual ~HandoverNotifyMsg();
  int encode2buffer(uint8_t* buf, int buf_size);
  bool decodefrompdu(Ngap_NGAP_PDU_t* ngap_msg_pdu);
  void setUserLocationInfoNR(struct NrCgi_s cig, struct Tai_s tai);
  unsigned long getAmfUeNgapId();  // return -1;
  uint32_t getRanUeNgapId();       // return -1;
  bool getUserLocationInfoNR(struct NrCgi_s& cig, struct Tai_s& tai);

 private:
  Ngap_NGAP_PDU_t* handoverNotifyPdu;
  Ngap_HandoverNotify_t* handoverNotifyIEs;
  AMF_UE_NGAP_ID* amfUeNgapId;
  RAN_UE_NGAP_ID* ranUeNgapId;
  UserLocationInformation* userLocationInformation;
};

}  // namespace ngap
#endif
