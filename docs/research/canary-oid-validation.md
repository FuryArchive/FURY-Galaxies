# Canary OID parsing is fail-closed

Canary selectors are full 64-bit Core3 object IDs stored as strings.

FURY accepts only decimal digits and explicitly checks uint64 overflow. Empty
strings and `"0"` mean "selector not set".

If either configured selector is malformed or overflows uint64, the execution
scope is marked invalid. An invalid scope rejects every candidate even when
`CanaryOnly=0`.

This prevents a typo from degrading a precise canary test into a broader
owner-only or unrestricted execution scope.
