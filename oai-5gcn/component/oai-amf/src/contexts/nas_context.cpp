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

/*! \file nas_context.cpp
 \brief
 \author  Keliang DU, BUPT
 \date 2020
 \email: contact@openairinterface.org
 */

#include "nas_context.hpp"

//------------------------------------------------------------------------------
nas_context::nas_context() : _vector(), _5g_he_av(), _5g_av(), kamf() {
  security_ctx                                          = nullptr;
  is_imsi_present                                       = false;
  is_stacs_available                                    = false;
  is_auth_vectors_present                               = false;
  auts                                                  = nullptr;
  ctx_avaliability_ind                                  = false;
  amf_ue_ngap_id                                        = 0;
  ran_ue_ngap_id                                        = 0;
  _5gmm_state                                           = {};
  registration_type                                     = 0;
  follow_on_req_pending_ind                             = false;
  ngKsi                                                 = 0;
  mmCapability                                          = 0;
  ueSecurityCapEnc                                      = 0;
  ueSecurityCapInt                                      = 0;
  ueSecurityCapEEA                                      = 0;
  ueSecurityCapEIA                                      = 0;
  requestedNssai                                        = {};
  is_specific_procedure_for_registration_running        = false;
  is_specific_procedure_for_deregistration_running      = false;
  is_specific_procedure_for_eCell_inactivity_running    = false;
  is_common_procedure_for_authentication_running        = false;
  is_common_procedure_for_identification_running        = false;
  is_common_procedure_for_security_mode_control_running = false;
  is_common_procedure_for_nas_transport_running         = false;
  _security                                             = {};
  security_ctx                                          = nullptr;
  is_current_security_available                         = false;
  registration_attempt_counter                          = 0;
  is_imsi_present                                       = false;
  is_5g_guti_present                                    = false;
  is_auth_vectors_present                               = false;
  to_be_register_by_new_suci                            = false;
  ueSecurityCaplen                                      = 0;
}

//------------------------------------------------------------------------------
nas_context::~nas_context() {}
