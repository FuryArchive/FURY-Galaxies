#include "gtest/gtest.h"

#include "server/zone/managers/fury/economy/FuryDemandBook.h"

TEST(FuryDemandBookTest, SeparatesPlanetRegionAndProduct) {
	FuryDemandBook book;
	FuryDemandState fallback;
	fallback.current = 0.8f;
	fallback.purchaseImpact = 0.1f;

	FuryDemandKey mosEisleyPistol;
	mosEisleyPistol.planetCrc = 1;
	mosEisleyPistol.regionId = 10;
	mosEisleyPistol.productKey = 100;

	FuryDemandKey bestinePistol = mosEisleyPistol;
	bestinePistol.regionId = 20;

	auto sold = book.applyPurchase(mosEisleyPistol, fallback, 2);

	EXPECT_NEAR(sold.current, 0.6f, 0.0001f);
	EXPECT_NEAR(book.get(bestinePistol, fallback).current, 0.8f, 0.0001f);
}

TEST(FuryDemandBookTest, DifferentTemplatesDoNotShareDemand) {
	FuryDemandBook book;
	FuryDemandState fallback;
	fallback.current = 0.7f;
	fallback.purchaseImpact = 0.1f;

	FuryDemandKey cdef;
	cdef.planetCrc = 1;
	cdef.regionId = 10;
	cdef.productKey = 1111;

	FuryDemandKey dh17 = cdef;
	dh17.productKey = 2222;

	book.applyPurchase(cdef, fallback, 1);

	EXPECT_NEAR(book.get(cdef, fallback).current, 0.6f, 0.0001f);
	EXPECT_NEAR(book.get(dh17, fallback).current, 0.7f, 0.0001f);
}

TEST(FuryDemandBookTest, TickRecoversStoredStatesOnly) {
	FuryDemandBook book;
	FuryDemandKey key;
	key.planetCrc = 1;
	key.regionId = 10;
	key.productKey = 1111;

	FuryDemandState state;
	state.current = 0.3f;
	state.target = 0.5f;
	state.recoveryPerTick = 0.05f;
	book.put(key, state);

	book.tickAll();

	EXPECT_NEAR(book.get(key, state).current, 0.35f, 0.0001f);
}
