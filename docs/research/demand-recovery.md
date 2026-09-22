# Persistent demand recovery

When persistent demand is enabled, each FURY market tick moves every stored
regional-product demand value toward `DefaultDemand`.

Default:
`DemandRecoveryPerTick = 0.02`

Recovery is clamped to `[0, 1]` and never overshoots the target.

The recovery mutation lives on `FuryEconomyState` itself, keeping persistent
state behavior separate from the auction adapter.

A purchase reduces demand by:
`PurchaseImpact * purchasedUnits`

A later market tick then recovers that stored value toward the regional
baseline. This prevents a large factory-crate purchase from permanently
saturating a product market.

At this milestone recovery is tick-based rather than wall-clock catch-up:
server downtime does not simulate missed recovery ticks.
