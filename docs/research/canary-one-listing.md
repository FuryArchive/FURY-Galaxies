# Canary is one exact listing

`CanaryOnly=1` now requires a non-zero `CanaryListingId`.

`CanaryOwnerId` may additionally pin the expected seller, but owner-only
execution is rejected. This prevents a canary configuration from authorizing
multiple listings belonging to one account/character.

The normal per-tick purchase limit remains in force, but canary safety no
longer depends on that aggregate limit.
