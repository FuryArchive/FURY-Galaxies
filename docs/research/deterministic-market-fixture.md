# Deterministic FURY market fixture

The runtime crash proof no longer depends on a hand-crafted player listing.

With `CanaryFixtureCreate=1`, AuctionManager builds an isolated deterministic
market fixture during startup:

- one real CDEF pistol listing at 100 credits;
- two identical CDEF comparables at 1,000 credits each;
- identical weapon stats: 100 min / 100 max damage, 1.0 attack speed,
  1000/1000 condition;
- persistent regional demand set to 0.8.

The seller is an existing character OID with a valid CreditObject. The seller
does not need to be online. `CanaryFixtureVendorId=0` auto-selects the lowest
loaded static bazaar-terminal OID; a specific vendor/bazaar OID can be supplied
instead.

Before creating anything, FURY refuses to proceed if any listing with the same
CDEF server-template CRC already exists. This prevents unrelated market data
from contaminating the median reference.

All three SceneObjects and AuctionItems are created non-persistent first. FURY
then runs the real observer + dry-run model against the in-memory fixture and
requires:

- exactly three comparable listings;
- reference price exactly 1,000;
- known quality;
- the 100-credit target to be `wouldPurchase=true`.

Only after that check passes are the six objects plus persistent demand queued
into one local Berkeley commit.

The fixture is idempotent. If the exact three-listing fixture already exists,
a rerun validates it and reconstructs the fixture manifest instead of creating
duplicates.

The manifest contains target listing OID, AuctionItem DB OID, seller/vendor
OIDs, comparable OIDs, template CRC, prices, demand and decision scores.
