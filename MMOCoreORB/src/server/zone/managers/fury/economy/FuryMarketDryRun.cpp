#include "FuryMarketDryRun.h"

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace {
int medianPrice(std::vector<int>& prices) {
	if (prices.empty())
		return 0;

	std::sort(prices.begin(), prices.end());
	const std::size_t middle = prices.size() / 2;

	if ((prices.size() % 2) == 1)
		return prices[middle];

	const long long left = prices[middle - 1];
	const long long right = prices[middle];
	return static_cast<int>((left + right) / 2);
}
}

FuryMarketDryRunSummary FuryMarketDryRun::evaluate(
	const std::vector<FuryMarketListing>& listings,
	float defaultDemand,
	float purchaseThreshold) {

	FuryMarketDryRunSummary summary;
	std::unordered_map<int, std::vector<int>> pricesByType;

	for (const auto& listing : listings) {
		if (listing.auction || listing.askingPrice <= 0)
			continue;

		pricesByType[listing.effectiveItemType].push_back(listing.askingPrice);
	}

	std::unordered_map<int, int> referencePriceByType;

	for (auto& entry : pricesByType)
		referencePriceByType[entry.first] = medianPrice(entry.second);

	for (const auto& listing : listings) {
		if (listing.auction || listing.askingPrice <= 0)
			continue;

		const auto pricesFound = pricesByType.find(listing.effectiveItemType);
		const auto referenceFound = referencePriceByType.find(listing.effectiveItemType);

		if (pricesFound == pricesByType.end() || referenceFound == referencePriceByType.end())
			continue;

		FuryMarketDryRunDecision result;
		result.listing = listing;
		result.comparableListings = static_cast<int>(pricesFound->second.size());
		result.referencePrice = referenceFound->second;

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
