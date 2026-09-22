# FURY NPC market runtime canary runbook

This is the first runtime validation path for real NPC market settlement.
Do not disable canary mode for this pass.

## Preconditions

Use an isolated/local Core3 database snapshot.

Choose one fixed-price listing that:

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

Finally reset once more, set `FailureInjectionStage = 0`, and perform one
canary purchase.

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
