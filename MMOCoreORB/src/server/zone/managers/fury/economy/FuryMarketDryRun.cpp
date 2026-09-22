#include "FuryMarketDryRun.h"

#include <unordered_map>

namespace {
struct PriceAccumulator {
	long long total = 0;
	int count = 0;
};
}

FuryMarketDryRunSummary FuryMarketDryRun::evaluate(
	const std::vector<FuryMarketListing>& listings,
	float defaultDemand,
	float purchaseThreshold) {

	FuryMarketDryRunSummary summary;
	std::unordered_map<int, PriceAccumulator> prices;

	for (const auto& listing : listings) {
		if (listing.auction || listing.askingPrice <= 0)
			continue;

		auto& accumulator = prices[listing.effectiveItemType];
		accumulator.total += listing.askingPrice;
		accumulator.count++;
	}

	for (const auto& listing : listings) {
		if (listing.auction || listing.askingPrice <= 0)
			continue;

		const auto found = prices.find(listing.effectiveItemType);

		if (found == prices.end() || found->second.count <= 0)
			continue;

		FuryMarketDryRunDecision result;
		result.listing = listing;
		result.comparableListings = found->second.count;
		result.referencePrice = static_cast<int>(found->second.total / found->second.count);

		FuryMarketDecisionInput input;
		input.demand = defaultDemand;
		input.qualityKnown = false;
		input.askingPrice = listing.askingPrice;
		input.referencePrice = result.referencePrice;
		input.listingId = listing.listingId;

		result.decision = FuryMarketModel::evaluate(input, purchaseThreshold);

		summary.eligibleListings++;

		if (result.decision.purchase)
			summary.wouldPurchase++;

		summary.decisions.push_back(result);
	}

	return summary;
}
