/* Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

import { localeStrings as onboardingStrings } from '../../shared/components/onboarding/stories/locale_strings'

export const localeStrings = {
  ...onboardingStrings,

  appErrorTitle: 'Something went wrong',
  claim: 'Claim',
  connectWalletHeader: 'Choose where you\'d like to receive BAT',
  connectWalletCustodialHeader: 'Custodial accounts (requires ID verification)',
  connectWalletCustodialTooltip: 'Custodial accounts are accounts with trusted third-party services that hold your BAT for you.',
  connectWalletText: 'To begin earning Basic Attention Token (BAT) rewards, connect an account. BAT you earn will be automatically transferred to your account every month.',
  connectWalletLearnMore: 'Learn more about regions and support.',
  connectWalletProviderNotAvailable: 'Currently not available in your region',
  connectWalletSelfCustodyHeader: 'Self-custody',
  connectWalletSelfCustodyTooltip: 'Self-custody gives you exclusive control over your BAT, along with complete responsibility for its security.',
  connectWalletSelfCustodyNote: 'By choosing a self-custody wallet, Rewards will be paid out as SPL-BAT on the Solana network.',
  connectWalletSolanaLabel: 'Brave Wallet',
  connectWalletSolanaMessage: 'Or other wallet that supports Solana',
  connectWalletSelfCustodyTerms: '$1Terms of Use$2 and $3Privacy Policy$4.',
  connectWalletSelfCustodyError: 'Something went wrong. Please try again',
  connectWalletLoginText: 'Log in or sign up',
  rewardsLearnMore: 'Learn more',
  newBadgeText: 'NEW',

  braveRewards: 'Brave Rewards',
  pendingContributions: 'Pending Contributions',
  donationAbility: 'Show Tip buttons on sites',
  donationTitle: 'Contributions',
  donationDesc: 'See the on-demand contributions you\'ve given to websites and creators.',
  donationDescLearnMore: 'Learn more about contributions.',
  donationVisitSome: 'Have you contributed to your favorite content creator today?',
  donationTotalDonations: 'Total on-demand contributions this month',
  monthlyTipsEmpty: 'No monthly contributions set up yet.',
  monthlyTipsTitle: 'Monthly Contributions',
  monthlyTipsDesc: 'Set up recurring monthly contributions so you can continually support your favorite creators.',
  relaunch: 'Relaunch',
  on: 'on',
  site: 'Site',
  date: 'Date',
  tokens: 'Tokens',
  showAll: 'Show all',
  contributionVisitSome: 'Sites will appear as you browse',
  tosAndPp: 'By turning on $1, you agree to the $2Terms of Service$3 and $4Privacy Policy$5.',
  contributionTitle: 'Auto-Contribute',
  contributionDesc: 'An automatic way to support sites and content creators. Set a monthly contribution amount and browse normally. $1Verified sites and creators$2 you visit will receive contributions from you each month based on the amount of time you spend on their content.',
  contributionDisabledText1: 'Reward creators for the content you love.',
  contributionDisabledText2: 'Your monthly payment gets distributed across the sites you visit.',
  contributionMonthly: 'Monthly payment',
  contributionUpTo: 'Up to',
  contributionNextDate: 'Next contribution date',
  contributionSites: 'Sites viewed',
  nextContribution: 'Next contribution',
  rewardsContribute: 'Auto-Contribute',
  rewardsContributeAttention: 'Attention',
  contributionMinTime: 'Minimum page time before logging a visit',
  contributionTime5: '5 seconds',
  contributionTime8: '8 seconds',
  contributionTime60: '1 minute',
  contributionMinVisits: 'Minimum visits for publisher relevancy',
  contributionVisit1: '1 visit',
  contributionVisit5: '5 visits',
  contributionVisit10: '10 visits',

  adsSubdivisionTargetingDisabled: 'Disabled',
  adsSubdivisionTargetingDisable: 'Disable',
  adsSubdivisionTargetingAutoDetectedAs: 'Auto-detected as $1',
  adsSubdivisionTargetingAutoDetect: 'Auto-detect',
  adsSubdivisionTargetingTitle: 'Regional ad relevance',
  adsSubdivisionTargetingDescription: 'This allows Brave to show you ads meant specifically for your region.',
  adsSubdivisionTargetingLearn: 'Learn More',
  adsDescription: 'Control what kinds of Brave Ads you see, and how often.',
  adsDescriptionEarn: 'The more you see, the more you can earn.',
  adsPerHour: 'Notification ad frequency',
  adsPerHour0: 'None',
  adsPerHour1: 'Max 1 per hour',
  adsPerHour2: 'Max 2 per hour',
  adsPerHour3: 'Max 3 per hour',
  adsPerHour4: 'Max 4 per hour',
  adsPerHour5: 'Max 5 per hour',
  adsPerHour10: 'Max 10 per hour',
  adsTitle: 'Manage Brave Ads',
  adsCurrentEarnings: 'Earnings so far this month',
  adsPaymentDate: 'Next earnings payout date',
  adsTotalReceivedLabel: 'Total ads received this month',
  openAdsHistory: '30-day Ads History',
  settings: 'Settings',
  rewardsBrowserCannotReceiveAds: 'Oops! Your browser cannot receive Brave Private Ads.',
  rewardsBrowserNeedsUpdateToSeeAds: 'Your browser needs to be updated to continue seeing ads.',
  adsNotSupportedRegion: 'Sorry! Ads are not yet available in your region.',
  connectAccountText: '$1Ready to start earning?$2 Connect or create an account with one of our partners.',
  learnMore: 'Learn more',
  newTabAdCountLabel: 'New tab page ads',
  notificationAdCountLabel: 'Notification ads',
  searchAdCountLabel: 'Search ads',
  searchAdInfo: 'Brave Search Ads appear on $1Brave Search$2.',
  searchAdConnectedInfo: 'You cannot earn BAT from Brave Search Ads yet, but you help support Brave by having them on.',
  newsAdCountLabel: 'Brave News ads',
  newsAdInfo: 'Brave News contains ads that cannot be independently disabled.',
  newsAdInfoDisabled: 'Brave News is currently disabled.',

  resetWallet: 'Reset Brave Rewards',
  rewardsResetText: 'By resetting, your current Brave Rewards profile will be deleted, and Brave Rewards will no longer be enabled. If you enable Brave Rewards again later, you will start with a new profile. Read our $1support article about resetting$2 for more details.',
  rewardsResetConsent: 'I understand that my current Brave Rewards profile and Brave Rewards data will be deleted from my device, and I\'ve read the $1support article about resetting$2.',
  reset: 'Reset',

  rewardsTosUpdateHeading: 'Updated Terms of Service',
  rewardsTosUpdateText: 'We’ve updated the Terms of Service for Brave Rewards. We’ve made these updates to clarify our terms and ensure they cover new features. If you continue to use Brave Rewards, you are agreeing to the updated Terms of Service. If you do not agree, you can $1reset$2 Brave Rewards, which will disable the feature.',
  rewardsTosUpdateLinkText: 'See $1Brave Rewards Terms of Service$2',
  rewardsTosUpdateButtonLabel: 'I agree',

  rewardsVBATNoticeTitle1: 'Action required: Connect a custodial account or your vBAT will be lost',
  rewardsVBATNoticeText1: 'On $1, we will be discontinuing support for existing virtual BAT balances. Connect a custodial account before this date so we can transfer your earned balance to your custodial account, otherwise your balance will be lost.',
  rewardsConnectAccount: 'Connect account',

  rewardsPaymentCompleted: 'The payout for $1 rewards has completed.',
  rewardsPaymentPending: 'The payout for $1 rewards will begin processing by $2',
  rewardsPaymentProcessing: 'The payout for $1 rewards is in progress.',
  rewardsPaymentSupport: 'Support'
}