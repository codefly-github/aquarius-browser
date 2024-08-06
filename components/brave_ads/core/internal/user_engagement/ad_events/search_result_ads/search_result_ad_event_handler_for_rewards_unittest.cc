/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <cstddef>
#include <string>

#include "base/check.h"
#include "base/test/mock_callback.h"
#include "base/test/scoped_feature_list.h"
#include "brave/components/brave_ads/core/internal/account/deposits/deposits_database_table.h"
#include "brave/components/brave_ads/core/internal/ad_units/ad_test_constants.h"
#include "brave/components/brave_ads/core/internal/ad_units/search_result_ad/search_result_ad_builder.h"
#include "brave/components/brave_ads/core/internal/ad_units/search_result_ad/search_result_ad_info.h"
#include "brave/components/brave_ads/core/internal/common/test/test_base.h"
#include "brave/components/brave_ads/core/internal/creatives/conversions/creative_set_conversion_database_table.h"
#include "brave/components/brave_ads/core/internal/creatives/search_result_ads/creative_search_result_ad_test_util.h"
#include "brave/components/brave_ads/core/internal/serving/permission_rules/permission_rules_test_util.h"
#include "brave/components/brave_ads/core/internal/user_engagement/ad_events/ad_event_test_util.h"
#include "brave/components/brave_ads/core/internal/user_engagement/ad_events/search_result_ads/search_result_ad_event_handler.h"
#include "brave/components/brave_ads/core/internal/user_engagement/ad_events/search_result_ads/search_result_ad_event_handler_delegate_mock.h"
#include "brave/components/brave_ads/core/mojom/brave_ads.mojom-shared.h"
#include "brave/components/brave_ads/core/public/account/confirmations/confirmation_type.h"
#include "brave/components/brave_ads/core/public/ad_units/search_result_ad/search_result_ad_feature.h"
#include "brave/components/brave_ads/core/public/ads_feature.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

namespace {

void VerifyDepositForCreativeInstanceIdExpectation(
    const std::string& creative_instance_id) {
  base::MockCallback<database::table::GetDepositsCallback> callback;
  EXPECT_CALL(callback, Run(/*success=*/::testing::_,
                            /*deposit=*/::testing::Ne(std::nullopt)));
  const database::table::Deposits database_table;
  database_table.GetForCreativeInstanceId(creative_instance_id, callback.Get());
}

void VerifyCreativeSetConversionExpectation(const size_t expected_count) {
  base::MockCallback<database::table::GetCreativeSetConversionsCallback>
      callback;
  EXPECT_CALL(
      callback,
      Run(/*success=*/::testing::_,
          /*creative_set_conversions=*/::testing::SizeIs(expected_count)));
  const database::table::CreativeSetConversions database_table;
  database_table.GetUnexpired(callback.Get());
}

}  // namespace

class BraveAdsSearchResultAdEventHandlerForRewardsTest : public test::TestBase {
 protected:
  void SetUp() override {
    test::TestBase::SetUp();

    scoped_feature_list_.InitAndEnableFeature(
        kShouldAlwaysTriggerBraveSearchResultAdEventsFeature);

    test::ForcePermissionRules();

    event_handler_.SetDelegate(&delegate_mock_);
  }

  void FireEventAndVerifyExpectations(
      const mojom::CreativeSearchResultAdInfoPtr& mojom_creative_ad,
      const mojom::SearchResultAdEventType event_type,
      const bool should_fire_event) {
    CHECK(mojom_creative_ad);

    base::MockCallback<FireSearchResultAdEventHandlerCallback> callback;
    EXPECT_CALL(callback, Run(/*success=*/should_fire_event,
                              mojom_creative_ad->placement_id, event_type));
    event_handler_.FireEvent(mojom_creative_ad.Clone(), event_type,
                             callback.Get());

    size_t expected_count = 0;

    if (should_fire_event &&
        event_type == mojom::SearchResultAdEventType::kViewedImpression) {
      const SearchResultAdInfo ad =
          FromMojomBuildSearchResultAd(mojom_creative_ad);
      expected_count =
          mojom_creative_ad->creative_set_conversion && ad.IsValid() ? 1 : 0;

      VerifyDepositForCreativeInstanceIdExpectation(
          mojom_creative_ad->creative_instance_id);
    }

    VerifyCreativeSetConversionExpectation(expected_count);
  }

