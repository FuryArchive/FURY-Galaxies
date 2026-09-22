#ifndef FURYDEMANDBOOK_H_
#define FURYDEMANDBOOK_H_

#include <cstddef>
#include <unordered_map>

#include "FuryDemandKey.h"
#include "FuryDemandModel.h"

class FuryDemandBook {
	std::unordered_map<FuryDemandKey, FuryDemandState, FuryDemandKeyHash> states;

public:
	FuryDemandState get(const FuryDemandKey& key, const FuryDemandState& fallback) const;
	void put(const FuryDemandKey& key, const FuryDemandState& state);
	FuryDemandState applyPurchase(const FuryDemandKey& key, const FuryDemandState& fallback, int units = 1);
	void tickAll();

	std::size_t size() const {
		return states.size();
	}
};

#endif // FURYDEMANDBOOK_H_
