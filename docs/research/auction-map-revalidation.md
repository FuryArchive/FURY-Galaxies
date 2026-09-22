# Auction-map identity revalidation

A listing can be removed from the transient AuctionsMap before another code
path finishes changing its AuctionItem fields.

Therefore post-cross-lock validation now also performs a synchronized lookup
of `auctionMap[listingId]` and requires it to be the exact same AuctionItem
instance captured by the settlement task.

If the mapping is gone or points elsewhere, settlement stops before any
mutation even when the stale object still temporarily looks FORSALE.
