# FURY Galaxies Status

## 2026-09-22 — pre-TRE runtime proof boundary

### Foundation
- Fork: `FuryArchive/FURY-Galaxies`
- Upstream base: `unstable`
- Active branch: `fury-dev`
- Native Core3 auction, persistence, credit, city and object systems remain authoritative.
- FURY-specific economy policy/state code lives under
  `MMOCoreORB/src/server/zone/managers/fury/`.
- Docker bootstrap clones `FuryArchive/FURY-Galaxies:fury-dev` while preserving
  the inherited `workspace/Core3` layout.

### Economy vertical slice

#### Observation / valuation
Implemented:
- player-vendor **and bazaar** market observation;
- fixed-price filtering;
- exact-template comparison keys;
- median reference prices;
- factory-crate per-unit comparison while settlement retains gross listing price;
- weapon quality from damage / attack speed;
- armor quality from protection values;
- factory-crate quality from prototype;
- deterministic purchase decisions;
- unknown-quality categories remain observable but fail closed for mutation by default.

#### Persistent demand
Implemented:
- `FuryEconomyState` persisted in Berkeley DB;
- demand keys by planet + city/region + exact comparison key;
- purchase impact and recovery;
- dry-run is non-mutating;
- settlement rechecks live demand while holding the demand-state lock;
- duplicate persistent demand-state objects fail closed.

#### Guarded settlement
Implemented:
- exact listing canary scope with optional seller second factor;
- strict full-width uint64 OID parsing;
- live listing / auction-map identity revalidation;
- owner, vendor, price, quantity, template and quality revalidation;
- seller CreditObject ownership/integrity checks;
- seller bank/cash capacity checks;
- city treasury overflow protection;
- gross-per-purchase / gross-per-tick limits;
- sold persistent-object-graph size cap;
- temporary execution cap of one NPC purchase per market tick;
- native Core3 deletion lifecycle before durable persistence;
- post-commit audit/log/client notifications outside settlement locks.

#### Exactly-once persistence
Implemented:
- worker-local DB transaction isolated before mutation;
- seller credits, city treasury and demand explicitly serialized;
- AuctionItem and complete persistent sold-object graph explicitly deleted;
- one engine3 Berkeley local transaction;
- synchronous `commitSync()` durable boundary;
- failure stages 1-5 abort before durable commit;
- failure stage 6 aborts immediately after durable commit;
- stages 1-5 are expected to restore unchanged state after restart;
- stage 6 is expected to restore one fully committed settlement.

### Deterministic runtime fixture

Implemented:
- native CDEF fixture created through Core3 objects/AuctionMap, not SQL injection;
- target listing: 100 credits;
- two identical comparables: 1,000 credits each;
- identical deterministic weapon stats;
- fixture demand: 0.8;
- target must evaluate as a real `wouldPurchase=true` candidate before persistence;
- fixture uses an existing seller character but seller may be offline;
- vendor OID 0 auto-selects the lowest loaded bazaar terminal;
- fixture refuses contaminated markets containing unrelated listings with the same template key;
- all fixture objects are transient until validation succeeds;
- failed pre-commit fixture creation cleans transient AuctionItems/weapons from RAM;
- fixture + demand become durable in one local transaction;
- exact 64-bit OIDs are emitted as decimal strings for shell orchestration.

### Runtime proof harness

Top-level bootstrap:

```bash
fury-market-fixture <seller-oid> baseline
```

This:
1. enables the deterministic fixture only for the bootstrap run;
2. starts Core3 and creates/validates the fixture;
3. performs native graceful shutdown;
4. removes fixture-creation config so later restarts cannot recreate/reset it;
5. captures a machine baseline probe;
6. saves matching MariaDB + Berkeley snapshot;
7. prints the exact target listing and seller OIDs.

Complete proof:

```bash
fury-market-canary-suite <listing-oid> <seller-oid> baseline
```

The suite runs:

`1 → 2 → 3 → 4 → 5 → 6 → 0`

For every stage it restores the same baseline, runs the expected crash/success
path, restarts through the probe path and verifies the resulting state.

Supporting helpers:
- `fury-market-snapshot`
- `fury-market-probe`
- `fury-market-verify`
- `fury-market-canary`
- `fury-market-fixture`
- `fury-market-canary-suite`

Snapshot restore rolls back **both** Berkeley and MariaDB if SQL restore fails.
Temporary restore paths are process-unique.

Shell-side verification never relies on JSON numeric conversion for Core3 OIDs;
exact decimal-string OID fields are authoritative.

### Stage 0 automation

Normal success is automated too.

After the exact canary purchase commits and post-commit effects finish,
`FuryCanaryShutdownTask` retries until Core3's console pipe is available, then
queues native `shutdown 0`. The runner requires:
- durable-settlement marker;
- successfully queued graceful-shutdown marker;
- clean Core3 exit;
- post-restart probe/verifier PASS.

### Default runtime posture

Real purchases remain fail-closed:

```lua
Fury.Economy.DryRun = 1
Fury.Economy.ExecutePurchases = 0
Fury.Economy.PersistDemand = 0
Fury.Economy.CanaryOnly = 1
Fury.Economy.CanaryListingId = "0"
Fury.Economy.CanaryOwnerId = "0"
```

An accidental `ExecutePurchases=1` still cannot buy anything without an exact
canary selector.

### Verification gates

Green:
1. **Pure FURY market-model CI**
   - decision model;
   - dry-run pricing;
   - demand model;
   - settlement planner/capacity;
   - execution-scope parsing.
2. **Full Core3 build CI**
   - deterministic fixture;
   - bazaar+vendor observation;
   - exact-OID probe output;
   - guarded settlement;
   - canary shutdown retry task.
   - validated code candidate: `31a07b74`.
3. **Docker runtime-harness smoke**
   - branch/bootstrap indirection;
   - canary config generation;
   - canary runner;
   - deterministic fixture helper;
   - seven-stage suite;
   - atomic snapshot restore helper;
   - verifier including OIDs above IEEE-754 exact integer range.
   - validated harness candidate: `fdae0149`.

### Boundary reached

**Steps 1-3 before live runtime validation are complete.**

The next step is the first real Core3 runtime proof. It requires:
- mounted SWG TRE data;
- an isolated local database;
- one existing test character OID with a valid CreditObject.

Then run:

```bash
fury-market-fixture <seller-oid> baseline
# use listing/seller printed by the helper:
fury-market-canary-suite <listing-oid> <seller-oid> baseline
```

No normal-play rollout should set `CanaryOnly=0` until all seven stages pass on
the real runtime.

### After the seven-stage proof
- allow guarded canary purchases in a real local play session;
- remove the temporary one-purchase-per-tick safety cap only after live comparable
  recomputation exists;
- expand item valuation beyond weapon/armor;
- begin production/consumption chains, professions, regional shortages and
  longer-horizon NPC economy simulation.

### Deferred until settlement proof is complete
- custom client UI;
- companion systems;
- city/settlement progression;
- broad progression rebalance;
- larger world-simulation layers.
