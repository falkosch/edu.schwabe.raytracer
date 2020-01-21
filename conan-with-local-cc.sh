#!/usr/bin/env bash

. __with-local-cc.sh

conan install . --install-folder ${OUTPUT_DIR} --build missing
