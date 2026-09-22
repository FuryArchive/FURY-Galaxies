# FURY simulated purchase execution slice

This commit introduces the execution path but keeps it unreachable under the
default configuration.

A real settlement requires all of the following:

- `ObserveVendorMarket = 1`;
- `DryRun = 0`;
- `ExecutePurchases = 1`;
- `PersistDemand = 1`;
- a successfully loaded `FuryEconomyState`.

`DryRun = 1` suppresses purchases even if `ExecutePurchases` is accidentally
enabled.

Each selected listing is scheduled as its own
`FuryMarketSettlementTask`. The task re-fetches and revalidates the live
listing's vendor, owner, price, quantity and exact product key before any
mutation.

Seller payout uses the character's persistent `CreditObject` through
`CreditManager`, so the seller does not need to be online.

The settlement task:
1. removes the live auction record;
2. removes commodity-limit accounting;
3. credits the seller net amount;
4. credits player-city treasury tax where native Core3 would do so;
5. reduces persistent regional demand;
6. destroys the sold object directly in the same task;
7. writes a `BAZAARSYSTEM` transaction log with FURY-specific state.

The sold object is deliberately **not** deleted through
`AuctionsMap::deleteItem(..., true)`, because that would defer object deletion
to another task and break the intended atomic transaction boundary.

This execution path must still pass full Core3 compilation and deliberate
crash/failure-injection runtime tests before it is enabled in a playable
configuration.
