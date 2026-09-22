# FURY Galaxies Status

## 2026-09-22

### Foundation
- Fork created from `swgemu/Core3`.
- Development branch: `fury-dev`.
- Upstream tracking branch: `unstable`.
- FURY code isolated under `MMOCoreORB/src/server/zone/managers/fury/`.
- Local bootstrap, environment doctor and upstream-sync helpers added.
- Lightweight market CI is active.
- Full Core3 build gate is active and uses the real Core3 C++14/IDL build.

### Economy Spike 001

Implemented:
- read-only `FuryMarketObserver` over native Core3 vendor listings;
- portable `FuryMarketListing` snapshots from real `AuctionItem` data;
- factory-crate prototype type handling;
- deterministic `FuryMarketModel`;
- explicit unknown-quality handling (no invented average quality);
- category-relative dry-run pricing;
- fixed-price-only dry-run buyer decisions;
- dedicated `FuryMarketTickTask` independent of Core3's hourly auction maintenance;
- configurable tick interval, demand and purchase threshold;
- smoke/unit coverage for pure market logic.

Current runtime behavior:
```
native player vendor
    -> AuctionsMap / AuctionItem
    -> FuryMarketObserver
    -> FuryMarketListing
    -> FuryMarketDryRun
    -> log: eligible / wouldPurchase
```

No item, credit, vendor, owner or demand state is mutated.

### Native settlement research

Core3's `doInstantBuy` is not suitable for a synthetic NPC buyer as-is. It combines:
- buyer bank debit;
- auction ownership transfer;
- buyer identity;
- SOLD/retrieval lifecycle;
- buyer and seller mail;
- seller payment;
- city sales tax.

Decision: do **not** create fake NPC player creatures. The real purchase milestone will introduce a reusable server-side settlement primitive with separate player-purchase and simulated-market semantics.

### Immediate gates
1. Full Core3 build green on current `fury-dev`.
2. Local server boot with legally obtained TRE assets.
3. Real crafted item listed on a native player vendor.
4. FURY tick observes that listing.
5. Dry-run buyer scores that real listing.
6. Add item-type-specific quality extraction, starting with weapons/armor.
7. Implement exactly-once simulated settlement.
8. Persist regional demand across restart.

### Still deliberately deferred
- actual NPC purchases;
- settlement/city progression;
- companions;
- global profession/skill-point rebalance;
- custom client UI.

The rule remains: no destructive simulation until the read-only path is proven end to end.
