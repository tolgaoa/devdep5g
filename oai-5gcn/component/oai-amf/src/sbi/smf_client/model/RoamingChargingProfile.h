/**
 * Nsmf_PDUSession
 * SMF PDU Session Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 * NOTE: This class is auto generated by OpenAPI-Generator 4.1.1-SNAPSHOT.
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

/*
 * RoamingChargingProfile.h
 *
 *
 */

#ifndef OAI_SMF_MODEL_RoamingChargingProfile_H_
#define OAI_SMF_MODEL_RoamingChargingProfile_H_

#include "../ModelBase.h"

#include "Trigger.h"
#include "PartialRecordMethod.h"
#include <vector>

namespace oai {
namespace smf {
namespace model {

/// <summary>
///
/// </summary>
class RoamingChargingProfile : public ModelBase {
 public:
  RoamingChargingProfile();
  virtual ~RoamingChargingProfile();

  /////////////////////////////////////////////
  /// ModelBase overrides

  void validate() override;

  web::json::value toJson() const override;
  void fromJson(const web::json::value& json) override;

  void toMultipart(
      std::shared_ptr<MultipartFormData> multipart,
      const utility::string_t& namePrefix) const override;
  void fromMultiPart(
      std::shared_ptr<MultipartFormData> multipart,
      const utility::string_t& namePrefix) override;

  /////////////////////////////////////////////
  /// RoamingChargingProfile members

  /// <summary>
  ///
  /// </summary>
  std::vector<std::shared_ptr<Trigger>>& getTriggers();
  bool triggersIsSet() const;
  void unsetTriggers();

  void setTriggers(const std::vector<std::shared_ptr<Trigger>>& value);

  /// <summary>
  ///
  /// </summary>
  std::shared_ptr<PartialRecordMethod> getPartialRecordMethod() const;
  bool partialRecordMethodIsSet() const;
  void unsetPartialRecordMethod();

  void setPartialRecordMethod(
      const std::shared_ptr<PartialRecordMethod>& value);

 protected:
  std::vector<std::shared_ptr<Trigger>> m_Triggers;
  bool m_TriggersIsSet;
  std::shared_ptr<PartialRecordMethod> m_PartialRecordMethod;
  bool m_PartialRecordMethodIsSet;
};

}  // namespace model
}  // namespace smf
}  // namespace oai

#endif /* OAI_SMF_MODEL_RoamingChargingProfile_H_ */