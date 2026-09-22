#include <cassert>
#include <iostream>

#include "server/zone/managers/fury/economy/FuryDemandKeyCodec.h"

int main() {
	FuryDemandKey key;
	key.planetCrc = 7;
	key.regionId = 42;
	key.productKey = 99;

	assert(FuryDemandKeyCodec::encode(key) == "7:42:99");
	std::cout << "FURY demand key codec smoke OK\n";
	return 0;
}
