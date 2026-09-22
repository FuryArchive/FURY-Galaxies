-- FURY Galaxies example overrides.
-- Copy the values you want into conf/config-local.lua.
--
-- FURY features remain disabled by default until their runtime slice has been
-- validated on a local server.

Fury = Fury or {}

Fury.Economy = {
	ObserveVendorMarket = 1,
	DryRun = 1,
	ExecutePurchases = 0,
	PersistDemand = 0, -- enable only after the persistence slice is runtime-validated
	TickSeconds = 600,
	DefaultDemand = 0.50,
	PurchaseThreshold = 0.62,
	DryRunLogLimit = 20,
}
