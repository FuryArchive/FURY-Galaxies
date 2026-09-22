#!/bin/bash
set -eo pipefail

repo_root="$(cd "$(dirname "$0")/../.." && pwd)"
snapshot="${repo_root}/docker/files/usr/local/bin/fury-market-snapshot"

tmpdir="$(mktemp -d)"
trap 'rm -rf "${tmpdir}"' EXIT

export HOME_DIR="${tmpdir}/home"
export FURY_CANARY_SNAPSHOT_ROOT="${tmpdir}/snapshots"
mkdir -p "${FURY_CANARY_SNAPSHOT_ROOT}/baseline"
mkdir -p "${FURY_CANARY_SNAPSHOT_ROOT}/z-last"
mkdir -p "${FURY_CANARY_SNAPSHOT_ROOT}/a-first"

mapfile -t found < <("${snapshot}" list)
[ "${found[0]}" = "a-first" ]
[ "${found[1]}" = "baseline" ]
[ "${found[2]}" = "z-last" ]

if "${snapshot}" restore '../bad'; then
    echo "invalid snapshot name unexpectedly accepted" >&2
    exit 1
fi

"${snapshot}" --help | grep -F 'fury-market-snapshot save'

echo "FURY market snapshot smoke OK"
