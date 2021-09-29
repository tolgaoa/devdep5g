#include "AMFApiServer.hpp"
#include "logger.hpp"

using namespace oai::amf::api;

void AMFApiServer::init(size_t thr) {
  auto opts = Pistache::Http::Endpoint::options().threads(thr);
  opts.flags(Pistache::Tcp::Options::ReuseAddr);
  opts.maxRequestSize(PISTACHE_SERVER_MAX_PAYLOAD);
  m_httpEndpoint->init(opts);
  m_individualSubscriptionDocumentApiImpl->init();
  m_individualUeContextDocumentApiImpl->init();
  m_n1N2IndividualSubscriptionDocumentApiImpl->init();
  m_n1N2MessageCollectionDocumentApiImpl->init();
  m_n1N2SubscriptionsCollectionForIndividualUEContextsDocumentApiImpl->init();
  m_nonUEN2MessageNotificationIndividualSubscriptionDocumentApiImpl->init();
  m_nonUEN2MessagesCollectionDocumentApiImpl->init();
  m_nonUEN2MessagesSubscriptionsCollectionDocumentApiImpl->init();
  m_subscriptionsCollectionDocumentApiImpl->init();
  Logger::amf_server().debug("Initiate AMF server endpoints done!");
}

void AMFApiServer::start() {
  if (m_individualSubscriptionDocumentApiImpl != nullptr)
    Logger::amf_server().debug(
        "AMF handler for IndividualSubscriptionDocumentApiImpl");
  if (m_individualUeContextDocumentApiImpl != nullptr)
    Logger::amf_server().debug(
        "AMF handler for IndividualUeContextDocumentApiImpl");
  if (m_n1N2IndividualSubscriptionDocumentApiImpl != nullptr)
    Logger::amf_server().debug(
        "AMF handler for N1N2IndividualSubscriptionDocumentApiImpl");
  if (m_n1N2MessageCollectionDocumentApiImpl != nullptr)
    Logger::amf_server().debug(
        "AMF handler for N1N2MessageCollectionDocumentApiImpl");
  if (m_n1N2SubscriptionsCollectionForIndividualUEContextsDocumentApiImpl !=
      nullptr)
    Logger::amf_server().debug(
        "AMF handler for "
        "N1N2SubscriptionsCollectionForIndividualUEContextsDocumentApiImpl");
  if (m_nonUEN2MessageNotificationIndividualSubscriptionDocumentApiImpl !=
      nullptr)
    Logger::amf_server().debug(
        "AMF handler for "
        "NonUEN2MessageNotificationIndividualSubscriptionDocumentApiImpl");
  if (m_nonUEN2MessagesCollectionDocumentApiImpl != nullptr)
    Logger::amf_server().debug(
        "AMF handler for NonUEN2MessagesCollectionDocumentApiImpl");
  if (m_nonUEN2MessagesSubscriptionsCollectionDocumentApiImpl != nullptr)
    Logger::amf_server().debug(
        "AMF handler for "
        "NonUEN2MessagesSubscriptionsCollectionDocumentApiImpl");
  if (m_subscriptionsCollectionDocumentApiImpl != nullptr)
    Logger::amf_server().debug(
        "AMF handler for SubscriptionsCollectionDocumentApiImpl");
  m_httpEndpoint->setHandler(m_router->handler());
  m_httpEndpoint->serve();
}
void AMFApiServer::shutdown() {
  m_httpEndpoint->shutdown();
}
