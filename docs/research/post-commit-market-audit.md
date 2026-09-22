# Post-commit market audit log

FURY writes a `TransactionLog` entry for an NPC market purchase only after
the explicit Berkeley `commitSync()` boundary succeeds.

Recorded fields include:

- listing ID;
- sold object ID;
- gross price;
- city sales tax;
- seller net payout;
- units;
- comparison key;
- planet CRC;
- region ID;
- demand after purchase;
- `furyMarket=true`;
- `durableCommit=true`.

The audit log is intentionally not the source of truth for exactly-once
settlement. It is a post-commit observability artifact. A process crash in the
tiny interval after Berkeley commit and before the log write can produce a
committed sale without a corresponding log entry.

That asymmetry is preferable to the reverse: FURY never emits a successful
market audit entry for a transaction that has not reached the durable database
boundary.
