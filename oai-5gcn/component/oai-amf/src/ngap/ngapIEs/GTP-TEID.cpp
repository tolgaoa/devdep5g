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

#include "GTP-TEID.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
GtpTeid::GtpTeid() {
  gtp_teid = 0;
}

//------------------------------------------------------------------------------
GtpTeid::~GtpTeid() {}

//------------------------------------------------------------------------------
void GtpTeid::setGtpTeid(const uint32_t m_gtp_teid) {
  gtp_teid = m_gtp_teid;
}

//------------------------------------------------------------------------------
bool GtpTeid::getGtpTeid(uint32_t& m_gtp_teid) {
  m_gtp_teid = gtp_teid;

  return true;
}

//------------------------------------------------------------------------------
bool GtpTeid::encode2GtpTeid(Ngap_GTP_TEID_t& gtpTeid) {
  gtpTeid.size = sizeof(uint32_t);
  gtpTeid.buf  = (uint8_t*) calloc(1, sizeof(uint32_t));
  if (!gtpTeid.buf) return false;

  for (int i = 0; i < gtpTeid.size; i++) {
    gtpTeid.buf[i] = (gtp_teid >> (gtpTeid.size - i - 1) * 8) & 0xff;
  }

  return true;
}

//------------------------------------------------------------------------------
bool GtpTeid::decodefromGtpTeid(Ngap_GTP_TEID_t& gtpTeid) {
  if (!gtpTeid.buf) return false;

  gtp_teid = 0;
  for (int i = 0; i < gtpTeid.size; i++) {
    gtp_teid = gtp_teid << 8;
    gtp_teid |= gtpTeid.buf[i];
  }

  return true;
}

}  // namespace ngap
