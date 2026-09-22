# FURY canary state probe

A read-only canary probe is available through:

- `Fury.Economy.CanaryProbe = 1`
- `Fury.Economy.CanaryProbeOutput = "<path>"`
- optional `Fury.Economy.CanaryProbeBaseline = "<baseline-json>"`

The probe runs after the auction map is fully reconstructed from Berkeley but
before startup auction-maintenance tasks are queued.

It records machine-readable JSON covering:

- exact listing presence and status;
- AuctionItem persistent-object presence;
- sold root object and persistent child graph;
- seller bank/cash;
- city treasury;
- regional demand;
- listing/vendor/location/comparison identifiers.

When no baseline JSON is supplied it also records the expected committed
seller balances, city treasury and demand after one settlement. When a baseline
is supplied, those expectations are carried forward even if the listing and
sold object have already disappeared after a stage-6 commit.

The intended headless verification launch is `core3 shutdown`: Core3 performs
normal initialization (including Berkeley recovery and probe generation), then
uses its native graceful shutdown path instead of being killed.
