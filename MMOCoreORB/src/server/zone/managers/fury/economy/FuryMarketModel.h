#ifndef FURYMARKETMODEL_H_
#define FURYMARKETMODEL_H_

#include <cstdint>

struct FuryMarketDecisionInput {
	float demand = 0.5f;
	float quality = 0.5f;
	int askingPrice = 0;
	int referencePrice = 0;
	std::uint64_t listingId = 0;
};

struct FuryMarketDecision {
	float score = 0.0f;
	float demandScore = 0.0f;
	float qualityScore = 0.0f;
	float priceScore = 0.0f;
	float deterministicRoll = 0.0f;
	bool purchase = false;
};

/**
 * Pure deterministic decision model.
 *
 * No Core3 objects, no random global state and no side effects. This allows
 * the market policy to be tuned and tested independently of settlement.
 */
class FuryMarketModel {
public:
	static FuryMarketDecision evaluate(const FuryMarketDecisionInput& input, float purchaseThreshold = 0.62f);

private:
	static float clamp01(float value);
	static float deterministicRoll(std::uint64_t listingId);
};

#endif // FURYMARKETMODEL_H_
