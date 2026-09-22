# Serialized NPC purchase execution

The market evaluator can rank several listings from one snapshot, but
settlement currently rechecks live listing identity, product state, raw quality
signal and demand while keeping the reference price / relative-quality
reference captured by the evaluation tick.

Scheduling several NPC settlements from that same snapshot would therefore let
later settlements reuse a market reference that no longer includes the listing
removed by an earlier FURY purchase.

Until settlement gains lock-consistent live comparable recomputation, real
execution is hard-capped to **one NPC purchase per market tick**.

`Fury.Economy.MaxPurchasesPerTick` remains in config for the future rollout.
Values above 1 currently log a warning and execute as 1; values <= 0 still
disable scheduling.

This removes FURY's own multi-settlement stale-snapshot race. It does not claim
to eliminate arbitrary player-driven market changes in the small
schedule-to-settlement interval.
