#ifndef FURYMARKETTICKTASK_H_
#define FURYMARKETTICKTASK_H_

#include "server/zone/managers/auction/AuctionManager.h"

class FuryMarketTickTask : public Task {
	ManagedWeakReference<AuctionManager*> auctionManager;

public:
	explicit FuryMarketTickTask(AuctionManager* manager) {
		auctionManager = manager;
		setCustomTaskQueue("slowQueue");
	}

	void run() {
		ManagedReference<AuctionManager*> strongRef = auctionManager.get();

		if (strongRef == nullptr)
			return;

		auto* zoneServer = strongRef->getZoneServer();

		if (zoneServer == nullptr || zoneServer->isServerShuttingDown())
			return;

		strongRef->runFuryMarketTick();
	}
};

#endif // FURYMARKETTICKTASK_H_
