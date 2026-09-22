# Persistent object-graph purchase cap

Before any mutation, settlement enumerates the complete persistent sold-object
graph with Core3's recursive child traversal.

`Fury.Economy.MaxPersistentObjectsPerPurchase` defaults to 64.

If the root item plus its persistent container/slotted/child objects exceed
that cap, the purchase is rejected. A value below 1 disables all settlement.

This is a blast-radius guard for malformed/unexpected listed objects. Normal
weapon/armor items and their factory crates should remain far below the cap.
