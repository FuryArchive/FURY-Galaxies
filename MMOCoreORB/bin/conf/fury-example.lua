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
	CanaryOnly = 1, -- fail-closed until an explicit listing/owner is selected
	CanaryListingId = "0", -- full 64-bit auctioned item/listing OID
	CanaryOwnerId = "0", -- optional full 64-bit seller OID
	PersistDemand = 0, -- enable only after the persistence slice is runtime-validated
	TickSeconds = 600,
	DefaultDemand = 0.50,
	PurchaseThreshold = 0.62,
	DryRunLogLimit = 20,
	MaxPurchasesPerTick = 5,
	PurchaseImpact = 0.05,
	DemandRecoveryPerTick = 0.02,
	RequireKnownQualityForPurchases = 1,
	MinComparablesForPurchase = 2,
	MaxGrossPricePerPurchase = 250000,
	MaxGrossCreditsPerTick = 500000,
	MaxPersistentObjectsPerPurchase = 64,
	FailureInjectionStage = 0,
}
