#include "FurySettlementPlan.h"

FurySettlementPlan FurySettlementPlanner::plan(const FurySettlementInput& input) {
	FurySettlementPlan result;
	result.grossPrice = input.grossPrice;

	if (!input.forSale) {
		result.rejection = FurySettlementRejection::NotForSale;
		return result;
	}

	if (input.auction) {
		result.rejection = FurySettlementRejection::AuctionUnsupported;
		return result;
	}

	if (input.grossPrice <= 0) {
		result.rejection = FurySettlementRejection::InvalidPrice;
		return result;
	}

	if (!input.sellerExists) {
		result.rejection = FurySettlementRejection::MissingSeller;
		return result;
	}

	if (!input.itemExists) {
		result.rejection = FurySettlementRejection::MissingItem;
		return result;
	}

	float taxPercent = input.citySalesTaxPercent;

	if (taxPercent < 0.0f)
		taxPercent = 0.0f;

	// Core3 stores vendor listing price inclusive of city sales tax and derives
	// the tax from that gross amount during doInstantBuy().
	if (taxPercent > 0.0f) {
		const float divisor = 1.0f + (taxPercent / 100.0f);
		result.tax = static_cast<int>(
			static_cast<float>(input.grossPrice) -
			(static_cast<float>(input.grossPrice) / divisor));
	}

	if (result.tax < 0)
		result.tax = 0;

	if (result.tax > result.grossPrice)
		result.tax = result.grossPrice;

	result.sellerNet = result.grossPrice - result.tax;
	result.eligible = true;
	result.rejection = FurySettlementRejection::None;
	return result;
}
