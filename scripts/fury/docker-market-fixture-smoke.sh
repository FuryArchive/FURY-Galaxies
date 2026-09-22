#!/bin/bash
set -eo pipefail

repo_root="$(cd "$(dirname "$0")/../.." && pwd)"
helper="${repo_root}/docker/files/usr/local/bin/fury-market-fixture"

tmpdir="$(mktemp -d)"
trap 'rm -rf "${tmpdir}"' EXIT

export HOME_DIR="${tmpdir}/home"
export FURY_CORE3_REPO="${HOME_DIR}/workspace/Core3"
mkdir -p "${FURY_CORE3_REPO}/MMOCoreORB/bin/conf"

"${helper}" 18446744073709551615 baseline --vendor 9223372036854775808 --config-only

cfg="${FURY_CORE3_REPO}/MMOCoreORB/bin/conf/config-local.lua"

grep -F -- '-- BEGIN FURY MARKET FIXTURE' "${cfg}"
grep -F 'Fury.Economy.CanaryFixtureSellerId = "18446744073709551615"' "${cfg}"
grep -F 'Fury.Economy.CanaryFixtureVendorId = "9223372036854775808"' "${cfg}"
grep -F 'Fury.Economy.CanaryFixtureCreate = 1' "${cfg}"
grep -F 'Fury.Economy.PersistDemand = 1' "${cfg}"

if "${helper}" 18446744073709551616 baseline --config-only; then
    echo "overflowing seller OID unexpectedly accepted" >&2
    exit 1
fi

if "${helper}" 0 baseline --config-only; then
    echo "zero seller OID unexpectedly accepted" >&2
    exit 1
fi

echo "FURY market fixture helper smoke OK"
