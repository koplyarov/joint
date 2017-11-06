BUILD_DIR="$MAIN_DIR/build"

UpdatePath() {
	if [ "$COVERITY_BUILD" -ne 0 ]; then
		Verbose export PATH="$PATH:$(pwd)/$(echo cov-analysis-linux64-*)/bin"
	fi
}

Configure() {
	(
		[ "$COMPILER" ] && export CC="$COMPILER"
		[ "$COMPILERXX" ] && export CXX="$COMPILERXX"
		[ "$TEST_COVERAGE" -ne 0 ] && CMAKE_FLAGS="$CMAKE_FLAGS -DPYTHON_COVERAGERC=$CI_WORKING_DIR/.coveragerc -DPYTHON_COVERAGE_RESULT=$CI_WORKING_DIR/.coverage"
		Verbose cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DJOINT_TREAT_WARNINGS_AS_ERRORS=FALSE $CMAKE_FLAGS "$SOURCE_DIR"
	)
}

Make() {
	if [ "$COVERITY_BUILD" -ne 0 ]; then
		Verbose cov-build --dir cov-int make "-j$NUM_CORES"
	else
		Verbose make "-j$NUM_CORES"
	fi
}

Verbose UpdatePath &&
Verbose mkdir -p "$BUILD_DIR" &&
Verbose cd "$BUILD_DIR" &&
Verbose Configure &&
Verbose Make ||
exit 1
