#!/usr/bin/env bash
set -euo pipefail

root="$(git rev-parse --show-toplevel)"
cd "$root"

if ! git remote get-url upstream >/dev/null 2>&1; then
  git remote add upstream https://github.com/swgemu/Core3.git
fi

git fetch upstream unstable
git checkout unstable
git merge --ff-only upstream/unstable

echo
echo "Upstream branch updated."
echo "FURY work remains on fury-dev; merge/rebase it deliberately after reviewing upstream changes."
