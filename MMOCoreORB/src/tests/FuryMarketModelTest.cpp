#include "gtest/gtest.h"

#include "server/zone/managers/fury/economy/FuryMarketModel.h"

TEST(FuryMarketModelTest, HighDemandFairPriceCanPurchase) {
	FuryMarketDecisionInput input;
	input.demand = 0.9f;
	input.quality = 0.7f;
	input.qualityKnown = true;
	input.askingPrice = 1000;
	input.referencePrice = 1000;
	input.listingId = 42;

	auto result = FuryMarketModel::evaluate(input, 0.60f);

	EXPECT_GT(result.score, 0.60f);
	EXPECT_TRUE(result.purchase);
}

TEST(FuryMarketModelTest, LowDemandSevereOverpriceIsRejected) {
	FuryMarketDecisionInput input;
	input.demand = 0.1f;
	input.quality = 0.5f;
	input.qualityKnown = true;
	input.qualityKnown = true;
	input.askingPrice = 5000;
	input.referencePrice = 1000;
	input.listingId = 42;

	auto result = FuryMarketModel::evaluate(input, 0.60f);

	EXPECT_LT(result.score, 0.40f);
	EXPECT_FALSE(result.purchase);
}

TEST(FuryMarketModelTest, DecisionIsStableForSameListing) {
	FuryMarketDecisionInput input;
	input.demand = 0.6f;
	input.quality = 0.5f;
	input.askingPrice = 1250;
	input.referencePrice = 1000;
	input.listingId = 0xDEADBEEF;

	auto first = FuryMarketModel::evaluate(input);
	auto second = FuryMarketModel::evaluate(input);

	EXPECT_FLOAT_EQ(first.deterministicRoll, second.deterministicRoll);
	EXPECT_FLOAT_EQ(first.score, second.score);
	EXPECT_EQ(first.purchase, second.purchase);
}

TEST(FuryMarketModelTest, InputsAreClamped) {
	FuryMarketDecisionInput input;
	input.demand = 5.0f;
	input.quality = -2.0f;
	input.qualityKnown = true;
	input.askingPrice = 0;
	input.referencePrice = 1000;
	input.listingId = 1;

	auto result = FuryMarketModel::evaluate(input);

	EXPECT_FLOAT_EQ(result.demandScore, 1.0f);
	EXPECT_FLOAT_EQ(result.qualityScore, 0.0f);
	EXPECT_FLOAT_EQ(result.priceScore, 1.0f);
}

TEST(FuryMarketModelTest, UnknownQualityDoesNotPretendToBeAverage) {
	FuryMarketDecisionInput input;
	input.demand = 0.8f;
	input.quality = 1.0f;
	input.qualityKnown = false;
	input.askingPrice = 1000;
	input.referencePrice = 1000;
	input.listingId = 77;

	auto unknown = FuryMarketModel::evaluate(input, 0.60f);

	input.qualityKnown = true;
	auto knownExcellent = FuryMarketModel::evaluate(input, 0.60f);

	EXPECT_FLOAT_EQ(unknown.qualityScore, 0.0f);
	EXPECT_NE(unknown.score, knownExcellent.score);
}
