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

#ifndef _QOSRULES_H_
#define _QOSRULES_H_

#include <stdint.h>
#include "bstrlib.h"
#include "OCTET_STRING.h"

#define QOS_RULES_MINIMUM_LENGTH 7
#define QOS_RULES_MAXIMUM_LENGTH 65538

#define QOS_RULES_MINIMUM_LENGTH_TLVE 7
#define QOS_RULES_MAXIMUM_LENGTH_TLVE 65538
#define QOS_RULES_MINIMUM_LENGTH_LVE 6
#define QOS_RULES_MAXIMUM_LENGTH_LVE 65538

// Rule operation code (bits 8 to 6 of octet 7)
#define CREATE_NEW_QOS_RULE 0b001
#define DELETE_EXISTING_QOS_RULE 0b010
#define MODIFY_EXISTING_QOS_RULE_AND_ADD_PACKET_FILTERS 0b011
#define MODIFY_EXISTING_QOS_RULE_AND_REPLACE_ALL_PACKET_FILTERS 0b100
#define MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS 0b101
#define MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS 0b110

// Segregation bit (bit 7 of octet m+2)
#define SEGREGATION_NOT_REQUESTED 0
#define SEGREGATION_REQUESTED 1

// DQR bit (bit 5 of octet 7)
#define THE_QOS_RULE_IS_NOT_THE_DEFAULT_QOS_RULE 0
#define THE_QOS_RULE_IS_DEFAULT_QOS_RULE 1

// Packet filter component type identifier
#define QOS_RULE_MATCHALL_TYPE 0b00000001
#define QOS_RULE_IPV4_REMOTE_ADDRESS_TYPE 0b00010000
#define QOS_RULE_IPV4_LOCAL_ADDRESS_TYPE 0b00010001
#define QOS_RULE_IPV6_REMOTE_ADDRESSORPREFIX_LENGTH_TYPE 0b00100001
#define QOS_RULE_IPV6_LOCAL_ADDRESSORPREFIX_LENGTH_TYPE 0b00100011
#define QOS_RULE_PROTOCOL_IDENTIFIERORNEXT_HEADER_TYPE 0b00110000
#define QOS_RULE_SINGLE_LOCAL_PORT_TYPE 0b01000000
#define QOS_RULE_LOCAL_PORT_RANGE_TYPE 0b01000001
#define QOS_RULE_SINGLE_REMOTE_PORT_TYPE 0b01010000
#define QOS_RULE_REMOTE_PORT_RANGE_TYPE 0b01010001
#define QOS_RULE_SECURITY_PARAMETER_INDEX_TYPE 0b01100000
#define QOS_RULE_TYPE_OF_SERVICEORTRAFFIC_CLASS_TYPE 0b01110000
#define QOS_RULE_FLOW_LABEL_TYPE 0b10000000
#define QOS_RULE_DESTINATION_MAC_ADDRESS_TYPE 0b10000001
#define QOS_RULE_SOURCE_MAC_ADDRESS_TYPE 0b10000010
#define QOS_RULE_8021Q_CTAG_VID_TYPE 0b10000011
#define QOS_RULE_8021Q_STAG_VID_TYPE 0b10000100
#define QOS_RULE_8021Q_CTAG_PCPORDEI_TYPE 0b10000101
#define QOS_RULE_8021Q_STAG_PCPORDEI_TYPE 0b10000110
#define QOS_RULE_ETHERTYPE_TYPE 0b10000111

typedef struct {
  uint8_t spare : 4;
  uint8_t packetfilteridentifier : 4;
} __attribute__((__packed__)) ModifyAndDelete;

typedef struct {
  uint8_t component_type;
  bstring component_value;
} PacketFilterContents;

typedef struct {
  uint8_t spare : 2;
  uint8_t packetfilterdirection : 2;
  uint8_t packetfilteridentifier : 4;
  uint8_t lenghtofpacketfiltercontents;
  PacketFilterContents packetfiltercontents;

} __attribute__((__packed__)) Create_ModifyAndAdd_ModifyAndReplace;

typedef struct {
  uint8_t qosruleidentifer;
  uint16_t LengthofQoSrule;
  uint8_t ruleoperationcode : 3;
  uint8_t dqrbit : 1;
  uint8_t numberofpacketfilters : 4;
  union {
    ModifyAndDelete* modifyanddelete;
    Create_ModifyAndAdd_ModifyAndReplace* create_modifyandadd_modifyandreplace;
  } packetfilterlist;
  // uint16_t packetfilterlistnumber;
  uint8_t qosruleprecedence;
  uint8_t spare : 1;
  uint8_t segregation : 1;
  uint8_t qosflowidentifer : 6;
} __attribute__((__packed__)) QOSRulesIE;

typedef struct {
  uint16_t lengthofqosrulesie;
  QOSRulesIE* qosrulesie;
} QOSRules;

int encode_qos_rules(
    QOSRules qosrules, uint8_t iei, uint8_t* buffer, uint32_t len);
int decode_qos_rules(
    QOSRules* qosrules, uint8_t iei, uint8_t* buffer, uint32_t len);
void free_decode_qos_rules(QOSRules* qosrules);

#endif
