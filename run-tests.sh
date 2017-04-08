#!/bin/bash

VG() {
	valgrind --suppressions=etc/valgrind.sup --tool=memcheck --leak-check=full --gen-suppressions=all --error-exitcode=1 -q ./build/bin/joint-test $@
}

export PYTHONPATH="`pwd`/build/bin"
clear
VG build/bin/cpp/Tests/Tests.jm
VG build/bin/c/Tests/Tests.jm
VG build/bin/python/Tests/Tests.jm
./build/bin/joint-test build/bin/java/Tests/Tests.jm
