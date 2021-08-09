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

#ifndef _MAPPEDEPSBEARERCONTEXTS_H_
#define _MAPPEDEPSBEARERCONTEXTS_H_

#include <stdint.h>
#include "bstrlib.h"

#define MAPPED_EPS_BEARER_CONTEXTS_MINIMUM_LENGTH 7
#define MAPPED_EPS_BEARER_CONTEXTS_MAXIMUM_LENGTH 65538

#if 0
//MappedExtendedEPSQoSParameters
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_200KBPS 0b00000001
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1MBPS 0b00000010
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4MBPS 0b00000011
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16MBPS 0b00000100
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64MBPS 0b00000101
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256MBPS 0b00000110
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1GBPS 0b00000111
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4GBPS 0b00001000
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16GBPS 0b00001001
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64GBPS 0b00001010
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256GBPS 0b00001011
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1TBPS 0b00001100
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4TBPS 0b00001101
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16TBPS 0b00001110
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64TBPS 0b00001111
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256TBPS 0b00010000
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_1PBPS 0b00010001
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_4PBPS 0b00010010
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_16PBPS 0b00010011
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_64PBPS 0b00010100
#define MAPPED_EPS_VALUE_IS_INCREMENTED_IN_MULTIPLES_OF_256PBPS 0b00010101


typedef struct{
	uint8_t lengthofepsqualityofservicecontents;
	
	uint8_t qci;
	uint8_t maximumbitrateforuplink;
	uint8_t maximumbitratefordownlink;
	uint8_t guaranteedbitrateforuplink;
	uint8_t guaranteedbitratefordownlink;
	uint8_t maximumbitrateforuplinkextended;
	uint8_t maximumbitratefordownlinkextended;
	uint8_t guaranteedbitrateforuplinkextended;
	uint8_t guaranteedbitratefordownlinkextended;
	uint8_t maximumbitrateforuplinkextended2;
	uint8_t maximumbitratefordownlinkextended2;
	uint8_t guaranteedbitrateforuplinkextended2;
	uint8_t guaranteedbitratefordownlinkextended2;
}MappedEPSQosParameters;

typedef struct{
	uint8_t lengthofextendedqualityofservicecontents;
	
	uint8_t Unitformaximumbitrate;
	uint8_t Maximumbitrateforuplink;
	uint8_t Maximumbitrateforuplinkcontinued;
	uint8_t Maximumbitratefordownlink;
	uint8_t Maximumbitratefordownlinkcontinued;
	uint8_t Unitforguaranteedbitrate;
	uint8_t Guaranteedbitrateforuplink;
	uint8_t Guaranteedbitrateforuplinkcontinued;
	uint8_t Guaranteedbitratefordownlink;
	uint8_t Guaranteedbitratefordownlinkcontinued;
}MappedExtendedEPSQoSParameters;

//*********Traffic Flow Template start************


//Packet filter component type identifier
#define TFT_IPV4_REMOTE_ADDRESS_TYPE 0b00010000
#define TFT_IPV4_LOCAL_ADDRESS_TYPE 0b00010001
#define TFT_IPV6_REMOTE_ADDRESS_TYPE 0b00100000
#define TFT_IPV6_REMOTE_ADDRESSORPREFIX_LENGTH_TYPE 0b00100001
#define TFT_IPV6_LOCAL_ADDRESSORPREFIX_LENGTH_TYPE 0b00100011
#define TFT_PROTOCOL_IDENTIFIERORNEXT_HEADER_TYPE 0b00110000
#define TFT_SINGLE_LOCAL_PORT_TYPE 0b01000000
#define TFT_LOCAL_PORT_RANGE_TYPE 0b01000001
#define TFT_SINGLE_REMOTE_PORT_TYPE 0b01010000
#define TFT_REMOTE_PORT_RANGE_TYPE 0b01010001
#define TFT_SECURITY_PARAMETER_INDEX_TYPE 0b01100000
#define TFT_TYPE_OF_SERVICEORTRAFFIC_CLASS_TYPE 0b01110000
#define TFT_FLOW_LABEL_TYPE 0b10000000
#define TFT_DESTINATION_MAC_ADDRESS_TYPE 0b10000001
#define TFT_SOURCE_MAC_ADDRESS_TYPE 0b10000010
#define TFT_8021Q_CTAG_VID_TYPE 0b10000011
#define TFT_8021Q_STAG_VID_TYPE 0b10000100
#define TFT_8021Q_CTAG_PCPORDEI_TYPE 0b10000101
#define TFT_8021Q_STAG_PCPORDEI_TYPE 0b10000110
#define TFT_ETHERTYPE_TYPE 0b10000111

typedef struct{
	uint8_t packetfilteridentifier:4;
}DeletePacketFilters;

typedef struct{
	uint8_t component_type;
	bstring component_value;
}TFTPacketFilterContents;
typedef struct{
	uint8_t packetfilterdirection:2;
	uint8_t packetfilteridentifier:4;
	uint8_t packetfilterevaluationprecedence;
	//uint8_t lengthofpacketfiltercontents;
	TFTPacketFilterContents tftpacketfiltercontents;
}Create_Add_Replace;

typedef struct{
	uint8_t parameteridentifier;
	//uint8_t lengthofparametercontents;
	//δ���
	Parameter Contents 
}TFTParametersList;

typedef struct{
	uint8_t lengthoftrafficflowtemplate;
	
	uint8_t tftoperationcode:3;
	uint8_t e:1;
	uint8_t numberofpacketfilters:4;
	union{
		DeletePacketFilters *deletepacketfilters;
		Create_Add_Replace *create_add_replace;
	}packetfilterlist;
	TFTParametersList tftparameterslist;
}EPSTrafficFlowTemplate;
//*********Traffic Flow Template end**************

typedef struct{
	uint8_t epsparameteridentifier;
	//uint8_t lengthofepsparametercontents;
	union{
		MappedEPSQosParameters mappedepsqosparameters;
		MappedExtendedEPSQoSParameters mappedextendedepsqosparameters;
		EPSTrafficFlowTemplate epstrafficflowtemplate; 
	}epsparametercontents;
}EPSParametersList;

typedef struct{
	uint8_t epsbeareridentity;
	//uint16_t lengthofmappedepsbearercontext;
	uint8_t operationcode:2;
	uint8_t e:1;
	uint8_t numberofepsparameters:4;
	EPSParametersList *epsparameterslist;
}MappedEPSBearerContext;


typedef struct{
	uint16_t mappedepsbearercontextnumber;
	MappedEPSBearerContext *mappedepsbearercontext;
}MappedEPSBearerContexts;
#endif

typedef bstring MappedEPSBearerContexts;

int encode_mapped_eps_bearer_contexts(
    MappedEPSBearerContexts mappedepsbearercontexts, uint8_t iei,
    uint8_t* buffer, uint32_t len);
int decode_mapped_eps_bearer_contexts(
    MappedEPSBearerContexts* mappedepsbearercontexts, uint8_t iei,
    uint8_t* buffer, uint32_t len);

#endif
