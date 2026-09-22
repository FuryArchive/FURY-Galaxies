#include "FuryExecutionScope.h"

#include <limits>

bool FuryExecutionScopeGuard::parseDecimalOid(const char* text, std::uint64_t& value) {
	value = 0;

	if (text == nullptr || *text == '\0')
		return true;

	std::uint64_t parsed = 0;
	const std::uint64_t maxValue = std::numeric_limits<std::uint64_t>::max();

	for (const char* cursor = text; *cursor != '\0'; ++cursor) {
		const char ch = *cursor;

		if (ch < '0' || ch > '9')
			return false;

		const std::uint64_t digit = static_cast<std::uint64_t>(ch - '0');

		if (parsed > (maxValue - digit) / 10)
			return false;

		parsed = (parsed * 10) + digit;
	}

	value = parsed;
	return true;
}

bool FuryExecutionScopeGuard::allows(
	const FuryExecutionScope& scope,
	std::uint64_t candidateListingId,
	std::uint64_t candidateOwnerId) {

	if (!scope.valid)
		return false;

	if (!scope.canaryOnly)
		return true;

	// Canary mode is deliberately fail-closed.
	if (scope.listingId == 0 && scope.ownerId == 0)
		return false;

	if (scope.listingId != 0 && candidateListingId != scope.listingId)
		return false;

	if (scope.ownerId != 0 && candidateOwnerId != scope.ownerId)
		return false;

	return true;
}
