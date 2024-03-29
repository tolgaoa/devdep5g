/**
 * NRF NFManagement Service
 * NRF NFManagement Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */
/*
 * InterfaceUpfInfoItem.h
 *
 *
 */

#ifndef InterfaceUpfInfoItem_H_
#define InterfaceUpfInfoItem_H_

#include <string>
#include "Ipv6Addr.h"
#include <vector>
#include "UPInterfaceType.h"
#include <nlohmann/json.hpp>

namespace oai {
namespace smf_server {
namespace model {

/// <summary>
///
/// </summary>
class InterfaceUpfInfoItem {
 public:
  InterfaceUpfInfoItem();
  virtual ~InterfaceUpfInfoItem();

  void validate();

  /////////////////////////////////////////////
  /// InterfaceUpfInfoItem members

  /// <summary>
  ///
  /// </summary>
  UPInterfaceType getInterfaceType() const;
  void setInterfaceType(UPInterfaceType const& value);
  /// <summary>
  ///
  /// </summary>
  std::vector<std::string>& getIpv4EndpointAddresses();
  void setIpv4EndpointAddresses(std::vector<std::string> const& value);
  bool ipv4EndpointAddressesIsSet() const;
  void unsetIpv4EndpointAddresses();
  /// <summary>
  ///
  /// </summary>
  std::vector<Ipv6Addr>& getIpv6EndpointAddresses();
  void setIpv6EndpointAddresses(std::vector<Ipv6Addr> const& value);
  bool ipv6EndpointAddressesIsSet() const;
  void unsetIpv6EndpointAddresses();
  /// <summary>
  ///
  /// </summary>
  std::string getEndpointFqdn() const;
  void setEndpointFqdn(std::string const& value);
  bool endpointFqdnIsSet() const;
  void unsetEndpointFqdn();
  /// <summary>
  ///
  /// </summary>
  std::string getNetworkInstance() const;
  void setNetworkInstance(std::string const& value);
  bool networkInstanceIsSet() const;
  void unsetNetworkInstance();

  friend void to_json(nlohmann::json& j, const InterfaceUpfInfoItem& o);
  friend void from_json(const nlohmann::json& j, InterfaceUpfInfoItem& o);

 protected:
  UPInterfaceType m_InterfaceType;

  std::vector<std::string> m_Ipv4EndpointAddresses;
  bool m_Ipv4EndpointAddressesIsSet;
  std::vector<Ipv6Addr> m_Ipv6EndpointAddresses;
  bool m_Ipv6EndpointAddressesIsSet;
  std::string m_EndpointFqdn;
  bool m_EndpointFqdnIsSet;
  std::string m_NetworkInstance;
  bool m_NetworkInstanceIsSet;
};

}  // namespace model
}  // namespace smf_server
}  // namespace oai

#endif /* InterfaceUpfInfoItem_H_ */
