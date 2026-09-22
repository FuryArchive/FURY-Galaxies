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

TEST(FuryMarketDryRunTest, DifferentTemplatesOfSameClientTypeDoNotPollutePrice) {
	std::vector<FuryMarketListing> listings;

	FuryMarketListing cheapPistol;
	cheapPistol.listingId = 500;
	cheapPistol.effectiveItemType = 100;
	cheapPistol.comparisonKey = 1111;
	cheapPistol.askingPrice = 1000;
	listings.push_back(cheapPistol);

	FuryMarketListing premiumPistol = cheapPistol;
	premiumPistol.listingId = 501;
	premiumPistol.comparisonKey = 2222;
	premiumPistol.askingPrice = 10000;
	listings.push_back(premiumPistol);

	auto result = FuryMarketDryRun::evaluate(listings, 0.5f, 0.60f);

	ASSERT_EQ(result.decisions.size(), 2u);
	EXPECT_EQ(result.decisions[0].referencePrice, 1000);
	EXPECT_EQ(result.decisions[1].referencePrice, 10000);
}

TEST(FuryMarketDryRunTest, FactoryCratePriceIsComparedPerContainedItem) {
	std::vector<FuryMarketListing> listings;

	FuryMarketListing single;
	single.listingId = 600;
	single.comparisonKey = 7777;
	single.askingPrice = 1000;
	listings.push_back(single);

	FuryMarketListing crate = single;
	crate.listingId = 601;
	crate.factoryCrate = true;
	crate.units = 25;
	crate.askingPrice = 25000;
	listings.push_back(crate);

	auto result = FuryMarketDryRun::evaluate(listings, 0.5f, 0.60f);

	ASSERT_EQ(result.decisions.size(), 2u);
	EXPECT_EQ(result.decisions[0].comparisonPrice, 1000);
	EXPECT_EQ(result.decisions[1].comparisonPrice, 1000);
	EXPECT_EQ(result.decisions[0].referencePrice, 1000);
	EXPECT_EQ(result.decisions[1].referencePrice, 1000);
}

TEST(FuryMarketDryRunTest, FactoryCrateNormalizationUsesCeilingDivision) {
	FuryMarketListing crate;
	crate.listingId = 610;
	crate.comparisonKey = 8888;
	crate.factoryCrate = true;
	crate.units = 3;
	crate.askingPrice = 1000;

	auto result = FuryMarketDryRun::evaluate({crate}, 0.5f, 0.60f);

	ASSERT_EQ(result.decisions.size(), 1u);
	EXPECT_EQ(result.decisions[0].comparisonPrice, 334);
	EXPECT_EQ(result.decisions[0].referencePrice, 334);
}

TEST(FuryMarketDryRunTest, ListingSpecificDemandOverridesDefaultDemand) {
	std::vector<FuryMarketListing> listings;

	FuryMarketListing lowDemand;
	lowDemand.listingId = 700;
	lowDemand.comparisonKey = 9999;
	lowDemand.askingPrice = 1000;
	lowDemand.demandKnown = true;
	lowDemand.demand = 0.1f;
	listings.push_back(lowDemand);

	FuryMarketListing highDemand = lowDemand;
	highDemand.listingId = 701;
	highDemand.demand = 0.9f;
	listings.push_back(highDemand);

	auto result = FuryMarketDryRun::evaluate(listings, 0.5f, 0.60f);

	ASSERT_EQ(result.decisions.size(), 2u);
	EXPECT_NEAR(result.decisions[0].decision.demandScore, 0.1f, 0.0001f);
	EXPECT_NEAR(result.decisions[1].decision.demandScore, 0.9f, 0.0001f);
	EXPECT_GT(result.decisions[1].decision.score, result.decisions[0].decision.score);
}
