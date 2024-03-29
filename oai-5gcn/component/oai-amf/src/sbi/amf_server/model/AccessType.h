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
/*
 * AccessType.h
 *
 *
 */

#ifndef AccessType_H_
#define AccessType_H_

#include <nlohmann/json.hpp>

namespace oai {
namespace amf {
namespace model {

/// <summary>
///
/// </summary>
class AccessType {
 public:
  AccessType();
  virtual ~AccessType();

  void validate();

  /////////////////////////////////////////////
  /// AccessType members

  friend void to_json(nlohmann::json& j, const AccessType& o);
  friend void from_json(const nlohmann::json& j, AccessType& o);

 protected:
};

}  // namespace model
}  // namespace amf
}  // namespace oai

#endif /* AccessType_H_ */
