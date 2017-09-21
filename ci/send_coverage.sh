[ "$TEST_COVERAGE" -eq 0 ] && exit 0

echo "Sending coverage info"

ToCoveralls() {
	coveralls-lcov --repo-token ${COVERALLS_TOKEN} coverage.info || ( echo "Coveralls did not collect coverage reports"; return 1 )
}

ToCodecov() {
	bash <(curl -s https://codecov.io/bash) || ( echo "Codecov did not collect coverage reports"; return 1 )
}

Upload() {
	local RET=0
	ToCodecov || RET=1
	ToCoveralls || RET=1
	return $RET
}

lcov --directory . --capture --output-file coverage.info &&
lcov --remove coverage.info '/usr/*' 'test/*' 'benchmarks/*' 'examples/*' 'build/core/joint/JsonParser.*.cpp' 'bindings/java/*' --output-file coverage.info &&
lcov --list coverage.info && 
Upload ||
exit 1
