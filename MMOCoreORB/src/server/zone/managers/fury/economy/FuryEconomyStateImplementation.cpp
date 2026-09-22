#include "server/zone/managers/fury/economy/FuryEconomyState.h"

int FuryEconomyStateImplementation::recoverDemand(float target, float recoveryPerTick) {
	if (target < 0.0f)
		target = 0.0f;
	else if (target > 1.0f)
		target = 1.0f;

	if (recoveryPerTick < 0.0f)
		recoveryPerTick = 0.0f;

	int changed = 0;

	for (int i = 0; i < demand.size(); ++i) {
		const String key = demand.elementAt(i).getKey();
		float current = demand.get(key);
		float next = current;

		if (current < target) {
			next = current + recoveryPerTick;

			if (next > target)
				next = target;
		} else if (current > target) {
			next = current - recoveryPerTick;

			if (next < target)
				next = target;
		}

		if (next < 0.0f)
			next = 0.0f;
		else if (next > 1.0f)
			next = 1.0f;

		if (next != current) {
			demand.put(key, next);
			changed++;
		}
	}

	lastDemandTick.updateToCurrentTime();
	return changed;
}
