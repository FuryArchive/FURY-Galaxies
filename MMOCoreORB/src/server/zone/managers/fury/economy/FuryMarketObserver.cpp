#include "FuryMarketObserver.h"

FuryMarketSnapshot FuryMarketObserver::scan(TerminalListVector* items) {
	FuryMarketSnapshot snapshot;

	if (items == nullptr)
		return snapshot;

	snapshot.terminalCount = items->size();

	for (int i = 0; i < items->size(); ++i) {
		Reference<TerminalItemList*> terminalList = items->get(i);

		if (terminalList == nullptr || terminalList->size() == 0)
			continue;

		// Work from a copy, matching AuctionManager maintenance behavior.
		Reference<TerminalItemList*> list = new TerminalItemList(*terminalList);

		for (int j = 0; j < list->size(); ++j) {
			ManagedReference<AuctionItem*> item = list->get(j);

			if (item == nullptr || item->getStatus() != AuctionItem::FORSALE)
				continue;

			const int price = item->getPrice();

			snapshot.activeListings++;
			snapshot.totalAskingPrice += price > 0 ? static_cast<unsigned long long>(price) : 0;

			if (item->isAuction())
				snapshot.auctionListings++;
			else
				snapshot.fixedPriceListings++;

			if (snapshot.activeListings == 1 || price < snapshot.minPrice)
				snapshot.minPrice = price;

			if (snapshot.activeListings == 1 || price > snapshot.maxPrice)
				snapshot.maxPrice = price;
		}
	}

	return snapshot;
}


std::vector<FuryMarketListing> FuryMarketObserver::collectListings(TerminalListVector* items) {
	std::vector<FuryMarketListing> listings;

	if (items == nullptr)
		return listings;

	for (int i = 0; i < items->size(); ++i) {
		Reference<TerminalItemList*> terminalList = items->get(i);

		if (terminalList == nullptr || terminalList->size() == 0)
			continue;

		Reference<TerminalItemList*> list = new TerminalItemList(*terminalList);

		for (int j = 0; j < list->size(); ++j) {
			ManagedReference<AuctionItem*> item = list->get(j);

			if (item == nullptr || item->getStatus() != AuctionItem::FORSALE)
				continue;

			FuryMarketListing listing;
			listing.listingId = item->getAuctionedItemObjectID();
			listing.vendorId = item->getVendorID();
			listing.ownerId = item->getOwnerID();
			listing.itemType = item->getItemType();
			listing.effectiveItemType =
				item->isFactoryCrate() && item->getCratedItemType() > 0
					? item->getCratedItemType()
					: item->getItemType();
			listing.askingPrice = item->getPrice();
			listing.size = item->getSize();
			listing.auction = item->isAuction();
			listing.factoryCrate = item->isFactoryCrate();
			listing.onBazaar = item->isOnBazaar();

			listings.push_back(listing);
		}
	}

	return listings;
}
