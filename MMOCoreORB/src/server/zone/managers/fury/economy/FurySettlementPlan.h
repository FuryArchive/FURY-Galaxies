#ifndef FURYSETTLEMENTPLAN_H_
#define FURYSETTLEMENTPLAN_H_

enum class FurySettlementRejection {
	None = 0,
	NotForSale,
	AuctionUnsupported,
	InvalidPrice,
	MissingSeller,
	MissingItem
};

struct FurySettlementInput {
	int grossPrice = 0;
	float citySalesTaxPercent = 0.0f;
	bool forSale = true;
	bool auction = false;
	bool sellerExists = true;
	bool itemExists = true;
};

struct FurySettlementPlan {
	bool eligible = false;
	FurySettlementRejection rejection = FurySettlementRejection::None;
	int grossPrice = 0;
	int tax = 0;
	int sellerNet = 0;
};

class FurySettlementPlanner {
public:
	static FurySettlementPlan plan(const FurySettlementInput& input);
};

#endif // FURYSETTLEMENTPLAN_H_
