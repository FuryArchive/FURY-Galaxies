# FURY Galaxies Status

## 2026-09-22

### Foundation
- Fork created from `swgemu/Core3`.
- Development branch: `fury-dev`.
- Upstream branch retained as `unstable`.
- Server architecture confirmed: C++/IDL + Lua, native Docker workflow, native AuctionManager/AuctionsMap persistence.

### Milestone 0 — prove the fork is a viable single-player conversion
In progress.

Current implementation target:
**read-only observation of real player-vendor listings from native AuctionManager maintenance.**

Why this first:
it exercises a difficult, central MMO dependency (player economy) through real Core3 objects while carrying effectively zero risk to player state.

### Next gates
1. Compile observer hook.
2. Run server with FURY observer enabled.
3. Create/craft/list an item on a player vendor.
4. Confirm listing is visible in FURY market snapshot.
5. Add deterministic dry-run purchase scoring.
6. Refactor native sale settlement for synthetic-market purchases.
7. Persist regional demand.

### Deliberately deferred
- custom client UI;
- companions;
- settlement/city progression;
- global balance changes;
- skill-point redesign.

Those begin after the economy spike proves the server can be changed cleanly.
