# FURY Galaxies Status

## 2026-09-22

### Foundation
- Fork: `FuryArchive/FURY-Galaxies`
- Upstream tracking base: `unstable`
- FURY development: `fury-dev`
- Draft integration PR: #1
- FURY-specific server policy/model code lives under
  `MMOCoreORB/src/server/zone/managers/fury/`.
- Native Core3 auction, persistence, object, credit and city systems remain
  authoritative.
- Docker dev bootstrap can now clone
  `FuryArchive/FURY-Galaxies:fury-dev` while retaining the internal
  `workspace/Core3` layout.

### Economy vertical slice

#### Implemented and compile-validated

Observation / valuation:
- native vendor-market scan on a dedicated FURY tick;
- fixed-price filtering;
- deterministic purchase decisions;
- exact-template comparison keys;
- robust median reference prices;
- factory crates compared by per-unit price while settlement keeps the real
  gross listing price;
- weapon quality from damage / attack speed;
- armor quality from protection values;
- factory-crate quality from the crate prototype;
- unknown categories remain observable but are excluded from mutation by
  default.

Demand:
- persistent regional demand in `FuryEconomyState`;
- demand key includes planet, city/region and exact comparison key;
- purchase impact + recovery model;
- dry-run can read persistent demand but is strictly non-mutating;
- settlement rechecks current demand under lock before mutation, preventing
  multiple tasks from consuming a stale high-demand snapshot;
- duplicate persistent demand-state objects fail closed instead of selecting an
  arbitrary state.

Guarded settlement:
- seller payout and native inclusive city-tax calculation;
- seller bank/cash capacity validation against Core3 `CREDITCAP`;
- seller CreditObject ownership/integrity checks;
- city treasury overflow guard;
- gross per-purchase and per-tick caps;
- persistent sold-object graph cap;
- one exact-listing canary scope with optional owner second factor;
- strict uint64 canary OID parsing;
- live listing, auction-map, owner/vendor/price, quantity, template and quality
  revalidation after cross-lock acquisition;
- Core3 in-memory deletion lifecycle for AuctionItem + sold object graph;
- post-commit seller bank/cash delta and system-message notification;
- best-effort market audit only after durable commit.

Exactly-once persistence:
- settlement starts from a clean worker-local DB transaction;
- seller credits, city treasury and demand are explicitly serialized;
- AuctionItem + full persistent sold-object graph are explicitly deleted;
- all settlement writes/deletes reach one engine3 Berkeley transaction;
- the durable boundary is synchronous `commitSync()`;
- pre-commit failures abort pending DB writes and fail-stop the process;
- failure-injection stages 1-5 exercise crash-before-commit;
- stage 6 exercises crash-after-durable-commit.

### Default runtime posture

Real purchases remain disabled by default:

```lua
Fury.Economy.DryRun = 1
Fury.Economy.ExecutePurchases = 0
Fury.Economy.PersistDemand = 0
Fury.Economy.CanaryOnly = 1
Fury.Economy.CanaryListingId = "0"
Fury.Economy.CanaryOwnerId = "0"
```

Even an accidental `ExecutePurchases=1` does not authorize a purchase while
`CanaryOnly=1` and no exact listing ID is configured.

### Verification gates

Green:
1. **Pure FURY market-model CI**
   - market decision model;
   - per-unit dry-run pricing;
   - demand model/book;
   - settlement planner;
   - settlement capacity guards;
   - full-width canary OID parsing and exact-listing scope.
2. **Full Core3 build CI**
   - IDL generation;
   - engine3/Core3 integration;
   - current guarded settlement implementation.
   - green code candidate: `126be6fc`.

Prepared:
3. **Docker bootstrap smoke**
   - shell syntax;
   - FURY fork/branch clone indirection.

Still required:
4. **Runtime crash/restart proof**
   - isolated local DB snapshot;
   - real seller/vendor/listing fixture;
   - SWG TRE data mounted into the runtime;
   - failure stages 1-5 -> unchanged state after restart;
   - failure stage 6 -> fully committed state after restart;
   - normal stage 0 -> committed sale + post-commit notification/audit.

The exact procedure is in
`docs/research/runtime-canary-runbook.md`.

### Current release boundary

The economy implementation is no longer “read-only code”, but the shipped
configuration still is.

Do **not** set `CanaryOnly=0` for normal play until the runtime crash/restart
canary passes. Compile-green is necessary, not sufficient, for enabling the
credit-creating market simulation.

### Next work
1. finish Docker/runtime harness validation;
2. create a deterministic local seller/vendor/listing fixture;
3. execute crash-recovery stages 1-6 and normal success;
4. only then consider enabling guarded canary settlement for actual play;
5. after settlement is proven, expand valuation beyond weapon/armor and begin
   longer-horizon production/consumption and credit-source/sink tuning.

### Deferred until the economy proof is complete
- custom client UI;
- companions;
- settlement/city progression;
- broad progression rebalance;
- larger world-simulation systems.
