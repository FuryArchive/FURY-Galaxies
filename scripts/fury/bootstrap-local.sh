#!/usr/bin/env bash
set -euo pipefail

root="$(git rev-parse --show-toplevel)"
cd "$root"

if ! git remote get-url upstream >/dev/null 2>&1; then
  git remote add upstream https://github.com/swgemu/Core3.git
fi

git submodule update --init --recursive

if ! command -v docker >/dev/null 2>&1; then
  echo "Docker is required for the preferred FURY development environment." >&2
  exit 1
fi

docker volume inspect shared-tre >/dev/null 2>&1 || docker volume create shared-tre >/dev/null

if [[ "${1:-}" != "" ]]; then
  tre_dir="$(realpath "$1")"

  if ! compgen -G "$tre_dir/*.tre" >/dev/null; then
    echo "No .tre files found in: $tre_dir" >&2
    exit 1
  fi

  echo "Importing TRE files from: $tre_dir"
  tar -C "$tre_dir" -cf - ./*.tre | docker run -i --rm -v shared-tre:/tre debian:bookworm sh -c 'tar xf - -C /tre'
fi

echo
echo "Bootstrap complete."
echo "Next:"
echo "  ./scripts/fury/doctor.sh"
echo "  cd docker && ./build.sh"
echo "  cd docker && ./run.sh"
echo
echo "Pass a directory containing legally obtained SWG client .tre files as the first argument to import them."
