# Exactly-once NPC market settlement

## Engine3 transaction boundary

Core3 tasks already provide the transaction primitive FURY needs.

In non-STM builds, `Task::run` is wrapped by Engine3 so that:

1. a local object-database transaction is established for the task;
2. the task mutates managed objects;
3. `ObjectDatabaseManager::commitLocalTransaction()` flushes the task's queued object updates/deletes through one BerkeleyDB transaction.

The local transaction contains updates across the object databases participating in that task. On abort, pending object updates are discarded.

`TransactionLog` is still useful for audit/debugging, but it is **not** the atomicity mechanism.

## FURY settlement unit

One simulated purchase must execute in **one dedicated Task**.

Do not settle multiple unrelated listings as one giant market tick transaction.

Flow:

```
FuryMarketTickTask
  -> read-only evaluation
  -> selected listing IDs
  -> schedule one FuryMarketSettlementTask per listing

FuryMarketSettlementTask(listingId)
  -> lock/reload live AuctionItem
  -> revalidate FORSALE + fixed-price + seller + item
  -> compute FurySettlementPlan
  -> mutate native auction state
  -> credit seller
  -> apply city tax
  -> consume sold SceneObject
  -> update persistent regional demand
  -> write TransactionLog
  -> return

Task wrapper
  -> one BerkeleyDB commit for the whole settlement
```

## Revalidation matters

A dry-run decision is not authority.

Between market scan and settlement another action may:
- buy the item;
- cancel the listing;
- change/remove the vendor;
- invalidate the seller;
- remove the underlying object.

Therefore the settlement task re-fetches the listing by object ID and validates current state under lock immediately before mutation.

If validation fails, the task exits without payment or item deletion.

## Crash semantics

Because one purchase is one Engine3 task transaction:

- process crash before Berkeley commit -> none of the queued persistent mutations become durable;
- successful Berkeley commit -> the queued listing/payment/tax/demand changes become durable together.

However, non-STM `Task::run` catches ordinary C++ exceptions and still calls `commitLocalTransaction()`. Therefore an exception is **not** a rollback mechanism.

FURY must use a two-phase implementation shape:

1. **prevalidation phase** — perform every operation that can legitimately fail before changing credits/listing/item/demand;
2. **mutation phase** — only deterministic, non-failable managed-object mutations and auction-map removal remain.

Unexpected exceptions in the mutation phase are treated as a server correctness bug, not normal control flow.

## Remaining runtime proof

Before `ExecutePurchases` can ever default on:

1. test process termination before the settlement task reaches database commit;
2. test process termination during Berkeley commit;
3. verify restart recovery after the committed sale;
4. separately test validation failures and ensure they happen before mutation;
5. instrument unexpected mutation-phase errors as fatal correctness failures.

The crash/restart cases must resolve to either the entire persistent sale committed or the entire persistent sale absent — never double payment and never item loss without payment.


## Core3 auction deletion trap

Do **not** use `AuctionsMap::deleteItem(vendor, item, true)` for a simulated-market settlement.

With `deleteAuctionedObject=true`, `AuctionItem::destroyAuctionItemFromDatabase` schedules destruction of the underlying sold `SceneObject` as a separate `slowQueue` task. That puts object consumption outside the settlement task transaction.

FURY settlement must instead:

1. hold a strong reference to the sold SceneObject;
2. call `auctionMap->deleteItem(vendor, item, false)` to remove/delete only the AuctionItem record;
3. call `sellingObject->destroyObjectFromDatabase(true)` directly inside the same settlement task.

This keeps auction-record deletion and sold-object deletion in the same local transaction as credits/tax/demand.
