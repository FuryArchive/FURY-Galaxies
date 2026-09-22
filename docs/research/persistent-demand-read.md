# Persistent demand read path

The first persistence integration is deliberately read-only.

When `Fury.Economy.PersistDemand = 1` successfully loads a
`FuryEconomyState`, each vendor listing receives a demand value from the
persistent map before dry-run evaluation.

Stable key format:

`planetCRC:cityRegionObjectID:serverTemplateCRC`

If no stored value exists, `DefaultDemand` is used.

This stage does **not** write demand, consume listings, pay sellers, or modify
city treasuries. It only proves that native persistent state can feed the
portable market-decision model.
