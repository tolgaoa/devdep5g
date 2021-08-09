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

#ifndef _QOSFLOWDESCRIPTIONS_H_
#define _QOSFLOWDESCRIPTIONS_H_

#include <stdint.h>
#include "bstrlib.h"

#define QOS_FLOW_DESCRIPTIONS_MINIMUM_LENGTH 6
#define QOS_FLOW_DESCRIPTIONS_MAXIMUM_LENGTH 65538

#define QOS_FLOW_DESCRIPTIONS_MINIMUM_LENGTH_TLVE 6
#define QOS_FLOW_DESCRIPTIONS_MAXIMUM_LENGTH_TLVE 65538

// Operation code (bits 8 to 6 of octet 5)
#define CREATE_NEW_QOS_FLOW_DESCRIPTION 0b001
#define DELETE_EXISTING_QOS_FLOW_DESCRIPTION 0b010
#define MODIFY_EXISTING_QOS_FLOW_DESCRIPTION 0b011

// E bit (bit 7 of octet 6)
// CREATE_NEW_QOS_FLOW_DESCRIPTION
#define PARAMETERS_LIST_IS_INCLUDED 0x01
// DELETE_EXISTING_QOS_FLOW_DESCRIPTION
#define PARAMETERS_LIST_IS_NOT_INCLUDED 0x00
// MODIFY_EXISTING_QOS_FLOW_DESCRIPTION
#define EXTENSION_OF_PREVIOUSLY_PROVIDED_PARAMETERS 0x00
#define REPLACEMENT_OF_ALL_PREVIOUSLY_PROVIDED_PARAMETERS 0x01

// Parameter identifier
#define PARAMETER_IDENTIFIER_5QI 0x01
#define PARAMETER_IDENTIFIER_GFBR_UPLINK 0x02
#define PARAMETER_IDENTIFIER_GFBR_DOWNLINK 0x03
#define PARAMETER_IDENTIFIER_MFBR_UPLINK 0x04
#define PARAMETER_IDENTIFIER_MFBR_DOWNLINK 0x05
#define PARAMETER_IDENTIFIER_AVERAGING_WINDOW 0x06
#define PARAMETER_IDENTIFIER_EPS_BEARER_IDENTITY 0x07

// Parameter contents
// PARAMETER_IDENTIFIER_5QI

// PARAMETER_IDENTIFIER_GFBR_UPLINK/GFBR_DOWNLINK/MFBR_UPLINK/MFBR_DOWNLINK UINT
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1KBPS 0b00000001
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4KBPS 0b00000010
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16KBPS 0b00000011
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64KBPS 0b00000100
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256KBPS 0b00000101
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS 0b00000110
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4MBPS 0b00000111
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16MBPS 0b00001000
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64MBPS 0b00001001
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256MBPS 0b00001010
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1GBPS 0b00001011
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4GBPS 0b00001100
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16GBPS 0b00001101
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64GBPS 0b00001110
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256GBPS 0b00001111
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1TBPS 0b00010000
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4TBPS 0b00010001
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16TBPS 0b00010010
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64TBPS 0b00010011
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256TBPS 0b00010100
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1PBPS 0b00010101
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4PBPS 0b00010110
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16PBPS 0b00010111
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64PBPS 0b00011000
#define GFBRORMFBR_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256PBPS 0b00011001

// PARAMETER_IDENTIFIER_EPS_BEARER_IDENTITY
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE1 0b0001
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE2 0b0010
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE3 0b0011
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE4 0b0100
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE5 0b0101
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE6 0b0110
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE7 0b0111
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE8 0b1000
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE9 0b1001
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE10 0b1010
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE11 0b1011
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE12 0b1100
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE13 0b1101
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE14 0b1110
#define QOS_FLOW_EPS_BEARER_IDENTITY_VALUE15 0b1111

typedef struct {
  uint8_t uint;
  uint16_t value;
} GFBROrMFBR_UpLinkOrDownLink;

typedef struct {
  uint8_t uplinkinmilliseconds;
  uint8_t downlinkinmilliseconds;
} AveragingWindow;

typedef struct {
  uint8_t parameteridentifier;
  // uint8_t lengthofparametercontents;
  union {
    uint8_t _5qi;
    GFBROrMFBR_UpLinkOrDownLink gfbrormfbr_uplinkordownlink;
    AveragingWindow averagingwindow;
    uint8_t epsbeareridentity : 4;
  } parametercontents;
} ParametersList;

typedef struct {
  uint8_t qfi : 6;
  uint8_t operationcode : 3;
  uint8_t e : 1;
  uint8_t numberofparameters : 6;
  ParametersList* parameterslist;
} QOSFlowDescriptionsContents;

typedef struct {
  uint16_t
      qosflowdescriptionsnumber;  // Custom variables are protocol independent
  QOSFlowDescriptionsContents* qosflowdescriptionscontents;
} QOSFlowDescriptions;

int encode_qos_flow_descriptions(
    QOSFlowDescriptions qosflowdescriptions, uint8_t iei, uint8_t* buffer,
    uint32_t len);
int decode_qos_flow_descriptions(
    QOSFlowDescriptions* qosflowdescriptions, uint8_t iei, uint8_t* buffer,
    uint32_t len);
void free_decode_qos_flow_descriptions(
    QOSFlowDescriptions* qosflowdescriptions);

#endif
