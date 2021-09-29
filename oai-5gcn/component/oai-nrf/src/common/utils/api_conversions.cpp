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

/*! \file api_conversions.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: tien-thinh.nguyen@eurecom.fr
 */

#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
#include <nlohmann/json.hpp>
#include <regex>

#include "3gpp_29.510.h"
#include "AmfInfo.h"
#include "api_conversions.hpp"
#include "logger.hpp"
#include "nrf.h"
#include "string.hpp"

using namespace oai::nrf::model;
using namespace oai::nrf::app;
using namespace oai::nrf;

//------------------------------------------------------------------------------
bool api_conv::profile_api_to_nrf_profile(
    const NFProfile& api_profile, std::shared_ptr<nrf_profile>& profile) {
  Logger::nrf_app().debug(
      "Convert a json-type profile to a NF profile (profile ID: %s)",
      api_profile.getNfInstanceId().c_str());

  profile.get()->set_nf_instance_id(api_profile.getNfInstanceId());
  profile.get()->set_nf_instance_name(api_profile.getNfInstanceName());
  Logger::nrf_app().debug(
      "\tInstance name: %s", profile.get()->get_nf_instance_name().c_str());

  profile.get()->set_nf_status(api_profile.getNfStatus());
  Logger::nrf_app().debug(
      "\tStatus: %s", profile.get()->get_nf_status().c_str());
  profile.get()->set_nf_heartBeat_timer(api_profile.getHeartBeatTimer());
  Logger::nrf_app().debug(
      "\tHeartBeart timer: %d", profile.get()->get_nf_heartBeat_timer());
  profile.get()->set_nf_priority(api_profile.getPriority());
  Logger::nrf_app().debug("\tPriority: %d", profile.get()->get_nf_priority());
  profile.get()->set_nf_capacity(api_profile.getCapacity());
  Logger::nrf_app().debug("\tCapacity: %d", profile.get()->get_nf_capacity());
  // SNSSAIs
  std::vector<Snssai> snssai = api_profile.getSNssais();
  for (auto s : snssai) {
    snssai_t sn = {};
    sn.sD       = s.getSd();
    sn.sST      = s.getSst();
    profile.get()->add_snssai(sn);
    Logger::nrf_app().debug(
        "\tSNSSAI (SD, SST): %d, %s", sn.sST, sn.sD.c_str());
  }

  std::vector<std::string> ipv4_addr_str = api_profile.getIpv4Addresses();
  for (auto address : ipv4_addr_str) {
    struct in_addr addr4 = {};
    unsigned char buf_in_addr[sizeof(struct in_addr)];
    if (inet_pton(AF_INET, util::trim(address).c_str(), buf_in_addr) == 1) {
      memcpy(&addr4, buf_in_addr, sizeof(struct in_addr));
    } else {
      Logger::nrf_app().warn(
          "Address conversion: Bad value %s", util::trim(address).c_str());
    }

    Logger::nrf_app().debug("\tIPv4 Addr: %s", address.c_str());
    profile.get()->add_nf_ipv4_addresses(addr4);
  }

  nf_type_t nf_type = string_to_nf_type(api_profile.getNfType());

  switch (nf_type) {
    case NF_TYPE_AMF: {
      Logger::nrf_app().debug("\tAMF profile, AMF Info");
      profile.get()->set_nf_type(NF_TYPE_AMF);
      amf_info_t info      = {};
      AmfInfo amf_info_api = api_profile.getAmfInfo();
      info.amf_region_id   = amf_info_api.getAmfRegionId();
      info.amf_set_id      = amf_info_api.getAmfSetId();

      Logger::nrf_app().debug(
          "\t\tAMF Set ID: %s, AMF Region ID: %s", info.amf_set_id.c_str(),
          info.amf_region_id.c_str());

      for (auto g : amf_info_api.getGuamiList()) {
        guami_t guami  = {};
        guami.amf_id   = g.getAmfId();
        guami.plmn.mcc = g.getPlmnId().getMcc();
        guami.plmn.mnc = g.getPlmnId().getMnc();
        info.guami_list.push_back(guami);
        Logger::nrf_app().debug(
            "\t\tAMF GUAMI, AMF_ID:  %s", guami.amf_id.c_str());
        Logger::nrf_app().debug(
            "\t\tAMF GUAMI, PLMN (MCC: %s, MNC: %s)", guami.plmn.mcc.c_str(),
            guami.plmn.mnc.c_str());
      }
      (std::static_pointer_cast<amf_profile>(profile))
          .get()
          ->add_amf_info(info);
    } break;
    case NF_TYPE_SMF: {
      Logger::nrf_app().debug("\tSMF profile, SMF Info");
      profile.get()->set_nf_type(NF_TYPE_SMF);
      smf_info_t info      = {};
      SmfInfo smf_info_api = api_profile.getSmfInfo();

      for (auto s : smf_info_api.getSNssaiSmfInfoList()) {
        snssai_smf_info_item_t snssai = {};
        snssai.snssai.sD              = s.getSNssai().getSd();
        snssai.snssai.sST             = s.getSNssai().getSst();
        Logger::nrf_app().debug(
            "\t\tNSSAI SD: %s, SST: %d", snssai.snssai.sD.c_str(),
            snssai.snssai.sST);
        for (auto d : s.getDnnSmfInfoList()) {
          dnn_smf_info_item_t dnn = {};
          dnn.dnn                 = d.getDnn();
          snssai.dnn_smf_info_list.push_back(dnn);
          Logger::nrf_app().debug("\t\tDNN: %s", dnn.dnn.c_str());
        }
        info.snssai_smf_info_list.push_back(snssai);
      }

      (std::static_pointer_cast<smf_profile>(profile))
          .get()
          ->add_smf_info(info);

    } break;
    case NF_TYPE_UPF: {
      Logger::nrf_app().debug("\tUPF profile, UPF Info");
      profile.get()->set_nf_type(NF_TYPE_UPF);
      upf_info_t info      = {};
      UpfInfo upf_info_api = api_profile.getUpfInfo();

      for (auto s : upf_info_api.getSNssaiUpfInfoList()) {
        snssai_upf_info_item_t snssai = {};
        snssai.snssai.sD              = s.getSNssai().getSd();
        snssai.snssai.sST             = s.getSNssai().getSst();
        Logger::nrf_app().debug(
            "\t\tNSSAI SD: %s, SST: %d", snssai.snssai.sD.c_str(),
            snssai.snssai.sST);
        for (auto d : s.getDnnUpfInfoList()) {
          dnn_upf_info_item_t dnn = {};
          dnn.dnn                 = d.getDnn();
          snssai.dnn_upf_info_list.push_back(dnn);
          Logger::nrf_app().debug("\t\tDNN: %s", dnn.dnn.c_str());
        }
        info.snssai_upf_info_list.push_back(snssai);
      }

      (std::static_pointer_cast<upf_profile>(profile))
          .get()
          ->add_upf_info(info);

    } break;
    default: {
    }
  }

  // nf_services
  if (api_profile.nfServicesIsSet()) {
    std::vector<NFService> nf_services = api_profile.getNfServices();
    for (auto service : nf_services) {
      nf_service_t ns        = {};
      ns.service_instance_id = service.getServiceInstanceId();
      ns.service_name        = service.getServiceName();
      ns.scheme              = service.getScheme();
      for (auto v : service.getVersions()) {
        nf_service_version_t version = {};
        version.api_full_version     = v.getApiFullVersion();
        version.api_version_in_uri   = v.getApiVersionInUri();
        ns.versions.push_back(version);
      }
      ns.nf_service_status = service.getNfServiceStatus();
      profile.get()->add_nf_service(ns);
    }
  }
  return true;
}

