#include "FuryExecutionPolicy.h"

int FuryExecutionPolicy::effectiveMaxPurchasesPerTick(int configuredMax) {
	if (configuredMax <= 0)
		return 0;

	return 1;
}
