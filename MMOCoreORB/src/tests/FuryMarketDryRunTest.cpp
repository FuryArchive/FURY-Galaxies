#include "gtest/gtest.h"

#include "server/zone/managers/fury/economy/FuryMarketDryRun.h"

TEST(FuryMarketDryRunTest, IgnoresAuctionsAndUsesTypeRelativePrice) {
	std::vector<FuryMarketListing> listings;

	FuryMarketListing cheap;
	cheap.listingId = 10;
	cheap.effectiveItemType = 100;
	cheap.askingPrice = 500;
	listings.push_back(cheap);

	FuryMarketListing expensive = cheap;
	expensive.listingId = 11;
	expensive.askingPrice = 1500;
	listings.push_back(expensive);

	FuryMarketListing auction = cheap;
	auction.listingId = 12;
	auction.auction = true;
	listings.push_back(auction);

	auto result = FuryMarketDryRun::evaluate(listings, 0.75f, 0.60f);

	ASSERT_EQ(result.eligibleListings, 2);
	ASSERT_EQ(result.decisions.size(), 2u);

	EXPECT_EQ(result.decisions[0].referencePrice, 1000);
	EXPECT_EQ(result.decisions[1].referencePrice, 1000);
	EXPECT_EQ(result.decisions[0].comparableListings, 2);
	EXPECT_GT(result.decisions[0].decision.priceScore, result.decisions[1].decision.priceScore);
}

TEST(FuryMarketDryRunTest, DifferentTypesGetIndependentReferencePrices) {
	std::vector<FuryMarketListing> listings;

	FuryMarketListing weapon;
	weapon.listingId = 20;
	weapon.effectiveItemType = 100;
	weapon.askingPrice = 1000;
	listings.push_back(weapon);

	FuryMarketListing food;
	food.listingId = 21;
	food.effectiveItemType = 200;
	food.askingPrice = 100;
	listings.push_back(food);

	auto result = FuryMarketDryRun::evaluate(listings, 0.5f, 0.60f);

	ASSERT_EQ(result.decisions.size(), 2u);
	EXPECT_EQ(result.decisions[0].referencePrice, 1000);
	EXPECT_EQ(result.decisions[1].referencePrice, 100);
}

TEST(FuryMarketDryRunTest, MedianPriceResistsSingleExtremeListing) {
	std::vector<FuryMarketListing> listings;

	for (int i = 0; i < 3; ++i) {
		FuryMarketListing item;
		item.listingId = 100 + i;
		item.effectiveItemType = 300;
		item.askingPrice = (i == 0 ? 900 : (i == 1 ? 1000 : 999999));
		listings.push_back(item);
	}

	auto result = FuryMarketDryRun::evaluate(listings, 0.5f, 0.60f);

	ASSERT_EQ(result.decisions.size(), 3u);
	EXPECT_EQ(result.decisions[0].referencePrice, 1000);
	EXPECT_EQ(result.decisions[1].referencePrice, 1000);
	EXPECT_EQ(result.decisions[2].referencePrice, 1000);
}

TEST(FuryMarketDryRunTest, RealQualitySignalAffectsSameTypeRanking) {
	std::vector<FuryMarketListing> listings;

	FuryMarketListing ordinary;
	ordinary.listingId = 400;
	ordinary.effectiveItemType = 900;
	ordinary.askingPrice = 1000;
	ordinary.qualitySignalKnown = true;
	ordinary.qualitySignal = 100.0f;
	listings.push_back(ordinary);

	FuryMarketListing exceptional = ordinary;
	exceptional.listingId = 401;
	exceptional.qualitySignal = 200.0f;
	listings.push_back(exceptional);

	auto result = FuryMarketDryRun::evaluate(listings, 0.5f, 0.60f);

	ASSERT_EQ(result.decisions.size(), 2u);
	EXPECT_TRUE(result.decisions[0].decision.qualityScore > 0.0f);
	EXPECT_GT(
		result.decisions[1].decision.qualityScore,
		result.decisions[0].decision.qualityScore);
}
