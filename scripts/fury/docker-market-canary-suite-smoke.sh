#!/bin/bash
set -eo pipefail

repo_root="$(cd "$(dirname "$0")/../.." && pwd)"
suite="${repo_root}/docker/files/usr/local/bin/fury-market-canary-suite"

tmpdir="$(mktemp -d)"
trap 'rm -rf "${tmpdir}"' EXIT

export HOME_DIR="${tmpdir}/home"
export FURY_CANARY_PROBE_ROOT="${tmpdir}/probes"
mkdir -p "${FURY_CANARY_PROBE_ROOT}"
echo '{"valid":true}' > "${FURY_CANARY_PROBE_ROOT}/baseline.json"

call_log="${tmpdir}/calls.log"

fake_canary="${tmpdir}/fake-canary"
cat > "${fake_canary}" <<'EOF'
#!/bin/bash
printf '%s\n' "$*" >> "${FURY_TEST_SUITE_CALL_LOG}"
EOF
chmod +x "${fake_canary}"

fake_snapshot="${tmpdir}/fake-snapshot"
cat > "${fake_snapshot}" <<'EOF'
#!/bin/bash
if [ "$1" = "list" ]; then
    echo baseline
    exit 0
fi
exit 2
EOF
chmod +x "${fake_snapshot}"

export FURY_MARKET_CANARY_BIN="${fake_canary}"
export FURY_MARKET_SNAPSHOT_BIN="${fake_snapshot}"
export FURY_TEST_SUITE_CALL_LOG="${call_log}"

"${suite}" 18446744073709551615 9223372036854775808 baseline --rebuild

mapfile -t calls < "${call_log}"
[ "${#calls[@]}" -eq 7 ]

expected=(1 2 3 4 5 6 0)
for i in "${!expected[@]}"; do
    stage="${expected[$i]}"
    prefix="${stage} 18446744073709551615 9223372036854775808 --restore baseline --verify baseline"
    [[ "${calls[$i]}" == "${prefix}"* ]] ||
        { echo "unexpected suite call: ${calls[$i]}" >&2; exit 1; }
done

[[ "${calls[0]}" == *" --rebuild" ]]
[[ "${calls[1]}" != *" --rebuild" ]]
[[ "${calls[6]}" != *" --rebuild" ]]

rm -f "${FURY_CANARY_PROBE_ROOT}/baseline.json"
if "${suite}" 1 2 baseline; then
    echo "suite unexpectedly accepted missing baseline probe" >&2
    exit 1
fi

echo "FURY market canary suite smoke OK"
