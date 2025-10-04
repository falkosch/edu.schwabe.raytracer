#!/usr/bin/env bash

. _build-env.sh

CONAN_ARCH_TYPE="${CONAN_ARCH_TYPE:-${ARCH_TYPE}}"
if [ "${CONAN_ARCH_TYPE}" = "x64" ]; then
    CONAN_ARCH_TYPE="x86_64"
fi

# Try having conan available in cygwin although conan is already installed for the parent Windows system
PYTHON=${PYTHON:-$(command -v python || command -v python3)}
[ -f "${PYTHON}" ] || { echo "Please make sure 'python3' is available."; exit 1; }
${PYTHON} -m pip install conan

CONAN=${CONAN:-$(command -v conan)}
[ -f "${CONAN}" ] || { echo "Please make sure 'conan' is available."; exit 1; }

# Safety check to prevent dangerous deletion if CONAN_DEP_INSTALL_DIR is empty or root
CONAN_DEP_INSTALL_DIR="${CONAN_DEP_INSTALL_DIR:-build/conan/${ARCH_TYPE}}"
if [ -z "${CONAN_DEP_INSTALL_DIR}" ] || [ "${CONAN_DEP_INSTALL_DIR}" = "/" ]; then
    echo "Error: CONAN_DEP_INSTALL_DIR has an unsafe value. Aborting for safety."
    exit 1
fi

mkdir -p "${CONAN_DEP_INSTALL_DIR}"
# Safely remove contents of the directory (not the directory itself)
if [ -d "${CONAN_DEP_INSTALL_DIR}" ]; then
    find "${CONAN_DEP_INSTALL_DIR}" -mindepth 1 -delete
fi

${CONAN} install . \
    --profile:build build/conan-profiles/windows-x64 \
    --settings:build arch="${CONAN_ARCH_TYPE}" \
    --output-folder "${CONAN_DEP_INSTALL_DIR}" \
    --build missing \
    ${CONAN_ADD_ARGS}
