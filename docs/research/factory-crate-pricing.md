# FURY factory-crate market pricing

Core3's `AuctionItem.size` is the vendor slot footprint returned by
`getSizeOnVendorRecursive()`. It is **not** the number of products in a
factory crate.

For `FactoryCrate` the number of products is the crate object's own
`getUseCount()`. `getPrototypeUseCount()` describes the use-count/charges
of one extracted prototype and must not be used as crate quantity.

FURY therefore snapshots factory-crate quantity as `listing.units` and
compares prices on a per-contained-item basis. The actual purchase settlement
still pays the full listed gross price for the whole crate.

Example:

- one CDEF pistol: 1,000 credits;
- crate of 25 identical CDEF pistols: 25,000 credits.

Both contribute a 1,000-credit comparison price to the median.

This prevents factory crates from corrupting reference prices for otherwise
identical crafted products.
