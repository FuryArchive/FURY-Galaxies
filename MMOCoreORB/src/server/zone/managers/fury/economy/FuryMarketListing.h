#ifndef FURYMARKETLISTING_H_
#define FURYMARKETLISTING_H_

#include <cstdint>

struct FuryMarketListing {
	std::uint64_t listingId = 0;
	std::uint64_t vendorId = 0;
	std::uint64_t ownerId = 0;
	std::uint32_t planetCrc = 0;
	std::uint64_t regionId = 0;

	int itemType = 0;
	int effectiveItemType = 0;
	std::uint32_t comparisonKey = 0;
	int askingPrice = 0;
	int size = 1;
	int units = 1;

	bool auction = false;
	bool factoryCrate = false;
	bool onBazaar = false;

	bool qualitySignalKnown = false;
	float qualitySignal = 0.0f;
};

#endif // FURYMARKETLISTING_H_
