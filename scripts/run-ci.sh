#!/bin/bash

SCRIPT_DIR="$(dirname $(readlink -f $0))"
ROOT_DIR="$SCRIPT_DIR/.."

RunCi() {
	rm -rf "$ROOT_DIR/.ci" &&
	"$ROOT_DIR/ci/do.sh" prepare &&
	"$ROOT_DIR/ci/do.sh" run &&
	"$ROOT_DIR/ci/do.sh" finalize ||
	return 1
}

USE_VALGRIND=1 BUILD_TYPE=Debug RunCi &&
RUN_BENCHMARKS=1 BENCHMARKS_NUM_PASSES=2 BUILD_TYPE=RelWithDebInfo RunCi &&
echo "Everything is OK"
