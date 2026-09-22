#ifndef FURYITEMQUALITY_H_
#define FURYITEMQUALITY_H_

#include <cstdint>

namespace server { namespace zone { namespace objects { namespace scene { class SceneObject; } } } }

enum class FuryItemQualityKind {
	Unknown = 0,
	Weapon,
	Armor
};

struct FuryItemQuality {
	bool known = false;
	FuryItemQualityKind kind = FuryItemQualityKind::Unknown;
	float rawSignal = 0.0f;
	std::uint32_t templateCrc = 0;
	float conditionRatio = 1.0f;
};

class FuryItemQualityExtractor {
public:
	static FuryItemQuality inspect(server::zone::objects::scene::SceneObject* object);
};

#endif // FURYITEMQUALITY_H_
