#!/usr/bin/env bash
set -u

fail=0
root="$(git rev-parse --show-toplevel 2>/dev/null || true)"

ok()   { printf '  [OK] %s\n' "$*"; }
warn() { printf '  [!!] %s\n' "$*"; fail=1; }

echo "FURY Galaxies doctor"

if [[ -n "$root" ]]; then
  ok "repository: $root"
else
  warn "not inside a Git checkout"
  exit 1
fi

branch="$(git -C "$root" branch --show-current 2>/dev/null || true)"
[[ "$branch" == "fury-dev" ]] && ok "branch: fury-dev" || warn "branch is '$branch' (expected fury-dev for development)"

if git -C "$root" remote get-url upstream >/dev/null 2>&1; then
  ok "upstream remote: $(git -C "$root" remote get-url upstream)"
else
  warn "upstream remote missing (run scripts/fury/sync-upstream.sh once)"
fi

if [[ -d "$root/MMOCoreORB/utils/engine3/.git" || -f "$root/MMOCoreORB/utils/engine3/.git" ]]; then
  ok "engine3 submodule initialized"
else
  warn "engine3 submodule not initialized (git submodule update --init --recursive)"
fi

command -v docker >/dev/null 2>&1 && ok "docker available" || warn "docker not found"

if command -v docker >/dev/null 2>&1; then
  if docker volume inspect shared-tre >/dev/null 2>&1; then
    count="$(docker run --rm -v shared-tre:/tre debian:bookworm sh -c 'find /tre -maxdepth 1 -type f -name "*.tre" | wc -l' 2>/dev/null || echo '?')"
    ok "shared-tre volume exists ($count TRE files)"
  else
    warn "Docker volume shared-tre is missing"
  fi
fi

if [[ -f "$root/MMOCoreORB/bin/conf/config-local.lua" ]]; then
  ok "local Core3 config exists"
else
  warn "MMOCoreORB/bin/conf/config-local.lua not present yet"
fi

if [[ -f "$root/MMOCoreORB/bin/conf/fury-example.lua" ]]; then
  ok "FURY config example present"
else
  warn "FURY config example missing"
fi

echo
if [[ "$fail" -eq 0 ]]; then
  echo "FURY environment looks ready."
else
  echo "FURY environment has setup items above; no changes were made."
fi

exit "$fail"
