UpdatePath() {
	if [ "$COVERITY_BUILD" -ne 0 ]; then
		Verbose export PATH="$PATH:$(pwd)/$(echo cov-analysis-linux64-*)/bin"
	fi
}

Configure() {
	(
		[ "$COMPILER" ] && export CC="$COMPILER"
		[ "$COMPILERXX" ] && export CXX="$COMPILERXX"
		Verbose cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DJOINT_TREAT_WARNINGS_AS_ERRORS=TRUE $CMAKE_FLAGS ..
	)
}

Make() {
	if [ "$COVERITY_BUILD" -ne 0 ]; then
		Verbose cov-build --dir cov-int make -j2
	else
		Verbose make -j2
	fi
}

Verbose UpdatePath &&
Verbose mkdir build &&
Verbose cd build && 
Verbose Configure &&
Verbose Make ||
exit 1
