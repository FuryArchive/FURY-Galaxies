#include <cassert>
#include <iostream>

#include "server/zone/managers/fury/economy/FuryExecutionPolicy.h"

int main() {
	assert(FuryExecutionPolicy::effectiveMaxPurchasesPerTick(-5) == 0);
	assert(FuryExecutionPolicy::effectiveMaxPurchasesPerTick(0) == 0);
	assert(FuryExecutionPolicy::effectiveMaxPurchasesPerTick(1) == 1);
	assert(FuryExecutionPolicy::effectiveMaxPurchasesPerTick(2) == 1);
	assert(FuryExecutionPolicy::effectiveMaxPurchasesPerTick(500) == 1);

	std::cout << "FURY execution policy smoke OK\n";
	return 0;
}
