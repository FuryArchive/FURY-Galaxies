#!/bin/bash
set -eo pipefail

repo_root="$(cd "$(dirname "$0")/../.." && pwd)"
source "${repo_root}/docker/files/firstboot/functions"

msg() { :; }
warning() { :; }
error() {
    echo "ERROR: $1" >&2
    exit "${2:-100}"
}

tmpdir="$(mktemp -d)"
trap 'rm -rf "${tmpdir}"' EXIT

export HOME_DIR="${tmpdir}/home"
export RUN_USER="$(id -un)"
mkdir -p "${HOME_DIR}/workspace/Core3/MMOCoreORB/bin/conf"

export GALAXY_ID=2
export FURY_ECONOMY_OBSERVE_VENDOR_MARKET=1
export FURY_ECONOMY_DRY_RUN=0
export FURY_ECONOMY_EXECUTE_PURCHASES=1
export FURY_ECONOMY_PERSIST_DEMAND=1
export FURY_ECONOMY_CANARY_ONLY=1
export FURY_ECONOMY_CANARY_LISTING_ID=18446744073709551615
export FURY_ECONOMY_CANARY_OWNER_ID=9223372036854775808
export FURY_ECONOMY_FAILURE_INJECTION_STAGE=6
export FURY_ECONOMY_DEMAND_RECOVERY_PER_TICK=0.0
export FURY_ECONOMY_MAX_PURCHASES_PER_TICK=1
export FURY_ECONOMY_TICK_SECONDS=10

core3_init_config >/dev/null

cfg="${HOME_DIR}/workspace/Core3/MMOCoreORB/bin/conf/config-local.lua"

grep -F 'Fury = Fury or {}' "${cfg}"
grep -F 'Fury.Economy = Fury.Economy or {}' "${cfg}"
grep -F 'Fury.Economy.ObserveVendorMarket = 1' "${cfg}"
grep -F 'Fury.Economy.DryRun = 0' "${cfg}"
grep -F 'Fury.Economy.ExecutePurchases = 1' "${cfg}"
grep -F 'Fury.Economy.PersistDemand = 1' "${cfg}"
grep -F 'Fury.Economy.CanaryOnly = 1' "${cfg}"
grep -F 'Fury.Economy.CanaryListingId = "18446744073709551615"' "${cfg}"
grep -F 'Fury.Economy.CanaryOwnerId = "9223372036854775808"' "${cfg}"
grep -F 'Fury.Economy.FailureInjectionStage = 6' "${cfg}"
grep -F 'Fury.Economy.DemandRecoveryPerTick = 0.0' "${cfg}"
grep -F 'Fury.Economy.MaxPurchasesPerTick = 1' "${cfg}"
grep -F 'Fury.Economy.TickSeconds = 10' "${cfg}"

echo "FURY Docker canary config smoke OK"
