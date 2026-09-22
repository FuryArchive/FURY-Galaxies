# Purchase-category safety gate

Dry-run observation is intentionally broad: FURY may inspect every fixed-price
vendor listing so the server can collect diagnostics and reveal future model
gaps.

Mutation is narrower.

By default `Fury.Economy.RequireKnownQualityForPurchases = 1`. A settlement
is only scheduled and accepted when `FuryItemQualityExtractor` recognizes the
live product.

At the current milestone that means:
- weapons;
- armor;
- factory crates whose prototype is a weapon or armor item.

Unknown categories (resources, consumables, generic components, miscellaneous
items and other not-yet-modeled products) remain visible in dry-run diagnostics
but cannot be consumed by the NPC market.

This gate should stay enabled until category-specific valuation is implemented.
