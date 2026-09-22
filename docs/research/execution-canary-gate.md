# Execution canary gate

Real NPC market settlement is fail-closed behind an explicit canary scope.

Defaults:

- `CanaryOnly = 1`
- `CanaryListingId = "0"`
- `CanaryOwnerId = "0"`

With those defaults, even if `ExecutePurchases=1`, no settlement task is
scheduled. Canary mode requires an exact non-zero listing selector.

For a controlled runtime test, set an exact auctioned-item/listing OID.
`CanaryOwnerId` is optional and acts only as a second-factor owner check.
The values are strings so full 64-bit Core3 object IDs are preserved.

The gate is enforced twice: before scheduling and again inside
`settleFuryMarketListing()`. This prevents an accidentally queued or directly
invoked task from escaping the canary scope.

Only after crash-recovery verification should a deployment intentionally set
`CanaryOnly = 0`.
