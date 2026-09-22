# FURY Galaxies — Engineering Contract

FURY Galaxies turns SWGEmu/Core3 into a single-player-first sandbox RPG while preserving the systems that make Star Wars Galaxies distinctive: professions, resource quality, crafting, housing, harvesting, factories, vendors, exploration, missions, and persistent world simulation.

## Source of truth
- Upstream: `swgemu/Core3`, branch `unstable`.
- FURY development branch: `fury-dev`.
- Preserve upstream behavior unless a FURY change is intentional and documented.
- Keep FURY-specific code isolated under `MMOCoreORB/src/server/zone/managers/fury/` where practical.
- Prefer server-side changes that use existing SWG client surfaces. Do not require custom client behavior unless explicitly documented as a client dependency.

## Working rules
1. A FURY feature must be switchable by configuration until proven stable.
2. New economy/world-simulation logic must be deterministic or seedable and testable.
3. Never destroy player inventory, listings, structures, or credits in a prototype path.
4. First implementation of a risky feature is observe-only or dry-run.
5. Pure decision logic belongs outside manager side effects and gets unit tests.
6. Persistence changes require an explicit migration/rollback note.
7. Upstream merges must remain feasible; avoid gratuitous edits to upstream files.
8. Do not commit client TRE assets, credentials, database dumps, or local config secrets.

## Current milestone
Prove that FURY can observe and later consume real player-vendor listings through Core3's native AuctionManager without replacing the SWG vendor UI.

Success path:
`crafted item -> native vendor listing -> Fury market scan -> deterministic decision -> native sale settlement -> persistence across restart`.

The first merged slice is intentionally read-only.