  base::test::ScopedFeatureList scoped_feature_list_;

  SearchResultAdEventHandler event_handler_;
  ::testing::StrictMock<SearchResultAdEventHandlerDelegateMock> delegate_mock_;
};

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       FireServedEventWithConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  // Act & Assert
  EXPECT_CALL(delegate_mock_, OnDidFireSearchResultAdServedEvent(ad));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kServedImpression,
      /*should_fire_event=*/true);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       FireServedEventWithoutConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAd(/*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  // Act & Assert
  EXPECT_CALL(delegate_mock_, OnDidFireSearchResultAdServedEvent(ad));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kServedImpression,
      /*should_fire_event=*/true);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       FireViewedEventWithConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvent(ad, ConfirmationType::kServedImpression);

  // Act & Assert
  EXPECT_CALL(delegate_mock_, OnDidFireSearchResultAdViewedEvent(ad));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kViewedImpression,
      /*should_fire_event=*/true);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       FireViewedEventWithoutConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAd(/*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvent(ad, ConfirmationType::kServedImpression);

  // Act & Assert
  EXPECT_CALL(delegate_mock_, OnDidFireSearchResultAdViewedEvent(ad));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kViewedImpression,
      /*should_fire_event=*/true);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       DoNotFireViewedEventIfAdPlacementWasAlreadyViewed) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvents(ad, {ConfirmationType::kServedImpression,
                            ConfirmationType::kViewedImpression});

  // Act & Assert
  EXPECT_CALL(delegate_mock_,
              OnFailedToFireSearchResultAdEvent(
                  ad, mojom::SearchResultAdEventType::kViewedImpression));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kViewedImpression,
      /*should_fire_event=*/false);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       DoNotFireViewedEventIfAdPlacementWasNotServed) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  // Act & Assert
  EXPECT_CALL(delegate_mock_,
              OnFailedToFireSearchResultAdEvent(
                  ad, mojom::SearchResultAdEventType::kViewedImpression));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kViewedImpression,
      /*should_fire_event=*/false);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       FireClickedEventWithConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvents(ad, {ConfirmationType::kServedImpression,
                            ConfirmationType::kViewedImpression});

  // Act & Assert
  EXPECT_CALL(delegate_mock_, OnDidFireSearchResultAdClickedEvent(ad));
  FireEventAndVerifyExpectations(mojom_creative_ad,
                                 mojom::SearchResultAdEventType::kClicked,
                                 /*should_fire_event=*/true);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       FireClickedEventWithoutConversion) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAd(/*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvents(ad, {ConfirmationType::kServedImpression,
                            ConfirmationType::kViewedImpression});

  // Act & Assert
  EXPECT_CALL(delegate_mock_, OnDidFireSearchResultAdClickedEvent(ad));
  FireEventAndVerifyExpectations(mojom_creative_ad,
                                 mojom::SearchResultAdEventType::kClicked,
                                 /*should_fire_event=*/true);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       DoNotFireClickedEventIfAdPlacementWasAlreadyClicked) {
  // Arrange
  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvents(
      ad, {ConfirmationType::kServedImpression,
           ConfirmationType::kViewedImpression, ConfirmationType::kClicked});

  // Act & Assert
  EXPECT_CALL(delegate_mock_,
              OnFailedToFireSearchResultAdEvent(
                  ad, mojom::SearchResultAdEventType::kClicked));
  FireEventAndVerifyExpectations(mojom_creative_ad,
                                 mojom::SearchResultAdEventType::kClicked,
                                 /*should_fire_event=*/false);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       DoNotFireEventIfMissingAdPlacement) {
  // Arrange
  mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  mojom_creative_ad->placement_id = test::kMissingPlacementId;
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  // Act & Assert
  EXPECT_CALL(delegate_mock_,
              OnFailedToFireSearchResultAdEvent(
                  ad, mojom::SearchResultAdEventType::kViewedImpression));

  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kViewedImpression,
      /*should_fire_event=*/false);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       DoNotFireEventWithInvalidPlacementId) {
  // Arrange
  mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  mojom_creative_ad->placement_id = test::kInvalidPlacementId;
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  // Act & Assert
  EXPECT_CALL(delegate_mock_,
              OnFailedToFireSearchResultAdEvent(
                  ad, mojom::SearchResultAdEventType::kServedImpression));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kServedImpression,
      /*should_fire_event=*/false);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       DoNotFireEventWithInvalidCreativeInstanceId) {
  // Arrange
  mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  mojom_creative_ad->creative_instance_id = test::kInvalidCreativeInstanceId;
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  // Act & Assert
  EXPECT_CALL(delegate_mock_,
              OnFailedToFireSearchResultAdEvent(
                  ad, mojom::SearchResultAdEventType::kServedImpression));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kServedImpression,
      /*should_fire_event=*/false);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       FireEventIfAdsPerHourCapNotReached) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kSearchResultAdFeature, {{"maximum_ads_per_hour", "3"}});

  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvents(ad, ConfirmationType::kServedImpression,
                       kMaximumSearchResultAdsPerHour.Get() - 1);

  AdvanceClockBy(base::Hours(1) - base::Milliseconds(1));

  // Act & Assert
  EXPECT_CALL(delegate_mock_, OnDidFireSearchResultAdServedEvent(ad));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kServedImpression,
      /*should_fire_event=*/true);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       DoNotFireEventIfAdsPerHourCapReached) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kSearchResultAdFeature, {{"maximum_ads_per_hour", "3"}});

  mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvents(ad, ConfirmationType::kServedImpression,
                       kMaximumSearchResultAdsPerHour.Get());

  AdvanceClockBy(base::Hours(1) - base::Milliseconds(1));

  // Act & Assert
  EXPECT_CALL(delegate_mock_,
              OnFailedToFireSearchResultAdEvent(
                  ad, mojom::SearchResultAdEventType::kServedImpression));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kServedImpression,
      /*should_fire_event=*/false);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       FireEventIfAdsPerDayCapNotReached) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kSearchResultAdFeature, {{"maximum_ads_per_day", "3"}});

  const mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvents(ad, ConfirmationType::kServedImpression,
                       kMaximumSearchResultAdsPerDay.Get() - 1);

  AdvanceClockBy(base::Days(1) - base::Milliseconds(1));

  // Act & Assert
  EXPECT_CALL(delegate_mock_, OnDidFireSearchResultAdServedEvent(ad));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kServedImpression,
      /*should_fire_event=*/true);
}

TEST_F(BraveAdsSearchResultAdEventHandlerForRewardsTest,
       DoNotFireEventIfAdsPerDayCapReached) {
  // Arrange
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitAndEnableFeatureWithParameters(
      kSearchResultAdFeature, {{"maximum_ads_per_day", "3"}});

  mojom::CreativeSearchResultAdInfoPtr mojom_creative_ad =
      test::BuildCreativeSearchResultAdWithConversion(
          /*should_generate_random_uuids=*/true);
  const SearchResultAdInfo ad = FromMojomBuildSearchResultAd(mojom_creative_ad);

  test::RecordAdEvents(ad, ConfirmationType::kServedImpression,
                       kMaximumSearchResultAdsPerDay.Get());

  AdvanceClockBy(base::Days(1) - base::Milliseconds(1));

  // Act & Assert
  EXPECT_CALL(delegate_mock_,
              OnFailedToFireSearchResultAdEvent(
                  ad, mojom::SearchResultAdEventType::kServedImpression));
  FireEventAndVerifyExpectations(
      mojom_creative_ad, mojom::SearchResultAdEventType::kServedImpression,
      /*should_fire_event=*/false);
}

}  // namespace brave_ads