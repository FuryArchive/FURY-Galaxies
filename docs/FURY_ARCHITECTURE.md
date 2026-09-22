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

Current economy controls:
- `Fury.Economy.ObserveVendorMarket`
- `Fury.Economy.TickSeconds`
- `Fury.Economy.DryRun`
- `Fury.Economy.ExecutePurchases`
- `Fury.Economy.PersistDemand`
- `Fury.Economy.CanaryOnly`
- `Fury.Economy.CanaryListingId`
- `Fury.Economy.CanaryOwnerId`
- `Fury.Economy.RequireKnownQualityForPurchases`
- `Fury.Economy.MinComparablesForPurchase`
- `Fury.Economy.MaxPurchasesPerTick`
- `Fury.Economy.MaxGrossPricePerPurchase`
- `Fury.Economy.MaxGrossCreditsPerTick`
- `Fury.Economy.FailureInjectionStage`

Default execution posture is fail-closed:
`DryRun=1`, `ExecutePurchases=0`, `PersistDemand=0`,
`CanaryOnly=1`, with no canary selector configured.

## Economy vertical slice

### Phase 0 — observe — implemented
- scan native vendor listings;
- produce market snapshots;
- no mutation.

### Phase 1 — score — implemented
- exact-template comparison groups;
- factory-crate comparison on per-unit price while retaining gross settlement price;
- deterministic purchase score;
- weapon/armor quality extraction;
- dry-run decisions.

### Phase 2 — guarded settlement — implementation complete, runtime canary pending
- fixed-price NPC purchases only;
- weapon/armor categories only by default;
- live listing/product revalidation after cross-lock acquisition;
- live auction-map identity revalidation;
- seller/city capacity guards;
- explicit canary execution scope;
- seller notification only after durable commit.

Settlement durability does not rely on Task exception semantics. FURY explicitly
serializes seller credits, city treasury and demand plus AuctionItem/sold-object
deletes into the worker-local database transaction, then uses one synchronous
Berkeley commit. Pre-commit failure injection aborts pending DB writes and
fail-stops the process; post-commit stage 6 verifies the opposite recovery
boundary.

### Phase 3 — persistent regional demand — implementation complete, runtime canary pending
- persistent `FuryEconomyState`;
- dry-run may read persistent demand but never mutates it;
- purchase impact is committed atomically with the purchase;
- demand recovery runs only in execution mode;
- settlement rechecks current demand under lock before mutation so multiple
  decisions from one tick consume demand serially.

### Phase 4 — simulation — future
- broader item-category valuation;
- city/planet production and consumption;
- events and trade routes;
- longer-horizon credit-source/sink balancing.

### Runtime gate
Compile/smoke validation is not the release gate for real purchasing.
`docs/research/runtime-canary-runbook.md` defines the required failure stages
1-6 and normal-success restart checks. `CanaryOnly=0` must remain out of
normal configuration until that pass is green.

## Upstream strategy
`unstable` stays close to `swgemu/Core3:unstable`.
FURY work lands on `fury-dev`.
Sync upstream into `unstable`, then merge/rebase `fury-dev` deliberately and resolve only real FURY conflicts.
