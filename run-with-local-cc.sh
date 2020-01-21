#!/usr/bin/env bash

. __with-local-cc.sh

RAYTRACERUI_EXE="$(pwd)/${OUTPUT_DIR}/sources/raytracerui/raytracerui.exe"

(cd data && exec ${RAYTRACERUI_EXE})
