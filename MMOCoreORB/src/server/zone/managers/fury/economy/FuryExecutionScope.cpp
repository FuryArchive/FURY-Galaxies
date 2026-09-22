#include "FuryExecutionScope.h"

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
