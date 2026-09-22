#include <cassert>
#include <cmath>
#include <iostream>

#include "server/zone/managers/fury/economy/FuryMarketModel.h"

int main() {
	FuryMarketDecisionInput strong;
	strong.demand = 0.9f;
	strong.quality = 0.7f;
	strong.qualityKnown = true;
	strong.askingPrice = 1000;
	strong.referencePrice = 1000;
	strong.listingId = 42;

	auto strongResult = FuryMarketModel::evaluate(strong, 0.60f);
	assert(strongResult.purchase);

	FuryMarketDecisionInput bad;
	bad.demand = 0.1f;
	bad.quality = 0.5f;
	bad.qualityKnown = true;
	bad.askingPrice = 5000;
	bad.referencePrice = 1000;
	bad.listingId = 42;

	auto badResult = FuryMarketModel::evaluate(bad, 0.60f);
	assert(!badResult.purchase);

	auto repeat = FuryMarketModel::evaluate(strong, 0.60f);
	assert(std::fabs(repeat.deterministicRoll - strongResult.deterministicRoll) < 0.000001f);
	assert(repeat.purchase == strongResult.purchase);

	FuryMarketDecisionInput liveDemand;
	liveDemand.demand = 0.80f;
	liveDemand.qualityKnown = true;
	liveDemand.quality = 0.60f;
	liveDemand.askingPrice = 100;
	liveDemand.referencePrice = 100;
	liveDemand.listingId = 424242;

	auto beforeDemandDrop = FuryMarketModel::evaluate(liveDemand, 0.62f);
	liveDemand.demand = 0.20f;
	auto afterDemandDrop = FuryMarketModel::evaluate(liveDemand, 0.62f);

	assert(beforeDemandDrop.purchase);
	assert(!afterDemandDrop.purchase);

	std::cout << "FURY market model smoke OK\n";
	std::cout << "strong score=" << strongResult.score << " roll=" << strongResult.deterministicRoll << "\n";
	std::cout << "bad score=" << badResult.score << " roll=" << badResult.deterministicRoll << "\n";
	return 0;
}
