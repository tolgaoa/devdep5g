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

#include "UserLocationInformationNR.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformationNR::UserLocationInformationNR() {
  nR_CGI = NULL;
  tAI    = NULL;
  /*istimeStampSet = false;
   timeStamp = NULL;*/
}

//------------------------------------------------------------------------------
UserLocationInformationNR::~UserLocationInformationNR() {}

//------------------------------------------------------------------------------
void UserLocationInformationNR::setInformationNR(NR_CGI* m_nR_CGI, TAI* m_tAI) {
  nR_CGI = m_nR_CGI;
  tAI    = m_tAI;
}

//------------------------------------------------------------------------------
/*void UserLocationInformationEUTRA::setInformationEUTRA(EUTRA_CGI*
 m_eUTRA_CGI,TAI* m_tAI,TimeStamp* m_timeStamp)
 {
 eUTRA_CGI = m_eUTRA_CGI;
 tAI = m_tAI;
 istimeStampSet = true;
 timeStamp = m_timeStamp;
 }*/

//------------------------------------------------------------------------------
bool UserLocationInformationNR::encode2UserLocationInformationNR(
    Ngap_UserLocationInformationNR_t* userLocationInformationNR) {
  if (!nR_CGI->encode2NR_CGI(&userLocationInformationNR->nR_CGI)) {
    cout << "[Warning] nR_CGI->encode2NR_CGI() error!" << endl;
    return false;
  }
  if (!tAI->encode2TAI(&userLocationInformationNR->tAI)) {
    cout << "[Warning] tAI->encode2TAI() error!" << endl;
    return false;
  }
#if 0
		if(istimeStampSet)
		{
			Ngap_TimeStamp_t *ieTimeStamp = (Ngap_TimeStamp_t *)calloc(1,sizeof(Ngap_TimeStamp_t));
			if(!timeStamp->encodefromTimeStamp(ieTimeStamp))
			{
				cout<<"[Warning] timeStamp->encodefromTimeStamp() error!"<<endl;
				free(ieTimeStamp);
				return false;
			}
			userLocationInformationEUTRA->timeStamp = ieTimeStamp;
		}
#endif
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformationNR::decodefromUserLocationInformationNR(
    Ngap_UserLocationInformationNR_t* userLocationInformationNR) {
  if (nR_CGI == nullptr) nR_CGI = new NR_CGI();
  if (!nR_CGI->decodefromNR_CGI(&userLocationInformationNR->nR_CGI)) {
    cout << "[Warning] nR_CGI->decodefromNR_CGI() error!" << endl;
    return false;
  }
  tAI = new TAI();
  if (!tAI->decodefromTAI(&userLocationInformationNR->tAI)) {
    cout << "[Warning] tAI->decodefromTAI() error!" << endl;
    return false;
  }
#if 0
		if(userLocationInformationEUTRA->timeStamp)
		{
			istimeStampSet = true;
			timeStamp = new TimeStamp();
			if(!timeStamp->decodefromTimeStamp(userLocationInformationEUTRA->timeStamp))
			{
				cout<<"[Warning] timeStamp->decodefromTimeStamp() error!"<<endl;
				return false;
			}
		}
#endif
  return true;
}

//------------------------------------------------------------------------------
/*bool UserLocationInformationEUTRA::getTimeStampPresence()
 {
 return istimeStampSet;
 }*/
/*void UserLocationInformationEUTRA::getInformationEUTRA(EUTRA_CGI*
 &m_eUTRA_CGI,TAI* &m_tAI,TimeStamp* &m_timeStamp)
 {
 m_eUTRA_CGI = eUTRA_CGI;
 m_tAI = tAI;
 m_timeStamp = timeStamp;
 }*/

//------------------------------------------------------------------------------
void UserLocationInformationNR::getInformationNR(
    NR_CGI*& m_nR_CGI, TAI*& m_tAI) {
  m_nR_CGI = nR_CGI;
  m_tAI    = tAI;
}
}  // namespace ngap
