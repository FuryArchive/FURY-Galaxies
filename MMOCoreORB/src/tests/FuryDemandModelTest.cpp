#include "gtest/gtest.h"

#include "server/zone/managers/fury/economy/FuryDemandModel.h"

TEST(FuryDemandModelTest, PurchaseSaturatesDemand) {
	FuryDemandState state;
	state.current = 0.8f;
	state.purchaseImpact = 0.1f;

	auto next = FuryDemandModel::applyPurchase(state, 3);

	EXPECT_NEAR(next.current, 0.5f, 0.0001f);
}

TEST(FuryDemandModelTest, DemandRecoversTowardTarget) {
	FuryDemandState state;
	state.current = 0.2f;
	state.target = 0.6f;
	state.recoveryPerTick = 0.05f;

	auto next = FuryDemandModel::tick(state);

	EXPECT_NEAR(next.current, 0.25f, 0.0001f);
}

TEST(FuryDemandModelTest, RecoveryDoesNotOvershoot) {
	FuryDemandState state;
	state.current = 0.58f;
	state.target = 0.6f;
	state.recoveryPerTick = 0.05f;

	auto next = FuryDemandModel::tick(state);

	EXPECT_NEAR(next.current, 0.6f, 0.0001f);
}

TEST(FuryDemandModelTest, InvalidValuesAreClamped) {
	FuryDemandState state;
	state.current = 3.0f;
	state.target = -1.0f;
	state.recoveryPerTick = -5.0f;

	auto next = FuryDemandModel::tick(state);

	EXPECT_FLOAT_EQ(next.current, 1.0f);
	EXPECT_FLOAT_EQ(next.target, 0.0f);
}
