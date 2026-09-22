#include <cassert>
#include <cmath>
#include <iostream>

#include "server/zone/managers/fury/economy/FuryDemandModel.h"

int main() {
	FuryDemandState state;
	state.current = 0.8f;
	state.target = 0.6f;
	state.purchaseImpact = 0.1f;
	state.recoveryPerTick = 0.05f;

	auto afterSale = FuryDemandModel::applyPurchase(state, 2);
	assert(std::fabs(afterSale.current - 0.6f) < 0.0001f);

	afterSale.current = 0.3f;
	auto recovered = FuryDemandModel::tick(afterSale);
	assert(std::fabs(recovered.current - 0.35f) < 0.0001f);

	std::cout << "FURY demand model smoke OK\n";
	return 0;
}
