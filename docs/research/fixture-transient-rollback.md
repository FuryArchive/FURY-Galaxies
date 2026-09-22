# Fixture transient rollback hardening

The deterministic market fixture validates the complete three-listing market
shape before making any of its SceneObjects or AuctionItems persistent.

If object creation, auction-map insertion, comparable-count validation or the
purchase-decision check fails before the durable boundary, FURY now removes
every already-added fixture AuctionItem from the auction map / commodity limit
and sends both AuctionItems and weapons through Core3's normal deletion
lifecycle.

This keeps a failed fixture attempt fail-closed in memory as well as on disk.

The same change also makes the fixture portable across Core3's typedefs by
parsing OIDs into `std::uint64_t` and explicitly converting to Core3
`uint64`, and disambiguates the ZoneServer `createObject` overload.
