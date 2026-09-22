#!/bin/bash
set -eo pipefail

repo_root="$(cd "$(dirname "$0")/../.." && pwd)"
runner="${repo_root}/docker/files/usr/local/bin/fury-market-canary"

tmpdir="$(mktemp -d)"
trap 'rm -rf "${tmpdir}"' EXIT

export HOME_DIR="${tmpdir}/home"
mkdir -p "${HOME_DIR}/workspace/Core3/MMOCoreORB/bin/conf"
cfg="${HOME_DIR}/workspace/Core3/MMOCoreORB/bin/conf/config-local.lua"

cat > "${cfg}" <<'EOF'
-- unrelated local config must survive
ZoneGalaxyID = 2
EOF

"${runner}" 6 18446744073709551615 9223372036854775808 --config-only

grep -F 'ZoneGalaxyID = 2' "${cfg}"
grep -F -- '-- BEGIN FURY MARKET CANARY' "${cfg}"
grep -F 'Fury.Economy.CanaryListingId = "18446744073709551615"' "${cfg}"
grep -F 'Fury.Economy.CanaryOwnerId = "9223372036854775808"' "${cfg}"
grep -F 'Fury.Economy.FailureInjectionStage = 6' "${cfg}"
grep -F 'Fury.Economy.DemandRecoveryPerTick = 0.0' "${cfg}"

"${runner}" 3 123 456 --config-only
[ "$(grep -c -- '-- BEGIN FURY MARKET CANARY' "${cfg}")" -eq 1 ]
grep -F 'Fury.Economy.CanaryListingId = "123"' "${cfg}"
grep -F 'Fury.Economy.CanaryOwnerId = "456"' "${cfg}"
grep -F 'Fury.Economy.FailureInjectionStage = 3' "${cfg}"
! grep -F '18446744073709551615' "${cfg}"

if "${runner}" 1 18446744073709551616 1 --config-only; then
    echo "overflowing listing OID unexpectedly accepted" >&2
    exit 1
fi

if "${runner}" 7 1 1 --config-only; then
    echo "invalid stage unexpectedly accepted" >&2
    exit 1
fi

echo "FURY market canary runner smoke OK"
