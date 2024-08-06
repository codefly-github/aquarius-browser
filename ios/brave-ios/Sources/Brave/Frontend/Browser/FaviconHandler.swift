// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at https://mozilla.org/MPL/2.0/.

import Favicon
import Foundation
import SDWebImage
import Shared
import Storage
import UIKit

class FaviconHandler {
  static let maximumFaviconSize = 1 * 1024 * 1024  // 1 MiB file size limit

  private var tabObservers: TabObservers!
  private let backgroundQueue = OperationQueue()

  init() {
    self.tabObservers = registerFor(.didLoadPageMetadata, queue: backgroundQueue)
  }

  deinit {
    unregister(tabObservers)
  }

  func loadFaviconURL(
    _ url: URL,
    forTab tab: Tab
  ) async throws -> Favicon {
    let favicon = try await FaviconFetcher.loadIcon(
      url: url,
      kind: .smallIcon,
      persistent: !tab.isPrivate
    )
    tab.favicon = favicon
    return favicon
  }
}

extension FaviconHandler: TabEventHandler {
  func tab(_ tab: Tab, didLoadPageMetadata metadata: PageMetadata) {
    if let currentURL = tab.url {
      Task { @MainActor in
        if let favicon = await FaviconFetcher.getIconFromCache(for: currentURL) {
          tab.favicon = favicon
        } else {
          tab.favicon = Favicon.default
        }
        let favicon = try await loadFaviconURL(currentURL, forTab: tab)
        TabEvent.post(.didLoadFavicon(favicon), for: tab)
      }
    } else {
      tab.favicon = Favicon.default
    }
  }
}
