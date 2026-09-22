#ifndef FURYSETTLEMENTSAFETY_H_
#define FURYSETTLEMENTSAFETY_H_

#include <cstdint>

class FurySettlementSafety {
public:
	static std::int64_t remainingCreditCapacity(
		int bankCredits,
		int cashCredits,
		int creditCap);

	static bool canReceiveFullPayout(
		int bankCredits,
		int cashCredits,
		int payout,
		int creditCap);

	static bool canReceiveCityTax(
		double treasury,
		int tax,
		double treasuryCap);
};

#endif // FURYSETTLEMENTSAFETY_H_
