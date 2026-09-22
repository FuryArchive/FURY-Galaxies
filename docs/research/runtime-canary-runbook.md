# FURY NPC market runtime canary runbook

This is the first runtime validation path for real NPC market settlement.
Do not disable canary mode for this pass.

## Deterministic baseline bootstrap

With an existing test character OID, the baseline is now one command:

```bash
fury-market-fixture <seller-oid> baseline
```

The helper creates the native three-CDEF deterministic fixture, validates that
the 100-credit target is a real purchase candidate against two 1,000-credit
comparables, performs a normal Core3 shutdown, removes the fixture-creation
switch from config, captures the baseline probe and saves the matching MariaDB
+ Berkeley snapshot.

The vendor defaults to the lowest loaded bazaar terminal. Use
`--vendor <oid>` only when a specific terminal is required.

The helper prints the exact target listing and seller OIDs. Those values are
decimal strings end-to-end; shell tooling never round-trips them through JSON
floating-point numbers.

## Preconditions

Use an isolated/local Core3 database snapshot.

For a manual/non-fixture listing, choose one fixed-price listing that:

- belongs to a known test seller;
- is a weapon, armor item, or a factory crate containing one;
- appears as `wouldPurchase=1` in dry-run;
- satisfies the configured comparable-count and price caps.

Record before every run:

- auctioned item/listing OID;
- AuctionItem database OID if available;
- seller creature OID;
- seller bank + cash;
- city treasury when the vendor is in a player city;
- current regional demand for the listing key;
- sold object and persistent child OIDs.

## Canary configuration

Keep the scope exact:

```lua
Fury.Economy.ObserveVendorMarket = 1
Fury.Economy.DryRun = 0
Fury.Economy.ExecutePurchases = 1
Fury.Economy.PersistDemand = 1
Fury.Economy.CanaryOnly = 1
Fury.Economy.CanaryListingId = "<auctioned-item-oid>"
Fury.Economy.CanaryOwnerId = "<seller-creature-oid>"
Fury.Economy.MaxPurchasesPerTick = 1
Fury.Economy.DemandRecoveryPerTick = 0.0 -- isolate purchase-impact durability
```

Leave all normal gross-price and category safety gates enabled.

## Machine-verifiable state probe

Before saving the database baseline, capture the live canary state:

```bash
fury-market-probe capture <listing-oid> <seller-oid> baseline
fury-market-snapshot save baseline
```

After a crash stage, inspect the recovered database with a normal Core3
initialization followed by native graceful shutdown:

```bash
fury-market-probe inspect <listing-oid> <seller-oid> baseline stage-3-after
fury-market-verify 3 baseline stage-3-after
```

The probe JSON carries exact AuctionItem/sold-object IDs, persistent child
objects, seller balances, city treasury and demand. The baseline also stores
the expected one-settlement committed balances and demand, so stage 6 remains
verifiable even though the listing no longer exists.

## Reusable baseline snapshot

With Core3 stopped after creating the deterministic test listing, save one
baseline:

```bash
fury-market-snapshot save baseline
```

The snapshot contains both the MariaDB application database and the complete
Berkeley `MMOCoreORB/bin/databases` directory. Both stores are required for a
repeatable crash/restart proof.

Each crash stage can then restore that baseline automatically. If a baseline
probe with the same name exists, `--verify` turns the whole crash/restart proof
into one command:

```bash
fury-market-canary 1 <listing-oid> <seller-oid> --restore baseline --verify baseline
```

The runner restores the database snapshot, executes the intentional crash,
launches a read-only `core3 shutdown` probe against the crashed state, then
runs the machine verifier.

Without `--verify`, the restore-only form remains available:


```bash
fury-market-canary 1 <listing-oid> <seller-oid> --restore baseline
fury-market-canary 2 <listing-oid> <seller-oid> --restore baseline
# ... through stage 6
```

Use `fury-market-snapshot list` to inspect saved baselines. Saving/restoring
fails closed while a `core3` process is running.

## Docker one-command runner

Inside the FURY Docker runtime, use:

```bash
fury-market-canary <stage> <auctioned-item-oid> <seller-oid>
```

The runner writes/replaces one marked canary block at the end of
`config-local.lua`, forces exact-listing canary mode, disables demand
recovery, verifies `/tre`, starts MariaDB when needed, and automatically builds
Core3 when the current git SHA differs from the last canary build.

For stages 1-6 it launches `core3` directly rather than through the inherited
gdb wrapper. This is intentional: the inherited gdb settings catch SIGABRT,
which would otherwise stop the intentional failure-injection crash inside the
debugger. The runner verifies that the expected stage marker appeared before
accepting the crash as expected.

Use `--config-only` to update the canary block without starting the server,
or `--rebuild` to force a compile.

## Complete seven-stage proof

After `fury-market-fixture` has created `baseline`, run:

```bash
fury-market-canary-suite <listing-oid> <seller-oid> baseline
```

The suite runs `1,2,3,4,5,6,0` in that order. Before every stage it restores
the exact same baseline and after every stage it executes the restart probe and
machine verifier. It stops at the first failed invariant.

Use `--rebuild` to force one compile before stage 1; the remaining six stages
reuse the same binary when the repository SHA is unchanged.

## Failure stages 1-5: must roll back by restart

Run the same untouched database snapshot separately with
`FailureInjectionStage` set to each value 1 through 5.

The process is expected to abort intentionally.

After restart, every stage must satisfy all of these invariants:

- listing is still present and FORSALE;
- AuctionItem still points at the same sold object;
- sold root object still exists;
- all persistent sold-object children still exist;
- seller bank and cash equal the pre-test values;
- city treasury equals the pre-test value;
- regional demand equals the pre-test value;
- no successful `durableCommit=true` FURY audit entry exists.

A failure of any single invariant blocks execution rollout.

## Failure stage 6: must survive as committed

Reset to the untouched snapshot and run with:

```lua
Fury.Economy.FailureInjectionStage = 6
```

The process again aborts intentionally, but this time only after synchronous
Berkeley commit has returned.

After restart all of these must be true:

- listing is absent;
- AuctionItem database row/object is absent;
- sold root object is absent;
- all persistent children are absent;
- seller received exactly the planned net payout;
- city treasury received exactly the planned tax, when applicable;
- regional demand contains exactly one purchase impact from the recorded pre-test value;
- the sale is not applied a second time on subsequent ticks.

A post-commit audit log is not required for stage 6: the deliberate crash occurs
before best-effort post-commit logging.

## Normal canary success

Stage 0 is automated as well:

```bash
fury-market-canary 0 <listing-oid> <seller-oid> --restore baseline --verify baseline
```

For this run only, the runner sets `CanaryAutoShutdown=1`. After the exact
canary listing commits, post-commit audit/notification code completes and the
settlement schedules a retrying shutdown task. The task waits until Core3's
console pipe exists, queues the native `shutdown 0` command, and emits the
queued-shutdown marker only after that succeeds. The runner requires a clean
process exit plus the durable-settlement and queued-shutdown markers before it
launches the same post-restart probe/verifier used by stage 6.

Verify:

- the same committed-state invariants as stage 6;
- seller receives correct live bank/cash deltas when online;
- seller receives the FURY sale system message when online;
- one post-commit audit entry has `furyMarket=true` and
  `durableCommit=true`;
- no second purchase occurs for the same listing.

Keep `DemandRecoveryPerTick=0` for all seven crash-recovery runs so the
expected demand delta is unambiguous. Restore the intended recovery rate only
after the settlement boundary itself is proven.

Only after all seven runs (1-6 plus normal success) pass should
`CanaryOnly=0` even be considered.
