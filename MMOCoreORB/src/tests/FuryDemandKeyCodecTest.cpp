#include "gtest/gtest.h"

#include "server/zone/managers/fury/economy/FuryDemandKeyCodec.h"

TEST(FuryDemandKeyCodecTest, EncodesStableRegionalProductKey) {
	FuryDemandKey key;
	key.planetCrc = 305419896u;
	key.regionId = 42u;
	key.productKey = 2596069104u;

	EXPECT_EQ(
		FuryDemandKeyCodec::encode(key),
		"305419896:42:2596069104");
}

TEST(FuryDemandKeyCodecTest, CitylessRegionUsesZero) {
	FuryDemandKey key;
	key.planetCrc = 7u;
	key.regionId = 0u;
	key.productKey = 9u;

	EXPECT_EQ(FuryDemandKeyCodec::encode(key), "7:0:9");
}
