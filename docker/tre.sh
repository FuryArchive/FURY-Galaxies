#!/bin/bash
set -eo pipefail
cd "$(dirname "$0")"

[ ! -f ./env-run ] || source ./env-run
if [ -f ./env-base ]; then source ./env-base; elif [ -f ./default-env ]; then source ./default-env; fi

IMAGE="${IMAGE:-swgemu/core3-dev:latest}"
TRE_VOLUME="${TRE_VOLUME:-shared-tre}"

usage(){ cat <<'EOF'
Usage:
  ./tre.sh import /path/to/SWG/files [--replace]
  ./tre.sh status
  ./tre.sh clear
EOF
}
die(){ echo "FURY TRE ERROR: $*" >&2; exit 2; }
ensure_image(){ docker image inspect "${IMAGE}" >/dev/null 2>&1 || die "Docker image ${IMAGE} is missing; run ./build.sh first"; }
ensure_volume(){ docker volume inspect "${TRE_VOLUME}" >/dev/null 2>&1 || docker volume create "${TRE_VOLUME}" >/dev/null; }
count_files(){
  ensure_image; ensure_volume
  docker run --rm --entrypoint /bin/bash -v "${TRE_VOLUME}:/tre:ro" "${IMAGE}" -lc 'find /tre -type f 2>/dev/null | wc -l'
}

case "${1:-}" in
  import)
    [ "$#" -ge 2 ] || { usage; exit 2; }
    src="$(realpath "$2")"
    [ -d "${src}" ] || die "source directory does not exist: ${src}"
    find "${src}" -type f -print -quit | grep -q . || die "source directory contains no files"
    replace=0
    if [ "${3:-}" = "--replace" ]; then replace=1; elif [ "$#" -gt 2 ]; then die "unknown option: ${3}"; fi
    ensure_image; ensure_volume
    if [ "${replace}" -eq 1 ]; then
      docker run --rm --entrypoint /bin/bash -v "${TRE_VOLUME}:/tre" "${IMAGE}" -lc 'find /tre -mindepth 1 -maxdepth 1 -exec rm -rf -- {} +'
    fi
    docker run --rm --entrypoint /bin/bash -v "${TRE_VOLUME}:/tre" -v "${src}:/source:ro" "${IMAGE}" -lc 'cp -a /source/. /tre/'
    count="$(count_files)"
    [ "${count}" -gt 0 ] || die "TRE volume is empty after import"
    echo "FURY TRE ready: volume=${TRE_VOLUME}, files=${count}"
    ;;
  status)
    ensure_volume
    if docker image inspect "${IMAGE}" >/dev/null 2>&1; then
      echo "FURY TRE volume: ${TRE_VOLUME}; files=$(count_files)"
    else
      echo "FURY TRE volume exists: ${TRE_VOLUME}; image not built"
    fi
    ;;
  clear)
    ensure_image; ensure_volume
    docker run --rm --entrypoint /bin/bash -v "${TRE_VOLUME}:/tre" "${IMAGE}" -lc 'find /tre -mindepth 1 -maxdepth 1 -exec rm -rf -- {} +'
    echo "FURY TRE volume cleared: ${TRE_VOLUME}"
    ;;
  -h|--help) usage ;;
  *) usage; exit 2 ;;
esac
