#ifndef FURYITEMQUALITY_H_
#define FURYITEMQUALITY_H_

class SceneObject;

enum class FuryItemQualityKind {
	Unknown = 0,
	Weapon,
	Armor
};

struct FuryItemQuality {
	bool known = false;
	FuryItemQualityKind kind = FuryItemQualityKind::Unknown;
	float rawSignal = 0.0f;
	float conditionRatio = 1.0f;
};

class FuryItemQualityExtractor {
public:
	static FuryItemQuality inspect(SceneObject* object);
};

#endif // FURYITEMQUALITY_H_
