#ifndef FURYMARKETOBSERVER_H_
#define FURYMARKETOBSERVER_H_

#include "server/zone/objects/auction/AuctionItem.h"
#include "server/zone/managers/auction/TerminalListVector.h"

struct FuryMarketSnapshot {
	int terminalCount = 0;
	int activeListings = 0;
	int fixedPriceListings = 0;
	int auctionListings = 0;
	int minPrice = 0;
	int maxPrice = 0;
	unsigned long long totalAskingPrice = 0;

	int averagePrice() const {
		return activeListings > 0 ? static_cast<int>(totalAskingPrice / activeListings) : 0;
	}
};

/**
 * Read-only adapter over Core3's native vendor listing data.
 *
 * This class intentionally performs no mutation. It is the first integration
 * seam for the FURY single-player economy and can be left enabled while the
 * purchase/settlement path is still under development.
 */
class FuryMarketObserver {
public:
	static FuryMarketSnapshot scan(TerminalListVector* items);
};

#endif // FURYMARKETOBSERVER_H_
