# FURY Galaxies Status

## 2026-09-22

### Foundation
- Fork: `FuryArchive/FURY-Galaxies`
- Upstream tracking base: `unstable`
- FURY development: `fury-dev`
- Draft integration PR: #1
- FURY-specific server code isolated under `MMOCoreORB/src/server/zone/managers/fury/`
- Bootstrap, environment doctor and upstream-sync helpers are present.

### Economy vertical slice

Implemented:
- native `AuctionManager` integration;
- dedicated FURY market scheduler independent of Core3's hourly auction maintenance;
- configurable tick interval;
- read-only collection of real native vendor listings;
- fixed-price listing filtering;
- robust median reference pricing per real item type;
- deterministic purchase decision model;
- unknown-quality handling without fabricated scores;
- real weapon quality signal from crafted min/max damage + attack speed + condition;
- real armor quality signal from protection stats + condition;
- factory crate quality via the crate prototype;
- demand saturation/recovery model;
- settlement planner matching Core3's inclusive city-tax semantics;
- explicit `ExecutePurchases = 0` hard default;
- detailed dry-run logging for individual candidate listings.

Still read-only:
- no listing is removed;
- no item is destroyed;
- no seller receives credits from FURY;
- no city receives FURY tax;
- no dynamic demand is persisted.

### Current verification gates

1. **Pure model smoke CI**
   - market decision;
   - dry-run pricing;
   - demand dynamics;
   - settlement planning.

2. **Full Core3 build CI**
   - validates IDL generation and actual integration with Core3.

3. **Runtime proof**
   - boot local server;
   - enable observer;
   - craft/list a real item on a native vendor;
   - verify listing, real stats, price reference and dry-run decision in server log.

### Next implementation after green full build
- add regional demand keys from the listing's actual vendor/planet;
- persist demand state through Core3 persistence;
- extract shared seller-payment/tax settlement from native `doInstantBuy`;
- add simulated-market settlement behind `ExecutePurchases`;
- enforce exactly-once purchase semantics;
- test restart/crash behavior before enabling mutations.

### Non-goals until the proof is complete
- custom client UI;
- companions;
- settlement/city progression;
- broad progression rebalance;
- economy-wide tuning.

Those come after the vendor vertical slice is demonstrably safe.
