#!/usr/bin/env bash

. ./build-with-local-cc.sh

SOURCES_DIR="$(pwd)/${OUTPUT_DIR}/sources"

(cd data && find "${SOURCES_DIR}" -executable -type f -print -exec {} \;)
