#include <cassert>
#include <iostream>

#include "server/zone/managers/fury/economy/FurySettlementPlan.h"

int main() {
	FurySettlementInput input;
	input.grossPrice = 1050;
	input.citySalesTaxPercent = 5.0f;

	auto result = FurySettlementPlanner::plan(input);

	assert(result.eligible);
	const int nativeTax = input.grossPrice -
		(input.grossPrice / (1.0f + (input.citySalesTaxPercent / 100.0f)));

	assert(result.tax == nativeTax);
	assert(result.sellerNet == input.grossPrice - nativeTax);

	input.auction = true;
	result = FurySettlementPlanner::plan(input);
	assert(!result.eligible);
	assert(result.rejection == FurySettlementRejection::AuctionUnsupported);

	std::cout << "FURY settlement plan smoke OK\n";
	return 0;
}
