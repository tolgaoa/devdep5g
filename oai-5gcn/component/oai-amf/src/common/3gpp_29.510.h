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

#ifndef FILE_3GPP_29_510_NRF_SEEN
#define FILE_3GPP_29_510_NRF_SEEN

#include <vector>

#include "3gpp_23.003.h"

enum class nf_status_e { REGISTERED = 0, SUSPENDED = 1, UNDISCOVERABLE = 2 };

static const std::vector<std::string> nf_status_e2str = {
    "REGISTERED", "SUSPENDED", "UNDISCOVERABLE"};

typedef struct amf_info_s {
  std::string amf_set_id;
  std::string amf_region_id;
  std::vector<guami_5g_t> guami_list;
} amf_info_t;

typedef struct dnn_smf_info_item_s {
  std::string dnn;
} dnn_smf_info_item_t;

typedef struct snssai_smf_info_item_s {
  snssai_t snssai;
  std::vector<dnn_smf_info_item_t> dnn_smf_info_list;

} snssai_smf_info_item_t;

typedef struct smf_info_s {
  std::vector<snssai_smf_info_item_t> snssai_smf_info_list;
} smf_info_t;

typedef struct dnn_upf_info_item_s {
  std::string dnn;
  // std::vector<std::string> dnai_list
  // std::vector<std::string> pdu_session_types
} dnn_upf_info_item_t;

typedef struct snssai_upf_info_item_s {
  snssai_t snssai;
  std::vector<dnn_upf_info_item_t> dnn_upf_info_list;
} snssai_upf_info_item_t;

typedef struct upf_info_s {
  std::vector<snssai_upf_info_item_t> snssai_upf_info_list;
} upf_info_t;

enum subscr_condition_type_e {  // TODO: use enum class
  UNKNOWN_CONDITION   = 0,
  NF_INSTANCE_ID_COND = 1,
  NF_TYPE_COND        = 2,
  SERVICE_NAME_COND   = 3,
  AMF_COND            = 4,
  GUAMI_LIST_COND     = 5,
  NETWOTK_SLICE_COND  = 6,
  NF_GROUP_COND       = 7
};

static const std::vector<std::string> subscription_condition_type_e2str = {
    "UNKNOWN_CONDITION",  "NF_INSTANCE_ID_COND",
    "NF_TYPE_COND",       "SERVICE_NAME_COND",
    "AMF_COND",           "GUAMI_LIST_COND",
    "NETWOTK_SLICE_COND", "NF_GROUP_COND"};

typedef struct amf_cond_s {
  std::string amf_set_id;
  std::string amf_region_id;
} amf_cond_t;

typedef struct network_slice_cond_s {
  std::vector<snssai_t> snssai_list;
  std::vector<std::string> nsi_list;
} network_slice_cond_t;

typedef struct nf_group_cond_s {
  std::string nf_type;

  std::string nf_group_id;
} nf_group_cond_t;

