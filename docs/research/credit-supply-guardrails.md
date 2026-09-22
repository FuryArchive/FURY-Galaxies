# NPC market credit-supply guardrails

Execution must not treat a lone seller's asking price as sufficient evidence to
mint arbitrary credits.

Default mutation gates:

- `MinComparablesForPurchase = 2`
- `MaxGrossPricePerPurchase = 250000`
- `MaxGrossCreditsPerTick = 500000`
- `MaxPurchasesPerTick = 5`

Dry-run evaluation is not restricted by these caps; diagnostics still reveal
what the scoring model would prefer.

The execution scheduler only creates settlement tasks when all limits pass.
The settlement task independently rechecks the per-purchase gross cap against
the live listing so a stale or malformed task cannot bypass it.

The per-tick budget is a scheduler guard rather than persistent fiscal policy.
A later FURY economy phase can replace it with explicit NPC treasury/reserve
accounts once the vertical slice is proven safe.
