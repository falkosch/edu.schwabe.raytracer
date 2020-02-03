#!/usr/bin/env bash

. ./build-with-local-cc.sh

(cd data && find "../${SOURCES_DIR}" -executable -type f -print -exec {} \;)
