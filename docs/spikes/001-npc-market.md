# Spike 001 — Native Vendor Market

## Question
Can FURY turn SWG's player-vendor economy into a useful single-player economy without replacing the vendor client UI?

## Existing Core3 seam
`AuctionManager` owns an `AuctionsMap`, loads persisted `AuctionItem` objects at startup, exposes vendor terminal lists and already performs periodic vendor maintenance.

That gives FURY a native path to real listings:
`AuctionManager -> AuctionsMap -> TerminalListVector -> AuctionItem -> auctioned SceneObject`.

## Stage A — read-only observer
Implement `FuryMarketObserver`.

Input:
- a native `TerminalListVector` returned from `getVendorTerminalData("", "", 0)`.

Output:
- terminal count;
- active FORSALE listing count;
- fixed-price vs auction count;
- min/max/average asking price;
- aggregate asking value.

Hook:
- run after native vendor maintenance;
- controlled by `Fury.Economy.ObserveVendorMarket`;
- no listing, item, credit or owner mutation.

This proves:
- Core3 build integration;
- AuctionManager integration;
- access to persisted native listings;
- safe runtime scheduling point.

## Stage B — dry-run buyer
Add a pure `FuryMarketModel`:
- demand: 0..1;
- reference price;
- quality/value signal;
- deterministic listing hash;
- purchase threshold.

It returns a decision object only. No Core3 writes.

## Stage C — real settlement
Trace Core3's native instant-buy path and factor the sale settlement into a server-side operation that can use a synthetic market buyer without faking a connected CreatureObject.

Required invariants:
- seller receives exactly one payment;
- listing leaves sale state exactly once;
- item ownership/location remains valid;
- vendor/bazaar bookkeeping remains consistent;
- transaction is recoverable after restart;
- duplicate tick cannot double-buy.

## Kill criteria
If native settlement cannot be safely invoked without extensive client/player-session assumptions, do not hack around it. Introduce a dedicated server-side settlement primitive and make both player instant-buy and FURY buyer call the same primitive.

## Definition of success
A crafted item listed on a native player vendor is detected, scored, purchased by the simulated market, pays the seller, disappears from the listing, and remains correct after server restart.
