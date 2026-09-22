#ifndef FURYMARKETDRYRUN_H_
#define FURYMARKETDRYRUN_H_

#include <vector>

#include "FuryMarketListing.h"
#include "FuryMarketModel.h"

struct FuryMarketDryRunDecision {
	FuryMarketListing listing;
	int comparisonPrice = 0;
	int referencePrice = 0;
	int comparableListings = 0;
	FuryMarketDecision decision;
};

struct FuryMarketDryRunSummary {
	int eligibleListings = 0;
	int wouldPurchase = 0;
	std::vector<FuryMarketDryRunDecision> decisions;
};

/**
 * Pure read-only market pass over portable listing snapshots.
 *
 * The first FURY market intentionally ignores auctions and evaluates only
 * fixed-price listings. Reference price is derived from a robust median of comparable per-unit
 * prices for the exact product template when available.
 */
class FuryMarketDryRun {
public:
	static FuryMarketDryRunSummary evaluate(
		const std::vector<FuryMarketListing>& listings,
		float defaultDemand,
		float purchaseThreshold);
};

#endif // FURYMARKETDRYRUN_H_
