/**
 * Namf_Communication
 * AMF Communication Service © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "N1N2MessageCollectionDocumentApiImpl.h"
#include "itti.hpp"

#include "amf_app.hpp"
#include "amf_n11.hpp"
#include "pdu_session_context.hpp"
using namespace amf_application;

extern void msg_str_2_msg_hex(std::string msg, bstring& b);
extern void convert_string_2_hex(std::string& input, std::string& output);
extern itti_mw* itti_inst;
extern amf_n11* amf_n11_inst;
extern amf_app* amf_app_inst;
extern void print_buffer(
    const std::string app, const std::string commit, uint8_t* buf, int len);

namespace oai {
namespace amf {
namespace api {

using namespace oai::amf::model;

N1N2MessageCollectionDocumentApiImpl::N1N2MessageCollectionDocumentApiImpl(
    std::shared_ptr<Pistache::Rest::Router> rtr,
    amf_application::amf_app* amf_app_inst)
    : N1N2MessageCollectionDocumentApi(rtr), m_amf_app(amf_app_inst) {}

void N1N2MessageCollectionDocumentApiImpl::n1_n2_message_transfer(
    const std::string& ueContextId,
    const N1N2MessageTransferReqData& n1N2MessageTransferReqData,
    Pistache::Http::ResponseWriter& response) {
  Logger::amf_server().debug("Response OK");
  response.send(
      Pistache::Http::Code::Ok,
      "N1N2MessageCollectionDocumentApiImpl::n1_n2_message_transfer API has "
      "not been implemented yet!");
}

void N1N2MessageCollectionDocumentApiImpl::n1_n2_message_transfer(
    const std::string& ueContextId,
    const N1N2MessageTransferReqData& n1N2MessageTransferReqData,
    std::string& n1sm_str, Pistache::Http::ResponseWriter& response) {
  Logger::amf_server().debug(
      "Receive N1N2MessageTransfer Request, handling...");

  bstring n1sm;
  msg_str_2_msg_hex(n1sm_str, n1sm);
  print_buffer(
      "amf_server", "Received N1 SM", (uint8_t*) bdata(n1sm), blength(n1sm));
  response.send(
      Pistache::Http::Code::Ok,
      "N1N2MessageCollectionDocumentApiImpl::n1_n2_message_transfer API has "
      "not been implemented yet!");
}

void N1N2MessageCollectionDocumentApiImpl::n1_n2_message_transfer(
    const std::string& ueContextId,
    const N1N2MessageTransferReqData& n1N2MessageTransferReqData,
    std::string& n1sm_str, std::string& n2sm_str,
    Pistache::Http::ResponseWriter& response) {
  Logger::amf_server().debug(
      "Receive N1N2MessageTransfer Request, handling...");
  response.send(Pistache::Http::Code::Ok, "OK");

  std::string supi = ueContextId;
  Logger::amf_server().debug(
      "Key for PDU Session context: SUPI (%s)", supi.c_str());
  std::shared_ptr<pdu_session_context> psc;

  if (!amf_app_inst->find_pdu_session_context(
          supi, (uint8_t) n1N2MessageTransferReqData.getPduSessionId(), psc)) {
    Logger::amf_server().error(
        "Cannot get pdu_session_context with SUPI (%s)", supi.c_str());
  }

  bstring n1sm;
  msg_str_2_msg_hex(
      n1sm_str.substr(0, n1sm_str.length()), n1sm);  // TODO: verify n1sm_length

  bstring n2sm;
  msg_str_2_msg_hex(n2sm_str, n2sm);

  psc.get()->n1sm             = n1sm;
  psc.get()->isn1sm_avaliable = true;
  psc.get()->n2sm             = n2sm;
  psc.get()->isn2sm_avaliable = true;
  Logger::amf_server().debug(
      "n2sm size in amf_server(%d)", blength(psc.get()->n2sm));

  itti_n1n2_message_transfer_request* itti_msg =
      new itti_n1n2_message_transfer_request(AMF_SERVER, TASK_AMF_APP);
  itti_msg->supi        = ueContextId;
  itti_msg->n1sm        = n1sm;
  itti_msg->is_n1sm_set = true;
  itti_msg->n2sm        = n2sm;
  itti_msg->is_n2sm_set = true;
  itti_msg->pdu_session_id =
      (uint8_t) n1N2MessageTransferReqData.getPduSessionId();
  std::shared_ptr<itti_n1n2_message_transfer_request> i =
      std::shared_ptr<itti_n1n2_message_transfer_request>(itti_msg);
  int ret = itti_inst->send_msg(i);
  if (0 != ret) {
    Logger::amf_server().error(
        "Could not send ITTI message %s to task TASK_AMF_N2",
        i->get_msg_name());
  }
}

}  // namespace api
}  // namespace amf
}  // namespace oai
