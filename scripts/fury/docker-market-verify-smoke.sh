#!/bin/bash
set -eo pipefail

repo_root="$(cd "$(dirname "$0")/../.." && pwd)"
verify="${repo_root}/docker/files/usr/local/bin/fury-market-verify"

tmpdir="$(mktemp -d)"
trap 'rm -rf "${tmpdir}"' EXIT
export HOME_DIR="${tmpdir}/home"
export FURY_CANARY_PROBE_ROOT="${tmpdir}/probes"
mkdir -p "${FURY_CANARY_PROBE_ROOT}"

cat > "${FURY_CANARY_PROBE_ROOT}/baseline.json" <<'EOF'
{
  "valid": true,
  "baselineLoaded": false,
  "listingPresent": true,
  "listingForSale": true,
  "expectedEligible": true,
  "auctionItemObjectPresent": true,
  "auctionItemObjectId": 18446744073709551615,
  "auctionItemObjectIdText": "18446744073709551615",
  "soldObjectPresent": true,
  "soldObjectId": 18446744073709551614,
  "soldObjectIdText": "18446744073709551614",
  "vendorId": 9223372036854775808,
  "vendorIdText": "9223372036854775808",
  "comparisonKey": 42,
  "grossPrice": 1000,
  "units": 2,
  "sellerBank": 100,
  "sellerCash": 10,
  "cityPresent": true,
  "cityTreasury": 5000.0,
  "demandKey": "1:2:42",
  "demand": 0.5,
  "persistentObjectIds": [18446744073709551614, 18446744073709551613],
  "persistentObjectIdsText": ["18446744073709551614", "18446744073709551613"],
  "persistentObjectPresence": {"18446744073709551614": true, "18446744073709551613": true},
  "expectedSellerBankAfter": 1050,
  "expectedSellerCashAfter": 10,
  "expectedCityTreasuryAfter": 5050.0,
  "expectedDemandAfter": 0.4
}
EOF

cat > "${FURY_CANARY_PROBE_ROOT}/rollback.json" <<'EOF'
{
  "valid": true,
  "baselineLoaded": true,
  "listingPresent": true,
  "listingForSale": true,
  "auctionItemObjectPresent": true,
  "auctionItemObjectId": 18446744073709551615,
  "auctionItemObjectIdText": "18446744073709551615",
  "soldObjectPresent": true,
  "soldObjectId": 18446744073709551614,
  "soldObjectIdText": "18446744073709551614",
  "vendorId": 9223372036854775808,
  "vendorIdText": "9223372036854775808",
  "comparisonKey": 42,
  "grossPrice": 1000,
  "units": 2,
  "sellerBank": 100,
  "sellerCash": 10,
  "cityPresent": true,
  "cityTreasury": 5000.0,
  "demandKey": "1:2:42",
  "demand": 0.5,
  "persistentObjectPresence": {"18446744073709551614": true, "18446744073709551613": true}
}
EOF

cat > "${FURY_CANARY_PROBE_ROOT}/committed.json" <<'EOF'
{
  "valid": true,
  "baselineLoaded": true,
  "listingPresent": false,
  "auctionItemObjectPresent": false,
  "soldObjectPresent": false,
  "sellerBank": 1050,
  "sellerCash": 10,
  "cityPresent": true,
  "cityTreasury": 5050.0,
  "demand": 0.4,
  "persistentObjectPresence": {"18446744073709551614": false, "18446744073709551613": false}
}
EOF

[ "$(jq -r '.auctionItemObjectIdText' "${FURY_CANARY_PROBE_ROOT}/baseline.json")" = "18446744073709551615" ]
[ "$(jq -r '.persistentObjectIdsText[0]' "${FURY_CANARY_PROBE_ROOT}/baseline.json")" = "18446744073709551614" ]

"${verify}" 3 baseline rollback
"${verify}" 6 baseline committed
"${verify}" 0 baseline committed

cp "${FURY_CANARY_PROBE_ROOT}/rollback.json" "${FURY_CANARY_PROBE_ROOT}/bad.json"
jq '.sellerBank = 101' "${FURY_CANARY_PROBE_ROOT}/bad.json" > "${FURY_CANARY_PROBE_ROOT}/bad.tmp"
mv "${FURY_CANARY_PROBE_ROOT}/bad.tmp" "${FURY_CANARY_PROBE_ROOT}/bad.json"

if "${verify}" 2 baseline bad; then
    echo "bad rollback unexpectedly verified" >&2
    exit 1
fi

echo "FURY market verifier smoke OK"
