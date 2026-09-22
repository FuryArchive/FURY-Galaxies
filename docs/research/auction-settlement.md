# Core3 Auction Settlement Research

## Native instant-buy behavior

The current `AuctionManagerImplementation::doInstantBuy(CreatureObject* player, AuctionItem* item)` is not a reusable server-side sale primitive. It mixes several responsibilities:

1. resolves vendor/city and sales tax;
2. resolves seller by player name;
3. changes auction ownership to the buyer;
4. marks the listing SOLD and sets buyer identity;
5. subtracts buyer bank credits;
6. sends the buyer protocol response;
7. sends seller and buyer mail;
8. credits the seller;
9. subtracts city tax from seller and adds it to treasury.

The purchased object then remains part of the auction retrieval flow for the buyer.

## Consequence for FURY

A simulated NPC market should **not** create a fake connected player merely to satisfy this function.

A fake buyer would introduce:
- fake account/creature lifecycle;
- buyer mail noise;
- buyer inventory/retrieval requirements;
- a permanent sink character holding purchased objects;
- fragile assumptions around online/offline player loading.

## Refactor target

Extract a server-side sale settlement layer with two explicit modes.

### Player purchase
Preserves current behavior:
- debit buyer;
- SOLD state;
- buyer identity;
- buyer/seller mail;
- later item retrieval.

### Simulated market purchase
Different semantics:
- no player debit;
- no buyer mail;
- seller is paid;
- tax is applied;
- listing is removed from the live market;
- sold item is consumed by the simulated economy;
- seller receives a market-sale notification.

The two paths should share validation, seller payment and tax calculation rather than copy those rules.

## Required invariants

### Exactly-once settlement
A market tick must never pay a seller twice for one listing.

Candidate strategy:
1. lock the AuctionItem;
2. verify status == FORSALE and fixed-price semantics;
3. transition to a terminal FURY settlement state or remove atomically from the auction map;
4. record settlement identity;
5. credit seller/tax;
6. consume item and auction record.

Do not implement this until Core3 persistence behavior around AuctionItem destruction and seller credit updates has been traced under crash/restart conditions.

### Object safety
Before consuming an object, explicitly handle:
- factory crates;
- intangible objects;
- nested/container objects;
- no-trade objects;
- missing auctioned SceneObject;
- missing/offline seller;
- missing vendor.

### Tax
The simulated market should initially preserve native city sales tax. This lets settlement match ordinary vendor sales and keeps player-city treasury mechanics relevant.

## Next code investigation
- `AuctionsMapImplementation::deleteItem/removeItem`;
- `AuctionItemImplementation::destroyAuctionItemFromDatabase`;
- `TransactionLog` persistence semantics;
- offline `PlayerManager::getPlayer(name)` behavior;
- native expiration/deletion paths for safe object destruction.

## Decision
Spike 001 stays read-only until those paths are understood. The current observer is intentionally useful without taking ownership of transaction safety.
