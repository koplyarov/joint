BUILD_DIR="$MAIN_DIR/build"

VG() {
    if [ "$USE_VALGRIND" -ne 0 ]; then
        valgrind "--suppressions=$SOURCE_DIR/etc/valgrind.sup" --tool=memcheck --leak-check=full --gen-suppressions=all --error-exitcode=1 -q "$BUILD_DIR/bin/joint-test" $@
    else
        "$BUILD_DIR/bin/joint-test" $@
    fi
}

HAS_ERRORS=0

VG "$BUILD_DIR/bin/c/Tests/Tests.jm" || HAS_ERRORS=1
VG "$BUILD_DIR/bin/cpp/Tests/Tests.jm" || HAS_ERRORS=1
VG "$BUILD_DIR/bin/cpp/Tests/Tests-noexcept.jm" || HAS_ERRORS=1
PYTHONPATH="$BUILD_DIR/bin" VG "$BUILD_DIR/bin/python/Tests/Tests.jm" || HAS_ERRORS=1
"$BUILD_DIR/bin/joint-test" "$BUILD_DIR/bin/java/Tests/Tests.jm" || HAS_ERRORS=1

exit $HAS_ERRORS
