# Core3 deletion lifecycle in atomic settlement

FURY now separates two responsibilities explicitly:

1. **RAM lifecycle:** after removing a listing from the auction index, settlement
   calls Core3's normal deletion APIs:
   - `AuctionItem::destroyAuctionItemFromDatabase(false, false)`
   - `SceneObject::destroyObjectFromDatabase(true)`

   These synchronously mark the AuctionItem and complete sold-object graph as
   non-persistent / pending deletion using Core3's existing recursive object
   lifecycle.

2. **Atomic persistence:** FURY still queues explicit
   `commitDestroyObjectToDB()` operations for the AuctionItem OID and every
   persistent OID in the sold-object graph, together with payout, city tax and
   demand updates, before one `commitLocalTransaction()`.

This avoids manually mutating child object flags after the durable commit.
The normal background save may later encounter the already-deleted OIDs; its
delete path tolerates `DB_NOTFOUND`, marks them deleted in RAM and completes
normal object-map cleanup.

On a pre-commit injected failure the process fail-stops immediately, so the
RAM deletion marks are discarded with the process and restart reconstructs the
unchanged listing from the old database state.
