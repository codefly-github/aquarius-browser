/* Copyright (c) 2021 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_ads/core/internal/account/statement/ads_summary_util.h"

#include "base/containers/flat_map.h"
#include "brave/components/brave_ads/core/internal/account/transactions/transactions_test_util.h"
#include "brave/components/brave_ads/core/internal/common/test/test_base.h"
#include "brave/components/brave_ads/core/internal/common/test/time_test_util.h"

// npm run test -- brave_unit_tests --filter=BraveAds*

namespace brave_ads {

class BraveAdsAdsSummaryUtilTest : public test::TestBase {};

TEST_F(BraveAdsAdsSummaryUtilTest, GetAdsSummaryForDateRange) {
  // Arrange
  AdvanceClockTo(test::TimeFromString("5 November 2020"));

  TransactionList transactions;

  const TransactionInfo transaction_1 = test::BuildUnreconciledTransaction(
      /*value=*/0.01, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_1);

  AdvanceClockTo(test::TimeFromString("25 December 2020"));

  const TransactionInfo transaction_2 = test::BuildUnreconciledTransaction(
      /*value=*/0.0, AdType::kNotificationAd, ConfirmationType::kClicked,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_2);

  const TransactionInfo transaction_3 = test::BuildUnreconciledTransaction(
      /*value=*/0.03, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_3);

  const base::Time from_time = test::Now();

  AdvanceClockTo(test::TimeFromString("1 January 2021"));

  const TransactionInfo transaction_4 = test::BuildUnreconciledTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_4);

  TransactionInfo transaction_5 = test::BuildUnreconciledTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transaction_5.ad_type = AdType::kNewTabPageAd;
  transactions.push_back(transaction_5);

  TransactionInfo transaction_6 = test::BuildUnreconciledTransaction(
      /*value=*/0.02, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transaction_6.ad_type = AdType::kInlineContentAd;
  transactions.push_back(transaction_6);

  // Act
  const base::flat_map<std::string, int32_t> ads_summary =
      GetAdsSummaryForDateRange(transactions, from_time, test::DistantFuture());

  // Assert
  const base::flat_map<std::string, int32_t> expected_ads_summary = {
      {"ad_notification", 2}, {"new_tab_page_ad", 1}, {"inline_content_ad", 1}};
  EXPECT_EQ(expected_ads_summary, ads_summary);
}

TEST_F(BraveAdsAdsSummaryUtilTest, DoNotGetAdsSummaryForDateRange) {
  // Arrange
  AdvanceClockTo(test::TimeFromString("5 November 2020"));

  TransactionList transactions;

  const TransactionInfo transaction_1 = test::BuildUnreconciledTransaction(
      /*value=*/0.01, AdType::kNotificationAd,
      ConfirmationType::kViewedImpression,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_1);

  const TransactionInfo transaction_2 = test::BuildUnreconciledTransaction(
      /*value=*/0.0, AdType::kNotificationAd, ConfirmationType::kClicked,
      /*should_generate_random_uuids=*/true);
  transactions.push_back(transaction_2);

  AdvanceClockTo(test::TimeFromString("1 January 2021"));

  // Act
  const base::flat_map<std::string, int32_t> ads_summary =
      GetAdsSummaryForDateRange(transactions, /*from_time=*/test::Now(),
                                /*to_time=*/test::DistantFuture());

  // Assert
  EXPECT_THAT(ads_summary, ::testing::IsEmpty());
}

TEST_F(BraveAdsAdsSummaryUtilTest, GetAdsSummaryForNoTransactions) {
  // Act
  const base::flat_map<std::string, int32_t> ads_summary =
      GetAdsSummaryForDateRange(/*transactions=*/{},
                                /*from_time=*/test::DistantPast(),
                                /*to_time=*/test::DistantFuture());

  // Assert
  EXPECT_THAT(ads_summary, ::testing::IsEmpty());
}

}  // namespace brave_ads
