#ifndef FURYEXECUTIONSCOPE_H_
#define FURYEXECUTIONSCOPE_H_

#include <cstdint>

struct FuryExecutionScope {
	bool canaryOnly = true;
	std::uint64_t listingId = 0;
	std::uint64_t ownerId = 0;
};

class FuryExecutionScopeGuard {
public:
	static bool allows(
		const FuryExecutionScope& scope,
		std::uint64_t candidateListingId,
		std::uint64_t candidateOwnerId);
};

#endif // FURYEXECUTIONSCOPE_H_