//------------------------------------------------------------------------------
bool api_conv::subscription_api_to_nrf_subscription(
    const SubscriptionData& api_sub, std::shared_ptr<nrf_subscription>& sub) {
  Logger::nrf_app().debug(
      "Convert a json-type Subscription data a NRF subscription data");
  sub.get()->set_notification_uri(api_sub.getNfStatusNotificationUri());
  subscription_condition_t sub_condition = {};

  /*
  if (api_sub.subscrCondIsSet()) {
    subscription_condition_api_t sub_condition_api = {};
    api_sub.getSubscrCond(sub_condition_api);
    switch (sub_condition_api.type) {
      case NF_INSTANCE_ID_COND: {
        Logger::nrf_app().debug(
            "Subscription condition type: NF_INSTANCE_ID_COND");
        sub_condition.type = NF_INSTANCE_ID_COND;
        sub_condition.nf_instance_id =
            sub_condition_api.nfInstanceIdCond.getNfInstanceId();
      } break;
      case NF_TYPE_COND: {
        Logger::nrf_app().debug("Subscription condition type: NF_TYPE_COND");
        sub_condition.type = NF_TYPE_COND;
        sub_condition.nf_type = sub_condition_api.nfTypeCond.getNfType();
      } break;
      case SERVICE_NAME_COND: {
        Logger::nrf_app().debug(
            "Subscription condition type: SERVICE_NAME_COND");
        sub_condition.type = SERVICE_NAME_COND;
        sub_condition.service_name =
            sub_condition_api.serviceNameCond.getServiceName();
      } break;
      case AMF_COND: {
        Logger::nrf_app().debug("Subscription condition type: AMF_COND");
        sub_condition.type = AMF_COND;
        sub_condition.amf_info.amf_set_id =
            sub_condition_api.amfCond.getAmfSetId();
        sub_condition.amf_info.amf_region_id =
            sub_condition_api.amfCond.getAmfRegionId();
      } break;
      case GUAMI_LIST_COND: {
        Logger::nrf_app().debug("Subscription condition type: GUAMI_LIST_COND");
        sub_condition.type = GUAMI_LIST_COND;
        // TODO:
      } break;
      case NETWOTK_SLICE_COND: {
        Logger::nrf_app().debug(
            "Subscription condition type: NETWOTK_SLICE_COND");
        sub_condition.type = NETWOTK_SLICE_COND;
        // TODO:
      } break;
      case NF_GROUP_COND: {
        Logger::nrf_app().debug("Subscription condition type: NF_GROUP_COND");
        sub_condition.type = NF_GROUP_COND;
        // TODO:
      } break;
      default: {
        Logger::nrf_app().debug("Subscription condition type: Unknown");
        return false;
      }
    }
  }
  */

  if (api_sub.subscrCondIsSet()) {
    nlohmann::json sub_condition_api = {};
    api_sub.getSubscrCond(sub_condition_api);

    if (sub_condition_api.find("NfInstanceIdCond") != sub_condition_api.end()) {
      sub_condition.type = NF_INSTANCE_ID_COND;
      sub_condition.nf_instance_id =
          sub_condition_api["NfInstanceIdCond"]["nfInstanceId"]
              .get<std::string>();
      Logger::nrf_app().debug(
          "Subscription condition type: NfInstanceIdCond, nfInstanceId: %s",
          sub_condition.nf_instance_id.c_str());
    }

    if (sub_condition_api.find("NfTypeCond") != sub_condition_api.end()) {
      sub_condition.type = NF_TYPE_COND;
      sub_condition.nf_type =
          sub_condition_api["NfTypeCond"]["nfType"].get<std::string>();
      Logger::nrf_app().debug(
          "Subscription condition type: NfTypeCond, nf_type: %s",
          sub_condition.nf_type.c_str());
    }

    if (sub_condition_api.find("ServiceNameCond") != sub_condition_api.end()) {
      sub_condition.type = SERVICE_NAME_COND;
      sub_condition.service_name =
          sub_condition_api["ServiceNameCond"]["serviceName"]
              .get<std::string>();
      Logger::nrf_app().debug(
          "Subscription condition type: ServiceNameCond, serviceName: %s",
          sub_condition.service_name.c_str());
    }
    if (sub_condition_api.find("AmfCond") != sub_condition_api.end()) {
      sub_condition.type = AMF_COND;
      sub_condition.amf_info.amf_set_id =
          sub_condition_api["AmfCond"]["amfSetId"].get<std::string>();
      sub_condition.amf_info.amf_region_id =
          sub_condition_api["AmfCond"]["amfRegionId"].get<std::string>();
      Logger::nrf_app().debug(
          "Subscription condition type: AmfCond, amfSetId: %s, amfRegionId: "
          "%s ",
          sub_condition.amf_info.amf_set_id.c_str(),
          sub_condition.amf_info.amf_region_id.c_str());
    }

    if (sub_condition_api.find("GuamiListCond") != sub_condition_api.end()) {
      sub_condition.type = GUAMI_LIST_COND;
      // TODO
    }

    Logger::nrf_app().debug(
        "Subscription condition type: %s",
        subscription_condition_type_e2str[sub_condition.type].c_str());

    if (sub_condition.type != UNKNOWN_CONDITION) {
      sub.get()->set_sub_condition(sub_condition);
    }
  }

  // NotificationEventType
  if (api_sub.reqNotifEventsIsSet()) {
    for (auto n : api_sub.getReqNotifEvents()) {
      if (n.compare("NF_REGISTERED") == 0) {
        sub.get()->add_notif_event(NOTIFICATION_TYPE_NF_REGISTERED);
        Logger::nrf_app().debug("ReqNotifEvents: %s", n.c_str());
      } else if (n.compare("NF_DEREGISTERED") == 0) {
        sub.get()->add_notif_event(NOTIFICATION_TYPE_NF_DEREGISTERED);
        Logger::nrf_app().debug("ReqNotifEvents: %s", n.c_str());
      } else if (n.compare("NF_PROFILE_CHANGED") == 0) {
        sub.get()->add_notif_event(NOTIFICATION_TYPE_NF_PROFILE_CHANGED);
      } else {
        sub.get()->add_notif_event(NOTIFICATION_TYPE_UNKNOWN_EVENT);
      }
    }
  }

  if (api_sub.validityTimeIsSet()) {
    std::string str = api_sub.getValidityTime();
    boost::posix_time::ptime p(boost::posix_time::from_iso_string(str));
    sub.get()->set_validity_time(p);
    Logger::nrf_app().debug("Validity Time: %s", str.c_str());
  }
  // TODO:
  return true;
}

