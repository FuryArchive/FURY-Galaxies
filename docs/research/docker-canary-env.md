# Docker environment controls for FURY market canary

The Docker firstboot generator can write FURY economy overrides into
`MMOCoreORB/bin/conf/config-local.lua` directly from environment variables.

Supported variables:

- `FURY_ECONOMY_OBSERVE_VENDOR_MARKET`
- `FURY_ECONOMY_DRY_RUN`
- `FURY_ECONOMY_EXECUTE_PURCHASES`
- `FURY_ECONOMY_PERSIST_DEMAND`
- `FURY_ECONOMY_CANARY_ONLY`
- `FURY_ECONOMY_CANARY_LISTING_ID`
- `FURY_ECONOMY_CANARY_OWNER_ID`
- `FURY_ECONOMY_FAILURE_INJECTION_STAGE`
- `FURY_ECONOMY_DEMAND_RECOVERY_PER_TICK`
- `FURY_ECONOMY_MAX_PURCHASES_PER_TICK`
- `FURY_ECONOMY_TICK_SECONDS`

The default Docker env remains fail-closed: observation off, dry-run on,
execution off, persistent demand off, canary-only on and both OID selectors
zero.

Canary OIDs are emitted as quoted decimal strings so 64-bit object IDs are not
truncated by Lua numeric conversion. CI explicitly covers the maximum uint64
value, `18446744073709551615`.

This removes the need to hand-edit Lua between crash stages. A local runtime
can change only the failure-stage environment value while restoring the same
database snapshot for each run.
