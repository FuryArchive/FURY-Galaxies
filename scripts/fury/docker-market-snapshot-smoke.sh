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


# Restore failure must roll back both Berkeley and MariaDB.
mockbin="${tmpdir}/mockbin"
mkdir -p "${mockbin}"

cat > "${mockbin}/mysqladmin" <<'EOF'
#!/bin/bash
exit 0
EOF

cat > "${mockbin}/mysqldump" <<'EOF'
#!/bin/bash
echo 'ROLLBACK DATABASE IMAGE'
EOF

cat > "${mockbin}/mysql" <<'EOF'
#!/bin/bash
set -e
count_file="${FURY_TEST_MYSQL_COUNT}"
count=0
[ ! -f "${count_file}" ] || count="$(cat "${count_file}")"
count=$((count + 1))
printf '%s\n' "${count}" > "${count_file}"

if [ "${count}" -eq 1 ]; then
    cat >/dev/null
    exit 1
fi

cat > "${FURY_TEST_ROLLBACK_CAPTURE}"
exit 0
EOF

chmod +x "${mockbin}/mysqladmin" "${mockbin}/mysqldump" "${mockbin}/mysql"

export PATH="${mockbin}:${PATH}"
export FURY_TEST_MYSQL_COUNT="${tmpdir}/mysql-count"
export FURY_TEST_ROLLBACK_CAPTURE="${tmpdir}/rollback-applied.sql"

export HOME_DIR="${tmpdir}/restore-home"
export FURY_CORE3_REPO="${HOME_DIR}/workspace/Core3"
export FURY_CANARY_SNAPSHOT_ROOT="${tmpdir}/restore-snapshots"
mkdir -p "${FURY_CORE3_REPO}/MMOCoreORB/bin/databases"
echo old > "${FURY_CORE3_REPO}/MMOCoreORB/bin/databases/old-marker"

mkdir -p "${tmpdir}/snapshot-build/databases"
echo new > "${tmpdir}/snapshot-build/databases/new-marker"
mkdir -p "${FURY_CANARY_SNAPSHOT_ROOT}/atomic"
tar -C "${tmpdir}/snapshot-build" -czf "${FURY_CANARY_SNAPSHOT_ROOT}/atomic/berkeley.tar.gz" databases
printf '%s\n' 'BROKEN SNAPSHOT SQL' | gzip -1 > "${FURY_CANARY_SNAPSHOT_ROOT}/atomic/mysql.sql.gz"

if "${snapshot}" restore atomic; then
    echo "failed MariaDB restore unexpectedly succeeded" >&2
    exit 1
fi

test -f "${FURY_CORE3_REPO}/MMOCoreORB/bin/databases/old-marker"
test ! -f "${FURY_CORE3_REPO}/MMOCoreORB/bin/databases/new-marker"
grep -F 'ROLLBACK DATABASE IMAGE' "${FURY_TEST_ROLLBACK_CAPTURE}"
[ "$(cat "${FURY_TEST_MYSQL_COUNT}")" -eq 2 ]

echo "FURY snapshot atomic rollback smoke OK"
