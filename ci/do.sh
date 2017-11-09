#!/bin/bash

. "$(dirname $0)/common.sh" || exit 1

[ $# -eq 1 ] || Fail "Usage: $0 <target>"

CI_SCRIPTS_DIR="$(NormalizedDirName "$0")"
SOURCE_DIR="$CI_SCRIPTS_DIR/.."
CI_WORKING_DIR="$(NormalizePath "$CI_SCRIPTS_DIR/../.ci")"
TARGETS_DIR="$CI_SCRIPTS_DIR/$1"

MAIN_DIR="$(NormalizePath "$CI_WORKING_DIR/0")"
REFERENCE_JOINT_DIR="$(NormalizePath "$CI_WORKING_DIR/1")"

mkdir -p "$CI_WORKING_DIR"
mkdir -p "$MAIN_DIR"
mkdir -p "$REFERENCE_JOINT_DIR"
cd "$CI_WORKING_DIR"

ZeroInit RUN_BENCHMARKS TEST_COVERAGE COVERITY_BUILD USE_VALGRIND

if [ "$COVERITY_BUILD" -ne 0 -a "$TRAVIS_EVENT_TYPE" != "cron" ]; then
    Log "Skipping COVERITY_BUILD=1 build (allowed only from cron builds)"
    exit 0
fi

for SCRIPT in $(ls -1 "$TARGETS_DIR"); do
    Log "=== $SCRIPT ==="
    ( . "$TARGETS_DIR/$SCRIPT" )
    [ $? -eq 0 ] || Fail "ERROR: $SCRIPT failed!"
done
