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
 \author  niuxiansheng-niu, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */
#include "PDUSessionResourceReleaseCommandTransfer.hpp"

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
PDUSessionResourceReleaseCommandTransfer::
    PDUSessionResourceReleaseCommandTransfer() {
  pduSessionResourceReleaseCommandTransferIEs =
      (Ngap_PDUSessionResourceReleaseCommandTransfer_t*) calloc(
          1, sizeof(Ngap_PDUSessionResourceReleaseCommandTransfer_t));
  causeValue = NULL;
}
PDUSessionResourceReleaseCommandTransfer::
    ~PDUSessionResourceReleaseCommandTransfer() {
  // if(pduSessionResourceReleaseCommandTransferIEs)
  // ASN_STRUCT_FREE(asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer,pduSessionResourceReleaseCommandTransferIEs);
  if (causeValue) delete causeValue;
  // if (pduSessionResourceReleaseCommandTransferIEs)
  // free(pduSessionResourceReleaseCommandTransferIEs);
}

void PDUSessionResourceReleaseCommandTransfer::setCauseRadioNetwork(
    e_Ngap_CauseRadioNetwork cause_value) {
  if (!causeValue) causeValue = new Cause();

  causeValue->setChoiceOfCause(Ngap_Cause_PR_radioNetwork);
  causeValue->setValue(cause_value);

  int ret = causeValue->encode2Cause(
      &pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    cout << "encode CauseRadioNetwork IE error" << endl;
    return;
  }
}
void PDUSessionResourceReleaseCommandTransfer::setCauseTransport(
    e_Ngap_CauseTransport cause_value) {
  if (!causeValue) causeValue = new Cause();

  causeValue->setChoiceOfCause(Ngap_Cause_PR_transport);
  causeValue->setValue(cause_value);

  int ret = causeValue->encode2Cause(
      &pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    cout << "encode CauseTransport IE error" << endl;
    return;
  }
}
void PDUSessionResourceReleaseCommandTransfer::setCauseNas(
    e_Ngap_CauseNas cause_value) {
  if (!causeValue) causeValue = new Cause();

  causeValue->setChoiceOfCause(Ngap_Cause_PR_nas);
  causeValue->setValue(cause_value);

  int ret = causeValue->encode2Cause(
      &pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    cout << "encode CauseNas IE error" << endl;
    return;
  }
}
void PDUSessionResourceReleaseCommandTransfer::setCauseProtocol(
    e_Ngap_CauseProtocol cause_value) {
  if (!causeValue) causeValue = new Cause();

  causeValue->setChoiceOfCause(Ngap_Cause_PR_protocol);
  causeValue->setValue(cause_value);

  int ret = causeValue->encode2Cause(
      &pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    cout << "encode CauseProtocol IE error" << endl;
    return;
  }
}
void PDUSessionResourceReleaseCommandTransfer::setCauseMisc(
    e_Ngap_CauseMisc cause_value) {
  if (!causeValue) causeValue = new Cause();

  causeValue->setChoiceOfCause(Ngap_Cause_PR_misc);
  causeValue->setValue(cause_value);

  int ret = causeValue->encode2Cause(
      &pduSessionResourceReleaseCommandTransferIEs->cause);
  if (!ret) {
    cout << "encode CauseMisc IE error" << endl;
    return;
  }
}

int PDUSessionResourceReleaseCommandTransfer::encode2buffer(
    uint8_t* buf, int buf_size) {
  asn_fprint(
      stderr, &asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer,
      pduSessionResourceReleaseCommandTransferIEs);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer, NULL,
      pduSessionResourceReleaseCommandTransferIEs, buf, buf_size);
  cout << "er.encoded(" << er.encoded << ")" << endl;
  return er.encoded;
}
// Decapsulation
bool PDUSessionResourceReleaseCommandTransfer::decodefromIE(
    uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER,
      &asn_DEF_Ngap_PDUSessionResourceReleaseCommandTransfer,
      (void**) &pduSessionResourceReleaseCommandTransferIEs, buf, buf_size);

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
  // asn_fprint(stderr,
  // &asn_DEF_Ngap_PDUSessionResourceSetupUnsuccessfulTransfer,
  // pduSessionResourceSetupUnsuccessfulTransferIEs);

  causeValue = new Cause();
  if (!causeValue->decodefromCause(
          &pduSessionResourceReleaseCommandTransferIEs->cause)) {
    cout << "decoded ngap Cause IE error" << endl;
    return false;
  }

  return true;
}

long PDUSessionResourceReleaseCommandTransfer::getChoiceOfCause() {
  if (!causeValue) return -1;

  return causeValue->getChoiceOfCause();
}
long PDUSessionResourceReleaseCommandTransfer::getCause() {
  if (!causeValue) return -1;

  return causeValue->getValue();
}
}  // namespace ngap
