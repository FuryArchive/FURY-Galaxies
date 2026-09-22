#include "FuryMarketDryRun.h"

#include <algorithm>
#include <unordered_map>
#include <vector>
#include <cstdint>

namespace {
std::uint32_t marketComparisonKey(const FuryMarketListing& listing) {
	return listing.comparisonKey != 0 ? listing.comparisonKey : static_cast<std::uint32_t>(listing.effectiveItemType);
}

int comparisonPrice(const FuryMarketListing& listing) {
	int units = listing.units > 0 ? listing.units : 1;
	const long long gross = listing.askingPrice;

	if (gross <= 0)
		return 0;

	const long long perUnit = (gross + units - 1) / units;
	return static_cast<int>(perUnit > 0 ? perUnit : 1);
}

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

float medianSignal(std::vector<float>& signals) {
	if (signals.empty())
		return 0.0f;

	std::sort(signals.begin(), signals.end());
	const std::size_t middle = signals.size() / 2;

	if ((signals.size() % 2) == 1)
		return signals[middle];

	return (signals[middle - 1] + signals[middle]) * 0.5f;
}

float relativeQuality(float signal, float referenceSignal) {
	if (signal < 0.0f || referenceSignal <= 0.0f)
		return 0.0f;

	float normalized = signal / (referenceSignal * 2.0f);

	if (normalized < 0.0f)
		return 0.0f;

	if (normalized > 1.0f)
		return 1.0f;

	return normalized;
}
}

FuryMarketDryRunSummary FuryMarketDryRun::evaluate(
	const std::vector<FuryMarketListing>& listings,
	float defaultDemand,
	float purchaseThreshold) {

	FuryMarketDryRunSummary summary;
	std::unordered_map<std::uint32_t, std::vector<int>> pricesByType;
	std::unordered_map<std::uint32_t, std::vector<float>> qualityByType;

	for (const auto& listing : listings) {
		if (listing.auction || listing.askingPrice <= 0)
			continue;

		pricesByType[marketComparisonKey(listing)].push_back(comparisonPrice(listing));

		if (listing.qualitySignalKnown && listing.qualitySignal >= 0.0f)
			qualityByType[marketComparisonKey(listing)].push_back(listing.qualitySignal);
	}

	std::unordered_map<std::uint32_t, int> referencePriceByType;
	std::unordered_map<std::uint32_t, float> referenceQualityByType;

	for (auto& entry : pricesByType)
		referencePriceByType[entry.first] = medianPrice(entry.second);

	for (auto& entry : qualityByType)
		referenceQualityByType[entry.first] = medianSignal(entry.second);

	for (const auto& listing : listings) {
		if (listing.auction || listing.askingPrice <= 0)
			continue;

		const auto pricesFound = pricesByType.find(marketComparisonKey(listing));
		const auto referenceFound = referencePriceByType.find(marketComparisonKey(listing));

		if (pricesFound == pricesByType.end() || referenceFound == referencePriceByType.end())
			continue;

		FuryMarketDryRunDecision result;
		result.listing = listing;
		result.comparableListings = static_cast<int>(pricesFound->second.size());
		result.comparisonPrice = comparisonPrice(listing);
		result.referencePrice = referenceFound->second;

		FuryMarketDecisionInput input;
		input.demand = defaultDemand;
		input.askingPrice = result.comparisonPrice;
		input.referencePrice = result.referencePrice;
		input.listingId = listing.listingId;

		const auto qualityReference = referenceQualityByType.find(marketComparisonKey(listing));

		if (listing.qualitySignalKnown &&
			qualityReference != referenceQualityByType.end() &&
			qualityReference->second > 0.0f) {
			input.qualityKnown = true;
			input.quality = relativeQuality(listing.qualitySignal, qualityReference->second);
		}

		result.decision = FuryMarketModel::evaluate(input, purchaseThreshold);

		summary.eligibleListings++;

		if (result.decision.purchase)
			summary.wouldPurchase++;

		summary.decisions.push_back(result);
	}

	return summary;
}
