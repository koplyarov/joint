#!/bin/bash

VG() {
	valgrind --suppressions=etc/valgrind.sup --tool=memcheck --leak-check=full --gen-suppressions=all --error-exitcode=1 -q ./build/bin/joint-test $@
}

export PYTHONPATH="`pwd`/test/python:`pwd`/build/bin"
clear
VG cpp tests-cpp
VG cpp tests-c
VG python Tests
./build/bin/joint-test java file://`pwd`/build/bin/tests.jar:test.Tests
