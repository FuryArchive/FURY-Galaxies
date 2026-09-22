-- FURY Galaxies example overrides.
-- Copy the values you want into conf/config-local.lua.
--
-- FURY features remain disabled by default until their runtime slice has been
-- validated on a local server.

Fury = Fury or {}

Fury.Economy = {
	ObserveVendorMarket = 1,
	DryRun = 1,
	TickSeconds = 600,
	PurchaseThreshold = 0.62,
}
