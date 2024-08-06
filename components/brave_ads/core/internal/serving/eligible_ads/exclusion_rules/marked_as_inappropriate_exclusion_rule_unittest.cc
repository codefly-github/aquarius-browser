/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/serving/eligible_ads/exclusion_rules/marked_as_inappropriate_exclusion_rule.h"

#include "brave/components/brave_ads/core/internal/ad_units/ad_test_constants.h"
#include "brave/components/brave_ads/core/internal/common/test/test_base.h"
#include "brave/components/brave_ads/core/internal/deprecated/client/client_state_manager.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

class BraveAdsMarkedAsInappropriateExclusionRuleTest : public test::TestBase {
 protected:
  const MarkedAsInappropriateExclusionRule exclusion_rule_;
};

TEST_F(BraveAdsMarkedAsInappropriateExclusionRuleTest, ShouldInclude) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = test::kCreativeInstanceId;
  creative_ad.creative_set_id = test::kCreativeSetId;

  // Act & Assert
  EXPECT_TRUE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

TEST_F(BraveAdsMarkedAsInappropriateExclusionRuleTest, ShouldExclude) {
  // Arrange
  CreativeAdInfo creative_ad;
  creative_ad.creative_instance_id = test::kCreativeInstanceId;
  creative_ad.creative_set_id = test::kCreativeSetId;

  AdHistoryItemInfo ad_history_item;
  ad_history_item.creative_set_id = test::kCreativeSetId;
  ad_history_item.is_marked_as_inappropriate = false;
  ClientStateManager::GetInstance().ToggleMarkAdAsInappropriate(
      ad_history_item);

  // Act & Assert
  EXPECT_FALSE(exclusion_rule_.ShouldInclude(creative_ad).has_value());
}

}  // namespace brave_ads