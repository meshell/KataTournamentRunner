#! /bin/bash
WORKSPACE_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd ${WORKSPACE_DIR}/build
../scripts/gcovr -x -r ${WORKSPACE_DIR} -e'.*/externals/.*' -e'.*/build/.*' -e'.*/tests/.*' -o ../$@
