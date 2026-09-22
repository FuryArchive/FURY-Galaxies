#include <cassert>
#include <iostream>
#include <vector>

#include "server/zone/managers/fury/economy/FuryMarketDryRun.h"

int main() {
	std::vector<FuryMarketListing> listings;

	FuryMarketListing cheap;
	cheap.listingId = 100;
	cheap.effectiveItemType = 7;
	cheap.askingPrice = 500;
	listings.push_back(cheap);

	FuryMarketListing expensive = cheap;
	expensive.listingId = 101;
	expensive.askingPrice = 1500;
	listings.push_back(expensive);

	auto result = FuryMarketDryRun::evaluate(listings, 0.8f, 0.60f);

	assert(result.eligibleListings == 2);
	assert(result.decisions.size() == 2);
	assert(result.decisions[0].referencePrice == 1000);
	assert(result.decisions[1].referencePrice == 1000);
	assert(result.decisions[0].decision.priceScore > result.decisions[1].decision.priceScore);

	std::cout << "FURY dry-run market smoke OK\n";
	std::cout << "eligible=" << result.eligibleListings
	          << " wouldPurchase=" << result.wouldPurchase << "\n";
	return 0;
}
