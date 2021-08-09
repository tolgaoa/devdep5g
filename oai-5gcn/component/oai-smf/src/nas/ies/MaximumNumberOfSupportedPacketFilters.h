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

#ifndef _MAXIMUMNUMBEROFSUPPORTEDPACKETFILTERS_H_
#define _MAXIMUMNUMBEROFSUPPORTEDPACKETFILTERS_H_

#include <stdint.h>
#include "bstrlib.h"

#define MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_MINIMUM_LENGTH 3
#define MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_MAXIMUM_LENGTH 3

#define MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_MINIMUM_LENGTH_TV 3
#define MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_MAXIMUM_LENGTH_TV 3

typedef uint16_t MaximumNumberOfSupportedPacketFilters;

int encode_maximum_number_of_supported_packet_filters(
    MaximumNumberOfSupportedPacketFilters maximumnumberofsupportedpacketfilters,
    uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_maximum_number_of_supported_packet_filters(
    MaximumNumberOfSupportedPacketFilters*
        maximumnumberofsupportedpacketfilters,
    uint8_t iei, uint8_t* buffer, uint32_t len);

#endif
