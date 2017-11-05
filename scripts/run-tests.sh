#!/bin/bash

SCRIPT_DIR="$(dirname $(readlink -f $0))"
ROOT_DIR="$SCRIPT_DIR/.."

VG() {
	valgrind "--suppressions=$ROOT_DIR/etc/valgrind.sup" --tool=memcheck --leak-check=full --gen-suppressions=all --error-exitcode=1 -q "$ROOT_DIR/build/bin/joint-test" $@
}

export PYTHONPATH="$ROOT_DIR/build/bin"
clear
VG $ROOT_DIR/build/bin/cpp/Tests/Tests.jm
VG $ROOT_DIR/build/bin/cpp/Tests/Tests-noexcept.jm
VG $ROOT_DIR/build/bin/c/Tests/Tests.jm
VG $ROOT_DIR/build/bin/python/Tests/Tests.jm
$ROOT_DIR/build/bin/joint-test build/bin/java/Tests/Tests.jm
