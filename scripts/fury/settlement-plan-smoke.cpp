#include <cassert>
#include <iostream>

#include "server/zone/managers/fury/economy/FurySettlementPlan.h"

int main() {
	FurySettlementInput input;
	input.grossPrice = 1050;
	input.citySalesTaxPercent = 5.0f;

	auto result = FurySettlementPlanner::plan(input);

	assert(result.eligible);
	assert(result.tax == 50);
	assert(result.sellerNet == 1000);

	input.auction = true;
	result = FurySettlementPlanner::plan(input);
	assert(!result.eligible);
	assert(result.rejection == FurySettlementRejection::AuctionUnsupported);

	std::cout << "FURY settlement plan smoke OK\n";
	return 0;
}
