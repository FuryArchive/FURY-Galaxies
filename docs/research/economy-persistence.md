# FURY economy persistence

FURY uses Core3's native object database rather than an external JSON/SQLite sidecar.

## State object

`FuryEconomyState` is a persistent Core3 `ManagedObject`.

Database:
`furyeconomy`

Persisted fields:
- demand map (`string -> float`);
- last demand tick.

The demand map key will encode the stable regional product identity:

`planetCRC : cityRegionObjectID : serverTemplateCRC`

This intentionally avoids localized city names and broad client item categories.

## Loading

When `Fury.Economy.PersistDemand = 1`, `AuctionManager` loads the `furyeconomy` object database during initialization.

If no state exists, one state object is created through `ObjectManager::persistObject`, matching existing Core3 manager patterns such as FRS.

If persistence is disabled (the default), no FURY economy database/state is created by this feature.

## Safety

Persistence is currently storage-only. It is not yet used to mutate demand from dry-run decisions and does not enable purchases.

`ExecutePurchases = 0` remains an independent hard gate.
