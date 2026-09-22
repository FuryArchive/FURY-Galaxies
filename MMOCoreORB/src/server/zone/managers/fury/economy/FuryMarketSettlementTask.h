#ifndef FURYMARKETSETTLEMENTTASK_H_
#define FURYMARKETSETTLEMENTTASK_H_

#include "server/zone/managers/auction/AuctionManager.h"

class FuryMarketSettlementTask : public Task {
	ManagedWeakReference<AuctionManager*> auctionManager;
	uint64 listingId = 0;
	uint64 expectedVendorId = 0;
	uint64 expectedOwnerId = 0;
	int expectedPrice = 0;
	int expectedUnits = 1;
	uint32 expectedComparisonKey = 0;
	int expectedReferencePrice = 0;
	float expectedQualitySignal = 0.0f;
	float expectedQualityScore = 0.0f;
	bool expectedQualityKnown = false;

public:
	FuryMarketSettlementTask(
		AuctionManager* manager,
		uint64 objectId,
		uint64 vendorId,
		uint64 ownerId,
		int price,
		int units,
		uint32 comparisonKey,
		int referencePrice,
		float qualitySignal,
		float qualityScore,
		bool qualityKnown) {

		auctionManager = manager;
		listingId = objectId;
		expectedVendorId = vendorId;
		expectedOwnerId = ownerId;
		expectedPrice = price;
		expectedUnits = units;
		expectedComparisonKey = comparisonKey;
		expectedReferencePrice = referencePrice;
		expectedQualitySignal = qualitySignal;
		expectedQualityScore = qualityScore;
		expectedQualityKnown = qualityKnown;
		setCustomTaskQueue("slowQueue");
	}

	void run() {
		ManagedReference<AuctionManager*> manager = auctionManager.get();

		if (manager == nullptr)
			return;

		manager->settleFuryMarketListing(
			listingId,
			expectedVendorId,
			expectedOwnerId,
			expectedPrice,
			expectedUnits,
			expectedComparisonKey,
			expectedReferencePrice,
			expectedQualitySignal,
			expectedQualityScore,
			expectedQualityKnown);
	}
};

#endif // FURYMARKETSETTLEMENTTASK_H_
