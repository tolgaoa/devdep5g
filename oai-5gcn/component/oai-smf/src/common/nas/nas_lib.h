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

#ifndef FILE_NAS_LIB_SEEN
#define FILE_NAS_LIB_SEEN
#include "bstrlib.h"

typedef struct pco_protocol_or_container_id_nas_s {
  uint16_t id;
  uint8_t length;
  bstring contents;
} pco_protocol_or_container_id_nas_t;

typedef struct protocol_configuration_options_nas_s {
  uint8_t ext : 1;
  uint8_t spare : 4;
  uint8_t configuration_protocol : 3;
  uint8_t num_protocol_or_container_id;
  // arbitrary value, can be greater than defined (250/3)
  // Setting this value to 30 to support maximum possible number of protocol id
  // or container id defined in 24.008 release 13
#define PCO_UNSPEC_MAXIMUM_PROTOCOL_ID_OR_CONTAINER_ID 30
  pco_protocol_or_container_id_nas_t
      protocol_or_container_ids[PCO_UNSPEC_MAXIMUM_PROTOCOL_ID_OR_CONTAINER_ID];
} protocol_configuration_options_nas_t;
#endif
