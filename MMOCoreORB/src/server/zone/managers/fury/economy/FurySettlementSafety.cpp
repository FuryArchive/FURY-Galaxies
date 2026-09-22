#include "FurySettlementSafety.h"

#include <algorithm>

std::int64_t FurySettlementSafety::remainingCreditCapacity(
	int bankCredits,
	int cashCredits,
	int creditCap) {

	if (creditCap < 0)
		return 0;

	const std::int64_t bankRoom =
		std::max<std::int64_t>(0, static_cast<std::int64_t>(creditCap) - bankCredits);
	const std::int64_t cashRoom =
		std::max<std::int64_t>(0, static_cast<std::int64_t>(creditCap) - cashCredits);

	return bankRoom + cashRoom;
}

bool FurySettlementSafety::canReceiveFullPayout(
	int bankCredits,
	int cashCredits,
	int payout,
	int creditCap) {

	if (payout <= 0)
		return false;

	return static_cast<std::int64_t>(payout) <=
		remainingCreditCapacity(bankCredits, cashCredits, creditCap);
}

bool FurySettlementSafety::canReceiveCityTax(
	double treasury,
	int tax,
	double treasuryCap) {

	if (tax <= 0)
		return true;

	if (treasuryCap < 0.0 || treasury < 0.0)
		return false;

	return treasury + static_cast<double>(tax) <= treasuryCap;
}