typedef struct subscription_condition_s {
  uint8_t type;
  union {
    std::string nf_instance_id;
    std::string nf_type;
    std::string service_name;
    amf_cond_t amf_info;
    std::vector<guami_5g_t> guami_list;
    network_slice_cond_t network_slice;
    nf_group_cond_t nf_group;
  };

  subscription_condition_s() : type(0), nf_instance_id() {}

  subscription_condition_s(uint8_t t) : type(t) {}

  subscription_condition_s(const subscription_condition_s& s)
      : subscription_condition_s() {
    type = s.type;
    switch (s.type) {
      case NF_INSTANCE_ID_COND: {
        nf_instance_id = s.nf_instance_id;
      } break;
      case NF_TYPE_COND: {
        nf_type = s.nf_type;
      } break;

      case SERVICE_NAME_COND: {
        service_name = s.service_name;
      } break;
      case AMF_COND: {
        amf_info.amf_set_id    = s.amf_info.amf_set_id;
        amf_info.amf_region_id = s.amf_info.amf_region_id;
      } break;

      case GUAMI_LIST_COND: {
        // TODO:
      } break;

      case NETWOTK_SLICE_COND: {
        // TODO:
      } break;

      case NF_GROUP_COND: {
        // TODO:
      } break;

      default: {
        // TODO:
      }
    }
    // TODO:
  }
  bool operator==(const struct subscription_condition_s& s) const {
    return (s.type == type);
  }

  bool operator==(const uint8_t& t) const { return (t == type); }

  subscription_condition_s& operator=(const subscription_condition_s& s) {
    type = s.type;
    switch (s.type) {
      case NF_INSTANCE_ID_COND: {
        nf_instance_id = s.nf_instance_id;
      } break;
      case NF_TYPE_COND: {
        nf_type = s.nf_type;
      } break;

      case SERVICE_NAME_COND: {
        service_name = s.service_name;
      } break;
      case AMF_COND: {
        amf_info.amf_set_id    = s.amf_info.amf_set_id;
        amf_info.amf_region_id = s.amf_info.amf_region_id;
      } break;

      case GUAMI_LIST_COND: {
        // TODO:
      } break;

      case NETWOTK_SLICE_COND: {
        // TODO:
      } break;

      case NF_GROUP_COND: {
        // TODO:
      } break;

      default: {
        // TODO:
      }
    }
    // TODO:
    return *this;
  }

  virtual ~subscription_condition_s(){};

  std::string to_string() const {
    std::string s = {};
    s.append("Type: ");
    s.append(subscription_condition_type_e2str[type]);
    s.append(", condition: ");
    switch (type) {
      case NF_INSTANCE_ID_COND: {
        s.append(nf_instance_id);
      } break;
      case NF_TYPE_COND: {
        s.append(nf_type);
      } break;
      case SERVICE_NAME_COND: {
        s.append(service_name);
      } break;
      case AMF_COND: {
        s.append(", AMF_Set_ID: ");
        s.append(amf_info.amf_set_id);
        s.append(", AMF_Region_ID: ");
        s.append(amf_info.amf_region_id);
      } break;

      case GUAMI_LIST_COND: {
        // TODO:
      } break;

      case NETWOTK_SLICE_COND: {
        // TODO:
      } break;

      case NF_GROUP_COND: {
        // TODO:
      } break;

      default: {
        // TODO:
      }
    }
    // TODO:

    return s;
  }

} subscription_condition_t;

enum notification_event_type_t {
  NOTIFICATION_TYPE_UNKNOWN_EVENT      = 0,
  NOTIFICATION_TYPE_NF_REGISTERED      = 1,
  NOTIFICATION_TYPE_NF_DEREGISTERED    = 2,
  NOTIFICATION_TYPE_NF_PROFILE_CHANGED = 3
};

static const std::vector<std::string> notification_event_type_e2str = {
    "UNKNOWN EVENT", "NF_REGISTERED", "NF_DEREGISTERED", "NF_PROFILE_CHANGED"};

typedef struct nf_service_version_s {
  std::string api_version_in_uri;  // apiVersionInUri
  std::string api_full_version;    // apiFullVersion

  nf_service_version_s& operator=(const nf_service_version_s& s) {
    api_version_in_uri = s.api_version_in_uri;
    api_full_version   = s.api_full_version;
    return *this;
  }

  std::string to_string() const {
    std::string s = {};
    s.append(", Version (");
    s.append("apiVersionInUri: ");
    s.append(api_version_in_uri);
    s.append(", apiFullVersion: ");
    s.append(api_full_version);
    s.append(" )");
    return s;
  }
} nf_service_version_t;

typedef struct ip_endpoint_s {
  std::vector<struct in_addr> ipv4_addresses;
  // std::vector<struct in6_addr> ipv6_addresses;
  std::string transport;  // TCP
  unsigned int port;
  std::string to_string() const {
    std::string s = {};
    s.append("Ipv4 Addresses: ");
    for (auto ipv4 : ipv4_addresses) {
      s.append(inet_ntoa(ipv4));
    }
    s.append(", TransportProtocol: ");
    s.append(transport);
    s.append(", Port: ");
    s.append(std::to_string(port));
    return s;
  }

} ip_endpoint_t;

typedef struct nf_service_s {
  std::string service_instance_id;
  std::string service_name;
  std::vector<nf_service_version_t> versions;
  std::string scheme;
  std::string nf_service_status;
  std::vector<ip_endpoint_t> ip_endpoints;

  std::string to_string() const {
    std::string s = {};
    s.append("Service Instance ID: ");
    s.append(service_instance_id);
    s.append(", Service name: ");
    s.append(service_name);
    for (auto v : versions) {
      s.append(v.to_string());
    }
    s.append(", Scheme: ");
    s.append(scheme);
    s.append(", Service status: ");
    s.append(nf_service_status);
    s.append(",  IpEndPoints: ");
    for (auto endpoint : ip_endpoints) {
      s.append(endpoint.to_string());
    }
    return s;
  }
} nf_service_t;

#endif
