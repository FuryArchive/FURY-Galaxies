#include "gtest/gtest.h"

#include "server/zone/managers/fury/economy/FurySettlementPlan.h"

TEST(FurySettlementPlanTest, PreservesCore3InclusiveTaxSemantics) {
	FurySettlementInput input;
	input.grossPrice = 1050;
	input.citySalesTaxPercent = 5.0f;

	auto plan = FurySettlementPlanner::plan(input);

	EXPECT_TRUE(plan.eligible);
	EXPECT_EQ(plan.tax, 50);
	EXPECT_EQ(plan.sellerNet, 1000);
}

TEST(FurySettlementPlanTest, ZeroTaxPaysFullGross) {
	FurySettlementInput input;
	input.grossPrice = 2500;
	input.citySalesTaxPercent = 0.0f;

	auto plan = FurySettlementPlanner::plan(input);

	EXPECT_TRUE(plan.eligible);
	EXPECT_EQ(plan.tax, 0);
	EXPECT_EQ(plan.sellerNet, 2500);
}

TEST(FurySettlementPlanTest, RefusesAuctionUntilAuctionSemanticsExist) {
	FurySettlementInput input;
	input.grossPrice = 2500;
	input.auction = true;

	auto plan = FurySettlementPlanner::plan(input);

	EXPECT_FALSE(plan.eligible);
	EXPECT_EQ(plan.rejection, FurySettlementRejection::AuctionUnsupported);
}

TEST(FurySettlementPlanTest, RefusesMissingSellerOrItem) {
	FurySettlementInput missingSeller;
	missingSeller.grossPrice = 1000;
	missingSeller.sellerExists = false;

	EXPECT_EQ(
		FurySettlementPlanner::plan(missingSeller).rejection,
		FurySettlementRejection::MissingSeller);

	FurySettlementInput missingItem;
	missingItem.grossPrice = 1000;
	missingItem.itemExists = false;

	EXPECT_EQ(
		FurySettlementPlanner::plan(missingItem).rejection,
		FurySettlementRejection::MissingItem);
}
