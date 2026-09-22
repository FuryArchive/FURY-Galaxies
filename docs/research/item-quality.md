# FURY crafted-item quality notes

## Core3 behavior verified

FURY must score the actual crafted object, not a guessed item tier.

For weapons, Core3's public getters already expose the effective item state:
- `getMinDamage(false)`
- `getMaxDamage(false)`
- `getAttackSpeed(false)`

The `false` argument excludes temporary power-up contribution, but the getters still apply slicing and condition degradation. Therefore FURY must **not** multiply those values by durability again.

For armor, `getKinetic()`, `getEnergy()` and the remaining protection getters call `getConditionReduction()` internally. They likewise already represent the worn item's effective protection.

## Current signals

### Weapons
`average effective damage / effective attack speed`

This is deliberately a compact first signal. Later category-specific scoring may also account for accuracy, wound chance, armor piercing, HAM costs and damage type.

### Armor
Mean effective protection across the nine resistance channels.

Later scoring may incorporate encumbrance, rating, vulnerabilities and slot coverage.

## Factory crates
A factory crate is scored from its prototype. Its product comparison key is also the prototype's server template CRC.

## Product identity
Price and quality comparisons use `SceneObject::getServerObjectCRC()` when available. This prevents unrelated items sharing a broad client game object type (for example, different pistol models) from contaminating each other's reference price.

## Condition
FURY records a separate `conditionRatio` for diagnostics, but does not apply it a second time to weapon/armor public getters.
