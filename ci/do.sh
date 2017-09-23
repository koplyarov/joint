#!/bin/bash

MY_DIR="$(dirname $0)"
. "$MY_DIR/common.sh" || exit 1

[ $# -eq 1 ] || Fail "Usage: $0 <target>"

TARGETS_DIR="$MY_DIR/$1"

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
