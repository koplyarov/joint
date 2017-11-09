[ "$RUN_BENCHMARKS" -ne 0 ] || exit 0

REFERENCE_SOURCE_DIR="$REFERENCE_JOINT_DIR"
REFERENCE_BUILD_DIR="$REFERENCE_SOURCE_DIR/build"

ReadReferenceHash() {
    REFERENCE_HASH="$(cat "$SOURCE_DIR/benchmarks/ci/reference_commit.txt")"
}

CloneJoint() {
    [ -d "$REFERENCE_SOURCE_DIR/.git" ] ||
    Verbose git clone https://github.com/koplyarov/joint.git "$REFERENCE_SOURCE_DIR"
}

CheckoutReferenceCommit() {
    Verbose git checkout "$REFERENCE_HASH" &&
    Verbose git submodule update --init
}

ConfigureReference() {
    [ "$COMPILER" ] && export CC="$COMPILER"
    [ "$COMPILERXX" ] && export CXX="$COMPILERXX"
    Verbose cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DJOINT_TREAT_WARNINGS_AS_ERRORS=TRUE $CMAKE_FLAGS "$REFERENCE_SOURCE_DIR"
}

MakeReference() {
    Verbose make "-j$NUM_CORES"
}

Verbose ReadReferenceHash &&
Verbose CloneJoint &&
Verbose cd "$REFERENCE_SOURCE_DIR" &&
Verbose CheckoutReferenceCommit &&
Verbose mkdir -p "$REFERENCE_BUILD_DIR" &&
Verbose cd "$REFERENCE_BUILD_DIR" &&
Verbose ConfigureReference &&
Verbose MakeReference ||
exit 1
