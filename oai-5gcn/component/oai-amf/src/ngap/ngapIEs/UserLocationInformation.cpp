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

#include "UserLocationInformation.hpp"

#include <iostream>
using namespace std;

namespace ngap {

//------------------------------------------------------------------------------
UserLocationInformation::UserLocationInformation() {
  userLocationInformationEUTRA = NULL;
  userLocationInformationNR    = NULL;
  // userLocationInformationN3IWF = NULL;
}

//------------------------------------------------------------------------------
UserLocationInformation::~UserLocationInformation() {}

//------------------------------------------------------------------------------
void UserLocationInformation::setInformation(
    UserLocationInformationEUTRA* informationEUTRA) {
  informationPresent =
      Ngap_UserLocationInformation_PR_userLocationInformationEUTRA;
  userLocationInformationEUTRA = informationEUTRA;
}

//------------------------------------------------------------------------------
void UserLocationInformation::setInformation(
    UserLocationInformationNR* informationNR) {
  informationPresent =
      Ngap_UserLocationInformation_PR_userLocationInformationNR;
  userLocationInformationNR = informationNR;
}

//------------------------------------------------------------------------------
#if 0
	void UserLocationInformation::setInformation(UserLocationInformationN3IWF* informationN3IWF)
	{
		informationPresent = Ngap_UserLocationInformation_PR_userLocationInformationN3IWF;
		userLocationInformationN3IWF = informationN3IWF;
	}
#endif

//------------------------------------------------------------------------------
bool UserLocationInformation::encodefromUserLocationInformation(
    Ngap_UserLocationInformation_t* userLocationInformation) {
  userLocationInformation->present = informationPresent;
  switch (informationPresent) {
    case Ngap_UserLocationInformation_PR_userLocationInformationEUTRA: {
      Ngap_UserLocationInformationEUTRA* ieEUTRA =
          (Ngap_UserLocationInformationEUTRA*) calloc(
              1, sizeof(Ngap_UserLocationInformationEUTRA));
      userLocationInformationEUTRA->encode2UserLocationInformationEUTRA(
          ieEUTRA);
      userLocationInformation->choice.userLocationInformationEUTRA = ieEUTRA;
      break;
    }
    case Ngap_UserLocationInformation_PR_userLocationInformationNR: {
      Ngap_UserLocationInformationNR* ieNR =
          (Ngap_UserLocationInformationNR*) calloc(
              1, sizeof(Ngap_UserLocationInformationNR));
      userLocationInformationNR->encode2UserLocationInformationNR(ieNR);
      userLocationInformation->choice.userLocationInformationNR = ieNR;
      break;
    }
#if 0
			case Ngap_UserLocationInformation_PR_userLocationInformationN3IWF:{
				Ngap_UserLocationInformationN3IWF *ieN3IWF = (Ngap_UserLocationInformationN3IWF *)calloc(1,sizeof(Ngap_UserLocationInformationN3IWF));
				userLocationInformationN3IWF->encode2UserLocationInformationN3IWF(ieN3IWF);
				userLocationInformation->choice.userLocationInformationN3IWF = ieN3IWF;
			break;}
#endif
    default:
      cout << "[Warning] UserLocationInformation encode error!" << endl;
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
bool UserLocationInformation::decodefromUserLocationInformation(
    Ngap_UserLocationInformation_t* userLocationInformation) {
  informationPresent = userLocationInformation->present;
  switch (informationPresent) {
    case Ngap_UserLocationInformation_PR_userLocationInformationEUTRA: {
      userLocationInformationEUTRA = new UserLocationInformationEUTRA();
      userLocationInformationEUTRA->decodefromUserLocationInformationEUTRA(
          userLocationInformation->choice.userLocationInformationEUTRA);
      break;
    }
    case Ngap_UserLocationInformation_PR_userLocationInformationNR: {
      userLocationInformationNR = new UserLocationInformationNR();
      userLocationInformationNR->decodefromUserLocationInformationNR(
          userLocationInformation->choice.userLocationInformationNR);
      break;
    }
#if 0
			case Ngap_UserLocationInformation_PR_userLocationInformationN3IWF:{
				userLocationInformationN3IWF  = new UserLocationInformationN3IWF();
				userLocationInformationN3IWF->decodefromUserLocationInformationN3IWF(userLocationInformation->choice.userLocationInformationN3IWF);
			break;}
#endif
    default:
      cout << "[Warning] UserLocationInformation decode error!" << endl;
      return false;
  }
  return true;
}

//------------------------------------------------------------------------------
Ngap_UserLocationInformation_PR
UserLocationInformation::getChoiceOfUserLocationInformation() {
  return informationPresent;
}

//------------------------------------------------------------------------------
void UserLocationInformation::getInformation(
    UserLocationInformationEUTRA*& informationEUTRA) {
  informationEUTRA = userLocationInformationEUTRA;
}

//------------------------------------------------------------------------------
void UserLocationInformation::getInformation(
    UserLocationInformationNR*& informationNR) {
  informationNR = userLocationInformationNR;
}
#if 0
	void UserLocationInformation::getInformation(UserLocationInformationN3IWF* &informationN3IWF)
	{
		informationN3IWF = userLocationInformationN3IWF;
	}
#endif
}  // namespace ngap
