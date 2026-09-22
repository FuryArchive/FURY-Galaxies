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

- crash before commit -> none of the persistent settlement mutations become durable;
- successful task commit -> listing/payment/tax/demand changes become durable together.

This is the exactly-once foundation FURY should rely on.

## Remaining runtime proof

Before `ExecutePurchases` can ever default on, test intentional failure injection at these points:

1. after validation;
2. after seller credit mutation;
3. after auction removal;
4. after item destruction request;
5. after demand mutation.

After restart, every case must resolve to either the entire sale committed or the entire sale absent — never double payment and never item loss without payment.
