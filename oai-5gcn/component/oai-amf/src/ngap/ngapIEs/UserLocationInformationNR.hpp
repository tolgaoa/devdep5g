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

#ifndef _USERLOCATIONINFORMATIONNR_H_
#define _USERLOCATIONINFORMATIONNR_H_

#include "NR-CGI.hpp"
#include "TAI.hpp"
//#include "TimeStamp.hpp"

extern "C" {
#include "Ngap_UserLocationInformationNR.h"
}

namespace ngap {
class UserLocationInformationNR {
 public:
  UserLocationInformationNR();
  virtual ~UserLocationInformationNR();

  void setInformationNR(NR_CGI*, TAI*);
  // void setInformationEUTRA(EUTRA_CGI* m_eUTRA_CGI,TAI* m_tAI,TimeStamp*
  // m_timeStamp);
  bool encode2UserLocationInformationNR(Ngap_UserLocationInformationNR_t*);
  bool decodefromUserLocationInformationNR(Ngap_UserLocationInformationNR_t*);
  // void getInformationEUTRA(EUTRA_CGI* &m_eUTRA_CGI,TAI* &m_tAI,TimeStamp*
  // &m_timeStamp);
  void getInformationNR(NR_CGI*&, TAI*&);
  // bool getTimeStampPresence();

 private:
  NR_CGI* nR_CGI;
  TAI* tAI;
  // bool istimeStampSet;
  // TimeStamp *timeStamp;
};

}  // namespace ngap

#endif
