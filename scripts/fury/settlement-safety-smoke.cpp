#include <cassert>
#include <iostream>

#include "server/zone/managers/fury/economy/FurySettlementSafety.h"

int main() {
	constexpr int cap = 2000000000;

	assert(FurySettlementSafety::remainingCreditCapacity(0, 0, cap) == 4000000000LL);
	assert(FurySettlementSafety::remainingCreditCapacity(cap, cap, cap) == 0);
	assert(FurySettlementSafety::remainingCreditCapacity(cap - 10, cap - 20, cap) == 30);

	assert(FurySettlementSafety::canReceiveFullPayout(cap - 10, cap - 20, 30, cap));
	assert(!FurySettlementSafety::canReceiveFullPayout(cap - 10, cap - 20, 31, cap));
	assert(!FurySettlementSafety::canReceiveFullPayout(0, 0, 0, cap));

	assert(FurySettlementSafety::canReceiveCityTax(99999990.0, 10, 100000000.0));
	assert(!FurySettlementSafety::canReceiveCityTax(99999991.0, 10, 100000000.0));
	assert(FurySettlementSafety::canReceiveCityTax(100000000.0, 0, 100000000.0));

	std::cout << "FURY settlement safety smoke OK\n";
	return 0;
}
