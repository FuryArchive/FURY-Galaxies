# FURY Galaxies Architecture

## Product target
A single-player-first Star Wars sandbox RPG built on SWGEmu/Core3.

FURY does not replace SWG's simulation with a conventional quest RPG. It preserves and strengthens the systems that are valuable specifically because SWG was an MMO:
- multi-profession progression;
- heterogeneous resources and experimentation;
- harvesting and industrial production;
- player housing and structures;
- vendors and trade;
- mission generation;
- creature/ecology systems;
- factions and world geography.

The redesign removes dependence on a large concurrent player population.

## Architectural boundary

### Keep upstream
Networking, object persistence, zones, inventory, crafting, resource spawning, structures, vendors, auctions, missions, combat, creature AI, pets, grouping and the client protocol remain Core3 systems.

### Add in FURY
FURY adds policy and simulation layers above those systems:
- economy demand and simulated buyers;
- solo/group scaling;
- companion lifecycle and roles;
- settlement progression;
- regional world state;
- single-player progression rules;
- deterministic world events.

### Avoid
Do not duplicate inventory, currency, item ownership, vendor, mission or creature systems in an external service. FURY should call or extend Core3's authoritative systems.

## Package layout

```
MMOCoreORB/src/server/zone/managers/fury/
  economy/
  companions/
  settlement/
  scaling/
  worldstate/
```

Each domain should separate:
1. pure model/decision logic;
2. Core3 adapters;
3. side effects/persistence.

## Feature flags
FURY features are disabled by default until validated.

Initial keys:
- `Fury.Economy.ObserveVendorMarket`
- `Fury.Economy.TickSeconds`
- `Fury.Economy.DryRun`

## Economy vertical slice

Phase 0 — observe:
- scan native vendor listings;
- produce a market snapshot;
- no mutation.

Phase 1 — score:
- classify listing category;
- calculate deterministic purchase score;
- emit dry-run decisions.

Phase 2 — settle:
- execute a purchase using Core3 ownership/credit primitives;
- preserve seller mail/vendor behavior where applicable.

Phase 3 — persist demand:
- regional demand survives restart;
- purchases feed back into demand.

Phase 4 — simulation:
- city/planet demand;
- production and consumption;
- events and trade routes.

## Upstream strategy
`unstable` stays close to `swgemu/Core3:unstable`.
FURY work lands on `fury-dev`.
Sync upstream into `unstable`, then merge/rebase `fury-dev` deliberately and resolve only real FURY conflicts.
