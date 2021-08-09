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

/*! \file networkDef.h
 \brief Contains network's global definitions
 \author  BUPT
 \company
 \date 2019
 \email:
 */

#ifndef __NETWORK_DEF_H__
#define __NETWORK_DEF_H__

/*
 * --------------------------------------
 * Network connection establishment cause
 * --------------------------------------
 */

#define NGAP_RRC_ESTABLISHMENT_CAUSE_EMERGENCY 0
#define NGAP_RRC_ESTABLISHMENT_CAUSE_HIGH_PRIORITY_ACCESS 1
#define NGAP_RRC_ESTABLISHMENT_CAUSE_MT_ACCESS 2
#define NGAP_RRC_ESTABLISHMENT_CAUSE_MO_SIGNALLING 3
#define NGAP_RRC_ESTABLISHMENT_CAUSE_MO_DATA 4
#define NGAP_RRC_ESTABLISHMENT_CAUSE_MO_VOICECALL 5
#define NGAP_RRC_ESTABLISHMENT_CAUSE_MO_VIDEOCALL 6
#define NGAP_RRC_ESTABLISHMENT_CAUSE_MO_SMS 7
#define NGAP_RRC_ESTABLISHMENT_CAUSE_MPS_PRIORITY_ACCESS 8
#define NGAP_RRC_ESTABLISHMENT_CAUSE_MCS_PRIORITY_ACCESS 9

#endif
