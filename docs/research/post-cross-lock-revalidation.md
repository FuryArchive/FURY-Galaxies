# Post-cross-lock revalidation

Engine3 cross-lock acquisition avoids deadlock by temporarily releasing the
already-held cross lock while the next lock is contested. Therefore validation
performed before building a cross-lock chain is only an optimization, not a
settlement guarantee.

The NPC settlement repeats all mutable invariants after the final lock chain is
held:

- listing status is still FORSALE and not an auction;
- vendor, owner and price still match the decision snapshot;
- AuctionItem still points at the same sold-object OID;
- factory-crate quantity still matches;
- effective type / exact template comparison key still matches;
- category still has a recognized quality model.

Only then does FURY construct the persistent sold-object graph and start
mutation.

The lock chain is:

`AuctionItem -> sold root SceneObject -> seller CreditObject -> FuryEconomyState -> CityRegion (when taxable)`.

This closes the stale-decision window introduced by cross-lock deadlock
avoidance.
