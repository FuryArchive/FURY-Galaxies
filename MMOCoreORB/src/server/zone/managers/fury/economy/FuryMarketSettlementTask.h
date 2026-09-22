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

public:
	FuryMarketSettlementTask(
		AuctionManager* manager,
		uint64 objectId,
		uint64 vendorId,
		uint64 ownerId,
		int price,
		int units,
		uint32 comparisonKey) {

		auctionManager = manager;
		listingId = objectId;
		expectedVendorId = vendorId;
		expectedOwnerId = ownerId;
		expectedPrice = price;
		expectedUnits = units;
		expectedComparisonKey = comparisonKey;
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
			expectedComparisonKey);
	}
};

#endif // FURYMARKETSETTLEMENTTASK_H_
