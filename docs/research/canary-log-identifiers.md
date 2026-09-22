# Canary identifiers in dry-run diagnostics

Every FURY market decision log now exposes the identifiers needed for a
runtime canary:

- `listing`: auctioned/sold SceneObject OID and AuctionsMap lookup key;
- `auctionRecord`: persistent AuctionItem object OID;
- `vendor`: vendor/bazaar object OID;
- `owner`: seller creature OID.

The canary scope uses `listing` and optionally `owner`. The
`auctionRecord` value is diagnostic evidence for restart/deletion checks.
