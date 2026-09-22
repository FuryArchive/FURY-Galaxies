# Settlement-time live demand recheck

Tick evaluation is a snapshot. Several purchase decisions can target the same
regional demand key before any settlement task runs.

FURY now carries the snapshot reference price and normalized quality score into
the settlement task, but re-reads the current persistent demand while holding
the FuryEconomyState lock.

Before any mutation it then re-runs the same deterministic
`FuryMarketModel::evaluate()` using:

- current persistent demand;
- live per-unit price from the revalidated gross price and quantity;
- the tick's reference median price;
- the tick's normalized quality score;
- the same listing ID / deterministic roll;
- the current configured purchase threshold.

The live product's raw quality signal and known/unknown state must still match
the tick snapshot. If product quality changed, settlement rejects instead of
using stale valuation.

Only if the live recheck still says `purchase=true` does settlement apply the
purchase impact and enter the mutation/commit section.

This serializes demand consumption naturally: earlier committed purchases can
lower demand enough to cancel later tasks from the same tick.
