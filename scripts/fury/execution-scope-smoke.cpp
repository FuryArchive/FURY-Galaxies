#include <cassert>
#include <iostream>

#include "server/zone/managers/fury/economy/FuryExecutionScope.h"

int main() {
	std::uint64_t parsed = 123;

	assert(FuryExecutionScopeGuard::parseDecimalOid("", parsed));
	assert(parsed == 0);
	assert(FuryExecutionScopeGuard::parseDecimalOid("0", parsed));
	assert(parsed == 0);
	assert(FuryExecutionScopeGuard::parseDecimalOid("18446744073709551615", parsed));
	assert(parsed == 18446744073709551615ULL);
	assert(!FuryExecutionScopeGuard::parseDecimalOid("18446744073709551616", parsed));
	assert(!FuryExecutionScopeGuard::parseDecimalOid("12x3", parsed));
	assert(!FuryExecutionScopeGuard::parseDecimalOid("-1", parsed));

	FuryExecutionScope scope;

	scope.valid = false;
	scope.canaryOnly = false;
	assert(!FuryExecutionScopeGuard::allows(scope, 123, 456));

	scope.valid = true;
	scope.canaryOnly = true;
	assert(!FuryExecutionScopeGuard::allows(scope, 123, 456));

	scope.listingId = 123;
	assert(FuryExecutionScopeGuard::allows(scope, 123, 456));
	assert(!FuryExecutionScopeGuard::allows(scope, 124, 456));

	scope.ownerId = 456;
	assert(FuryExecutionScopeGuard::allows(scope, 123, 456));
	assert(!FuryExecutionScopeGuard::allows(scope, 123, 457));

	scope.listingId = 0;
	assert(FuryExecutionScopeGuard::allows(scope, 999, 456));
	assert(!FuryExecutionScopeGuard::allows(scope, 999, 457));

	scope.canaryOnly = false;
	scope.ownerId = 0;
	assert(FuryExecutionScopeGuard::allows(scope, 999, 457));

	std::cout << "FURY execution scope smoke OK\n";
	return 0;
}
