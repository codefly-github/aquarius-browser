/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_HISTORY_SORTS_AD_HISTORY_ASCENDING_SORT_H_
#define BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_HISTORY_SORTS_AD_HISTORY_ASCENDING_SORT_H_

#include "brave/components/brave_ads/core/internal/history/sorts/ad_history_sort_interface.h"
#include "brave/components/brave_ads/core/public/history/ad_history_item_info.h"

namespace brave_ads {

class AdHistoryAscendingSort final : public AdHistorySortInterface {
 public:
  void Apply(AdHistoryList& ad_history) const override;
};

}  // namespace brave_ads

#endif  // BRAVE_COMPONENTS_BRAVE_ADS_CORE_INTERNAL_HISTORY_SORTS_AD_HISTORY_ASCENDING_SORT_H_
