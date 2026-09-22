#include "FuryMarketModel.h"

float FuryMarketModel::clamp01(float value) {
	if (value < 0.0f)
		return 0.0f;

	if (value > 1.0f)
		return 1.0f;

	return value;
}

float FuryMarketModel::deterministicRoll(std::uint64_t listingId) {
	// SplitMix64 finalizer: stable across process restarts and platforms.
	std::uint64_t z = listingId + 0x9e3779b97f4a7c15ULL;
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
	z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
	z ^= (z >> 31);

	const std::uint32_t bucket = static_cast<std::uint32_t>(z & 0xFFFFFFULL);
	return static_cast<float>(bucket) / static_cast<float>(0xFFFFFFU);
}

FuryMarketDecision FuryMarketModel::evaluate(const FuryMarketDecisionInput& input, float purchaseThreshold) {
	FuryMarketDecision decision;

	decision.demandScore = clamp01(input.demand);
	decision.qualityScore = clamp01(input.quality);

	if (input.askingPrice <= 0) {
		decision.priceScore = 1.0f;
	} else if (input.referencePrice <= 0) {
		decision.priceScore = 0.5f;
	} else {
		const float ratio = static_cast<float>(input.askingPrice) / static_cast<float>(input.referencePrice);

		// 50% of reference price -> 1.0; reference price -> 0.75;
		// 150% -> 0.5; 250% or more -> 0.
		decision.priceScore = clamp01(1.25f - (ratio * 0.5f));
	}

	decision.deterministicRoll = deterministicRoll(input.listingId);

	// Demand dominates. Quality matters, but a cheap mediocre item can still
	// move in a high-demand market.
	decision.score =
		(decision.demandScore * 0.50f) +
		(decision.priceScore * 0.30f) +
		(decision.qualityScore * 0.20f);

	const float threshold = clamp01(purchaseThreshold);

	// The deterministic roll introduces listing-level variation without making
	// behavior change between identical server restarts.
	const float rollAdjustedThreshold = clamp01(threshold + ((decision.deterministicRoll - 0.5f) * 0.10f));
	decision.purchase = decision.score >= rollAdjustedThreshold;

	return decision;
}
