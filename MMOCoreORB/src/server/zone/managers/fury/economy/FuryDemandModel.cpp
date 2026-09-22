#include "FuryDemandModel.h"

float FuryDemandModel::clamp01(float value) {
	if (value < 0.0f)
		return 0.0f;

	if (value > 1.0f)
		return 1.0f;

	return value;
}

FuryDemandState FuryDemandModel::tick(const FuryDemandState& state) {
	FuryDemandState next = state;
	next.current = clamp01(next.current);
	next.target = clamp01(next.target);

	const float recovery = next.recoveryPerTick < 0.0f ? 0.0f : next.recoveryPerTick;

	if (next.current < next.target) {
		next.current += recovery;

		if (next.current > next.target)
			next.current = next.target;
	} else if (next.current > next.target) {
		next.current -= recovery;

		if (next.current < next.target)
			next.current = next.target;
	}

	next.current = clamp01(next.current);
	return next;
}

FuryDemandState FuryDemandModel::applyPurchase(const FuryDemandState& state, int units) {
	FuryDemandState next = state;

	if (units <= 0) {
		next.current = clamp01(next.current);
		return next;
	}

	const float impact = next.purchaseImpact < 0.0f ? 0.0f : next.purchaseImpact;
	next.current = clamp01(next.current - (impact * units));
	return next;
}
