# Dry-run immutability

`Fury.Economy.DryRun = 1` is a hard no-mutation contract.

Even when `PersistDemand = 1` loads an existing persistent
`FuryEconomyState`, dry-run evaluation only reads stored demand values.

Demand recovery changes persistent simulation state, so recovery now runs only
when both conditions are true:

- `DryRun = 0`
- `ExecutePurchases = 1`

This keeps diagnostic dry-runs reproducible and prevents observing the economy
from advancing it.
