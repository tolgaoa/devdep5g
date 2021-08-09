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

#include "Cause.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
Cause::Cause() {
  causeValue = -1;
}

//------------------------------------------------------------------------------
Cause::~Cause() {}

//------------------------------------------------------------------------------
void Cause::setChoiceOfCause(Ngap_Cause_PR m_causePresent) {
  causePresent = m_causePresent;
}

//------------------------------------------------------------------------------
void Cause::setValue(long m_causeValue) {
  causeValue = m_causeValue;
}

//------------------------------------------------------------------------------
bool Cause::encode2Cause(Ngap_Cause_t* cause) {
  cause->present = causePresent;
  switch (causePresent) {
    case Ngap_Cause_PR_radioNetwork: {
      cause->choice.radioNetwork = causeValue;
      break;
    }
    case Ngap_Cause_PR_transport: {
      cause->choice.transport = causeValue;
      break;
    }
    case Ngap_Cause_PR_nas: {
      cause->choice.nas = causeValue;
      break;
    }
    case Ngap_Cause_PR_protocol: {
      cause->choice.protocol = causeValue;
      break;
    }
    case Ngap_Cause_PR_misc: {
      cause->choice.misc = causeValue;
      break;
    }
    default: {
      cout << "[Warning] Cause Present error!" << endl;
      return false;
      break;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool Cause::decodefromCause(Ngap_Cause_t* pdu) {
  causePresent = pdu->present;
  switch (causePresent) {
    case Ngap_Cause_PR_radioNetwork: {
      causeValue = pdu->choice.radioNetwork;
    } break;
    case Ngap_Cause_PR_transport: {
      causeValue = pdu->choice.transport;
    } break;
    case Ngap_Cause_PR_nas: {
      causeValue = pdu->choice.nas;
    } break;
    case Ngap_Cause_PR_protocol: {
      causeValue = pdu->choice.protocol;
    } break;
    case Ngap_Cause_PR_misc: {
      causeValue = pdu->choice.misc;
    } break;
    default: {
      cout << "[Warning] Cause Present error!" << endl;
      return false;
    }
  }
  return true;
}

//------------------------------------------------------------------------------
Ngap_Cause_PR Cause::getChoiceOfCause() {
  return causePresent;
}

//------------------------------------------------------------------------------
long Cause::getValue() {
  return causeValue;
}
}  // namespace ngap
