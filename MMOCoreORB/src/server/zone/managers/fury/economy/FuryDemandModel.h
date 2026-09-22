#ifndef FURYDEMANDMODEL_H_
#define FURYDEMANDMODEL_H_

struct FuryDemandState {
	float current = 0.5f;
	float target = 0.5f;
	float recoveryPerTick = 0.02f;
	float purchaseImpact = 0.05f;
};

class FuryDemandModel {
public:
	static FuryDemandState tick(const FuryDemandState& state);
	static FuryDemandState applyPurchase(const FuryDemandState& state, int units = 1);

private:
	static float clamp01(float value);
};

#endif // FURYDEMANDMODEL_H_
