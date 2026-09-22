#ifndef FURYEXECUTIONPOLICY_H_
#define FURYEXECUTIONPOLICY_H_

class FuryExecutionPolicy {
public:
	/**
	 * Until settlement recomputes its full comparable-price/quality reference
	 * from a lock-consistent market snapshot, only one NPC purchase may be
	 * scheduled from any evaluation tick.
	 */
	static int effectiveMaxPurchasesPerTick(int configuredMax);
};

#endif // FURYEXECUTIONPOLICY_H_