//------------------------------------------------------------------------------
nf_type_t api_conv::string_to_nf_type(const std::string& str) {
  if (str.compare("NRF") == 0) return NF_TYPE_NRF;
  if (str.compare("AMF") == 0) return NF_TYPE_AMF;
  if (str.compare("SMF") == 0) return NF_TYPE_SMF;
  if (str.compare("AUSF") == 0) return NF_TYPE_AUSF;
  if (str.compare("NEF") == 0) return NF_TYPE_NEF;
  if (str.compare("PCP") == 0) return NF_TYPE_PCF;
  if (str.compare("SMSF") == 0) return NF_TYPE_SMSF;
  if (str.compare("NSSF") == 0) return NF_TYPE_NSSF;
  if (str.compare("UDR") == 0) return NF_TYPE_UDR;
  if (str.compare("LMF") == 0) return NF_TYPE_LMF;
  if (str.compare("GMLC") == 0) return NF_TYPE_GMLC;
  if (str.compare("5G_EIR") == 0) return NF_TYPE_5G_EIR;
  if (str.compare("SEPP") == 0) return NF_TYPE_SEPP;
  if (str.compare("UPF") == 0) return NF_TYPE_UPF;
  if (str.compare("N3IWF") == 0) return NF_TYPE_N3IWF;
  if (str.compare("AF") == 0) return NF_TYPE_AF;
  if (str.compare("UDSF") == 0) return NF_TYPE_UDSF;
  if (str.compare("BSF") == 0) return NF_TYPE_BSF;
  if (str.compare("CHF") == 0) return NF_TYPE_CHF;
  if (str.compare("NWDAF") == 0) return NF_TYPE_NWDAF;
  // default
  return NF_TYPE_UNKNOWN;
}

//------------------------------------------------------------------------------
patch_op_type_t api_conv::string_to_patch_operation(const std::string& str) {
  if (str.compare("add") == 0) return PATCH_OP_ADD;
  if (str.compare("copy") == 0) return PATCH_OP_COPY;
  if (str.compare("move") == 0) return PATCH_OP_MOVE;
  if (str.compare("remove") == 0) return PATCH_OP_REMOVE;
  if (str.compare("replace") == 0) return PATCH_OP_REPLACE;
  if (str.compare("test") == 0) return PATCH_OP_TEST;
  // default
  return PATCH_OP_UNKNOWN;
}

bool api_conv::validate_uuid(const std::string& str) {
  // should be verified with Capital letter
  static const std::regex e(
      "[a-f0-9]{8}-[a-f0-9]{4}-[0-5][a-f0-9]{3}-[89aAbB][a-f0-9]{3}-[a-f0-9]{"
      "12}");
  return regex_match(str, e);
}
