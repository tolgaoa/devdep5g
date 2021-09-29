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

#ifndef _USERLOCATIONINFORMATION_H_
#define _USERLOCATIONINFORMATION_H_

#include "UserLocationInformationEUTRA.hpp"
#include "UserLocationInformationN3IWF.hpp"
#include "UserLocationInformationNR.hpp"

extern "C" {
#include "Ngap_UserLocationInformation.h"
}

namespace ngap {
class UserLocationInformation {
 public:
  UserLocationInformation();
  virtual ~UserLocationInformation();

  void setInformation(UserLocationInformationEUTRA*);
  void setInformation(UserLocationInformationNR*);
  // void setInformation(UserLocationInformationN3IWF*);
  bool encodefromUserLocationInformation(
      Ngap_UserLocationInformation_t* userLocationInformation);
  bool decodefromUserLocationInformation(
      Ngap_UserLocationInformation_t* userLocationInformation);
  Ngap_UserLocationInformation_PR getChoiceOfUserLocationInformation();
  void getInformation(UserLocationInformationEUTRA*&);
  void getInformation(UserLocationInformationNR*&);
  // void getInformation(UserLocationInformationN3IWF*&);

 private:
  Ngap_UserLocationInformation_PR informationPresent;
  UserLocationInformationEUTRA* userLocationInformationEUTRA;
  UserLocationInformationNR* userLocationInformationNR;
  // UserLocationInformationN3IWF *userLocationInformationN3IWF;
};

}  // namespace ngap

#endif
