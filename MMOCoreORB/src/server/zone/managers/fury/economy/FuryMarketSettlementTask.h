#ifndef FURYMARKETSETTLEMENTTASK_H_
#define FURYMARKETSETTLEMENTTASK_H_

#include "server/zone/managers/auction/AuctionManager.h"

class FuryMarketSettlementTask : public Task {
	ManagedWeakReference<AuctionManager*> auctionManager;
	uint64 listingId;

public:
	FuryMarketSettlementTask(AuctionManager* manager, uint64 id) {
		auctionManager = manager;
		listingId = id;
		setCustomTaskQueue("slowQueue");
	}

	void run() {
		ManagedReference<AuctionManager*> strongRef = auctionManager.get();

		if (strongRef == nullptr)
			return;

		ZoneServer* server = strongRef->getZoneServer();

		if (server == nullptr || server->isServerShuttingDown())
			return;

		strongRef->validateFuryMarketSettlement(listingId);
	}
};

#endif // FURYMARKETSETTLEMENTTASK_H_
