#include "FuryItemQuality.h"

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/factorycrate/FactoryCrate.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/objects/tangible/wearables/ArmorObject.h"

namespace {
float conditionRatio(TangibleObject* tangible) {
	if (tangible == nullptr || tangible->getMaxCondition() <= 0)
		return 1.0f;

	float ratio = 1.0f -
		(static_cast<float>(tangible->getConditionDamage()) /
		 static_cast<float>(tangible->getMaxCondition()));

	if (ratio < 0.0f)
		return 0.0f;

	if (ratio > 1.0f)
		return 1.0f;

	return ratio;
}

SceneObject* unwrapFactoryCrate(SceneObject* object, ManagedReference<TangibleObject*>& prototype) {
	if (object == nullptr || !object->isFactoryCrate())
		return object;

	ManagedReference<FactoryCrate*> crate = cast<FactoryCrate*>(object);

	if (crate == nullptr)
		return object;

	prototype = crate->getPrototype();
	return prototype != nullptr ? prototype.get() : object;
}
}

FuryItemQuality FuryItemQualityExtractor::inspect(SceneObject* object) {
	FuryItemQuality result;

	if (object == nullptr)
		return result;

	ManagedReference<TangibleObject*> cratePrototype = nullptr;
	SceneObject* subject = unwrapFactoryCrate(object, cratePrototype);

	if (subject == nullptr || !subject->isTangibleObject())
		return result;

	ManagedReference<TangibleObject*> tangible = cast<TangibleObject*>(subject);

	if (tangible == nullptr)
		return result;

	result.conditionRatio = conditionRatio(tangible);

	if (subject->isWeaponObject()) {
		ManagedReference<WeaponObject*> weapon = cast<WeaponObject*>(subject);

		if (weapon == nullptr)
			return result;

		const float speed = weapon->getAttackSpeed(false);

		if (speed <= 0.0f)
			return result;

		const float averageDamage =
			(weapon->getMinDamage(false) + weapon->getMaxDamage(false)) * 0.5f;

		result.known = true;
		result.kind = FuryItemQualityKind::Weapon;
		result.rawSignal = (averageDamage / speed) * result.conditionRatio;
		return result;
	}

	if (subject->isArmorObject()) {
		ManagedReference<ArmorObject*> armor = cast<ArmorObject*>(subject);

		if (armor == nullptr)
			return result;

		const float protectionSum =
			armor->getKinetic() +
			armor->getEnergy() +
			armor->getElectricity() +
			armor->getStun() +
			armor->getBlast() +
			armor->getHeat() +
			armor->getCold() +
			armor->getAcid() +
			armor->getLightSaber();

		result.known = true;
		result.kind = FuryItemQualityKind::Armor;
		result.rawSignal = (protectionSum / 9.0f) * result.conditionRatio;
		return result;
	}

	return result;
}
