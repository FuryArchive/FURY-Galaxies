# Atomic NPC market settlement implementation

The execution candidate no longer relies on normal dirty-object autosave for a
purchase. That would allow credits, city treasury, demand, AuctionItem deletion
and sold-item deletion to reach Berkeley in different save cycles.

For one settlement task FURY now:

1. locks the live AuctionItem;
2. cross-locks seller CreditObject and FuryEconomyState;
3. cross-locks the player CityRegion when city tax applies;
4. revalidates price, owner, vendor, product template, crate quantity and item
   category;
5. rejects the sale if seller credit capacity cannot accept the complete net
   payout;
6. rejects a taxed sale if the city treasury cap would discard part of the tax;
7. removes the listing from the in-memory auction index;
8. mutates seller credits, city treasury and regional demand with no client
   notification;
9. explicitly serializes those changed persistent objects through
   `commitUpdatePersistentObjectToDB()`;
10. explicitly queues deletion of the AuctionItem and the complete persistent
    sold-object graph through `commitDestroyObjectToDB()`;
11. calls one `commitLocalTransaction()`.

That final call is the durable boundary: all writes/deletes above are committed
by one Berkeley transaction.

The sold-object graph uses Core3's `getChildrenRecursive()`, so a factory
crate's prototype and any other persistent container/slotted/child objects are
deleted with the root item instead of becoming database orphans.

If any exception occurs before the durable boundary, FURY clears the pending
local DB transaction and immediately calls `System::abort()`. It deliberately
does not perform graceful shutdown, because graceful Core3 shutdown backs up
the current RAM image and could persist a partially-mutated state.

After a successful commit, deleted objects are marked non-persistent/deleted in
RAM to prevent a later background save from resurrecting them.

Failure injection semantics:

- stages 1-5: crash before durable commit; after restart, the sale must be fully
  absent (listing, item, payout, tax and demand all unchanged);
- stage 6: crash immediately after durable commit; after restart, the sale must
  be fully present as committed.

This is the first candidate that matches engine3's actual persistence model
rather than assuming Task exceptions provide rollback.
