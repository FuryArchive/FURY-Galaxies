# FURY settlement failure injection

`Fury.Economy.FailureInjectionStage` is **0 by default** and must remain zero
outside deliberate crash/recovery testing.

Non-zero values intentionally crash Core3 after a particular in-memory
mutation:

1. auction listing removed;
2. seller credited;
3. city tax credited;
4. persistent demand reduced;
5. sold object marked for deletion.

The settlement catch path first calls
`ObjectDatabaseManager::abortLocalTransaction()`, flushes an emergency log,
and then calls `System::abort()`.

Expected test result for every stage after restart:
- the listing is still present;
- the sold item still exists;
- seller balance is unchanged;
- city treasury is unchanged;
- demand is unchanged.

Only `FailureInjectionStage = 0` may produce a committed purchase.

This tests the actual engine3 transaction behavior instead of assuming C++
exceptions roll back Task state.
