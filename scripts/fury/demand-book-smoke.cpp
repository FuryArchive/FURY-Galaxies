#include <cassert>
#include <cmath>
#include <iostream>

#include "server/zone/managers/fury/economy/FuryDemandBook.h"

int main() {
	FuryDemandBook book;
	FuryDemandState fallback;
	fallback.current = 0.8f;
	fallback.target = 0.6f;
	fallback.purchaseImpact = 0.1f;

	FuryDemandKey tatooine;
	tatooine.planetCrc = 123;
	tatooine.regionId = 456;
	tatooine.productKey = 789;

	auto next = book.applyPurchase(tatooine, fallback, 2);
	assert(std::fabs(next.current - 0.6f) < 0.0001f);
	assert(book.size() == 1);

	std::cout << "FURY demand book smoke OK\n";
	return 0;
}
