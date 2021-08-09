#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#ifdef __linux__
#include <vector>
#include <signal.h>
#include <unistd.h>
#endif

#include "IndividualSubscriptionDocumentApiImpl.h"
#include "IndividualUeContextDocumentApiImpl.h"
#include "N1N2IndividualSubscriptionDocumentApiImpl.h"
#include "N1N2MessageCollectionDocumentApiImpl.h"
#include "N1N2SubscriptionsCollectionForIndividualUEContextsDocumentApiImpl.h"
#include "NonUEN2MessageNotificationIndividualSubscriptionDocumentApiImpl.h"
#include "NonUEN2MessagesCollectionDocumentApiImpl.h"
#include "NonUEN2MessagesSubscriptionsCollectionDocumentApiImpl.h"
#include "SubscriptionsCollectionDocumentApiImpl.h"

#define PISTACHE_SERVER_THREADS 2
#define PISTACHE_SERVER_MAX_PAYLOAD 32768

#include "amf_app.hpp"

using namespace oai::amf::api;

class AMFApiServer {
 public:
  AMFApiServer(
      Pistache::Address address, amf_application::amf_app* amf_app_inst)
      : m_httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(address)) {
    m_router = std::make_shared<Pistache::Rest::Router>();
    m_individualSubscriptionDocumentApiImpl =
        std::make_shared<IndividualSubscriptionDocumentApiImpl>(
            m_router, amf_app_inst);
    m_individualUeContextDocumentApiImpl =
        std::make_shared<IndividualUeContextDocumentApiImpl>(
            m_router, amf_app_inst);
    m_n1N2IndividualSubscriptionDocumentApiImpl =
        std::make_shared<N1N2IndividualSubscriptionDocumentApiImpl>(
            m_router, amf_app_inst);
    m_n1N2MessageCollectionDocumentApiImpl =
        std::make_shared<N1N2MessageCollectionDocumentApiImpl>(
            m_router, amf_app_inst);
    m_n1N2SubscriptionsCollectionForIndividualUEContextsDocumentApiImpl =
        std::make_shared<
            N1N2SubscriptionsCollectionForIndividualUEContextsDocumentApiImpl>(
            m_router, amf_app_inst);
    m_nonUEN2MessageNotificationIndividualSubscriptionDocumentApiImpl =
        std::make_shared<
            NonUEN2MessageNotificationIndividualSubscriptionDocumentApiImpl>(
            m_router, amf_app_inst);
    m_nonUEN2MessagesCollectionDocumentApiImpl =
        std::make_shared<NonUEN2MessagesCollectionDocumentApiImpl>(
            m_router, amf_app_inst);
    m_nonUEN2MessagesSubscriptionsCollectionDocumentApiImpl =
        std::make_shared<NonUEN2MessagesSubscriptionsCollectionDocumentApiImpl>(
            m_router, amf_app_inst);
    m_subscriptionsCollectionDocumentApiImpl =
        std::make_shared<SubscriptionsCollectionDocumentApiImpl>(
            m_router, amf_app_inst);
  }

  void init(size_t thr = 1);
  void start();
  void shutdown();

 private:
  std::shared_ptr<Pistache::Http::Endpoint> m_httpEndpoint;
  std::shared_ptr<Pistache::Rest::Router> m_router;
  std::shared_ptr<IndividualSubscriptionDocumentApiImpl>
      m_individualSubscriptionDocumentApiImpl;
  std::shared_ptr<IndividualUeContextDocumentApiImpl>
      m_individualUeContextDocumentApiImpl;
  std::shared_ptr<N1N2IndividualSubscriptionDocumentApiImpl>
      m_n1N2IndividualSubscriptionDocumentApiImpl;
  std::shared_ptr<N1N2MessageCollectionDocumentApiImpl>
      m_n1N2MessageCollectionDocumentApiImpl;
  std::shared_ptr<
      N1N2SubscriptionsCollectionForIndividualUEContextsDocumentApiImpl>
      m_n1N2SubscriptionsCollectionForIndividualUEContextsDocumentApiImpl;
  std::shared_ptr<
      NonUEN2MessageNotificationIndividualSubscriptionDocumentApiImpl>
      m_nonUEN2MessageNotificationIndividualSubscriptionDocumentApiImpl;
  std::shared_ptr<NonUEN2MessagesCollectionDocumentApiImpl>
      m_nonUEN2MessagesCollectionDocumentApiImpl;
  std::shared_ptr<NonUEN2MessagesSubscriptionsCollectionDocumentApiImpl>
      m_nonUEN2MessagesSubscriptionsCollectionDocumentApiImpl;
  std::shared_ptr<SubscriptionsCollectionDocumentApiImpl>
      m_subscriptionsCollectionDocumentApiImpl;
};
