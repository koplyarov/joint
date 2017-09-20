[ "$1" == "--no-valgrind" ] && NO_VALGRIND=1 || NO_VALGRIND=0

VG() {
	if [ "$NO_VALGRIND" -eq 0 ]; then
		valgrind --suppressions=etc/valgrind.sup --tool=memcheck --leak-check=full --gen-suppressions=all --error-exitcode=1 -q ./build/bin/joint-test $@
	else
		./build/bin/joint-test $@
	fi
}

HAS_ERRORS=0

VG build/bin/c/Tests/Tests.jm || HAS_ERRORS=1
VG build/bin/cpp/Tests/Tests.jm || HAS_ERRORS=1
PYTHONPATH="`pwd`/test/python:`pwd`/build/bin" VG build/bin/python/Tests/Tests.jm || HAS_ERRORS=1

exit $HAS_ERRORS
