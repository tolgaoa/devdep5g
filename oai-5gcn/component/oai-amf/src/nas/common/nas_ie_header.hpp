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

/*! \file nas_ie_header.hpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "5GMMCapability.hpp"
#include "5GSMobilityIdentity.hpp"
#include "5GSRegistrationType.hpp"
#include "ABBA.hpp"
#include "Additional_5G_Security_Information.hpp"
#include "Additional_Information.hpp"
#include "Allowed_PDU_Session_Status.hpp"
#include "Authentication_Failure_Parameter.hpp"
#include "Authentication_Parameter_AUTN.hpp"
#include "Authentication_Parameter_RAND.hpp"
#include "Authentication_Response_Parameter.hpp"
#include "DNN.hpp"
#include "EAP_Message.hpp"
#include "EPS_Bearer_Context_Status.hpp"
#include "EPS_NAS_Message_Container.hpp"
#include "EPS_NAS_Security_Algorithms.hpp"
#include "Extended_DRX_Parameters.hpp"
#include "GPRS_Timer_2.hpp"
#include "GPRS_Timer_3.hpp"
#include "IMEISV_Request.hpp"
#include "LADN_Indication.hpp"
#include "MA_PDU_Session_Information.hpp"
#include "MICO_Indication.hpp"
#include "NAS_Message_Container.hpp"
#include "NAS_Security_Algorithms.hpp"
#include "NSSAI.hpp"
#include "NSSAI_Inclusion_Mode.hpp"
#include "NasKeySetIdentifier.hpp"
#include "Network_Slicing_Indication.hpp"
#include "Non_3GPP_NW_Provided_Policies.hpp"
#include "PDU_Session_Identity_2.hpp"
#include "PDU_Session_Reactivation_Result.hpp"
#include "PDU_Session_Reactivation_Result_Error_Cause.hpp"
#include "PDU_Session_Status.hpp"
#include "PLMN_List.hpp"
#include "Payload_Container.hpp"
#include "Payload_Container_Type.hpp"
#include "Rejected_NSSAI.hpp"
#include "Release_Assistance_Indication.hpp"
#include "Request_Type.hpp"
#include "S1_UE_Security_Capability.hpp"
#include "SOR_Transparent_Container.hpp"
#include "S_NSSAI.hpp"
#include "ServiceType.hpp"
#include "UENetworkCapability.hpp"
#include "UES_Usage_Setting.hpp"
#include "UESecurityCapability.hpp"
#include "UE_Radio_Capability_ID.hpp"
#include "UE_Status.hpp"
#include "UplinkDataStatus.hpp"
#include "_5GMM_Cause.hpp"
#include "_5GSDeregistrationType.hpp"
#include "_5GSTrackingAreaIdList.hpp"
#include "_5GS_DRX_arameters.hpp"
#include "_5GS_Identity_Type.hpp"
#include "_5GS_Network_Feature_Support.hpp"
#include "_5GS_Registration_Result.hpp"
#include "_5GS_Tracking_Area_Identity.hpp"
#include "_5GS_Update_Type.hpp"
#include "nas_mm_plain_header.hpp"
#include "struct.hpp"
