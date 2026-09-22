# Runtime proof bootstrap complete

The FURY NPC market crash-proof harness now has two top-level commands.

First, on an isolated runtime with a valid test seller character:

```bash
fury-market-fixture <seller-oid> baseline
```

This creates and validates the deterministic native market fixture, performs a
graceful startup/shutdown cycle, captures the baseline state probe and saves a
paired MariaDB + Berkeley snapshot.

Second:

```bash
fury-market-canary-suite <listing-oid> <seller-oid> baseline
```

This restores the same baseline and runs failure stages 1-6 plus normal success
stage 0 with automatic post-restart verification.

Fixture creation configuration is removed before the baseline probe so later
crash/restart stages cannot silently recreate listings or reset demand.

All 64-bit object identifiers consumed by shell orchestration come from exact
decimal-string manifest/probe fields.

The remaining external prerequisite for a real end-to-end run is the SWG TRE
data required by Core3, plus an existing isolated test character in the local
database.
