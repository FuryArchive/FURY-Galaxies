#include "FuryDemandBook.h"

FuryDemandState FuryDemandBook::get(const FuryDemandKey& key, const FuryDemandState& fallback) const {
	const auto found = states.find(key);

	if (found == states.end())
		return fallback;

	return found->second;
}

void FuryDemandBook::put(const FuryDemandKey& key, const FuryDemandState& state) {
	states[key] = state;
}

FuryDemandState FuryDemandBook::applyPurchase(
	const FuryDemandKey& key,
	const FuryDemandState& fallback,
	int units) {

	const FuryDemandState current = get(key, fallback);
	const FuryDemandState next = FuryDemandModel::applyPurchase(current, units);
	states[key] = next;
	return next;
}

void FuryDemandBook::tickAll() {
	for (auto& entry : states)
		entry.second = FuryDemandModel::tick(entry.second);
}
